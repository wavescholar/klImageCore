#!/usr/bin/perl

##******************************************************************************
##  Copyright(C) 2012 Intel Corporation. All Rights Reserved.
##  
##  The source code, information  and  material ("Material") contained herein is
##  owned  by Intel Corporation or its suppliers or licensors, and title to such
##  Material remains  with Intel Corporation  or its suppliers or licensors. The
##  Material  contains proprietary information  of  Intel or  its  suppliers and
##  licensors. The  Material is protected by worldwide copyright laws and treaty
##  provisions. No  part  of  the  Material  may  be  used,  copied, reproduced,
##  modified, published, uploaded, posted, transmitted, distributed or disclosed
##  in any way  without Intel's  prior  express written  permission. No  license
##  under  any patent, copyright  or  other intellectual property rights  in the
##  Material  is  granted  to  or  conferred  upon  you,  either  expressly,  by
##  implication, inducement,  estoppel or  otherwise.  Any  license  under  such
##  intellectual  property  rights must  be express  and  approved  by  Intel in
##  writing.
##  
##  *Third Party trademarks are the property of their respective owners.
##  
##  Unless otherwise  agreed  by Intel  in writing, you may not remove  or alter
##  this  notice or  any other notice embedded  in Materials by Intel or Intel's
##  suppliers or licensors in any way.
##
##******************************************************************************
##  Content: Intel(R) IPP Samples projects creation and build
##******************************************************************************

$| = 1;

use strict;

use Cwd;
use Getopt::Long;
use File::Path;

my %build = ();
my $cmake = "";
my $comp  = "";
my $build = "";
my $clean = "";
my $msdk  = "";

my $test  = "";
my $verb  = "";

sub in_array {
  my %items;
  my ($arr, $search_for) = @_;
  map {$items{$_} = 1 } @$arr;
  return ($items{$search_for} eq 1) ? 1 : 0;
}

sub command {
  print "[ cmd ]: $_[0]\n" if $verb;
  open( PS,"$_[0] |" ) || die "Failed: $!\n";
    while ( <PS> ) {
      chomp( $_ );
      s/\r//g;
      print "$_\n";
    }
  close( PS );
  return $?;
}

sub nativepath {
  my $path = shift;
  $path =~ tr!/!\\! if $^O =~ /Win/;
  return $path;
}

opendir my($dh), nativepath("../sources") or exit 1;
  my @list_sample = grep { !/^\.\.?$/ } readdir $dh;
closedir $dh;

my @list_generator = qw(vc2012 vc2010 vc2008 vc2005 nmake make eclipse xcode);
my @list_arch      = qw(ia32 intel64);
my @list_linkage   = qw(d s);
my @list_threading = qw(st mt);
my @list_config    = qw(release debug);


sub usage {
  print "\n";
  print "Copyright (c) 2012 Intel Corporation. All rights reserved.\n";
  print "This script performs Intel(R) IPP Samples projects creation and build.\n\n";
  print "Usage: perl build.pl --cmake=SAMPLE,ARCH,GENERATOR,LINKAGE,THREADING,CONFIG [--clean] [--build] [--msdk] [--comp=native/intel]\n";
  print "Required variants:\n";
  print "\t             SAMPLE |  ARCH   | GENERATOR |  LINKAGE   |  THREADING |  CONFIG  \n";  
  print "\t     advanced-usage |         |           |            |            |          \n";
  print "\t audio-video-codecs |         |           |            |            |          \n";
  print "\t    computer-vision |         |  vc2012   |            |            |          \n";
  print "\t   data-compression |         |  vc2010   |            |            |          \n";
  print "\t       image-codecs |         |  vc2008   |            |            |          \n";
  print "\t   image-processing | ia32    |  nmake    | s =static  | st =single |  debug   \n";
  print "\t language-interface | intel64 |  make     | d =dynamic | mt =multi  |  release \n";
  print "\t  signal-processing |         |  eclipse  |            |            |          \n";
  print "\t      speech-codecs |         |  xcode    |            |            |          \n";
  print "\t  string-processing |         |           |            |            |          \n";
  print "\t                uic |         |           |            |            |          \n";
  print "\n\n";
  print "\tOptional flags:\n\n";
  print "\t--clean - clean build directory before projects generation / build\n";
  print "\t--build - try to build projects before generation\n";
  print "\t --comp - try to use custom compiler, native(=gcc/cl.exe) or intel(=icc/icl.exe). Need to call environment!\n";
  print "\n\n";
  print "\tExamples:\n\n";
  print "\tperl build.pl --cmake=audio-video-codecs,ia32,vc2010,s,st,release [ only generate projects    ]\n";
  print "\tperl build.pl --cmake=uic,intel64,make,s,mt,debug --build         [ generate and then build   ]\n";
  print "\tperl build.pl --cmake=uic,intel64,make,d,mt,debug --build --clean [ generate, clean and build ]\n";
  print "\n";

  exit;
}

usage( ) unless @ARGV;

GetOptions (
  '--cmake=s' => \$cmake,
  '--comp=s'  => \$comp,
  '--build'   => \$build,
  '--clean'   => \$clean,
  '--msdk'    => \$msdk,
  '--test'    => \$test,
  '--verbose' => \$verb,
);

(
  $build{'sample'},
  $build{'arch'},
  $build{'generator'},
  $build{'linkage'},
  $build{'threading'},
  $build{'config'}
) = split ',',$cmake;


my $configuration_valid = 0;
if(in_array(\@list_sample, $build{'sample'})) {
  if(in_array(\@list_arch, $build{'arch'})) {
    if(in_array(\@list_generator, $build{'generator'})) {
      if(in_array(\@list_linkage, $build{'linkage'})) {
        if(in_array(\@list_threading, $build{'threading'})) {
          if(in_array(\@list_config, $build{'config'})) {
            $configuration_valid = 1;
          }
        }
      }
    }
  }
}

