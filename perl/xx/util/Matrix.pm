#!/usr/bin/perl

BEGIN {
  $perlxx=$ENV{"PERLMK"} . "/xx";
}

use lib $perlxx;
use AutoLoader 'AUTOLOAD';
use Scalar::Util 'blessed';
use util::Vertex;

package matrix;

sub new {
  my($class)=shift;
  my $rows=int(shift);
  my $cols=int(shift);
  my $this={"data" => [] };
  my $row=0;
  my $col=0;
  my $val=.0;
  for ($row=0;$row<$rows;$row++) {
    push @{$this->{"data"}},[];
    for ($col=0;$col<$cols;$col++) {
      $val=shift;
      $val=($row==$col ? 1. : 0.) if (!defined($val));
      push @{$this->{"data"}->[$row]},$val;
    }
  }
  bless $this,$class;
}

sub clone {
  my($this) = shift;
  my $mm=new();
  my $row=0;
  my $col=0;
  for ($row=0;$row<scalar(@{$this->{"data"}});$row++) {
    push @{$mm->{"data"}},[];
    for ($col=0;$col<scalar(@{$this->{"data"}->[$row]});$col++) {
      push @{$mm->{"data"}->[$row]},$this->{"data"}->[$row]->[$col];
    }
  }
  return $mm;
}

sub nrows {
  my ($this)=shift;
  return scalar(@{$this->{"data"}});
}

sub ncols {
  my ($this)=shift;
  my $row=int(shift);
  $row=0 if (!defined($row) || $row<0 || $row>=scalar(@{$this->{"data"}}));
  return scalar(@{$this->{"data"}->[$row]});
}

sub currState {
  my($this) = shift;
  my $msg=Scalar::Util::blessed($this) . " :\n";
  my $row=0;
  my $col=0;
  for ($row=0;$row<$this->nrows();$row++) {
    for ($col=0;$col<$this->ncols($row);$col++) {
      $msg.=sprintf ("%e",$this->{"data"}->[$row]->[$col]) . " ";
    }
    $msg.="\n";
  }
  return $msg . "\n";
}

sub value {
  my ($this)=shift;
  my $row=int(shift);
  my $col=int(shift);
  return "nan" if ($row<0 || $row>=$this->nrows());
  return "nan" if ($col<0 || $col>=$this->ncols($row));
  return $this->{"data"}->[$row]->[$col];
}

sub setValue {
  my ($this)=shift;
  my $row=int(shift);
  my $col=int(shift);
  return 1 if ($row<0 || $row>=$this->nrows());
  return 1 if ($col<0 || $col>=$this->ncols($row));
  $this->{"data"}->[$row]->[$col]=shift;
  return 0;
}

sub row {
  my ($this)=shift;
  my $row=int(shift);
  my @rowarr=();
  return @rowarr if ($row<0 || $row>=$this->nrows());
  return @{$this->{"data"}->[$row]};
}

sub setRow {
  my ($this)=shift;
  my $row=int(shift);
  return 1 if ($row<0 || $row>=$this->nrows());
  pop (@_) while (scalar(@_)>$this->ncols($row));
  @{$this->{"data"}->[$row]}=@_;
  return 0;
}

sub removeRow {
  my ($this)=shift;
  my $row=int(shift);
  my $rows=$this->nrows();
  return 1 if ($row<0 || $row>=$rows);
  my $cols=$this->ncols();
  my $col=0;
  while (++$row<$rows) {
    for ($col=0;$col<$cols;$col++) {
      calc::swap($this->{"data"}->[$row-1]->[$col],$this->{"data"}->[$row]->[$col]);
    }
  }
  pop (@{$this->{"data"}});
  return 0;
}

sub col {
  my ($this)=shift;
  my $col=int(shift);
  my @rowarr=();
  return @rowarr if ($col<0 || $col>=$this->ncols());
  return @{$this->{"data"}->[$row]};
}

sub setCol {
  my ($this)=shift;
  my $col=int(shift);
  return 1 if ($col<0 || $col>=$this->ncols());
  my $row=0;
  for ($row=0;$row<$this->nrows();$row++) {
    last if ($row>=scalar(@_));
    $this->{"data"}->[$row]->[$col]=$_[$row];
  }
  return 0;
}

