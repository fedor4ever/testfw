::
:: Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
:: All rights reserved.
:: This component and the accompanying materials are made available
:: under the terms of "Eclipse Public License v1.0"
:: which accompanies this distribution, and is available
:: at the URL "http://www.eclipse.org/legal/epl-v10.html".
::
:: Initial Contributors:
:: Nokia Corporation - initial contribution.
::
:: Contributors:
:: 
:: Description: This file contains capsmodifier implementation.
::

@perl -x createtestmodule.bat %*
@goto end

#!perl -w
use strict;
use Term::ReadLine;

my $moduletype = "";
my $modulename = "";
my $modulepath = "";


my $hardcoded = "hardcoded";
my $testclass = "testclass";
my $kerneltestclass = "kerneltest";
my $capsmodifier = "capsmodifier";


my $hardcodedi = "h";
my $testclassi = "t";
my $kerneltestclassi = "k";
my $capsmodifieri = "c";

my $term = new Term::ReadLine 'Simple Perl calc';
my $prompttype = "Enter ModuleType (name/short cut): ";
my $promptname = "Enter ModuleName (or exit): ";  
my $promptpath = "Enter path [default is drive root] (or exit): ";  
my $exit = "exit";
my $exiti = "e";   
  
sub PrintHelp();

if ( $#ARGV >= 0 )
{
    $moduletype = $ARGV[0];
    if( $moduletype eq "help" )
    {
    PrintHelp();
    }
}
else
{
    print "Enter ModuleType :\n";
	
	print "\t($hardcodedi) $hardcoded = creates test module that uses hardcoded test cases.\n";
	print "\t($testclassi) $testclass = creates test class which is used with TestScripter.\n";
	print "\t($kerneltestclassi) $kerneltestclass = creates kernel test class which is used with TestScripter (only for kernel testing!).\n";
	print "\t($capsmodifieri) $capsmodifier = creates capability modification module\n";
  print "\t($exiti) $exit = Exit.\n";  


    $moduletype = $term->readline($prompttype);
    if( $moduletype eq $exit || $moduletype eq $exiti)
    {
        exit;
    }
}
if(  $moduletype ne $hardcoded && $moduletype ne $testclass && $moduletype ne $kerneltestclass && $moduletype ne $capsmodifier  && $moduletype ne $hardcodedi && $moduletype ne $testclassi && $moduletype ne $kerneltestclassi && $moduletype ne $capsmodifieri )
    {
        print "\nInvalid module type\n";
        print "See createtestmodule help\n";
        exit;
    }
    
if ( $#ARGV >= 1 )
{
    $modulename = $ARGV[1];
}
else
{
    
	
	if( $moduletype eq $hardcoded || $moduletype eq $hardcodedi )
	{
   	 	print "$hardcoded module selected.\n";
	}
	elsif( $moduletype eq $testclass || $moduletype eq $testclassi )
	{
  		print "$testclass module selected.\n";
	}
	elsif( $moduletype eq $kerneltestclass || $moduletype eq $kerneltestclassi )
	{
  		print "$kerneltestclass module selected.\n";
	}
	elsif( $moduletype eq $capsmodifier || $moduletype eq $capsmodifieri )
	{
 		print "$capsmodifier module selected.\n";
	}
	
	print "Enter ModuleName which has to be a valid C++ variable name.\n";
    
	$modulename = $term->readline($promptname);

    if( $modulename eq $exit || $modulename eq $exiti)
    {
        exit;
    }
}
if ( $#ARGV >= 2 )
{
    $modulepath = $ARGV[2];
}
else
{
    $modulepath = $term->readline($promptpath);
    if( $modulepath eq $exit || $modulepath eq $exiti )
    {
        exit;
    }
}

$modulepath.= "\\";

print "Create test module of type $moduletype with name $modulename ";


if ( $modulepath eq "" )
{ 
    print "to current drive root\n";
}
else
{
    print "to $modulepath\n";
}

my @args = ("$modulename", "$modulepath");


if( $moduletype eq $hardcoded || $moduletype eq $hardcodedi )
{
    chdir "HardCodedTestModuleXXX";
    system("perl -x createhardcodedmodule.bat @args");
}
elsif( $moduletype eq $testclass || $moduletype eq $testclassi )
{
    chdir "TemplateScriptXXX";
    system("perl -x CreateTestClass.bat @args");
}
elsif( $moduletype eq $kerneltestclass || $moduletype eq $kerneltestclassi )
{
    chdir "TemplateKernelScriptXXX";
    system("perl -x CreateKernelTestClass.bat @args");
}

elsif( $moduletype eq $capsmodifier || $moduletype eq $capsmodifieri )
{
    chdir "CapsModifierXXX";
    system("perl -x CreateCapsModifier.bat @args");
}



exit;

sub PrintHelp()
{
	print "CreateTestModule [ModuleType] [ModuleName] [path]\n";
	print "\n";
	print "Creates a new test module\n";
	print "ModuleType defines the type of test module:.\n";
  print "\thardcoded = creates test module that uses hardcoded test cases.\n";
	print "\ttestclass = creates test class which is used with TestScripter.\n";
	print "\tkerneltestclass = creates kernel test class which is used with TestScripter (only for kernel testing!).\n";
	print "\tcapsmodifier = creates capability modification module\n";
	print "If no arguments are given, they are asked from user.\n";
	print "If [path] is given, it must contain the final \'\\\' in path name.\n";
	print "Command must be executed in STIFTestFramework\\TestModuleTemplates directory\n";
	exit;
}

__END__
:end
