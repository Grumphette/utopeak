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


if [ $# -eq 2 ]
then
   if [ -d $2 ]
   then
		echo -n "  Stabilization ..."
      ./stabilization.py $2/*.csv
		echo "OK"
		echo -n "  Normalization ..."
      ./normalise.py $1 $2/*_stabilized.csv
		echo "OK"
		#echo -n "  Scoring ..."
      #./scoring.py $2/*_normalised.csv
		#echo "OK"
		echo -n "  Frequency sequence building ..."
      ./sequencer_index.py $2/*_normalised.csv
		echo "OK"
#      rm $1/*_normalised.csv
#      rm $1/*_normalised_score.csv
   else
      echo "Please give a folder to read"
   fi
else
   echo "USAGE: ./sequencer.sh inst_smpl_size  name_of_folder_with_results"
fi


