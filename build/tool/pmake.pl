#!/usr/bin/perl
##########################################################################
# File:         pmake.pl
# Copyright     2008, Research In Motion
# Author:       Bin Lu
# Created:      Nov 2008
# Description:  The main perl script to call other makefiles and scripts 
#				in order to build Platform software.  
# Revision history:
#               0.2   Bin Lu    2009/01/06  support of nessus build
#               0.1   Bin Lu    2008/11/28  script draft   
##########################################################################

#packages to include
#use Switch;
use Cwd;
use Cwd 'abs_path';

#If WORKSPACE_ROOT environment variable is unset.
if ( $ENV{'WORKSPACE_ROOT'} eq "" )
{
	$ENV{'WORKSPACE_ROOT'} = getcwd;
	$ENV{'WORKSPACE_ROOT'} =~ s/\/platformSW.*//;
}

# Perforce sync 
$P4_SYNC    = "$ENV{'WORKSPACE_ROOT'}";

#Constants
$PMAKE_VER = 0.2;
$allArgs = join(" ", @ARGV);

# Directories
# Environment variable WORKSPACE_ROOT is required
$DIR_PLATFORM     = "$ENV{'WORKSPACE_ROOT'}/platformSW";
$DIR_ARMOS_ROOT   = "$ENV{'WORKSPACE_ROOT'}/armos";
$DIR_ARMOS_BR     = "$ENV{'WORKSPACE_ROOT'}/armos/bootrom";
$DIR_ARMOS_OS     = "$ENV{'WORKSPACE_ROOT'}/armos/nessus";
$DIR_ARMOS_CMN    = "$ENV{'WORKSPACE_ROOT'}/armos/common";
#$DIR_PLATFORM_OVERLAY = "platformSW/build/overlay";

# Tools
#Use cygwins build in perl if possible.
if ( -e "/usr/bin/perl") {$PERL = "/usr/bin/perl"; }
else { $PERL  = "$DIR_ARMOS_CMN/tools/pl.exe -I $DIR_ARMOS_CMN/tools/perl/lib"; }
$MAKE           = "$DIR_ARMOS_CMN/tools/make.exe";
$FIND			= "$DIR_ARMOS_CMN/tools/find.exe";
$CAT			= "$DIR_ARMOS_CMN/tools/cat.exe";
$COPY			= "$DIR_ARMOS_CMN/tools/cp.exe";
$RM				= "$DIR_ARMOS_CMN/tools/rm.exe";
$MKDIR			= "$DIR_ARMOS_CMN/tools/mkdir.exe";
$XARGS			= "$DIR_ARMOS_CMN/tools/xargs.exe";
$BUGBUILD		= "$DIR_ARMOS_CMN/tools/bugbuild.exe";
$TOOL_H2MK      = "$DIR_ARMOS_CMN/tools/h_to_mak2.exe";
$TOOL_H2SETA    = "$PERL $DIR_ARMOS_CMN/tools/h_to_seta.pl";
$TOOL_H2D       = "$PERL $DIR_ARMOS_CMN/tools/h_to_d.pl";
$MAKEDEP		= "$DIR_ARMOS_CMN/tools/makedep.pl";
$SED			= "$DIR_ARMOS_CMN/tools/sed.exe";
$GREP			= "$DIR_ARMOS_CMN/tools/grep.exe";

# Global variables
$MAKE_FLAG      = "";

print "\n";

#------------------------------------------------------------------------------
#Check the arguments for the existence of each flag option
#  and do specific tasks for the given flags
if($#ARGV lt 0){
    printHelp();
    exit;
}

