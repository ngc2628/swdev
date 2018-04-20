#!/usr/bin/Perl

BEGIN {
  $perlxx=$ENV{"PERLMK"} . "/xx";
}

use lib $perlxx;
use AutoLoader 'AUTOLOAD';
use Scalar::Util 'blessed';
use util::Calc;
use POSIX;

package typeid;

sub new {
  my($class)=shift;
  my($this)={"name" => shift . "-" . calc::uniqueNum()};
  bless $this,$class;
}

sub fromname {
  my($class)=shift;
  my($this)={"name" => shift};
  bless $this,$class;
}

sub cmpTypeid {
  return -1 if ($_[0]->{"name"} lt $_[1]->{"name"});
  return 1 if ($_[1]->{"name"} lt $_[0]->{"name"});
  return 0;
}

sub currState {
  my($this) = shift;
  my $refinfo;
  my $msg=Scalar::Util::blessed($this) . "\n";
  foreach $key (sort keys %{$this}) {
    $refinfo=ref($this->{$key});
    if ($refinfo ne "") {
      $msg.="key=" . $key . " value=" . $refinfo . "\n";
      $msg.=$this->{$key}->currState()
        if ($refinfo ne "ARRAY" && $refinfo ne "HASH" && $refinfo ne "REF");
    }
    else {
      $msg.="key=" . $key . " value=" . $this->{$key} . "\n";
    }
  }
  return $msg . "\n";
}

return 1;
