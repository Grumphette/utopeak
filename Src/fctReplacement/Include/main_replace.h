/*
 * UtoPeak - Automatic Estimation of DVFS Potential
 * Copyright (C) 2013 Universite de Versailles
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef H_MAIN_REPLACE
#define H_MAIN_REPLACE

typedef int (*hijack_main_t)(int argc, char** argv, char** env);

typedef void (*hijack_lsm_t)(void);

typedef int (*hijack_libc_start_main_t)( hijack_main_t main, int argc, char** ubp_av, hijack_lsm_t init, hijack_lsm_t fini, hijack_lsm_t rtld_fini,void* stack_end );

#endif