sub removeCol {
  my ($this)=shift;
  my $rmcol=int(shift);
  my $cols=$this->ncols(0);
  return 1 if ($rmcol<0 || $rmcol>=$cols);
  my $row=0;
  my $col=0;
  for ($row=0;$row<$this->nrows();$row++) {
    $col=$rmcol;
    $cols=$this->ncols($row);
    while (++$col<$cols) {
      calc::swap($this->{"data"}->[$row]->[$col-1],$this->{"data"}->[$row]->[$col]);
    }
    pop (@{$this->{"data"}->[$row]});
  }
  return 0;
}

sub reset {
  my ($this)=shift;
  my $row=0;
  my $col=0;
  for ($row=0;$row<$this->nrows();$row++) {
    for ($col=0;$col<$this->ncols($row);$col++) {
      $this->{"data"}->[$row]->[$col]=($row==$col ? 1. : .0);
    }
  }
  return 0;
}

sub transpose {
  my ($this)=shift;
  my $rows=$this->nrows();
  my $cols=$this->ncols();
  $this->reset() if ($rows!=$cols);
  my $row=0;
  my $col=0;
  my @data=();
  for ($row=0;$row<$rows;$row++) {
    push @data,(@{$this->{"data"}->[$row]}); 
  }
  for ($row=0;$row<$rows;$row++) {
    for ($col=0;$col<$cols;$col++) {
      $this->{"data"}->[$row]->[$col]=$data[$col*$cols+$row];
    }
  }
  return 0;
}

sub mult {
  my ($this)=shift;
  my $mm=shift;
  my $mycols=$this->ncols();
  return 1 if (!defined($mm) || $mycols!=$mm->nrows());
  my $row=0;
  my $col=0;
  my $mcol=0;
  my $val=.0;
  my $myrows=$this->nrows();
  my $mmcols=$mm->ncols();
  my @data=();
  for ($row=0;$row<$myrows;$row++) {
    for ($mcol=0;$mcol<$mmcols;$mcol++) {
      $val=.0;
      for ($col=0;$col<$mycols;$col++) {
        $val+=$this->{"data"}->[$row]->[$col]*$mm->{"data"}->[$col]->[$mcol];
      }
      push @data,$val;
    }
  }
  while ($mmcols<$this->ncols()) {
    $this->removeCol($this->ncols()-1);
  }
  my $ii=0;
  for ($ii=0;$ii<scalar(@data);$ii++) {
    $row=int($ii/$mmcols);
    $col=$ii-$mmcols*$row;
    if ($this->ncols($row)<=$col) {
      push @{$this->{"data"}->[$row]},$data[$ii];
    }
    else {
      $this->{"data"}->[$row]->[$col]=$data[$ii];
    }
  }
  return 0;
}

package squarematrix;

@ISA=(matrix);

sub new {
  my($class)=shift;
  my $rowscols=shift;
  my($this)=matrix->new($rowscols,$rowscols,@_);
  $this->{"lum" => [] };
  $this->{"rowperm" => [] };
  $this->{"parity" => 1. };
  return(bless($this,$class));
}

sub invalidate {
  my ($this)=shift;
  $this->{"lum"}=[];
  $this->{"rowperm"}=[];
  $this->{"parity"}=1.;
  return 0;
}

