/*
   Copyright (C) 2012 Exascale Research Center

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2
   of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include <err.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include "ini.h"
#include "perfmon/pfmlib.h"
#include "perfmon/perf_event.h"
#include "perfmon/pfmlib_perf_event.h"

//#define INI_FILE "ccyclescounters.ini"


/** Expression operator or operand type */
typedef enum {
   ETP_OPERAND,   // value is the operand index in list
   ETP_OPERATOR   // operator as char in value
} exp_tp;

/** Expression operator or operand */
struct exp_op;
typedef struct exp_op {
   struct exp_op *left;
   struct exp_op *right;

   exp_tp etp;
   unsigned int value;
} exp_t;

/** @brief context for the library */
typedef struct {
   unsigned int nbCounters;
   char **counterNames;
   unsigned int core;
   exp_t *mergeExp;

   int *pfmFds;
   uint64_t *startVal;
} context;

/**
 * Frees the memory allocated for an expression tree.
 *
 * @param exp The expression tree to free.
 */
static void free_exp(exp_t *exp)
{
   if (exp == NULL)
   {
      return;
   }

   free_exp(exp->left);
   free_exp(exp->right);
   free(exp);
}

/**
 * Builds an arithmetic expression from a string. The string must use only a
 * restricted set of operators (+, -, *, /) applied on operands designated by
 * their index enclosed in '{' and '}'. Parenthesis are not allowed and operator
 * precedence is not respected: expressions are evaluated from left to right.
 *
 * @param string The expression as a string.
 *
 * @return the expression tree ready to be evaluated.
 */
static exp_t *build_exp(const char *string)
{
   unsigned int opval = 0;
   bool in_operand = false;
   const char *c;
   exp_t *root;

   root = NULL;

   for (c = string; *c != '\0'; c++)
   {
      // begin of operand
      if (*c == '{')
      {
         in_operand = true;
         opval = 0;
      }
      // operand value
      else if (*c >= '0' && *c <= '9')
      {
         // number outside of '{}'
         if (!in_operand)
         {
            free_exp(root);
            return NULL;
         }

         opval *= 10;
         opval += *c - '0';
      }
      // end of operand
      else if (*c == '}')
      {
         exp_t *opexp;

         in_operand = false;

         opexp = malloc(sizeof(*opexp));
         opexp->left = NULL;
         opexp->right = NULL;
            
         opexp->etp = ETP_OPERAND;
         opexp->value = opval;

         if (root == NULL)
         {
            root = opexp;
         }
         else if (root->left == NULL)
         {
            root->left = opexp;
         }
         else
         {
            root->right = opexp;
         }
      }
      else if (*c == '*' || *c == '/' || *c == '+' || *c == '-')
      {
         exp_t *opexp;

         // should not being seen in operand numbers
         if (in_operand)
         {
            free_exp(root);
            return NULL;
         }

         opexp = malloc(sizeof(*opexp));
         opexp->left = root;
         opexp->right = NULL;

         opexp->etp = ETP_OPERATOR;
         opexp->value = (unsigned int) *c;

         root = opexp;
      }
   }

   return root;
}

/**
 * Evaluates an expression using the provided values.
 *
 * @param exp The expression to evaluate
 * @param opval The operand values
 *
 * @return The expression value
 */
static double eval_expr(exp_t *exp, uint64_t *opval)
{
   if (exp->etp == ETP_OPERAND)
   {
      return opval[exp->value];
   }
   else
   {
      if (exp->value == (unsigned int) '+')
      {
         return eval_expr(exp->left, opval) + eval_expr(exp->right, opval);
      }
      if (exp->value == (unsigned int) '-')
      {
         return eval_expr(exp->left, opval) - eval_expr(exp->right, opval);
      }
      if (exp->value == (unsigned int) '*')
      {
         return eval_expr(exp->left, opval) * eval_expr(exp->right, opval);
      }
      if (exp->value == (unsigned int) '/')
      {
         return eval_expr(exp->left, opval) / eval_expr(exp->right, opval);
      }
   }

   return -1;
}

/**
 * @brief Callback for parsing the INI file.
 *
 * @param user The current context.
 * @param section The current section we're in.
 * @param name The property name.
 * @param value The property value.
 *
 * @return 0 on failure, non zero otherwise.
 */
static int iniHandler(void* user, const char* section, const char* name,
                       const char* value)
{
   context *ctx = (context *) user;
   unsigned int i;

   (void) (section);

   if (!strcmp(name, "counters"))
   {
      char *buf = strdup(value);
      char *tokintern;
      char *tmp, *tok;

      // count the counters
      ctx->nbCounters = 1;
      for (tmp = buf; *tmp != '\0'; tmp++)
      {
         if (*tmp == ',')
         {
            ctx->nbCounters++;
         }
      }
      ctx->counterNames = malloc(ctx->nbCounters * sizeof(*ctx->counterNames));
      ctx->pfmFds = malloc(ctx->nbCounters * sizeof(*ctx->pfmFds));
      ctx->startVal = malloc(ctx->nbCounters * sizeof(*ctx->startVal));

      // get all the counter names
      for (i = 0, tmp = buf;
           (tok = strtok_r(tmp, ",", &tokintern)) != NULL;
           i++, tmp = NULL)
      {
         ctx->counterNames[i] = strdup(tok);
         ctx->pfmFds[i] = 0;
         ctx->startVal[i] = 0;
      }

      free(buf);
   }
   else if (!strcmp(name, "core"))
   {
      ctx->core = strtoul(value, NULL, 10);
   }
   else if (!strcmp(name, "mergeStr"))
   {
      ctx->mergeExp = build_exp(value);
   }

   // no error
   return 1;
}


