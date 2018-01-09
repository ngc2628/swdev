#!/usr/bin/perl

BEGIN {
  $perlxx=$ENV{"PERLMK"} . "/XX";
}

use lib $perlxx;
use Cwd;
use Carp;
use util::TypeId;
use util::Calc;

package shape;

use base 'typeid';

%shapeprops=(
  "scale", 1., "rotate", .0, "translate", 0, "points", 0
);

sub new {
  my($class)=shift;
  my $classname=shift;
  my($this)=typeid->new($classname);
  for($ii=0;$ii<scalar(@_)-1;$ii+=2) {
    $this->{$_[$ii]}=$_[$ii+1] if (exists $shapeprops{$_[$ii]});
  }
  foreach my $key (keys %shapeprops) {
    $this->{$key}=$shapeprops{$key} if (!exists $this->{$key});
  }
  bless $this,$class;
}

1;
