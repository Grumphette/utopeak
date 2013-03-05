#!/usr/bin/perl

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


use strict;

my $ARGC = $#ARGV+1;
if( $ARGC != 1)
{
		  print "./findInstructionBlock.pl result_folder\n";
		  die;
}

my $folder = $ARGV[0];

# find the min frequency on the machine
my $freqList=`cat /sys/devices/system/cpu/cpu0/cpufreq/scaling_available_frequencies`;
chomp $freqList;
my @freqs = split(' ',$freqList);
my $fmin=$freqs[$#freqs];

# find the mini freq file in the result folder
my $minFreqFiles = `ls -r $folder | grep $fmin`;
my @minFreqFile = split("\n",$minFreqFiles);
#in case of several match, the correct frequency file the first file given by UtoPeak
chomp @minFreqFile[0];

#open the min freq file and find the lowest instruction step within all the measured samples
my $fullpath = ${folder}."/".${minFreqFile}[0];

open FILE, $fullpath or die $!;
my $minInstStep = 100000000000;
my $oldLineInfo = 0;
my $nbline = 0;
my $numberLines = `wc -l $fullpath`;
while(my $line = <FILE>)
{
	# skip the first line which is a text line, and skip the last one which can be sometime non representative
	if($nbline != 0 and $nbline < $numberLines)
	{
   	my @lineInfo = split(',',$line);

		if(($lineInfo[1]-$oldLineInfo) < $minInstStep and $lineInfo[1]-$oldLineInfo != 0)
		{
			$minInstStep = ($lineInfo[1]-$oldLineInfo); 
		}
		$oldLineInfo = $lineInfo[1];
	}
	
	$nbline++;
	 	
}
print $minInstStep."\n";
`export UTOPEAK_NUM_INST=$minInstStep`;
