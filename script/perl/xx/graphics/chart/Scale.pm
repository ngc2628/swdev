#!/usr/bin/perl

BEGIN {
  $perlxx=$ENV{"PERLMK"} . "/xx";
}

use lib $perlxx;

use POSIX;
use Cwd;
use Carp;
use util::Calc;

package tic;

%ticprops=(
  "value","dinf","size","min","vis",1
);

sub new {
  my($class)=shift;
  my($this)={};
  for($ii=0;$ii<scalar(@_)-1;$ii+=2) {
    $this->{$_[$ii]}=$_[$ii+1] if (exists $ticprops{$_[$ii]});
  }
  foreach my $key (keys %ticprops) {
    next if (exists $this->{$key});
    $this->{$key}=$ticprops{$key};
  }
  bless $this,$class;
}

sub cmpTic {
  my $res=calc::ddiff($_[0]->{"value"},$_[1]->{"value"});
  return ($res<.0 ? -1 : ($res>.0 ? 1 : 0));
}

sub currState {
  my($this)=shift;
  my $msg=Scalar::Util::blessed($this) . " : ";
  my $key;
  foreach $key (keys %{$this}) {
    $msg.=$key . "=" . $this->{$key} . " ";
  }
  $msg.="\n";
  return $msg;
}

package range;

%rangeprops=(
  "bmin",.0,"bmax",1.,"bcmin",.0,"bcmax",1.,"option",0
);

$boundAutoMin=1;
$boundAutoMax=2;
$boundStaticMin=4;
$boundStaticMax=8;

sub new {
  my($class)=shift;
  my($this)={};
  for($ii=0;$ii<scalar(@_)-1;$ii+=2) {
    $this->{$_[$ii]}=$_[$ii+1] if (exists $rangeprops{$_[$ii]});
  }
  foreach my $key (keys %rangeprops) {
    next if (exists $this->{$key});
    $this->{$key}=$rangeprops{$key};
  }
  bless $this,$class;
}

sub currState {
  my($this)=shift;
  my $msg=Scalar::Util::blessed($this) . " :\n";
  my $key;
  foreach $key (sort keys %{$this}) {
    $msg.=$key . "=" . $this->{$key} . " ";
  }
  $msg.="\n";
  return $msg;
}

package scale;

sub cmpSgn {
  my ($num1,$num2)=@_;
  return 1 if ($num1>=.0 && $num2<.0);
  return -1 if ($num1<.0 && $num2>=.0);
  return 0;
}

sub growIntervalSize {
  my $size=shift;
  my $lggrow=calc::log10(2.);
  my $lower=calc::log10($size);
  my $higher=$lower+$lggrow;
  my $cmpsgn=cmpSgn($lower,$higher);
  my $tmp=.0;
    
  if (int($higher)>int($lower) || $cmpsgn!=0) {
    $tmp=calc::ipow10(calc::nextlowint($higher));
    return $tmp if ($tmp>$size);
  }
 #in case we want an interval like 2.5 rather than 4
	if (int(calc::roundPos($higher))>int(calc::roundPos($lower)) || $cmpsgn!=0) {
	  $tmp=calc::ipow10(int(calc::roundPos($higher)))/4.0;
    return $tmp if ($tmp>$size);
  }
  $lower+=$lggrow;
  $higher+=$lggrow;
  
  $cmpsgn=cmpSgn($lower,$higher);
  if (int($higher)>int($lower) || $cmpsgn!=0) {
    $tmp=calc::ipow10(calc::nextlowint($higher))/2.;
    return $tmp if ($tmp>$size);
  }
  return 2.*$size;
}

%scaleprops=(
  "range",0,"interval","dinf","tics",0
);

sub new {
  my($class)=shift;
  my($this)={};
  for($ii=0;$ii<scalar(@_)-1;$ii+=2) {
    $this->{$_[$ii]}=$_[$ii+1] if (exists $scaleprops{$_[$ii]});
  }
  foreach my $key (keys %scaleprops) {
    next if (exists $this->{$key});
    if ($key eq "range") {
      $this->{$key}=range->new();
    }
    elsif ($key eq "tics") {
      $this->{$key}=[];
    }
    else {
      $this->{$key}=$scaleprops{$key};
    }
  }
  bless $this,$class;
}

