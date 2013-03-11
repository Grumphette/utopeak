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
TEST_NAME=`basename ${TEST}`
TEST_DIR=`dirname ${TEST}`

OUTPUT_PATH=$2
TYPE=$3

HERE=`pwd`

cd ${UTO_SRC_ROOT_DIR}/Normalization
source ./fullSequencing.sh ${OUTPUT_PATH}/${TEST_NAME}
cd ${HERE}

cp ${OUTPUT_PATH}/${TEST_NAME}/sequencing/sequence.utopik ${TEST_DIR}
cp ${INI_INSTRUCTION_LIB} ${TEST_DIR}
cp ${INI_CYCLES_LIB} ${TEST_DIR}

echo "--------- Evaluation phase --------"
echo "  Using : ${TEST_NAME}/sequencing/sequence.utopik"
echo -n "  Evaluation..." 
cd ${TEST_DIR}
if [ "$TYPE" = "SEQ" ] 
then
	LD_PRELOAD=${EVAL_LIB} ./${TEST_NAME}

elif [ "$TYPE" = "OMP" ]
then
	source $CONFIG_FILES/openmp.cfg
	LD_PRELOAD=${EVAL_LIB} ./${TEST_NAME}

else
	source $CONFIG_FILES/mpi.cfg 
	mpdboot
	LD_PRELOAD=${EVAL_LIB} mpirun -n $EXPORTED_NUM_PROC ./${TEST_NAME}
	mpdallexit 2> /dev/null
fi
echo "OK"

`cat tmp.csv >> ${OUTPUT_PATH}/${TEST_NAME}/sequencing/finalResults.csv ; rm tmp.csv`
echo "  Predicted : `head -n 1 ${OUTPUT_PATH}/${TEST_NAME}/sequencing/finalResults.csv` (J)"
echo "  Predicted : `head -n 1 ${OUTPUT_PATH}/${TEST_NAME}/sequencing/finalResults.csv` (J)" >> tmp.csv


echo "  Evaluated : `tail -n 1 ${OUTPUT_PATH}/${TEST_NAME}/sequencing/finalResults.csv` (J)"
echo "  Evaluated : `tail -n 1 ${OUTPUT_PATH}/${TEST_NAME}/sequencing/finalResults.csv` (J)" >> tmp.csv

# difference between evaluation and prediction computation
predicted=`head -n 1 ${OUTPUT_PATH}/${TEST_NAME}/sequencing/finalResults.csv`
evaluated=`tail -n 1 ${OUTPUT_PATH}/${TEST_NAME}/sequencing/finalResults.csv`
variation=$(echo "scale=3;($evaluated-$predicted)"| bc)
relativeVariation=$(echo "scale=3;($variation/$evaluated)"| bc)
percentage=$(echo "scale=5;($relativeVariation*100)"| bc | xargs printf "%0.2f")

echo "  Variation : ${percentage} %"
echo "  Variation : ${percentage} %" >> tmp.csv

#updating the finalResults.csv with the formated output
`cat tmp.csv > ${OUTPUT_PATH}/${TEST_NAME}/sequencing/finalResults.csv; rm tmp.csv `
echo "  Result extracted from : ${TEST_NAME}/sequencing/finalResults.csv"
cd ${HERE}

rm ${TEST_DIR}/sequence.utopik
rm ${TEST_DIR}/ccyclescounters.ini
rm ${TEST_DIR}/pfmcounters2.ini

echo "-----------------------------------"
