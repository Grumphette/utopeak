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



# Script to parse CSV file coming from uthopique and create a file with normalised results
# To do this we have to interpolate values between the two read values
import sys, os

# Instruction step
#INST_STEP = 568000000

if len(sys.argv) < 3:
   print './normalise.py INPUTS'
   sys.exit()

INST_STEP = int(sys.argv[1])
#print 'INST_STEP : {0}'.format(INST_STEP)

# Go through all the list, without taking the program name and the output
for fileName in sys.argv[2:]:
   if not os.path.exists(fileName):
      print 'Fail to open \'{0}\''.format(fileName)
      continue
      
   # Get the name for output file
   shortFileName, fileExtension = os.path.splitext(fileName)
   outputFileName = shortFileName + "_normalised" + fileExtension
   #print 'Output file : {0}'.format(outputFileName)
   
   inFile = open(fileName)
   dataFile = inFile.readlines()
   inFile.close()
   
   outFile = open(outputFileName,"w")
   
   values = []
   for line in dataFile:
      # Remove ending '\n'
      line = line.rstrip()
      # Test for empty string (we do nothing)
      if len(line) == 0:
         continue
      
      valuesStr = line.split(",")
      try:
         instructions = int(valuesStr[0])
         cycles = int(valuesStr[1])
         energy = float(valuesStr[2])
	 time = float(valuesStr[3])
         values.append((instructions,cycles,energy,time))
      except ValueError:
         print "Input file has invalid data"
         # In this case we are just copying the line
         print line
   
   # Here, we have to do the calculation.
   nbTotalInstructions = 0
   nbTotalCycles = 0
   nbTotalEnergy = 0
   nbTotalTime = 0
   for i in range(len(values)):
      #nbTotalInstructions = nbTotalInstructions + values[i][0]
      #nbTotalCycles = nbTotalCycles + values[i][1]
      #nbTotalEnergy = nbTotalEnergy + values[i][2]
      nbTotalInstructions = values[i][0]
      nbTotalCycles = values[i][1]
      nbTotalEnergy = values[i][2]
      nbTotalTime = values[i][3]
      #values[i] = (nbTotalInstructions,nbTotalCycles,nbTotalEnergy)
      values[i] = (values[i][0],values[i][1],values[i][2],values[i][3])
   
   #print "Test executed : {0} instructions".format(nbTotalInstructions)
   i = 0
   
   results = []
   
   valuesLenght = len(values)
   for intStep in range(INST_STEP, nbTotalInstructions, INST_STEP):
      # print "Instructions : {0}".format(intStep)
      if intStep < values[0][0]:
         # First step, when we are before the value array
         
         # interpolation when we start (beetween zero to the first value)
         quot = intStep / float(values[0][0])
         cycles = values[0][1] * quot;
         energy = values[0][2] * quot;
	 time = values[0][3] * quot;
         
         results.append((intStep,cycles,energy,time))
         
         continue
         
      # Pass all the values that are not use
      while i < valuesLenght-1 and values[i+1][0] < intStep:
         # We are in the array
         #interpolation has to be done
         # Checks if the present value is still good
         i = i + 1
      
      if i < valuesLenght-1:
         # interpolation
         quot = (intStep - values[i][0]) / float(values[i+1][0] - values[i][0])
         cycles = values[i][1] + (values[i+1][1] - values[i][1]) * quot;
         energy = values[i][2] + (values[i+1][2] - values[i][2]) * quot;
         time = values[i][3] + (values[i+1][3] - values[i][3]) * quot;

         results.append((intStep,cycles,energy,time))
         
      else:
         # We are outside, so we are just printing the last values
         results.append((intStep,values[i][1],values[i][2],values[i][3]))
         
   # Adds very last instructions number
   if i < valuesLenght:
      lastIndex = len(values)-1
      results.append((nbTotalInstructions,values[lastIndex][1],values[lastIndex][2],values[lastIndex][3]))
      
   outFile.write("{0},{1},{2},{3},{4}\n".format(results[0][0],results[0][1],results[0][2],results[0][0]/results[0][1],results[0][3]))
   for i in range(1,len(results)):
      outFile.write("{0},{1},{2},{3},{4}\n".format(results[i][0],results[i][1]-results[i-1][1],results[i][2]-results[i-1][2],INST_STEP/(results[i][1]-results[i-1][1]),results[i][3]-results[i-1][3]))
      
         
   outFile.close()