unless($configuration_valid) {
  print "ERROR: invalid configuration! Please call script without argument for usage.\n\n";
  usage( );
  exit 1;
}

if($comp) {
  if($comp eq 'intel') {
    if($build{'generator'} !~ /^nmake$|^make$/) {
      print "ERROR: unsupported configuration! Please use '--comp=intel' only with 'make','nmake' generators.\n";
      exit 1;
    }
  }
}


my $builder = getcwd;

my $cmake_target = "$build{'sample'}.$build{'arch'}.$build{'generator'}.$build{'linkage'}.$build{'threading'}";
$cmake_target.= ".$build{'config'}" if $build{'generator'} =~ /make|eclipse/;
$cmake_target.= ".test" if $test;

my $target_path = "$builder/../__cmake/$cmake_target";
my $sample_path = "$builder/../sources/$build{'sample'}" unless $test;
$sample_path = "$builder/../sources/$build{'sample'}.test"   if $test;

my $cmake_cmd_bld = "--build $target_path --use-stderr --config $build{'config'}";
my $cmake_cmd_gen = "--no-warn-unused-cli -Wno-dev -G\"__generator__\" -DCMAKE_CONFIGURATION_TYPES:STRING='release;debug' ";
$cmake_cmd_gen.= "-DCMAKE_BUILD_TYPE:STRING=$build{'config'} " if($build{'generator'} =~ /make|eclipse/);
$cmake_cmd_gen.= "-D__GENERATOR:STRING=$build{'generator'} -D__ARCH:STRING=$build{'arch'} -D__LINKAGE:STRING=$build{'linkage'} -D__THREADING:STRING=$build{'threading'} -D__CONFIG:STRING=$build{'config'} ";

if($msdk) {
  $cmake_cmd_gen.= "-DSAMPLES_MSDK:BOOL=TRUE ";
}

if($comp) {
  $clean = 1;

  if($comp eq 'intel') {
    if($^O =~ /Win/){
      $cmake_cmd_gen.= "-DCMAKE_C_COMPILER:STRING=icl.exe -DCMAKE_CXX_COMPILER:STRING=icl.exe ";
    } else {
      $cmake_cmd_gen.= "-DCMAKE_C_COMPILER:STRING=icc -DCMAKE_CXX_COMPILER:STRING=icpc ";
    }
  } elsif($comp eq 'native') {
    if($^O =~ /Win/){
      $cmake_cmd_gen.= "-DCMAKE_C_COMPILER:STRING=cl.exe -DCMAKE_CXX_COMPILER:STRING=cl.exe ";
    } else {
      $cmake_cmd_gen.= "-DCMAKE_C_COMPILER:STRING=gcc -DCMAKE_CXX_COMPILER:STRING=c++ ";
    }
  }
}

$cmake_cmd_gen.= nativepath($sample_path);

$cmake_cmd_gen =~ s/__generator__/Unix Makefiles/g                  if($build{'generator'} =~ /^make$/);
$cmake_cmd_gen =~ s/__generator__/NMake Makefiles/g                 if($build{'generator'} =~ /^nmake$/);
$cmake_cmd_gen =~ s/__generator__/Xcode/g                           if($build{'generator'} =~ /^xcode/);
$cmake_cmd_gen =~ s/__generator__/Eclipse CDT4 - Unix Makefiles/g   if($build{'generator'} =~ /^eclipse/);
$cmake_cmd_gen =~ s/__generator__/Visual Studio 11/g                if($build{'generator'} =~ /^vc2012$/ and $build{'arch'} eq 'ia32');
$cmake_cmd_gen =~ s/__generator__/Visual Studio 11 Win64/g          if($build{'generator'} =~ /^vc2012$/ and $build{'arch'} eq 'intel64');
$cmake_cmd_gen =~ s/__generator__/Visual Studio 10/g                if($build{'generator'} =~ /^vc2010$/ and $build{'arch'} eq 'ia32');
$cmake_cmd_gen =~ s/__generator__/Visual Studio 10 Win64/g          if($build{'generator'} =~ /^vc2010$/ and $build{'arch'} eq 'intel64');
$cmake_cmd_gen =~ s/__generator__/Visual Studio 9 2008/g            if($build{'generator'} =~ /^vc2008$/ and $build{'arch'} eq 'ia32');
$cmake_cmd_gen =~ s/__generator__/Visual Studio 9 2008 Win64/g      if($build{'generator'} =~ /^vc2008$/ and $build{'arch'} eq 'intel64');
$cmake_cmd_gen =~ s/__generator__/Visual Studio 8 2005/g            if($build{'generator'} =~ /^vc2005$/ and $build{'arch'} eq 'ia32');
$cmake_cmd_gen =~ s/__generator__/Visual Studio 8 2005 Win64/g      if($build{'generator'} =~ /^vc2005$/ and $build{'arch'} eq 'intel64');


rmtree($target_path) if   -d $target_path and $clean;
mkpath($target_path) if ! -d $target_path and -d $sample_path;
chdir ($target_path) or die "can't chdir -> $target_path: $!";

my $exit = "";
$exit = command ("cmake $cmake_cmd_gen");

if($build){
  $exit = command ("cmake $cmake_cmd_bld");

  my $status_target = "$build{'sample'}.$build{'arch'}.$build{'generator'}.$build{'linkage'}.$build{'threading'}.$build{'config'}";
  my $status_flags  = "--msdk" if $msdk;
  my $status_state  = (!$exit) ? "OK" : "FAIL";

  printf "\n[ %-50s %-6s State: %-4s ] \n\n\n", $status_target, $status_flags, $status_state;
}

exit $exit;
