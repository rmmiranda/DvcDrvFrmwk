#!/usr/bin/perl
##########################################################################
# File:         svp_cam_parser.pl
# Copyright     2009, Research In Motion
# Author:       Roger Su
# Created:      May 2009
# Description:  
# Revision history:

##########################################################################
import re

$in_file_name ='/cygdrive/c/p4/ro1/armos/common/firmware/camera/svp/svp_cam_st_987.h';		# Name the file
#$in_file_name ='/cygdrive/c/p4/ro1/platformSW/build/tool/dummy_svp_cam_st_987.h';		# Name the file
$out_file_name ='/cygdrive/c/p4/ro1/platformSW/build/tool/parsed_header.c';

open(INFILE, $in_file_name);		# Open the file
open(OUTFILE, ">$out_file_name");

@code = <INFILE>;		# Read it into an array
close(INFILE);			# Close the file

$comment_lines = 47;
$counter = 0;

foreach $line (@code){
	if ($counter++ > $comment_lines){
		#if line contains "CamBeginFuncEntry($cmd)", replace with "if( cam_func == $cmd){"
		if($line =~ /CamBeginFuncEntry\((.+)\),/i){
	#		$line =~ s/CamBeginFuncEntry\($1\),/if(cam_cmd == $1){/;
			$line =~ s/CamBeginFuncEntry\(/if(cam_cmd == /;
			$line =~ s/\),/){/;
			print $line;			
			print OUTFILE $line;			
		}
		#if line contains "CamEndFuncEntry()," replace with "}"	
		elsif ($line =~ /CamEndFuncEntry/i){
			$line =~ s/C.*/}/x;
			print $line;
			print OUTFILE $line;						
		}
		#if neither of the above, replace ")," in the line with ");"
		elsif ($line =~ /\),/){
			$line =~ s/\),/);/;
			print $line;
			print OUTFILE $line;						
		}
		elsif ($line =~ /CamVirtualPartStart,/i or $line =~ /__align\(4\) const BYTE svp_cam_st_987_data\[\] = \{/i or $line =~ /\};/ or $line =~ /#endif/){
		}	
		else{
			print $line;
			print OUTFILE $line;						
		}
	}
}
close(OUTFILE);			# Close the file	
exit;