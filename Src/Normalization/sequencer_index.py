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
#print "Output : " + outputFileName

energyFileName = dirName + "/energy.utopik"
energyFile = open(energyFileName,"w")

ipcFileName = dirName + "/ipc.utopik"
ipcFile = open(ipcFileName,"w")

cycleFileName = dirName + "/cycles.utopik"
cycleFile = open(cycleFileName,"w")

timeFileName = dirName + "/time.utopik"
timeFile = open(timeFileName,"w")

# Read
totalEnergy = 0
totalCycles = 0
for i in range(size):
   score = 47827429742970;
   energy = 47827429742970;
   cycle = 47827429742970;
   ipc = 47827429742970;
   j = 0
   bestIndex = 0
   
   # Browse the map in order of keys
   keys = sorted(inFilesData.keys())
   for freq in keys:
      if i < len(inFilesData[freq]):
         inScore = float(inFilesData[freq][i][0])
         
         # Test to get the lowest score
         if score > inScore:
            score = inScore
            
            cycle = float(inFilesData[freq][i][1])
            energy = float(inFilesData[freq][i][2])
            ipc = float(inFilesData[freq][i][3])
            time = float(inFilesData[freq][i][4])
	    bestIndex = j
            
      j+=1
            
   totalEnergy+=energy
   totalCycles+=cycle

   outFile.write(str(bestIndex) + "\n")
   energyFile.write(str(energy) + "\n")
   cycleFile.write(str(cycle) + "\n")
   ipcFile.write(str(ipc) + "\n")
   timeFile.write(str(time) + "\n")
   
outFile.close()
energyFile.close()
ipcFile.close()
cycleFile.close()
timeFile.close()

energyfilePath = dirName + "/finalResults.csv"
outputEnergyFile = open(energyfilePath,"w")
outputEnergyFile.write(str(totalEnergy)+"\n")
outputEnergyFile.close()
#print "Total energy {0}".format(totalEnergy)
#print "Total cycles {0}".format(totalCycles)

