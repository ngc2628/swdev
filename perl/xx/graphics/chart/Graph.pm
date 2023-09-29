#!/usr/bin/perl

BEGIN {
  $perlxx=$ENV{"PERLMK"} . "/xx";
}

use lib $perlxx;
use Cwd;
use Carp;
use util::TypeId;
use graphics::chart::Axis;
use GD;
use GD::Text;

package graph;

use base 'typeid';

%graphprops=(
  "font","arial","fontsz",10
);

sub new {
  my($class)=shift;
  my $classname=shift;
  my($this)=typeid->new($classname);
  for($ii=0;$ii<scalar(@_)-1;$ii+=2) {
    $this->{$_[$ii]}=$_[$ii+1] if (exists $graphprops{$_[$ii]});
  }
  foreach my $key (keys %graphprops) {
    $this->{$key}=$graphprops{$key} if (!exists $this->{$key});
  }
  bless $this,$class;
}

sub draw {
  my($this)=shift;
  print "baseclass draw\n";
}


package graph2;

@ISA = (graph);

%graph2props=(
  "scalex",0,
  "scaley",0
);

sub new {
  my($class)=shift;
  my($this)=graph->new($class,@_);
  for($ii=0;$ii<scalar(@_)-1;$ii+=2) {
    $this->{$_[$ii]}=$_[$ii+1] if (exists $graph2props{$_[$ii]});
  }
  foreach my $key (keys %graph2props) {
    $this->{$key}=$graph2props{$key} if (!exists $this->{$key});
  }
  return(bless($this,$class));
}

sub draw {
  my($this)=shift;
  $this->PARENT::draw();
}

1;
