#!/usr/bin/env python

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


# Script to parse CSV file coming from uthopique and create equivalent files
# mapped on the maximum amount of instructions
# For this, all instructions values are transposed
import sys, os

if len(sys.argv) < 2:
   print './stabilization.py INPUTS'
   sys.exit()

# Go through all the list, without taking the program name and the output
counter=0
values = {}

for fileName in sys.argv[1:]:
   if not os.path.exists(fileName):
      print 'Fail to open \'{0}\''.format(fileName)
      continue
      
   # Get the name for output file
   shortFileName, fileExtension = os.path.splitext(fileName)
   outputFileName = shortFileName + "_stabilized" + fileExtension
   
   values[outputFileName] = []
   
   # Read file
   inFile = open(fileName)
   dataFile = inFile.readlines()
   inFile.close()
   
   # But data line by line in our table
   for line in dataFile:
      # Remove ending '\n'
      line = line.rstrip()
      # Test for empty string (we do nothing)
      if len(line) == 0:
         continue
         
      valuesStr = line.split(",")
      try:
         time = float(valuesStr[0])
         instructions = int(valuesStr[1])
         cycles = int(valuesStr[2])
         energy = float(valuesStr[3])
         values[outputFileName].append((instructions,cycles,energy,time))
      except ValueError:
         # print "Input file has invalid data"
         # In this case we are just copying the line
         # print line
         continue

# From here, all files are read and values ready to use

# Research of the maximum number of instructions for all mesures
maxNbInstructions = 0
for key , valueList in values.iteritems():
   lastValue = valueList[len(valueList)-1][0]
   if lastValue > maxNbInstructions:
      maxNbInstructions = lastValue
      
#print "Max instructions number : {0}".format(maxNbInstructions)

# Now we write the values that get remmaped
for key , valueList in values.iteritems():
   outFile = open(key,"w")
   
   # max instructions for this set
   localMaxInstructions = valueList[len(valueList)-1][0]
   ratio = maxNbInstructions / float(localMaxInstructions);
   
   for i in range(len(valueList)):
      stepRatio = 1 + (((ratio-1) / len(valueList)) * (i+1))
      #print "{0}".format(stepRatio)
      
      realInstructions = valueList[i][0]
      transposedInstructions = valueList[i][0]*stepRatio
      instructions = transposedInstructions
      cycle = valueList[i][1]
      energy = valueList[i][2]
      time = valueList[i][3]
      # Linear extrapolation
#      if i == 0:
         
#         interpRatio = transposedInstructions / realInstructions
         
#         cycle = cycle * interpRatio;
#         energy = energy * interpRatio;
         
#      else:
         
#         interpRatio = (transposedInstructions - valueList[i-1][0]) / float(realInstructions - valueList[i-1][0])
         # interpRatio = (realInstructions * (1+(ratio-1)/len(valueList)) - valueList[i-1][0]) / float(realInstructions - valueList[i-1][0])
#         print "Interp {0}".format(interpRatio)
         
         # This is not interpolation, but just following the same change that 
#         cycle = cycle * stepRatio
#         energy = energy * stepRatio
         # cycle = cycle + interpRatio * (cycle-valueList[i-1][1]);
         #energy = energy + interpRatio * (energy-valueList[i-1][2]);
      
      outFile.write("{0},{1},{2},{3}\n".format(int(instructions),int(cycle),energy,time))
   
   outFile.close()
   
