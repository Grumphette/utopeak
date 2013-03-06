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



# Script to parse CSV file coming from uthopique and create a file with the score for each time
# The problem is that we are dependant of the input file (we are just guessing that the N elements are such)
import sys, os

if len(sys.argv) < 2:
   print './scoring.py INPUTS'
   sys.exit()

# Go through all the list, without taking the program name and the output
for fileName in sys.argv[1:]:
   if not os.path.exists(fileName):
      print 'Fail to open \'{0}\''.format(fileName)
      continue
      
   # Get the name for output file
   shortFileName, fileExtension = os.path.splitext(fileName)
   outputFileName = shortFileName + "_score" + fileExtension
   #print 'Output file : {0}'.format(outputFileName)
   
   inFile = open(fileName)
   dataFile = inFile.readlines()
   inFile.close()
   
   outFile = open(outputFileName,"w")
   
   for line in dataFile:
      # Remove ending '\n'
      line = line.rstrip()
      # Test for empty string (we do nothing)
      if len(line) == 0:
         continue
      
      valuesStr = line.split(",")
      try:
         instructions = float(valuesStr[0])
         cycles = float(valuesStr[1])
         energy = float(valuesStr[2])
         ipc = float(valuesStr[3])
	 time = float(valuesStr[4])

         # Here, I have to put a wonderful calculation to have a score from the values read
         #score = (1000000000/cycles) / energy
         # score = 1 / energy
         score = energy

         outFile.write(str(score)+","+str(cycles)+","+str(energy)+","+str(ipc)+","+str(time)+"\n");
      except ValueError:
         print "Input file as invalid data"
         # In this case we are just copying the line
         print line
      
   outFile.close()

