#!/bin/bash

#
# UtoPeak - Automatic Estimation of DVFS Potential
# Copyright (C) 2013 Universite de Versailles
# 
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#


TEST=$1
OUTPUT=$2
TYPE=$3

TESTNAME=`basename $TEST`

# Force the computer to have the lowest frequency
$UTILS_SCRIPTS/changeGovernor.sh userspace

mkdir -p $OUTPUT/$TESTNAME

for freq in `$UTILS_SCRIPTS/listFrequencies.sh`
do
	echo -n "Using frequency : "
	$UTILS_SCRIPTS/changeFrequency.sh $freq


	if [ "$TYPE" = "SEQ" ]
	then
		LD_PRELOAD=$PROFILING_LIB $TEST

	elif [ "$TYPE" = "OMP" ]
	then
		source $CONFIG_FILES/openmp.cfg
		LD_PRELOAD=$PROFILING_LIB $TEST

	else
		source $CONFIG_FILES/mpi.cfg	
		LD_PRELOAD=$PROFILING_LIB mpirun -n $EXPORTED_NUM_PROC $TEST

	fi
	
	if [ $? -ne 0 ]
	then
		exit 1
	fi
	mv output.csv $OUTPUT/$TESTNAME/${TESTNAME}_${freq}.csv

done
