#!/usr/bin/perl

BEGIN {
  $perlxx=$ENV{"PERLMK"} . "/xx";
}

use lib $perlxx;
use Cwd;
use Carp;
#use Class::ISA;
use Scalar::Util 'blessed';
use AutoLoader 'AUTOLOAD';
use util::TypeId;
use util::Vertex;
use graphics::chart::Scale;
use GD;
use GD::Text;

package axis;

use base 'typeid';

%axisprops=(
  "font","arial","fontsz",10,
  "pos","unknown",
  "scale",0
);

sub new {
  my($class)=shift;
  my $classname=shift;
  my($this)=typeid->new($classname);
  for($ii=0;$ii<scalar(@_)-1;$ii+=2) {
    $this->{$_[$ii]}=$_[$ii+1] if (exists $axisprops{$_[$ii]});
  }
  foreach my $key (keys %axisprops) {
    $this->{$key}=$axisprops{$key} if (!exists $this->{$key});
  }
  bless $this,$class;
}

sub setScale {
  my($this)=shift;
  my $sc=shift;
  delete $this->{"scale"} if (ref($this->{"scale"})!~/^$/);
  $this->{"scale"}=$sc;
  return $this->{"scale"};
}

sub calcTics {
  my($this)=shift;
  my $maxlen=shift;
  return 0 if (ref($this->{"scale"})=~/^$/);
  my $gdtxt = GD::Text->new();
  $gdtxt->set_font(this->{"font"},this->{"fontsz"});
  $gdtxt->set_text("O");
  my $space=$gdtxt->get('width');
  my $maxtics=int($maxlen/$space);
  my $ntics=$this->{"scale"}->calcTics($maxtics);
  my $sumlen=0;  

  while ($ntics>3) {
    foreach $ii (@{$this->{"scale"}->{"tics"}}) {
      next if ($ii->{"vis"}==0);
      $gdtxt->set_text($ii->{"value"});
      $sumlen+=($gdtxt->get('width')+$space);
    }
#print "sumlen=" . $sumlen . " maxtics=" . $maxtics . " ntics=" . $ntics . "\n";
    last if ($sumlen<=$maxlen); 
    $maxtics=$ntics-1;
    $sumlen=0;
    $ntics=$this->{"scale"}->calcTics($maxtics);
  }
  return $ntics;
}

sub draw {
  my($this)=shift;
  print "baseclass " . __PACKAGE__ . " draw\n";
  my $v1=vertex->new(11,54);
  my $v2=vertex->new(12,19);
  print $v1->currState();
  print $v2->currState();
  my $cmp1=$vertex::cmpf[1]->($v1,$v2);
  print "cmp=" . $cmp1 . "\n";
}


package xaxis;

@ISA=(axis);

sub new {
  my($class)= shift;
  my($this)=axis->new($class,@_);
  return(bless($this,$class));
}

sub draw {
  my($this)=shift;
  $this->SUPER::draw();
}


package yaxis;

@ISA=(axis);

sub new {
  my($class)=shift;
  my($this)=axis->new($class,@_);
  return(bless($this,$class));
}

sub draw {
  my($this)  = shift;
  $this->SUPER::draw();
}

package zaxis;

@ISA = (axis);

sub new {
  my($class)=shift;
  my($this)=axis->new($class,@_);
  return(bless($this,$class));
}

sub draw {
  my($this)=shift;
  $this->SUPER::draw();
}

1;
