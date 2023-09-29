#!/usr/bin/perl

use Encode("encode","decode");
use feature 'say';

sub usage {

  exit 1;

}

my $ii=0;
my $jj=0;
my $idxl=-1;
my $idxh=-1;
my $ch;
my $str;
my $outstr;
my $fname;
my @xL=();

while ($#ARGV>=0) {
  $arg=$ARGV[0];
  shift;
  if ($arg eq "--fname" && $#ARGV>=0 && substr($ARGV[0],0,2) ne "--") {
    $fname=$ARGV[0];
    shift
  }
  elsif ($arg eq "--help" || $arg eq "-h" || $arg eq "/?") {
    usage;
  }
  else {
    $idxl<0 ? $idxl=$arg : $idxh=$arg;
  }
}
$idxl=256 if ($idxl<0);
$idxh=16185 if ($idxh<0);
($idxh,$idxl)=($idxl,$idxh) if ($idxh<$idxl);

open(OFH,">",$fname) if ($fname);
for ($ii=$idxl;$ii<=$idxh;$ii++) {
  $str=encode("utf-8",chr($ii));
  foreach $ch (split('', $str)) {
    push @xL,sprintf("%X",ord($ch));
  }
  $outstr=sprintf ("%s %d U+%04X %s\n",$str,$ii,$ii,join('',@xL));
  $fname ? print OFH $outstr : print $outstr;
  @xL=();
}
close(OFH) if ($fname);

exit 0;

