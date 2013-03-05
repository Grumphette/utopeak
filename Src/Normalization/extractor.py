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


import sys, os

delimiter=","
fields=[]
filenames=[]

def usage():
   print "extractor.py [-d/--delim DELIMITER] [-f/--field \"1 2 ...\"] FILES"
   exit(0)

if len(sys.argv) <= 1:
   usage()

arg = 1;
while arg < len(sys.argv):
   if sys.argv[arg] == "-d" or sys.argv[arg] == "--delim":
      if arg+1 < len(sys.argv):
         delimiter=sys.argv[arg+1]
         arg+=1
      else:
         print "Missing arguments after {0}".format(sys.argv[arg])
         exit(0)
      
   elif sys.argv[arg] == "-f" or sys.argv[arg] == "--fields":
      if arg+1 < len(sys.argv):
         for field in sys.argv[arg+1].split(" "):
            try:
               fields.append(int(field))
            except ValueError:
               print "Fields can only be integers"
               exit(0)
         arg+=1
      else:
         print "Missing arguments after {0}".format(sys.argv[arg])
         exit(0)
   elif sys.argv[arg] == "-h" or sys.argv[arg] == "--help":
      usage()
      sys.exit(0)
         
   else:
      filenames = sys.argv[arg:]
      break;
      
   arg+=1

#print "Delimiter : '" + delimiter + "'"
#print "Fields : "
#for i in fields:
#   print "\t{0}".format(i)
   
#print "Files : "
#for i in filenames:
#   print "\t{0}".format(i)
   
# Go through all the list, without taking the program name and the output
for fileName in filenames:
   if not os.path.exists(fileName):
      print 'Fail to open \'{0}\''.format(fileName)
      continue
      
   # Get the name for output file
   shortFileName, fileExtension = os.path.splitext(fileName)
   outputFileName = shortFileName + "_extract" + fileExtension
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
      
      values = []
      valuesStr = line.split(delimiter)
      i = 0
      for value in valuesStr:
         if len(fields) == 0 or fields.count(i) != 0: # If nothing in fields, or if the value is in it
            values.append(value)
         i+=1
         
      # Writing the extracted fields
      for value in values:
         outFile.write(value + ",");
      outFile.write("\n");
   
   outFile.close()

