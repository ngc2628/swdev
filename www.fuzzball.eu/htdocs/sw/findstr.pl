#!/usr/bin/perl

use Cwd;
use File::Find;
use File::Path;
use File::Basename;
use Carp;

#print "src=" . $ARGV[0] . " fileglob=" . $ARGV[2] . " text=" . $ARGV[1]"\n";

$msg="usage : $0 source direcetory (full paths if needed) \[-i -u\] ...\n";
$msg=$msg . "... \"text\"" . " \"fileglob\"\n";

sub badusage {
  croak($msg);
}

@optionL=();
&parseOptions();

$fglob=$ARGV[2];
if ($fglob eq "") {
  $fglob=".*";
}
if ($fglob=~/^\*.*/) {
  $fglob="." . $fglob;
}
$text=$ARGV[1] or &badusage;
$srcdir = $ARGV[0];

print "src=" . $srcdir . " text=" . $text . " fileglob=" . $fglob . "\n";

$actdir=cwd();
chdir $srcdir or croak("cannot change to $srcdir ... giving up ..\n");
$srcdir=cwd();
chdir $actdir;

@allfiles=();

find (\&wanted,$srcdir);
eval{&doProcess};

sub wanted {
  my $fname,$fpath,$fsuffix,$f,$chk;
  ($fname,$fpath,$fsuffix)=fileparse($File::Find::name);
  $f=$fpath . $fname . $fsuffix;
  next if (-d $f);
  $chk=$fname;
  if ($fsuffix ne "") {
    $chk=$chk . "\." . $fsuffix
  }
  @allfiles=(@allfiles,$f)  if ($fname=~/$fglob/);
}

sub doProcess {
  my $line;
  my $linecnt=0;
  my $numfilesfound=$#allfiles+1;
  my $f;
  
  print "found $numfilesfound files in $srcdir to work on\n";
  die "... that means n o relevant file found... skipping\n" if ($numfilesfound<=0);

  foreach $f (@allfiles) {
    if (open (IN,"<$f")==0) {
      carp "infile $f not readable .. skipping ..";
      next;
    }
    print "no match $f\n" if (&findOption("-u")==1);
    while (<IN>) {
      $line=$_;
      $linecnt++;
      if (&findOption("-i")==1) {
        if($line=~/$text/i) {
          print $f . ":" . $linecnt . " " . $line;
        }
      }
      elsif($line=~/$text/) {
        print $f . ":" . $linecnt . " " . $line;
      }
    }
    close (IN);
    $linecnt=0;
  }

}

sub parseOptions {
  my $ii=0;
  my $jj;
  my $cntarg=$#ARGV;
  while ($ii<=$cntarg) {
    if ($ARGV[$ii]=~/^-.*/) {
      @optionL=(@optionL,$ARGV[$ii]);
      for ($jj=$ii+1;$jj<=$cntarg;$jj++) {
        $ARGV[$jj-1]=$ARGV[$jj];
      }
      $ARGV[$cntarg]="";
      $cntarg--;
    }
    else {
      $ii++;
    }
  }
}

sub findOption {
  my $ask=shift;
  my $opt;
  foreach $opt (@optionL) {
    if($opt eq $ask) {
      return 1; 
    }
  }
  return 0;
}