sub decomposition {
  my ($this)=shift;
  my $num=$this->nrows();
  return 1 if ($num<2);
  $this->invalidate();
  my $ii=0;
  my $jj=0;
  my $kk=0;
  my $imax=0;
  my $maxcoeff=.0;
  my $tmp=.0;
  my @rowscale=();
  $this->invalidate();
  for ($ii=0;$ii<$num;$ii++) {
    push @{$this->{"lum"}},[];
    $maxcoeff=.0;
    for ($jj=0;$jj<$num;$jj++) {
      push @{$this->{"lum"}->[$ii]},$this->{"data"}->[$ii]->[$jj];
      $tmp=$this->{"lum"}->[$ii]->[$jj];
      $tmp=-$tmp if($tmp<.0);
      $maxcoeff=$tmp if ($maxcoeff<$tmp);
    }
    if ($maxcoeff==.0) {
      $this->invalidate();
      return 1;
    }
    push(@rowscale,$maxcoeff); 
  }
  
  for ($ii=0;$ii<$num;$ii++) {
    push(@{$this->{"rowperm"}},$ii);
  }
  for ($jj=0;$jj<$num;$jj++) {
    for ($ii=0;$ii<$jj;$ii++) {
      for ($kk=0;$kk<$ii;$kk++) {
        $this->{"lum"}->[$ii]->[$jj]-=
          ($this->{"lum"}->[$ii]->[$kk]*$this->{"lum"}->[$kk]->[$jj]);
      }
    }
    $maxcoeff=.0;
    for ($ii=$jj;$ii<$num;$ii++) {
      for ($kk=0;$kk<$jj;$kk++) {
        $this->{"lum"}->[$ii]->[$jj]-=
          $this->{"lum"}->[$ii]->[$kk]*$this->{"lum"}->[$kk]->[$jj];
      }
      $tmp=$this->{"lum"}->[$ii]->[$jj];
      $tmp=-$tmp if ($tmp<.0);
      $tmp/=$rowscale[$ii];
      if ($maxcoeff<=$tmp) {
        $maxcoeff=$tmp;
        $imax=$ii;	
      }
    }
    if ($jj!=$imax) {
      for ($ii=0;$ii<$num;$ii++) {
        calc::swap($this->{"lum"}->[$imax]->[$ii],$this->{"lum"}->[$jj]->[$ii]);
      }
      $rowscale[$imax]=$rowscale[$jj];
      calc::swap($this->{"rowperm"}->[$jj],$this->{"rowperm"}->[$imax]);
      $this->{"parity"}=-$this->{"parity"};
    }
    if ($this->{"lum"}->[$jj]->[$jj]==.0) {
      $this->invalidate();
      return 1;
    }
    if ($jj<($num-1)) {
      for ($ii=($jj+1);$ii<$num;$ii++) {
        $this->{"lum"}->[$ii]->[$jj]/=$this->{"lum"}->[$jj]->[$jj];
      }
    }
  }
  return 0;
}

sub backsubstitution {
  my ($this)=shift;
  my $num=$this->nrows();
  my @vres=();
  return @vres if (scalar(@_)<$num);
  if (scalar(@{$this->{"rowperm"}})!=$num ||
      scalar(@{$this->{"lum"}})!=$num) {
    return @vres if ($this->decomposition()!=0);
  }
  my $ii=0;
  my $jj=0;
  my $tmp=.0;
  for ($ii=0;$ii<$num;$ii++) {
    push @vres,$_[$this->{"rowperm"}->[$ii]];
  }
  for ($ii=0;$ii<$num;$ii++) {
    for ($jj=0;$jj<$ii;$jj++) {
      $vres[$ii]-=$this->{"lum"}->[$ii]->[$jj]*$vres[$jj];
    }
  }
  for ($ii=($num-1);$ii>-1;$ii--) {
    $tmp=$vres[$ii];
    for ($jj=($ii+1);$jj<$num;$jj++) {
      $tmp-=$this->{"lum"}->[$ii]->[$jj]*$vres[$jj];
    }
    $vres[$ii]=$tmp/$this->{"lum"}->[$ii]->[$ii];
  } 
  return @vres;
}

sub invert {
  my ($this)=shift;
  my $num=$this->nrows();
  if (scalar(@{$this->{"rowperm"}})!=$num ||
      scalar(@{$this->{"lum"}})!=$num) {
    return 1 if ($this->decomposition()!=0);
  }
  my $ii=0;
  my $jj=0;
  my @identity=();
  my @inv=();
  for ($ii=0;$ii<$num;$ii++) {
    for ($jj=0;$jj<$num;$jj++) {
      $identity[$jj]=($ii==$jj ? 1. : 0.);
    }
    @inv=$this->backsubstitution(@identity);
    return 1 if (scalar(@inv)==0);
    for ($jj=0;$jj<$num;$jj++) {
      $this->{"data"}->[$jj]->[$ii]=$inv[$jj];
    } 
  }
  $this->invalidate();
  return 0;
}

sub solveFor {
  my ($this)=shift;
  return $this->backsubstitution(@_);
}

sub determinant() {
  my ($this)=shift;
  my $num=$this->nrows();
  return .0 if($num<=1);
  if (scalar(@{$this->{"rowperm"}})!=$num ||
      scalar(@{$this->{"lum"}})!=$num) {
    return .0 if ($this->decomposition()!=0);
  }
  my $ii=0;
  my $det=$this->{"parity"};
  for ($ii=0;$ii<$num;$ii++) {
    $det*=$this->{"lum"}->[$ii]->[$ii];
  }
  return $det;
}

package transformmatrix;

@ISA=(squarematrix);

sub new {
  my($class)=shift;
  my($this)=squarematrix->new(4,@_);
  return(bless($this,$class));
}

