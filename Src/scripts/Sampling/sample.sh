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
DEBUG=$4

TESTNAMETMP=`echo $TEST | cut -d" " -f1`
TESTNAME=`basename $TESTNAMETMP`

echo ""
echo "--------- Sampling phase ----------"

# Force the computer to have the lowest frequency
echo -n "  Setting governor : "
$UTILS_SCRIPTS/changeGovernor.sh userspace

OUTPUTFOLDER=$OUTPUT/$TESTNAME

echo "  Creating the output folder : $OUTPUTFOLDER "

mkdir -p $OUTPUT/$TESTNAME
i=0;
for freq in `$UTILS_SCRIPTS/listFrequencies.sh`
do
	let i++
	echo -n "  Sampling [ ${i}/`$UTILS_SCRIPTS/nbFreq.sh` ] Current Frequency : "
	$UTILS_SCRIPTS/changeFrequency.sh $freq

	trap "echo \" ... SIGINT trapped, quitting with status 1\";  rm -f output.csv ; exit 1" SIGINT
	trap "echo \" ... SIGTERM trapped, quitting with status 1\"; rm -f output.csv ; exit 1" SIGTERM
	trap "echo \" ... SIGKILL trapped, quitting with status 1\"; rm -f output.csv ; exit 1" SIGKILL

	if [ "$TYPE" = "SEQ" ]
	then
		if [ "$DEBUG" = "" ]
		then
			LD_PRELOAD=$PROFILING_LIB $TEST > /dev/null 2> /dev/null
		else
			LD_PRELOAD=$PROFILING_LIB $TEST
		fi
	elif [ "$TYPE" = "OMP" ]
	then
		source $CONFIG_FILES/openmp.cfg
		if [ "$DEBUG" = "" ]
		then
			LD_PRELOAD=$PROFILING_LIB $TEST > /dev/null 2> /dev/null
		else
			LD_PRELOAD=$PROFILING_LIB $TEST
		fi
	else
		source $CONFIG_FILES/mpi.cfg
		mpdboot
		if [ "$DEBUG" = "" ]
		then
			LD_PRELOAD=$PROFILING_LIB mpirun -n $EXPORTED_NUM_PROC $TEST > /dev/null 2> /dev/null
		else
			LD_PRELOAD=$PROFILING_LIB mpirun -n $EXPORTED_NUM_PROC $TEST
		fi
		mpdallexit 2> /dev/null
	fi
	
	if [ $? -ne 0 ]
	then
		exit 1
	fi
	echo ""
	mv output.csv $OUTPUT/$TESTNAME/${TESTNAME}_${freq}.csv

done

echo "-----------------------------------"
