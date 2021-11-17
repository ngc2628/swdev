#!/usr/bin/perl

BEGIN {
  $perlxx=$ENV{"PERLMK"} . "/xx";
}

use lib $perlxx;
use AutoLoader 'AUTOLOAD';
use Getopt::Long;
use Carp;
use DateTime;
use util::Autoans;

sub usage {
  my @msgL=@_;
  my $msg="";
  foreach ( @msgL ) {
    $msg=$msg . $_ . "\n";
  }
  if (length($msg)==0) {
    $msg="usage : \"$0\"\n";
    $msg=$msg . "  \"--fname filepath\"\n";
    $msg=$msg . "  \"[--help]\"\n";
  }
  return "\n" . $msg . "\n";
}

my $dt=DateTime->now;
print $dt->ymd . "T" . $dt->hms . "\n";
my $ans=autoans::getautoinp(7,"Tell me","the quick striped tiger","jumps","over","the","lazy","fox");
$dt=DateTime->now;
print "\n" . $dt->ymd . "T" . $dt->hms . " <" . $ans . ">\n";

$fname="";
$help=0;

GetOptions("fname=s"=>\$fname, "help"=>\$help);
die &usage() if ($help>0 || length($fname)==0);

open (FH, "/usr/bin/readlink -m $fname |");
$fname=<FH>;
chop $fname;
close (FH);

open(FH,"<","$fname") || die &usage ("error :","  cannot open \"$fname\""); 
while (<FH>) {
  $str=$_;
  chop($str);
  print __LINE__ . "<" . $str . ">\n";
  print $str . "\n" if ($str=~s/^(?!\s*#)\s*drau\s*=\s*(\w+).*/$1/);
}

print "check : ";




# perl -ne 'print if s/^(?!\s*#)\s*drau\s*=\s*(\w+).*/$1/' tst/re.txt





