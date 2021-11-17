#!/usr/bin/perl

use AutoLoader 'AUTOLOAD';

package autoans;

sub alarmhandler {
  die $$ . "\n";
}

sub askinp {
  my $timeout=shift;
  my @answerL=@_;
  alarm ($timeout);
  my $minput=<STDIN>;
  chop($minput);
#  $minput=~s/\s*//g;
  for (@answerL) {
    if ($_ eq $minput) {
      alarm (0);
      die $minput . "\n";
    }
  }
}

sub getautoinp {
  my $timeout=shift;
  my $msg=shift;
  my @answerL=@_;
  return 0 if (scalar @answerL==0);
  my $autoans=$answerL[0];
  return 0 if (length($autoans)==0);
  return $autoans if (scalar @answerL==1);
  $SIG{ALRM}=\&alarmhandler;
  my $inp="";
  while (1) {
    print $msg . " : ";
    eval { &askinp($timeout,@answerL); };
    $inp=$@;
    chop($inp);
    return $autoans if ($inp eq $$);
#   check answer
    for (@answerL) {
      return $_ if ($_ eq $inp);
    }
  } 
  return $inp;
}

1;