sub translate {
  my ($this)=shift;
  my $tm=transformmatrix->new();
  my $xyz=shift;
  $xyz=.0 if (!defined $xyz);
  $tm->{"data"}->[3]->[0]=$xyz;
  $xyz=shift;
  $xyz=.0 if (!defined $xyz);
  $tm->{"data"}->[3]->[1]=$xyz;
  $xyz=shift;
  $xyz=.0 if (!defined $xyz);
  $tm->{"data"}->[3]->[2]=$xyz;
  return $this->mult($tm);
}

sub scale {
  my ($this)=shift;
  my $tm=transformmatrix->new();
  my $xyz=shift;
  $xyz=.0 if (!defined $xyz);
  $tm->{"data"}->[0]->[0]=$xyz;
  $xyz=shift;
  $xyz=.0 if (!defined $xyz);
  $tm->{"data"}->[1]->[1]=$xyz;
  $xyz=shift;
  $xyz=.0 if (!defined $xyz);
  $tm->{"data"}->[2]->[2]=$xyz;
  return $this->mult($tm);
}

sub rotateZ {
  my ($this)=shift;
  my $tm=transformmatrix->new();
  my $degrees=shift;
  $degrees=.0 if (!defined $degrees);
  $degrees*=$calc::rad;
  $tm->{"data"}->[0]->[0]=cos($degrees);
  $tm->{"data"}->[0]->[1]=-sin($degrees);
  $tm->{"data"}->[1]->[0]=sin($degrees);
  $tm->{"data"}->[1]->[1]=cos($degrees);
  return $this->mult($tm);
}
	
sub rotateX {
  my ($this)=shift;
  my $tm=transformmatrix->new();
  my $degrees=shift;
  $degrees=.0 if (!defined $degrees);
  $degrees*=$calc::rad;
  $tm->{"data"}->[1]->[1]=cos($degrees);
  $tm->{"data"}->[1]->[2]=-sin($degrees);
  $tm->{"data"}->[2]->[1]=sin($degrees);
  $tm->{"data"}->[2]->[2]=cos($degrees);
  return $this->mult($tm);
}

sub rotateY {
  my ($this)=shift;
  my $tm=transformmatrix->new();
  my $degrees=shift;
  $degrees=.0 if (!defined $degrees);
  $degrees*=$calc::rad;
  $tm->{"data"}->[0]->[0]=cos($degrees);
  $tm->{"data"}->[0]->[2]=sin($degrees);
  $tm->{"data"}->[2]->[0]=-sin($degrees);
  $tm->{"data"}->[2]->[2]=cos($degrees);
  return $this->mult($tm);
}

sub shearXY {
  my ($this)=shift;
  my $tm=transformmatrix->new();
  my $xy=shift;
  $xy=.0 if (!defined $xy);
  $tm->{"data"}->[0]->[1]=$xy;
  $xy=shift;
  $xy=.0 if (!defined $xy);
  $tm->{"data"}->[1]->[0]=$xy;
  return $this->mult($tm);
}

sub shearXZ {
  my ($this)=shift;
  my $tm=transformmatrix->new();
  my $xz=shift;
  $xz=.0 if (!defined $xz);
  $tm->{"data"}->[0]->[2]=$xz;
  $xz=shift;
  $xz=.0 if (!defined $xz);
  $tm->{"data"}->[2]->[0]=$xz;
  return $this->mult($tm);
}

sub shearYZ {
  my ($this)=shift;
  my $tm=transformmatrix->new();
  my $yz=shift;
  $yz=.0 if (!defined $yz);
  $tm->{"data"}->[1]->[2]=$yz;
  $yz=shift;
  $yz=.0 if (!defined $yz);
  $tm->{"data"}->[2]->[1]=$yz;
  return $this->mult($tm);
}

sub transform {
  my ($this)=shift;
  my $v=shift;
  return 1 if (!defined $v);
  my @vv=map{$_} @{$v->{"xyzw"}};
  my $ii=0;
  my $jj=0;
  for ($ii=0;$ii<4;$ii++) {
    $v->{"xyzw"}->[$ii]=.0;
    $vv[$ii]=($ii==3 ? 1. : .0) if (calc::dbusted($vv[$ii]));
  }
  for ($ii=0;$ii<4;$ii++) {
    for ($jj=0;$jj<4;$jj++) {
      $v->{"xyzw"}->[$ii]+=$vv[$jj]*$this->{"data"}->[$jj]->[$ii];
    }
  }
  return 0;
}

1;
