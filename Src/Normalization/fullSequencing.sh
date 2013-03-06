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


if [ $# -eq 1 ]
then
   if [ -d $1 ]
   then
      
      ./extractor.py -f "0 1 2 3" $1/*0.csv
      mkdir -p $1/sequencing/
      mv $1/*_extract.csv $1/sequencing/
      
      for file in `ls $1/sequencing/*`
      do
         i=0
         while read line
         do
            # Let's forget the first line
            if [ $i -eq 0 ]
            then
               echo $line > ${file}_tmp
            else
               echo $line | sed -e "s/.000000,/,/ig" >> ${file}_tmp
            fi
            i=`expr $i + 1`
         done < $file
         
         mv ${file}_tmp $file
      done
	instSampleSize=`./findInstructionBlock.pl $1`
      ./sequencer.sh $instSampleSize $1/sequencing/
      export UTOPEAK_NUM_INST=${instSampleSize}
   fi
fi

