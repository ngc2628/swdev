#!/usr/bin/perl

BEGIN {
  $perlxx=$ENV{"PERLMK"} . "/xx";
}

use lib $perlxx;
use util::Calc;
use util::Vertex;
use util::Matrix;
use graphics::chart::Scale;
use graphics::chart::Axis;
use graphics::chart::Graph;
use graphics::chart::Simplechart;

my $v1=vertex->new(5.,3.);
my $v2=vertex->new(8.,4.8);
my $sc=2.;
my $vtr=matrixtransm($v1,$v2,$sc);
print "vtr=" . $vtr->currState() . "\n";

my $schart=simplecharttst();

sub matrixinvert {
  my @mdata1=(1,9,3,4,8,7,6,5,4);
  my $mm1=squarematrix->new(3,@mdata1);

  print $mm1->currState() . "\n";
  #$mm->removeCol(2);
  print $mm1->invert() . "\n";
  print $mm1->currState() . "\n";
  print $mm1->invert() . "\n";
  print $mm1->currState() . "\n";
}

sub matrixtransm {
  my $v1=shift;
  my $v2=shift;
  my $sc=shift;
  my $v3=vertex::diff($v2,$v1);
  my $vtr=vertex->new(.0,.0);
  my $vn=vertex->new(.0,1.);
  my $ll=$v3->len();
  $vn->setY($ll);
  my $ang=$v3->angdeg($vn);
  
  print "ll=" . $ll . " ang=" . $ang . "\n";
  
  my $tm=transformmatrix->new();
  $tm->translate(.0,$sc);
  $tm->rotateZ($ang);
  $tm->translate($v1->x(),$v1->y());
  $tm->transform($vtr);
  return $vtr;
}


sub simplecharttst {
  my $simplechart2 = simplechart2->new();
  my $yaxname=$simplechart2->setAxis(yaxis->new("pos"=>"left"));
  my $xaxname=$simplechart2->setAxis(xaxis->new("pos"=>"bottom"));
  my $graphname=$simplechart2->setGraph(graph2->new());

  print "######### chk ##########\n";
  print "xax=" . $xaxname . " yax=" . $yaxname . " graph=" . $graphname . "\n";

  my $yscale=scale->new();
  $yscale->setRangeOption(12);
  $yscale->setRange(10.857658,12.352791);
  $simplechart2->axis($yaxname)->setScale($yscale);
  my $ncalcedtics=$simplechart2->axis($yaxname)->calcTics(844);
  print "ntics=" . $ncalcedtics . "\n";
  $simplechart2->axis($yaxname)->draw();

  print "######### result ##########\n";
  print $simplechart2->currState();
}