sub currState {
  my($this) = shift;
  my $refinfo;
  my $key;
  my $tic;
  my $msg=Scalar::Util::blessed($this) . " :\n";
  foreach $key (sort keys %{$this}) {
    $refinfo=ref($this->{$key});
    if ($refinfo ne "") {
      if ($key eq "tics") {
        foreach $tic (@{$this->{"tics"}}) {
          $msg.=$tic->currState();  
        }
      }
      elsif ($key eq "range") {                   
        $msg.=$this->{$key}->currState();
      }
    }
    else {
      $msg.="key=" . $key . " value=" . $this->{$key} . "\n";
    }
  }
  return $msg . "\n";
}

sub setRangeOption {
  my($this)=shift;
  $this->{"range"}->{"option"}=shift;
  return $this->{"range"}->{"option"};
}

sub setRange {
  my($this)=shift;
  my $min=shift;
  my $max=shift;
  my $currrange=$this->{"range"};
  my $tmp=calc::dbusted($min);
  $min=$currrange->{"bmin"} if ($tmp ne "");
  $tmp=calc::dbusted($max);
  $max=$currrange->{"bmax"} if ($tmp ne "");
  calc::swap($min,$max) if ($max<$min);
  $currrange->{"bmin"}=$min;
  $currrange->{"bmax"}=$max;
  my $sgnmin=1.;
  my $sgnmax=1.;
  my $bamin=$min;
  my $bamax=$max;
  if ($min<.0) {
    $sgnmin=-$sgnmin;
    $bamin=-$bamin;
  }
  if ($max<.0) {
    $sgnmax=-$sgnmax;
    $bamax=-$bamax;
  }
  my $sgns=$bamin>$bamax ? 1 : 0;
  calc::swap($bamin,$bamax) if ($sgns==1);
  my $valdiff=$bamax-$bamin;
  my $magmax=calc::nextlowint(calc::log10($bamax));
  my $magdiff=calc::nextlowint(calc::log10($valdiff));
  my $rdprec=-$magmax;
  $tmp=$magmax-calc::nextlowint(calc::log10($bamin));
  $tmp=-$tmp if ($tmp<0);
  if ($tmp>1 || ($valdiff<calc::ipow10($magdiff-3) && $sgnmin!=$sgnmax)) {
    $bamax=calc::roundUp($bamax,$rdprec);
    $bamin=$sgnmin==$sgnmax ? .0 : $bamax;
  }
  elsif ($valdiff==.0 && $sgnmin==$sgnmax) {
    $bamin-=calc::ipow10($rdprec);
    $bamax+=calc::ipow10($rdprec);
  }
  else {
    $rdprec=-$magdiff;
    $bamin=calc::roundDown($bamin,$rdprec);
    $bamax=calc::roundUp($bamax,$rdprec);
  }
  if ($sgns==1) {
    aux::swap($bamin,$bamax);
    $sgnmax=$bamax<.0 ? -1. : 1.;
  }
  $bamin*=$sgnmin;
  $bamax*=$sgnmax;
  $currrange->{"bcmin"}=calc::roundPos($bamin,$rdprec);
  $currrange->{"bcmax"}=calc::roundPos($bamax,$rdprec);
  return $this->calcInterval(11);
}

sub calcInterval {
  my($this)=shift;
  my $maxtics=shift;
  my $currrange=$this->{"range"};
  my $bdiff=$currrange->{"bcmax"}-$currrange->{"bcmin"};
  my $interval=calc::ipow10(calc::nextlowint(calc::log10($bdiff))-2);
  my $ntics=int(1.+$bdiff/$interval);
  
  while ($ntics<$maxtics) {
    $interval/=10.;
    $ntics=int(1.+$bdiff/$interval);
  }
  while ($maxtics>2 && $ntics>$maxtics) {
#print "intv=" . $interval . " ntics=" . $ntics . "\n";
    $interval=growIntervalSize($interval);
    $ntics=int(1.+$bdiff/$interval);
#print "intv=" . $interval . " ntics=" . $ntics . " maxtics=" . $maxtics . "\n";
  }
  my $prec=-calc::nextlowint(calc::log10($interval));
  $prec++ if (calc::roundPos($interval,$prec)>$interval);
  $this->{"interval"}=calc::roundPos($interval,$prec);
  return $ntics;
}