while($#ARGV ge 0)
{
    $arg = shift @ARGV;
    
    if($arg eq "-h")        # "-h" print help    
    {
        printHelp();
        exit;
    }
    elsif(checkOneFlag($arg, "-p"))     # "-p=" Platform
    {	
        $ENV{'PLATFORM'} = $value;
    }
    elsif(lc($arg) eq "-lib")            # "-lib" Build the Nessus lib only
    {	
		$ENV{'NESSUSLIB'} = 1;
    }    
    elsif(checkOneFlag($arg, " -cf"))     # "-cf=" compiler/linker flags
    {
        $ENV{'BLD_FLAGS'} .= "$value";
    }
    elsif(lc($arg) eq "-clean" || lc($arg) eq "-cl")  # "-clean" clean up target
    {
        $ACTION = "CLEAN";
    }        
    elsif(lc($arg) eq "-d")     # "-d" debug flag
    {	
        $ENV{'BLD_FLAGS'} .= " -g -O0";        
        $ENV{'DEBUG'} = "_debug";
        $ENV{'EXTRAINFO'} = "true";
    }    
    elsif(checkOneFlag($arg, "-m"))     # "-m=" debug flag
    {	
        $MAKE_FLAG = $value;        
    }
    else
    {
        # Target should be the last argument
        if($#ARGV eq "-1")
        {
            # Update make option 
            if($MAKE_FLAG !~ /-d/ && $MAKE_FLAG !~ /--debug/)
            {
                $MAKE_FLAG .= " -s ";
            }            
            
            checkTarget($arg);
        }
        else
        {        
            print "\nERROR: Invalid option \"$arg\" in \"$allArgs\"\n";
            exit;
        }
    }
}

#=============================================================================
# Sub-routines

#------------------------------------------------------------------------------
# Process different build target
#------------------------------------------------------------------------------
sub checkOneFlag
{
	my $FLAG_PREFIX = "-";
	my $FLAG_SUFFIX = "=";

	#Extract the flag and its value (if it exists) from the argument
	# Examples: -flag ; -flag=value ; -flag="value1,value2"
	my $ifMatch = ($_[0] =~ /^$_[1]\=(.+)/);
	$value = $1;
    	
	return $ifMatch;
}

#------------------------------------------------------------------------------
# Handle different build targets
#------------------------------------------------------------------------------
sub checkTarget
{
    ($target, $subTarget) = ($_[0] =~ /(.*)/);
    
    if(lc($target) eq "sync") {
        #buildSync();
		print "\n Sync Disabled. Only required for Library, which can be done manually for now. \n";
        return;
    }
    if(lc($target) eq "diff") {
        #$DiffOnly = 1;
        #syncOverlay("$DIR_PLATFORM_OVERLAY");
		print "\n Diff Disabled. \n";
        return;
    }    
    elsif (lc($target) eq "help") {
        printHelp();
        return;
    }
    elsif (lc($target) eq "bootrom") {
        $TARGET = "bootrom";
    }
    elsif (lc($target) eq "nessus") {
        $TARGET = "nessus";    
    }
    elsif (lc($target) eq "minios") {
        $TARGET = "minios";    
    }
    elsif (lc($target) eq "framework") {
        print "\n Framework\n";
    }
    elsif (lc($target) eq "testapp") {
        print "\n testapp\n";
    }
    elsif (lc($target) eq "drivers") {
        print "\ndrivers $subTarget \n";
    }
    else {
        print "ERROR: Unknown target $target\n";
    }    
    
    buildTarget();    
}

#------------------------------------------------------------------------------
# Print help 
#------------------------------------------------------------------------------
sub printHelp
{
    print "pmake   : Platform Software Make script
Version : $PMAKE_VER
Usage   : pmake <options> [targets]
    options:    -p=[platform],          ex. -p=revelstoke
                -cf=[compiler flags],    ex. -c=\"-C\"
                -d  compiler debug flag, equivalent to -c=\"-g -O0\"
                -m  makefile debug flag
                -h  display help message
                -cl(-clean) clean up the target object folder
                -lib build nessus library only
    targets:
                sync    sync up the perforce for \\depot\main\armos
                bootrom build bootrom image
                nessus  build nessus image
Examples:
    1. TBD
 \n";
}

#------------------------------------------------------------------------------
# Display build result
#------------------------------------------------------------------------------
sub showResult
{
    $result = $_[0];
    $buildName = $_[1];
    
    print "==============================\n";
    if($result != 0)  {
        print "*** $buildName Build Failed! ***\n";
    } else {
        print "*** $buildName Build Passed! ***\n";
    }
    print "==============================\n";
}

#------------------------------------------------------------------------------
# Parse configuration file
#------------------------------------------------------------------------------
sub parseConfig
{
    $config_file = "$DIR_PLATFORM/build/config/$ENV{'PLATFORM'}$\.conf";
    open( file_hdl, $config_file) or die $!;
    
    while(<file_hdl>){
        unless(/^\s*#/) {
            if(/MODULE:\s*(\S+)\s*=\s*(\S+)/) {
                $MAK_DEFINE .= "-D$1 ";
                $MAK_MODULE .= "$2 ";
                $ENV{$1} = $2;
            } elsif (/DEFINE:\s*(\S+)/) {
                $MAK_DEFINE .= "-D$1 ";
            } elsif (/SYNC:\s*(\S+)/) {
                push(@SyncList, $1);
			} elsif (/LIBRARY:\s*(\S+)/) {
				$ENV{'BUILDLIB'} = $1;
				$ENV{'BUILDLIB'} =~ s/\//_/g;
            } elsif(/THREADS_APP:\s*(\S+)/) {
                $ENV{'THREADS_APP'} = $1;
            } elsif(/ARCH:\s*(\S+)/) {
                $ENV{'ARCH'} = $1;
            }
        }
    }
    
    if($TARGET eq "nessus")
    {
        $MAK_DEFINE .= "-DNESSUS";
        $MAK_MODULE .= " base/test";
    }
    elsif ($TARGET eq "minios")
    {
        $MAK_DEFINE .= " -DMINIOS";        
        $MAK_MODULE .= " base/rei";
        $MAK_MODULE .= " base/test";
    }
    elsif ($TARGET eq "bootrom")
    {
        $MAK_DEFINE .= "-DBOOTROM";        
    } 
    
    $MAK_MODULE .= " base/manager base/framework";
    
    $ENV{'BLD_FLAGS'} .= " $MAK_DEFINE";
    
    close(file_hdl);
}

#------------------------------------------------------------------------------
# cleanTarget
#------------------------------------------------------------------------------
sub cleanTarget
{
    $MAK_MODULE=~ s/^\s+//;
    @Modules = split(/\s+/, $MAK_MODULE);

    foreach (@Modules) {
        $MODLUE_OBJ_FOLDER = "$DIR_PLATFORM/$_/obj/$ENV{'PLATFORM'}_$TARGET";
        print ("Removing folder \"$MODLUE_OBJ_FOLDER\"\n");
        system("$RM -rf $MODLUE_OBJ_FOLDER");
    }

    print ("Removing folder \"$DIR_PLATFORM_RLS\"\n");      
    system("$RM -rf $DIR_PLATFORM_RLS");
}

#------------------------------------------------------------------------------
# buildTarget
#------------------------------------------------------------------------------
sub buildTarget
{
    $DIR_PLATFORM_RLS = "$DIR_PLATFORM/build/release/$ENV{'PLATFORM'}_$TARGET";

    if($ENV{'DEBUG'} eq "_debug") {
        
        if($TARGET eq "bootrom"){
            $ARMOS_BUILD_OPTION = "debug";
            $ARMOS_RLS_POSTFIX = $ENV{'DEBUG'};
        } elsif ($TARGET eq "nessus") {
            $ARMOS_BUILD_OPTION = "jtag -j 1";
        }
    } 
    $DIR_ARMOS_RLS = "$DIR_ARMOS_ROOT/$TARGET/obj/$ENV{'PLATFORM'}$ARMOS_RLS_POSTFIX";   

    $ENV{'BUILD_TGT'} = $TARGET;

    # make sure the platform is defined first
    if(! $ENV{'PLATFORM'}) {
        print "ERROR: Platform is not defined. Loadbuild failed!\n\n";
        printHelp();
        exit;
    }

    # Parse configuration file
    parseConfig();

    # clean the target if needed
    if($ACTION eq "CLEAN") {
        cleanTarget();               
            return;
    }        
   
    chdir $DIR_PLATFORM;
    unless(-e $DIR_PLATFORM_RLS) {
        system("$MKDIR -p $DIR_PLATFORM_RLS");
    }
    
	#GMATHER Build Dependencies for header files.
	buildDeps();
	
	# Invoke make
	print "\n***********************************************************\n";
	print "** Start building platform software ...\n**\n";
	$ENV{'MODULE_LIST'} = $MAK_MODULE;
	$result = system("$MAKE $MAKE_FLAG -f build/makefile/platform.mak");					
	
	#GMATHER Delete the dependency file as it is no longer needed.
	unlink("makefile.dep");
	
	unless($result eq 0) {
		print "ERROR: Failed to build platform software\n";
		exit;
	}         
		
	# Return unless the bootrom needs to be built or the nessus library.
	if( ($TARGET eq "nessus" || $TARGET eq "minios") && $ENV{'NESSUSLIB'} ne 1) 
	{
		print "\n**\n** Platform software successfully built! \n";
		print "\n**\n** $TARGET \n";
		print "\n**\n** $ENV{'NESSUSLIB'} \n";
		print "***********************************************************\n";
		return;
	} else 
	{
		print "\n**\n** Building the Nessus Library\n";
		print "***********************************************************\n";
	}
    
    #Create the target object folder if it doesn't exist
    unless(-e $DIR_ARMOS_RLS) {
        system("$MKDIR -p $DIR_ARMOS_RLS");
    }
    
    #Link the platform software with the nessus/bootrom
    if($TARGET eq "bootrom") {    
        # Copy the platform library to the target object folder
        system ("cd $DIR_ARMOS_RLS ; \
                 $COPY $DIR_PLATFORM_RLS/libPlatform_$TARGET.a .; \
                 $DIR_ARMOS_CMN/tools/rv22/bin/armar -x libPlatform_$TARGET.a");
    
        buildBootrom();
    } elsif($TARGET eq "nessus") {    
        # Copy the platform library to the target object folder
		# This still needs to happen if we are just interested in the library. 
        # The threads we inserted in nessus call our platform code so it needs to be
		# included. The build will fail with undefined symbols before the library is made.
        system("$COPY -f $DIR_PLATFORM_RLS/libPlatform_$TARGET.a $DIR_ARMOS_ROOT/$TARGET");
        
        buildNessus();
    } elsif($TARGET eq "minios") {    
        # Copy the platform library to the target object folder
		# This still needs to happen if we are just interested in the library. 
        # The threads we inserted in nessus call our platform code so it needs to be
		# included. The build will fail with undefined symbols before the library is made.
        system("$COPY -f $DIR_PLATFORM_RLS/libPlatform_$TARGET.a $DIR_ARMOS_ROOT/$TARGET");
        
        buildNessus();
    }
}

#------------------------------------------------------------------------------
# Target nessus
#------------------------------------------------------------------------------
sub buildNessus
{    
    print "\n***********************************************************\n";
    print "** Start building Nessus ...\n";    

    # Change to the armos/nessus directory            
    chdir $DIR_ARMOS_OS;

    # Invoke nessus make.bat
    $CMD_STR = "make.bat $ENV{'PLATFORM'} $ARMOS_BUILD_OPTION";
    print "** ($CMD_STR)\n**\n";	
    $result = system($CMD_STR);
    # Check result
    if($result eq 0) {
        # Copy the nessus librarys to the platform folder
        system("$MKDIR -p $DIR_PLATFORM/build/lib/$ENV{'PLATFORM'}_$ENV{'BUILDLIB'}");
        system("$COPY -f $DIR_ARMOS_RLS/nessus.lib $DIR_ARMOS_RLS/debug.dat $DIR_ARMOS_RLS/$ENV{'PLATFORM'}_exporttable.txt " . 
			"$DIR_ARMOS_RLS/nessus_hifi2.o " .
			"$DIR_ARMOS_RLS/nessus_msa_ldr.o " .
			"obj/$ENV{'PLATFORM'}/stubtask_radio.o " .
			"obj/$ENV{'PLATFORM'}/stub_radio.o " .
			"obj/$ENV{'PLATFORM'}/$ENV{'PLATFORM'}_radio_cpu.o " .
			"obj/$ENV{'PLATFORM'}/$ENV{'PLATFORM'}_radio_dsp.o " .
			"obj/$ENV{'PLATFORM'}/os_export_table.o " .
			"obj/$ENV{'PLATFORM'}/metrics.o " .
			"obj/$ENV{'PLATFORM'}/\*.sct " .
			"$DIR_PLATFORM/build/lib/$ENV{'PLATFORM'}_$ENV{'BUILDLIB'}");
		system("$MKDIR -p $DIR_PLATFORM/build/lib/$ENV{'PLATFORM'}_$ENV{'BUILDLIB'}/include");
        print "*** Please manually copy libraries in buildinfo.txt and libdspaudioxscale.lib ***";
		CopyNessusHeadersWithLib();
    }
        
    # Display final result
    showResult($result, "Nessus");    
}
    
#------------------------------------------------------------------------------
# Target bootrom
#------------------------------------------------------------------------------
sub buildBootrom
{
    # Setup environment variables
    $ENV{'OSAPI_ROOT'} = "../common/inc_pub";
            
    print "\n***********************************************************\n";
    print "** Start building Bootrom(bootloader) ...\n**\n";

    # Change to the armos/bootrom directory            
    chdir $DIR_ARMOS_BR;        
    
    # Generate product makefile and defines
    system("$TOOL_H2MK \/m config/$ENV{'PLATFORM'}/product.def " .
            "obj/$ENV{'PLATFORM'}$ARMOS_RLS_POSTFIX/product.mak > null");
    system("$TOOL_H2SETA config/$ENV{'PLATFORM'}/product.def > " .
            "obj/$ENV{'PLATFORM'}$ARMOS_RLS_POSTFIX/defines_asm.txt");
    system("$TOOL_H2D config/$ENV{'PLATFORM'}/product.def > " .
            "obj/$ENV{'PLATFORM'}$ARMOS_RLS_POSTFIX/defines_c.txt");
    
    # Invoke make
    $result = system("$MAKE $MAKE_FLAG");
    
    if($result eq 0)
    {
        # Copy the bootrom images to the platform folder
        system("$MKDIR -p $DIR_PLATFORM_RLS");
        system("$COPY $DIR_ARMOS_RLS/$ENV{'PLATFORM'}* $DIR_ARMOS_BR/*$ENV{'PLATFORM'}*.bin $DIR_PLATFORM_RLS");        
    }
    
    # Display final result
    showResult($result, "Bootrom");
}

#------------------------------------------------------------------------------
# Target sync
# Execute perforce sync, and update overlay files
#------------------------------------------------------------------------------
sub buildSync
{    
    # Parse configuration file
    parseConfig();
        
    foreach $sync (@SyncList) {
        print "Syncing folder \"$P4_SYNC/$sync\"\n";
        system("p4 sync $P4_SYNC/$sync");
    }
    
    opendir(DIR, ".");
    @files = grep(/\.html$/,readdir(DIR));
    closedir(DIR);

    foreach $file (@files) {
        print "$file\n";
    }
    
    $DiffOnly = 0;
    syncOverlay("$DIR_PLATFORM_OVERLAY");
}

#------------------------------------------------------------------------------
# syncOverlay
#------------------------------------------------------------------------------
sub syncOverlay
{
    my($path) = @_;
    
    chdir($P4_SYNC);

    # append a trailing / if it's not there
    $path .= '/' if($path !~ /\/$/);

    # loop through the files contained in the directory
    loop: for my $eachFile (glob($path.'*')) {
        
        #skip diff files
        next loop if($eachFile =~ /\.diff$/);

        # if the file is a directory
        if( -d $eachFile) {
            # pass the directory to the routine ( recursion )
            syncOverlay($eachFile);
        } else {
            # print the file ... tabbed for readability
            $armosFile = $eachFile;
            $armosFile =~ s/$DIR_PLATFORM_OVERLAY/armos/;
            
            if($DiffOnly == 1)
            {
                print "Diffing overlay file \"$armosFile\"\n";
                system("$CAT $eachFile\.diff; p4merge $eachFile $armosFile");
            }
            else
            {
                print "Syncing overlay file \"$armosFile\"\n";
                system("p4 sync $P4_SYNC/$armosFile");
            }            
        }
    }
}

#------------------------------------------------------------------------------
# Build Dependencies
# GMATHER Creates makefile.dep to allow modules to be rebuilt
# when their header files change. 
#------------------------------------------------------------------------------
sub buildDeps
{
	#Clean old dependency file
	unlink("makefile.dep");
	#Find the source files (.c) for the modules that are to be built
	$result = system("$FIND $MAK_MODULE -name \"*.c\" > Sources.tmp");
	unless($result eq 0) {
        print "ERROR: Failed to build nessus platform software\n";
        exit;
    }
	#Open the list of source files for reading.
	open(MYINPUTFILE, "<Sources.tmp");
	while(<MYINPUTFILE>)
	{
		my($line) = $_;
		chomp($line);
		#Get the filename of the source
		$line =~ /(\w+)\.c/;
		my $fileName = $1; 
		#Get the directory for the object file.
		$line =~ /(.+\/)(src|test)/;
		my $modDir = $1;
		my $objDir = $1 . "obj/" . $ENV{'PLATFORM'} . "_" . $target . "/";
		#print "$PERL $MAKEDEP -Idrivers/include/ -Ibase/include/ \'$objDir$fileName.o\' $line";
		#Search the source file for dependant headers and format the dependency file.
		my $output = `$PERL $MAKEDEP -Idrivers/include/ -Ibase/include/ \'$objDir$fileName.o\' $line`;
		#Search for the makefile that the source depends on.
		
		my $output2 = `$FIND $modDir -name "*.mak"`;
		#Write the dependency file.
		open (MYFILE, '>> makefile.dep');
		print MYFILE "$output";
		#Adding dependecy for module specfic makefile.
		print MYFILE "$objDir$fileName.o : $output2";
		close (MYFILE);
	}
	close(MYINPUTFILE);
	#Remove the temp files
	$result = system("$RM Sources.tmp");
	unless($result eq 0) {
        print "ERROR: Failed to remove Sources.tmp\n";
        exit;
    }
}

sub CopyNessusHeadersWithLib {
	my $hash;
	chdir $DIR_PLATFORM;
	#Search for all the headers used in the preproc files.
    # !!! make sure there are no other object folder under each module !!!
	$result = system("$FIND $MAK_MODULE -name \"*.pp\" > headers.tmp");
	unless($result eq 0) {
        print "ERROR: Failed to build nessus platform software\n";
        exit;
    }
	open(MYINPUTFILE, "<headers.tmp");
	while(<MYINPUTFILE>)
	{
        if($_ !~ /$ENV{'PLATFORM'}/)
        {
            next;
        }
        
		my($line) = $_;
		chomp($line);
		
		open(MYHEADERFILE, "<$line");
		while(<MYHEADERFILE>) {
			my $line2 = $_;
			#Search for the lines with file names and that are headers
			if ( $line2 =~ /\#.+/ and $line2 =~ /.+\.h\"/ ) {
				$line2 =~ /.+\"(.+)\"/;
				$line2 = $1;
				#Only take the filenames that are from the armos directory then copy to the lib directory.
				if ($hash{$line2} == 0  and $line2 =~ /^\.\./) {
					print "$line2\n";
					$result = system("$COPY -f $line2 $DIR_PLATFORM/build/lib/$ENV{'PLATFORM'}_$ENV{'BUILDLIB'}/include/");
					unless($result eq 0) {
						print "Failed to coppy file: $line2\n";
					}
					$hash{$line2} = 1;
				} 
			}
		}
		close(MYHEADERFILE);
	}
	close(MYINPUTFILE);
	#Remove the temp files
	$result = system("$RM headers.tmp");
	unless($result eq 0) {
        print "ERROR: Failed to remove headers.tmp\n";
        exit;
    }
}
