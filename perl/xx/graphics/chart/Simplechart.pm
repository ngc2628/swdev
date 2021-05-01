#!/usr/bin/perl

BEGIN {
  $perlxx=$ENV{"PERLMK"} . "/xx";
}

use lib $perlxx;
use Cwd;
use Carp;
use util::TypeId;
use graphics::chart::Axis;

package simplechart;

use base 'typeid';

%simplechartprops=(
  "font","arial","fontsz",10
);

sub new {
  my($class)  = shift;
  my $classname=shift;
  my($this)=typeid->new($classname);
  for($ii=0;$ii<scalar(@_)-1;$ii+=2) {
    $this->{$_[$ii]}=$_[$ii+1] if (exists $simplechartprops{$_[$ii]});
  }
  foreach my $key (keys %simplechartprops) {
    $this->{$key}=$simplechartprops{$key} if (!exists $this->{$key});
  }
  bless $this,$class;
}

sub draw {
  my($this)  = shift;
  print "draw " . $this . "\n";
}


package simplechart2;

@ISA = (simplechart);

%simplechart2props=(
  "axes",0,
  "graphs",0
);

sub new {
  my($class) = shift;
  my($this) = simplechart->new($class,@_);
  for($ii=0;$ii<scalar(@_)-1;$ii+=2) {
    $this->{$_[$ii]}=$_[$ii+1] if (exists $simplechart2props{$_[$ii]});
  }
  foreach my $key (keys %simplechart2props) {
    next if (exists $this->{$key});
    if ($key eq "axes" || $key eq "graphs") {
      $this->{$key}=[];
    }
    else {
      $this->{$key}=$simplechart2props{$key};
    }
  }
  return(bless($this,$class));
}

sub currState {
  my($this) = shift;
  my $ii;
  my $msg=Scalar::Util::blessed($this) . " :\n";
  foreach $key (sort keys %{$this}) {
    if (ref($this->{$key}) ne "") {
      if ($key eq "axes" || $key eq "graphs") {
        foreach $ii (@{$this->{$key}}) {
          $msg.=$ii->currState();  
        }
      }
    }
    else {
      $msg.="key=" . $key . " value=" . $this->{$key} . "\n";
    }
  }
  return $msg . "\n";
}

sub axis {
  my($this)=shift;
  my $ask=typeid->fromname(shift);
  my $idx=calc::bsearch1($ask,\@{$this->{"axes"}},\&typeid::cmpTypeid);
  return $this->{"axes"}->[$idx] if ($idx>=0);
  return 0;
}

sub setAxis {
  my($this)=shift;
  my $ax=shift;
  my $idx=calc::bsearch1($ax,\@{$this->{"axes"}},\&typeid::cmpTypeid);
  $idx=calc::inSort($ax,\@{$this->{"axes"}},\&typeid::cmpTypeid)
    if ($idx<0);
  return $this->{"axes"}->[$idx]->{"name"};
}

sub graph {
  my($this)=shift;
  my $ask=typeid->fromname(shift);
  my $idx=calc::bsearch1($ask,\@{$this->{"graphs"}},\&typeid::cmpTypeid);
  return $this->{"graphs"}->[$idx] if ($idx>=0);
  return 0;
}

sub setGraph {
  my($this)=shift;
  my $graph=shift;
  my $idx=calc::bsearch1($graph,\@{$this->{"graphs"}},\&typeid::cmpTypeid);
  $idx=calc::inSort($graph,\@{$this->{"graphs"}},\&typeid::cmpTypeid)
    if ($idx<0);
  return $this->{"graphs"}->[$idx]->{"name"};
}

sub draw {
  my($this)=shift;
  $this->PARENT::draw();
}


1;