/**
 * @brief Called to initialize the library.
 *
 * @return a handler for this specific instance of the library.
 */
extern void *evaluationInit (int pid)
{
   unsigned int i;
   int tmp;
	char *INI_FILE = NULL;

   context *ctx = malloc(sizeof(*ctx));	

   // default values
   ctx->nbCounters = 0;
   ctx->counterNames = NULL;
   ctx->core = 0;
   ctx->mergeExp = NULL;

   ctx->pfmFds = NULL;
   ctx->startVal = NULL;

	//load the path to INI_FILE
	INI_FILE = getenv("INI_CYCLES_LIB");
   // parse a configuration file if any
   ini_parse(INI_FILE, iniHandler, ctx);

   // initialize the libpfm library

   // libpfm init
   if ((tmp = pfm_initialize ()) != PFM_SUCCESS)
   {
      fprintf(stderr, "Failed to initialize libpfm: %s\n", pfm_strerror(tmp));
	   free(ctx);
	   return NULL;
   }

   // initialize counters
   for (i = 0; i < ctx->nbCounters; i++)
   {
      pfm_perf_encode_arg_t arg;
      struct perf_event_attr attr;

      // initialize the structure
      memset(&attr, 0, sizeof(attr));
      memset(&arg, 0, sizeof(arg));
      arg.attr = &attr;
      arg.fstr = NULL;
      arg.size = sizeof(pfm_perf_encode_arg_t);

      // encode the counter
      tmp = pfm_get_os_event_encoding (ctx->counterNames [i], PFM_PLM0 | PFM_PLM3,
                                       PFM_OS_PERF_EVENT_EXT, &arg);
      if (tmp != PFM_SUCCESS)
      {
         fprintf(stderr, "Failed to get counter %s\n", ctx->counterNames[i]);
	      free(ctx);
	      return NULL;
      }

      // request scaling in case of shared counters
      arg.attr->read_format = PERF_FORMAT_TOTAL_TIME_ENABLED
                              | PERF_FORMAT_TOTAL_TIME_RUNNING;
                          
      // open the file descriptor 
      ctx->pfmFds[i] = perf_event_open (&attr, pid, -1, -1, 0);
      if (ctx->pfmFds[i] == -1)
      {
         err(1, "Cannot open counter %s", ctx->counterNames[i]);
         free(ctx);
	      return NULL;
      }

      // Activate the counter
      if (ioctl (ctx->pfmFds[i], PERF_EVENT_IOC_ENABLE, 0))
      {
         fprintf(stderr, "Cannot enable event %s\n", ctx->counterNames[i]);
	      free(ctx);
	      return NULL;
      }
   }

   return ctx;
}

/**
 * @brief Freeing memory, closing stuff, ...
 */
extern int evaluationClose (void *data)
{
   unsigned int i;
   context *ctx = (context *) data;
   
   if (ctx == NULL)
   {
      return 1;
   }
   
   // close all counters
   for (i = 0; i < ctx->nbCounters; i++)
   {
      close(ctx->pfmFds[i]);
   }

   // close pfm
   pfm_terminate ();

   // free allocated memory
   if (ctx->counterNames != NULL)
   {
      for (i = 0; i < ctx->nbCounters; i++)
      {
         free(ctx->counterNames[i]);
      }
      free(ctx->counterNames);
   }
   
   if (ctx->startVal != NULL)
   {
      free(ctx->startVal);
   }
   
   if (ctx->pfmFds != NULL)
   {
      free(ctx->pfmFds);
   }

   if (ctx->mergeExp != NULL)
   {
      free_exp(ctx->mergeExp);
   }
   
   free(ctx);

   return 0;
}

extern double evaluationStart (void *data)
{
   context *ctx = (context *) data;
   unsigned int i;
   int tmp;
   
   if (ctx == NULL)
   {
      return 0.;
   }

   // read initial counter value
   for (i = 0; i < ctx->nbCounters; i++)
   {
      uint64_t buf [3];

      tmp = read (ctx->pfmFds[i], buf, sizeof (buf));
      if (tmp != sizeof (buf))
      {
         fprintf(stderr, "Failed to read counter %s\n", ctx->counterNames[i]);
      }

      // handle scaling to allow PMU sharing
      ctx->startVal[i] = (uint64_t)((double) buf [0] * buf [1] / buf [2]);
   }

   return 0;
}

extern double evaluationStop (void *data)
{
   context *ctx = (context *) data;
   unsigned int i;
   int tmp;
   uint64_t *values = malloc(ctx->nbCounters * sizeof(*values));
   
   if (ctx == NULL)
   {
      return 0.;
   }

   // just in case...
   if (ctx->nbCounters == 0)
   {
      return 0;
   }

   // read values
   for (i = 0; i < ctx->nbCounters; i++)
   {
      uint64_t buf [3];
      uint64_t value;

      tmp = read (ctx->pfmFds[i], buf, sizeof (buf));
      if (tmp != sizeof (buf))
      {
         fprintf(stderr, "Failed to read counter %s\n", ctx->counterNames[i]);
      }

      // handle scaling to allow PMU sharing
      value = (uint64_t)((double) buf [0] * buf [1] / buf [2]);
      if (ctx->startVal[i] <= value)
      {
         values [i] = value - ctx->startVal[i];
      }
      else  // overflow
      {
         values [i] = 0xFFFFFFFFFFFFFFFFUL - ctx->startVal[i] + value;
      }
   }

   // merge the counters
   return eval_expr(ctx->mergeExp, values);
}

