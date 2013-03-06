GCC=gcc

# Compilation variables

# can be either'OMP' or 'MPI' or 'SEQ'
# be careful with extra whitespace!
ifndef METHOD_NAME
   METHOD_NAME=SEQ
endif



# Checks to know if the variable is present
ifndef METHOD_NAME
   $(error METHOD_NAME is undefined)
endif

# Checks to set the right compilation rules depending of the method wanted
# VAR_OK is just to know if one of the method is recognised, if not, it will send a message and exit
ifeq ($(METHOD_NAME),OMP)
	FCT_REPLACE_OBJ := obj/main_replace.o
   VAR_OK = 1
endif
ifeq ($(METHOD_NAME),SEQ)
	FCT_REPLACE_OBJ := obj/main_seq_replace.o
   VAR_OK = 1
endif
ifeq ($(METHOD_NAME),MPI)
	FCT_REPLACE_OBJ := obj/mpi_replace.o
   LDFLAGS+=`pkg-config --libs-only-l mpich2-c 2> /dev/null` `pkg-config --libs-only-l ompi-c 2> /dev/null`
	CFLAGS+=`pkg-config --cflags-only-I mpich2-c 2> /dev/null` `pkg-config --cflags-only-I ompi-c 2> /dev/null`
	GCC=mpicc
   VAR_OK = 1
endif

ifndef VAR_OK
   $(error METHOD_NAME variable does not have a correct value)
endif


INCLUDES+= -I. -IfctReplacement/Include -Iutils/Include
CFLAGS+= -Wall -Wextra -Werror -O3 -fno-inline -m64 -msse3 -fPIC -std=gnu99 $(INCLUDES)
LDFLAGS+= -lpthread -ldl


UTILS_OBJ := obj/utils.o obj/CPUUtils.o obj/ExtLibTool.o obj/ExtLibTool2.o

SAMPLER_OBJ := $(FCT_REPLACE_OBJ) obj/libUtoPeak_instru.o $(UTILS_OBJ)
EVAL_OBJ := $(FCT_REPLACE_OBJ) obj/libUtoPeak_eval.o $(UTILS_OBJ)


all: libUtoPeak_instru.so libUtoPeak_eval.so
debug: DFLAGS=-DDEBUG
debug: all

libUtoPeak_instru.so:$(SAMPLER_OBJ)
	@echo "$(GCC) $^ -> $@";
	@$(GCC) -shared $^ -o $@ $(CFLAGS) $(DFLAGS) -I Sampler/Include $(LDFLAGS)
	@echo "";

obj/%.o: Sampler/Src/%.c
	@echo "$(GCC) $^ -> $@";
	@$(GCC) -fPIC $(CFLAGS) $(DFLAGS) -ISampler/Include -c $< -o $@ $(INCLUDES)

libUtoPeak_eval.so: $(EVAL_OBJ)
	@echo "$(GCC) $^ -> $@";
	@$(GCC) -shared $^ -o $@ $(CFLAGS) $(DFLAGS) $(LDFLAGS)
	@echo "";

obj/%.o: SequenceEvaluator/Src/%.c
	@echo "$(GCC) $^ -> $@";
	@$(GCC) -fPIC $(CFLAGS) $(DFLAGS) -ISequenceEvaluator/Include -c $< -o $@ $(INCLUDES)
	
$(FCT_REPLACE_OBJ):obj/%.o: fctReplacement/Src/%.c
	@echo "$(GCC) $^ -> $@";
	@$(GCC) -fPIC $(CFLAGS) $(DFLAGS) -IfctReplacement/Include -c $< -o $@ $(INCLUDES)
		
$(UTILS_OBJ):obj/%.o: utils/Src/%.c
	@echo "$(GCC) $^ -> $@";
	@$(GCC) -fPIC $(CFLAGS) $(DFLAGS) -Iutils/Include -c $< -o $@ $(INCLUDES)

clean:
	rm -f *.so obj/*.o *~ 

