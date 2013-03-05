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


# Script to read multiple files with score and to get the best frequency
# The frequency is simply outputed in a new file
import sys, os

if len(sys.argv) < 2:
   print './sequencer.py INPUTS'
   sys.exit()


# Go through all the list, without taking the program name and the output
inFilesData = {}
for fileName in sys.argv[1:]:
   if not os.path.exists(fileName):
      print 'Fail to open \'{0}\''.format(fileName)
      continue
      
   # Parse freq from the file name
   fileNameParts = fileName.split("_")
   freq = 0
   for part in fileNameParts:
      try:
         freq = int(part)
         break
      except:
         continue
         
   print "Freq found : {0}".format(freq)
   
   inFilesData[freq]= {}
   
   inFile = open(fileName)
   dataFile = inFile.readlines()
   
   i = 0
   for line in dataFile: 
      inFilesData[freq][i] = line.rstrip().split(",")
      i+=1
   inFile.close()

# Check if the size of all files is the same
size = -1
for data in inFilesData.values():
   if size == -1:
      size = len(data)
   elif size != len(data):
      print "File of the input files differs"
      #exit(-1)

dirName = os.path.dirname(fileName)
outputFileName = dirName + "/sequence.utopik"
outFile = open(outputFileName,"w")
print "Output : " + outputFileName

energyFileName = dirName + "/energy.utopik"
energyFile = open(energyFileName,"w")

ipcFileName = dirName + "/ipc.utopik"
ipcFile = open(ipcFileName,"w")

cycleFileName = dirName + "/cycles.utopik"
cycleFile = open(cycleFileName,"w")

# Read
totalEnergy = 0
totalCycles = 0
for i in range(size):
   score = 47827429742970;
   energy = 47827429742970;
   cycle = 47827429742970;
   ipc = 47827429742970;
   bestFreq = 0;
   for freq, data in inFilesData.iteritems():
      if i < len(data):
         inScore = float(data[i][0])
         
         # Test to get the lowest score
         if score > inScore:
            score = inScore
            cycle = float(data[i][1])
            energy = float(data[i][2])
            ipc = float(data[i][3])
            bestFreq = freq
            
   totalEnergy+=energy
   totalCycles+=cycle

   outFile.write(str(bestFreq) + "\n")
   cycleFile.write(str(cycle) + "\n")
   ipcFile.write(str(ipc) + "\n")
   
outFile.close()
ipcFile.close()
energyFile.close()
cycleFile.close()

print "Total energy {0}".format(totalEnergy)
print "Total cycles {0}".format(totalCycles)




