#!/usr/bin/Perl

BEGIN {
  $perlxx=$ENV{"PERLMK"} . "/xx";
}

use lib $perlxx;
use AutoLoader 'AUTOLOAD';
use Scalar::Util 'blessed';
use POSIX;

package vertex;

sub new {
  my($class)=shift;
  my($this)={"xyzw" => []};
  my $ii=0;
  my $val="nan";
  for ($ii=0;$ii<4;$ii++) {
    $val=shift;
    if (!defined($val)) {
      $val=($ii<3 ? .0  : 1.);
    }
    push $this->{"xyzw"},$val;
  }
  bless $this,$class;
}

sub x {
  my($this)=shift;
  return $this->{"xyzw"}->[0];  
}

sub setX {
  my($this)=shift;
  my $val=shift;
  $val="nan" if (!defined($val));
  $this->{"xyzw"}->[0]=$val;
  return 0;  
}

sub y {
  my($this)=shift;
  return $this->{"xyzw"}->[1];    
}

sub setY {
  my($this)=shift;
  my $val=shift;
  $val="nan" if (!defined($val));
  $this->{"xyzw"}->[1]=$val;
  return 0;  
}

sub z {
  my($this)=shift;
  return $this->{"xyzw"}->[2];    
}

sub setZ {
  my($this)=shift;
  my $val=shift;
  $val="nan" if (!defined($val));
  $this->{"xyzw"}->[2]=$val;
  return 0;  
}

sub w {
  my($this)=shift;
  return $this->{"xyzw"}->[3];    
}

sub setW {
  my($this)=shift;
  my $val=shift;
  $val="nan" if (!defined($val));
  $this->{"xyzw"}->[3]=$val;
  return 0;  
}

sub val {
  my($this)=shift;
  my $idx=shift;
  return "nan" if (!defined($idx) || $idx<0 || 3<$idx);
  return $this->{"xyzw"}->[$idx]; 
}

sub setVal {
  my($this)=shift;
  my $idx=shift;
  return 1 if (!defined($idx) || $idx<0 || 3<$idx);
  my $val=shift;
  $val="nan" if (!defined($val));
  $this->{"xyzw"}->[$idx]=$val;
  return 0; 
}

sub len {
  my($this)=shift;
  my $res=.0;
  my $ii=0;
  for ($ii=0;$ii<3;$ii++) {
    $res+=($this->{"xyzw"}->[$ii]*$this->{"xyzw"}->[$ii]);
  }
  return sqrt($res);
}

sub normalize {
  my($this)=shift;
  my $ll=$this->len();
  my $ii=0;
  for ($ii=0;$ii<3;$ii++) {
    $this->{"xyzw"}->[$ii]/=$ll;
  }
  return 0;
}

sub dot {
  my($this)=shift;
  my $pal=shift;
  return .0 if (!defined($pal));
  my $res=.0;
  my $ii=0;
  for ($ii=0;$ii<3;$ii++) {
    $res+=($this->{"xyzw"}->[$ii]*$pal->{"xyzw"}->[$ii]);
  }
  return $res;
}

sub cross {
  my($this)=shift;
  my $pal=shift;
  my $vres=new();
  return $vres if (!defined($pal));
  $vres->{"xyzw"}->[0]=
    $this->{"xyzw"}->[1]*$pal->{"xyzw"}->[2]-$this->{"xyzw"}->[2]*$pal->{"xyzw"}->[1];
  $vres->{"xyzw"}->[1]=
    $this->{"xyzw"}->[2]*$pal->{"xyzw"}->[0]-$this->{"xyzw"}->[0]*$pal->{"xyzw"}->[2];
  $vres->{"xyzw"}->[2]=
    $this->{"xyzw"}->[0]*$pal->{"xyzw"}->[1]-$this->{"xyzw"}->[1]*$pal->{"xyzw"}->[0];
  return $vres;
}

sub angrad {
  my($this)=shift;
  my $pal=shift;
  return .0 if (!defined($pal));
  my $den=$this->len()*$pal->len();
  return .0 if ($den==.0);
  return calc::acos($this->dot($pal)/$den);
}

sub angdeg {
  my($this)=shift;
  my $pal=shift;
  return .0 if (!defined($pal));
  my $den=$this->len()*$pal->len();
  return .0 if ($den==.0);
  return calc::acos($this->dot($pal)/$den)/$calc::rad;
}

sub add {
  my $v1=shift;
  my $v2=shift;
  my $vres=vertex->new();
  return $vres if (!defined($v1) || !defined($v2));
  my $ii=0;
  for ($ii=0;$ii<3;$ii++) {
    $vres->{"xyzw"}->[$ii]=$v1->{"xyzw"}->[$ii]+$v2->{"xyzw"}->[$ii];
  }
  return $vres;
}

sub diff {
  my $v1=shift;
  my $v2=shift;
  my $vres=vertex->new();
  return $vres if (!defined($v1) || !defined($v2));
  my $ii=0;
  for ($ii=0;$ii<3;$ii++) {
    $vres->{"xyzw"}->[$ii]=$v1->{"xyzw"}->[$ii]-$v2->{"xyzw"}->[$ii];
  }
  return $vres;
}

sub cmpX {
  return -1 if ($_[0]->{"xyzw"}->[0] lt $_[1]->{"xyzw"}->[0]);
  return 1 if ($_[1]->{"xyzw"}->[0] lt $_[0]->{"xyzw"}->[0]);
  return 0;
}

sub cmpY {
  return -1 if ($_[0]->{"xyzw"}->[1] lt $_[1]->{"xyzw"}->[1]);
  return 1 if ($_[1]->{"xyzw"}->[1] lt $_[0]->{"xyzw"}->[1]);
  return 0;
}

sub cmpZ {
  return -1 if ($_[0]->{"xyzw"}->[2] lt $_[1]->{"xyzw"}->[2]);
  return 1 if ($_[1]->{"xyzw"}->[2] lt $_[0]->{"xyzw"}->[2]);
  return 0;
}

sub cmpW {
  return -1 if ($_[0]->{"xyzw"}->[3] lt $_[1]->{"xyzw"}->[3]);
  return 1 if ($_[1]->{"xyzw"}->[3] lt $_[0]->{"xyzw"}->[3]);
  return 0;
}

@cmpf=(\&cmpX,\&cmpY,\&cmpZ,\&cmpW);

sub currState {
  my($this) = shift;
  my $msg=Scalar::Util::blessed($this) . " :";
  $msg.=" x=" . $this->{"xyzw"}->[0] if (defined $this->{"xyzw"}->[0]);
  $msg.=" y=" . $this->{"xyzw"}->[1] if (defined $this->{"xyzw"}->[1]);
  $msg.=" z=" . $this->{"xyzw"}->[2] if (defined $this->{"xyzw"}->[2]);
  $msg.=" w=" . $this->{"xyzw"}->[3] if (defined $this->{"xyzw"}->[3]);
  return $msg . "\n";
}

return 1;