sub calcTics {
  my($this)=shift;
  my $maxtics=shift;
  $maxtics=1000 if ($maxtics>1000);
  $maxtics=4 if ($maxtics<4);
  (@{$this->{"tics"}})=();
  my @calcedtics=();
  my $calcedtic;
  my $ntics=$this->calcInterval($maxtics);
  my $interval=$this->{"interval"};
  my $maginterval=calc::mag($interval);
  my $currrange=$this->{"range"};
  my $pos=$currrange->{"bcmin"};
  my $rangeopt=$currrange->{"option"};
  my $ii=0;
  my $jj=0;
  my $idx=-1;
  
  for ($ii=0;$ii<$ntics;$ii++) {
    $calcedtic=tic->new();
    $calcedtic->{"value"}=calc::roundPos($pos,1-$maginterval);
    last if ($pos>$currrange->{"bcmax"});
    push (@calcedtics,$calcedtic);
    $pos+=$interval;
  }
  
#foreach $ii (@calcedtics) {
#  print $ii->{"value"} . "\n";
#}
  
  if (($rangeopt&$range::boundStaticMin)>0) {
    $calcedtic=tic->new("value" => $currrange->{"bmin"});
    $idx=calc::nextIdx($calcedtic,\@calcedtics,\&tic::cmpTic);
    $idx=0 if ($idx<0);
    $calcedtic->{"vis"}=0;
    $calcedtics[0]=$calcedtic;
    if ($idx>1) {
      for ($ii=$idx,$jj=1;$ii<$ntics;$ii++,$jj++) {
        $calcedtics[$jj]=$calcedtics[$ii];     
      }
      for ($ii=0;$ii<$idx-1;$ii++) {
        delete $calcedtics[$ntics-1-$ii];
      }
      $ntics=scalar(@calcedtics);
    }
  }
  if (($rangeopt&$range::boundStaticMax)>0) {
    $calcedtic=tic->new("value" => $currrange->{"bmax"});
    $idx=calc::nextIdx($calcedtic,\@calcedtics,\&tic::cmpTic);
    $idx=$ntics-1 if ($idx>=$ntics);
    for ($ii=$ntics-1;$ii>=$idx;$ii--) {
      delete $calcedtics[$ii];
    }
    $calcedtic->{"vis"}=0;
    $calcedtics[$idx]=$calcedtic;
    $ntics=scalar(@calcedtics);
  }
  (@{$this->{"tics"}})=@calcedtics;
  $this->setMajTics();
  return $ntics;
}

sub setMajTics {
  my($this)=shift;
  my $ntics=scalar(@{$this->{"tics"}});
  my $interval=$this->{"interval"};
  my $maginterval=calc::mag($interval);
  my $iinterval=$interval*calc::ipow10(1-$maginterval);
  my $ii=0;
  my $idx=-1;
  my $val=.0;
  
  for ($ii=0;$ii<$ntics;$ii++) {
    ${$this->{"tics"}}[$ii]->{"size"}="min";
    next if ($idx>=0);
    $val=${$this->{"tics"}}[$ii]->{"value"};
    if (calc::ddiff($val,.0,calc::ipow10($maginterval-1))==.0) {
      ${$this->{"tics"}}[$ii]->{"size"}="maj";
      next;
    }
    $idx=$ii 
      if (calc::ddiff(calc::roundUp($val,-$maginterval-1),
                      calc::roundDown($val,-$maginterval-1),
                      calc::ipow10($maginterval))==.0 ||
          ($iinterval==10 &&
             calc::ddiff(calc::roundUp(2.*$val,-$maginterval-1)),
                         calc::roundDown(2.*$val,-$maginterval-1),
                         calc::ipow10($maginterval)==.0));
  }
  if ($idx>=0) {
    my $idxwarp=0;
    ${$this->{"tics"}}[$idx]->{"size"}="maj";
    ${$this->{"tics"}}[$idx-2]->{"size"}="maj"
      if ($iinterval==50 && $idx-2>=0 && ${$this->{"tics"}}[$idx-2]->{"vis"}==1);
    while ($idx<$ntics) {
      $idxwarp=(($iinterval==10 || $iinterval==20) ? 5 :
                 ($iinterval==25 ? 4 : ($iinterval==50 ? 2 : 0)));
      $idx+=$idxwarp;
      ${$this->{"tics"}}[$idx]->{"size"}="maj"
        if ($idx<$ntics && ${$this->{"tics"}}[$idx]->{"vis"}==1);
    }
  }
}

package tscale;

@ISA = (scale);

sub new {
  my($class) = shift;
  my($this) = scale->new(@_);
  return(bless($this,$class));
}

1;
