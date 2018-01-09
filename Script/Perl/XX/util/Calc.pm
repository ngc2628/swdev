#!/usr/bin/perl

use Time::HiRes 'gettimeofday';
use AutoLoader 'AUTOLOAD';

package calc;

$uilimit=4294967295;
$rad=0.0174532925199432957694;

@basechar=('0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F','G','H','I',
        'J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z','_','0');

@powtablarge=(1.,1.e1,1.e2,1.e3,1.e4,1.e5,1.e6,1.e7,1.e8,1.e9,1.e10,
        1.e11,1.e12,1.e13,1.e14,1.e15,1.e16,1.e17,1.e18,1.e19,1.e20,
        1.e21,1.e22,1.e23,1.e24,1.e25,1.e26,1.e27,1.e28,1.e29,1.e30,
        1.e31,1.e32,1.e33,1.e34,1.e35,1.e36,1.e37,1.e38,1.e39,1.e40,
        1.e41,1.e42,1.e43,1.e44,1.e45,1.e46,1.e47,1.e48,1.e49,1.e50,
        1.e51,1.e52,1.e53,1.e54,1.e55,1.e56,1.e57,1.e58,1.e59,1.e60,
        1.e61,1.e62,1.e63,1.e64,1.e65,1.e66,1.e67,1.e68,1.e69,1.e70,
        1.e71,1.e72,1.e73,1.e74,1.e75,1.e76,1.e77,1.e78,1.e79,1.e80,
        1.e81,1.e82,1.e83,1.e84,1.e85,1.e86,1.e87,1.e88,1.e89,1.e90,
        1.e91,1.e92,1.e93,1.e94,1.e95,1.e96,1.e97,1.e98,1.e99,1.e100,
        1.e101,1.e102,1.e103,1.e104,1.e105,1.e106,1.e107,1.e108,1.e109,1.e110,
        1.e111,1.e112,1.e113,1.e114,1.e115,1.e116,1.e117,1.e118,1.e119,1.e120,
        1.e121,1.e122,1.e123,1.e124,1.e125,1.e126,1.e127,1.e128,1.e129,1.e130,
        1.e131,1.e132,1.e133,1.e134,1.e135,1.e136,1.e137,1.e138,1.e139,1.e140,
        1.e141,1.e142,1.e143,1.e144,1.e145,1.e146,1.e147,1.e148,1.e149,1.e150,
        1.e151,1.e152,1.e153,1.e154,1.e155,1.e156,1.e157,1.e158,1.e159,1.e160,
        1.e161,1.e162,1.e163,1.e164,1.e165,1.e166,1.e167,1.e168,1.e169,1.e170,
        1.e171,1.e172,1.e173,1.e174,1.e175,1.e176,1.e177,1.e178,1.e179,1.e180,
        1.e181,1.e182,1.e183,1.e184,1.e185,1.e186,1.e187,1.e188,1.e189,1.e190,
        1.e191,1.e192,1.e193,1.e194,1.e195,1.e196,1.e197,1.e198,1.e199,1.e200,
        1.e201,1.e202,1.e203,1.e204,1.e205,1.e206,1.e207,1.e208,1.e209,1.e210,
        1.e211,1.e212,1.e213,1.e214,1.e215,1.e216,1.e217,1.e218,1.e219,1.e220,
        1.e221,1.e222,1.e223,1.e224,1.e225,1.e226,1.e227,1.e228,1.e229,1.e230,
        1.e231,1.e232,1.e233,1.e234,1.e235,1.e236,1.e237,1.e238,1.e239,1.e240,
        1.e241,1.e242,1.e243,1.e244,1.e245,1.e246,1.e247,1.e248,1.e249,1.e250,
        1.e251,1.e252,1.e253,1.e254,1.e255,1.e256,1.e257,1.e258,1.e259,1.e260,
        1.e261,1.e262,1.e263,1.e264,1.e265,1.e266,1.e267,1.e268,1.e269,1.e270,
        1.e271,1.e272,1.e273,1.e274,1.e275,1.e276,1.e277,1.e278,1.e279,1.e280,
        1.e281,1.e282,1.e283,1.e284,1.e285,1.e286,1.e287,1.e288,1.e289,1.e290,
        1.e291,1.e292,1.e293,1.e294,1.e295,1.e296,1.e297,1.e298,1.e299,1.e300,
        1.e301,1.e302,1.e303,1.e304,1.e305,1.e306,1.e307,1.e308);

@powtabtiny=(1.,1.e-1,1.e-2,1.e-3,1.e-4,1.e-5,1.e-6,1.e-7,1.e-8,1.e-9,1.e-10,
        1.e-11,1.e-12,1.e-13,1.e-14,1.e-15,1.e-16,1.e-17,1.e-18,1.e-19,1.e-20,
        1.e-21,1.e-22,1.e-23,1.e-24,1.e-25,1.e-26,1.e-27,1.e-28,1.e-29,1.e-30,
        1.e-31,1.e-32,1.e-33,1.e-34,1.e-35,1.e-36,1.e-37,1.e-38,1.e-39,1.e-40,
        1.e-41,1.e-42,1.e-43,1.e-44,1.e-45,1.e-46,1.e-47,1.e-48,1.e-49,1.e-50,
        1.e-51,1.e-52,1.e-53,1.e-54,1.e-55,1.e-56,1.e-57,1.e-58,1.e-59,1.e-60,
        1.e-61,1.e-62,1.e-63,1.e-64,1.e-65,1.e-66,1.e-67,1.e-68,1.e-69,1.e-70,
        1.e-71,1.e-72,1.e-73,1.e-74,1.e-75,1.e-76,1.e-77,1.e-78,1.e-79,1.e-80,
        1.e-81,1.e-82,1.e-83,1.e-84,1.e-85,1.e-86,1.e-87,1.e-88,1.e-89,1.e-90,
        1.e-91,1.e-92,1.e-93,1.e-94,1.e-95,1.e-96,1.e-97,1.e-98,1.e-99,1.e-100,
        1.e-101,1.e-102,1.e-103,1.e-104,1.e-105,1.e-106,1.e-107,1.e-108,1.e-109,1.e-110,
        1.e-111,1.e-112,1.e-113,1.e-114,1.e-115,1.e-116,1.e-117,1.e-118,1.e-119,1.e-120,
        1.e-121,1.e-122,1.e-123,1.e-124,1.e-125,1.e-126,1.e-127,1.e-128,1.e-129,1.e-130,
        1.e-131,1.e-132,1.e-133,1.e-134,1.e-135,1.e-136,1.e-137,1.e-138,1.e-139,1.e-140,
        1.e-141,1.e-142,1.e-143,1.e-144,1.e-145,1.e-146,1.e-147,1.e-148,1.e-149,1.e-150,
        1.e-151,1.e-152,1.e-153,1.e-154,1.e-155,1.e-156,1.e-157,1.e-158,1.e-159,1.e-160,
        1.e-161,1.e-162,1.e-163,1.e-164,1.e-165,1.e-166,1.e-167,1.e-168,1.e-169,1.e-170,
        1.e-171,1.e-172,1.e-173,1.e-174,1.e-175,1.e-176,1.e-177,1.e-178,1.e-179,1.e-180,
        1.e-181,1.e-182,1.e-183,1.e-184,1.e-185,1.e-186,1.e-187,1.e-188,1.e-189,1.e-190,
        1.e-191,1.e-192,1.e-193,1.e-194,1.e-195,1.e-196,1.e-197,1.e-198,1.e-199,1.e-200,
        1.e-201,1.e-202,1.e-203,1.e-204,1.e-205,1.e-206,1.e-207,1.e-208,1.e-209,1.e-210,
        1.e-211,1.e-212,1.e-213,1.e-214,1.e-215,1.e-216,1.e-217,1.e-218,1.e-219,1.e-220,
        1.e-221,1.e-222,1.e-223,1.e-224,1.e-225,1.e-226,1.e-227,1.e-228,1.e-229,1.e-230,
        1.e-231,1.e-232,1.e-233,1.e-234,1.e-235,1.e-236,1.e-237,1.e-238,1.e-239,1.e-240,
        1.e-241,1.e-242,1.e-243,1.e-244,1.e-245,1.e-246,1.e-247,1.e-248,1.e-249,1.e-250,
        1.e-251,1.e-252,1.e-253,1.e-254,1.e-255,1.e-256,1.e-257,1.e-258,1.e-259,1.e-260,
        1.e-261,1.e-262,1.e-263,1.e-264,1.e-265,1.e-266,1.e-267,1.e-268,1.e-269,1.e-270,
        1.e-271,1.e-272,1.e-273,1.e-274,1.e-275,1.e-276,1.e-277,1.e-278,1.e-279,1.e-280,
        1.e-281,1.e-282,1.e-283,1.e-284,1.e-285,1.e-286,1.e-287,1.e-288,1.e-289,1.e-290,
        1.e-291,1.e-292,1.e-293,1.e-294,1.e-295,1.e-296,1.e-297,1.e-298,1.e-299,1.e-300,
        1.e-301,1.e-302,1.e-303,1.e-304,1.e-305,1.e-306,1.e-307,1.e-308);

sub ipow10 {
  my $n=shift;
  $n=-308 if ($n<-308);
  $n=308 if ($n>308);
  my $res=($n<0 ? $powtabtiny[-$n] : $powtablarge[$n]);
  return $res;
}

sub log10 {
  my $n = shift;
  $n=-$n if ($n<0);
  return $n==0 ? 0 : log($n)*0.434294481903252;
}

sub nextlowint {
  my $n=shift;
  my $fln=int($n);
  my $df=$n-$fln;
  $df=-$df if ($df<.0);
  $fln-=1 if ($n<.0 && 1.e-15<$df);
  return $fln;
}

sub mag {
  my $num=shift;
  return 0 if ($num==.0);
  $num=-$num if ($num<.0);
  $num=log10($num)+ipow10(-15);
  return nextlowint($num);
}

sub swap {
  my $tmp=$_[0];
  $_[0]=$_[1];
  $_[1]=$tmp;
}

sub atan {
  my $valrad=shift;
  return .0 if (!defined($valrad) || $valrad==.0);
  return atan2($valrad,1.)
}

sub acos {
  my $valrad=shift;
  return .0 if (!defined($valrad) || $valrad==.0);
  return atan2(sqrt(1.-$valrad*$valrad),$valrad);
}

sub asin {
  my $valrad=shift;
  return .0 if (!defined($valrad) || $valrad==.0);
  return atan2($valrad,sqrt(1.-$valrad*$valrad));
}

sub d2b {
  return unpack('b*',pack('d',shift));
}

sub b2d {
  return unpack('d',pack('b*',shift));
}

sub dsgn {
  return substr(d2b(shift),63,1) eq "0" ? 1 : -1;
}

sub dbusted {
  my $num=shift;
  return "nan" if (!defined($num));
  $num=~s/^\s*(\S+)\s*$/$1/;
  return "nan" if ($num=~/^$/);
  return "" if ($num=~/^[+-]?0*[\.]?0*([eE][+-]?[0-9]+)?$/);
  $num=d2b($num);
  return "nan" if ($num=~/^0*$/);
  return "" if (substr($num,52,11)!~/^1{11}$/);
  my $sgn=substr($num,63,1) eq "0" ? 1 : -1;
  return $sgn==1 ? "inf" : "-inf" if (substr($num,0,52)=~/^0{52}$/);
  return $sgn==1 ? "nan" : "-nan" 
}

sub isZero {
  my $num=shift;
  $num=~s/^\s*(\S+)\s*$/$1/;
  return 1 if ($num!~/^$/ && $num=~/^[+-]?0*[\.]?0*([eE][+-]?[0-9]+)?$/);
  return 0;
}

sub i2a {
  my $num=shift;
  my $base=int(shift);
  $base=10 if (!defined $base || $base<2 || $base>37);
  my $pos=0;
  my @strarr=();
  do {
    $strarr[$pos]=$basechar[int($num%$base)];
    $num=int($num/$base);
    $pos++;
  } while ($num>0);
  return join("",reverse(@strarr));
}

sub a2i {
  my $numstr=uc(shift);
  my $pbase=shift;
  $numstr=~s/^\s*(\S+)\s*$/$1/;
  return "nan" if ($numstr=~/^$/);
  my $base=(defined($pbase) ? (ref($pbase) ne "" ? $$pbase : $pbase) : -1);
  $base=-1 if ($base eq "" || $base<2 || $base>37);
  my @numarr=split("",$numstr);
  my $sgn=$numarr[0];
  shift(@numarr) if ($sgn eq "+" || $sgn eq "-");
  if ($base==-1 || $base==16) {
    if ($numarr[0] eq "#" || ($numarr[0] eq "0" && $numarr[1] eq "X")) {
      shift(@numarr);
      shift(@numarr) if ($numarr[0] eq "X");
      $base=16 if ($base==-1);
    }
  }
  my $ii=0;
  my $jj=0;
  my $kk=-1;
  my $ll=-1;
  for ($ii=0;$ii<scalar(@numarr);$ii++) {
    $ll=-1;
    for ($jj=0;$jj<37;$jj++) {
      if ($numarr[$ii] eq $basechar[$jj]) {
        $kk=$jj if ($jj>$kk);
        $ll=$jj;
        last;
      }
    }
    return "nan" if ($ll==-1);
  }
  return 0 if ($kk==0);
  if ($base==-1 && $numarr[0] eq "0" && $kk<8) {
    $base=8;
    shift(@numarr);
  }
  if ($base==-1) {
    if ($kk<10) {
      $base=10;
    }
    else {
      $base=37;
    }
  }
  $$pbase=$base if (defined($pbase) && ref($pbase) ne "");
  my $num=0;
  my $mult=1;
  for ($ii=scalar(@numarr)-1;$ii>-1;$ii--)  {
    $kk=-1;
    for ($jj=0;$jj<$base;$jj++) {
      if ($numarr[$ii] eq $basechar[$jj]) {
        $kk=$jj;
        last;
      }

    }
    $num+=$kk*$mult;
    $mult*=$base;
  }
  return ($sgn eq "-" ? -$num : $num);
}

sub uniqueNum {
  my ($t,$mt)=Time::HiRes::gettimeofday();
  return i2a($t,37) . i2a($mt,37);
}

sub roundPos {
  my $num=shift;
  my $pos=shift;
  my $eps=shift;
  my $bust=dbusted($num);
  return $bust if ($bust ne "");
  my $sgn=dsgn($num);
  my $res=($sgn==-1 ? -$num : $num);
  return $sgn*int($res+.5+$eps) if ($pos==0 && $res<$uilimit);
  my $sh=$res*ipow10($pos);
  return $sgn*int($sh+.5+$eps)*ipow10(-$pos) if ($sh<$uilimit);
  return $num;
}

sub roundUp {
  my $num=shift;
  my $pos=shift;
  return ipow10(-$pos) if ($num==.0);
  my $rd=roundPos($num,$pos);
  if ($rd<$num) {
    $num+=ipow10(-$pos);
    return .0 if ($num>.0 && $rd<.0);
    $rd=roundPos($num,$pos);
  }
  return $rd; 
}

sub roundDown {
  my $num=shift;
  my $pos=shift;
  return ipow10(-$pos) if ($num==.0);
  my $rd=roundPos($num,$pos);
  if ($num<$rd) {
    $num-=ipow10(-$pos);
    return .0 if ($num<.0 && $rd>.0);
    $rd=roundPos($num,$pos);
  }
  return $rd; 
}

sub ddiff {
  my $df=$_[0]-$_[1];
  return .0 if ($df==.0);
  if (defined($_[2])) {
    return .0 if (($df>.0 && $df<$_[2]) || ($df<.0 && -$df<$_[2]));
    return $df;
  }
  my $ma1=mag($_[0]);
  return .0 if ($ma1==mag($_[1]) && mag($df)<$ma1-15);
  return $df;
}

sub hsort1 {
  my ($arr,$fcmp)=@_;
  my $nn=scalar(@{$arr});
  return if ($nn<2);
  my $ii=0;
  my $jj=0;
  my $ub=$nn-1;
  my $mb=int($nn/2);
  my $tmp;
    
  while ($mb>-1) {
    if ($mb==0) { 
      $tmp=$arr->[$ub]; 
      $arr->[$ub]=$arr->[0]; 
      if (--$ub==0) { 
        $arr->[0]=$tmp;
        last;
      }
    }
    else {
      $tmp=$arr->[--$mb];
    }
    $ii=$mb;
    $jj=$ii+$ii+1;
    while($jj<=$ub) { 
      if ($jj<$ub) {
        $jj++ if ($fcmp->($arr->[$jj],$arr->[$jj+1])<0);
      }
      if ($fcmp->($tmp,$arr->[$jj])<0) {
        $arr->[$ii]=$arr->[$jj]; 
        $ii=$jj;
        $jj+=($ii+1);
      }
      else {
        last;
      }
    }
    $arr->[$ii]=$tmp;
  }
}

sub bsearch1 {
  my ($ask,$arr,$fcmp)=@_;
  my $lb=-1;
  my $mb=0;
  my $ub=scalar(@{$arr});
  my $cmp=0;
  
  while (($ub-$lb)>1) {
    $mb=int(($ub+$lb)/2);
    $cmp=$fcmp->($arr->[$mb],$ask);
    return $mb if ($cmp==0);
    if ($cmp<0) {
      $lb=$mb;
    }
    else {
      $ub=$mb;
    }
  }
  return -1; 
}

sub nextIdx {
  my ($ask,$arr,$fcmp)=@_;
  my $lb=-1;
  my $mb=0;
  my $mblast=-1;
  my $cnt=scalar(@{$arr});
  my $ub=$cnt;
  my $cmpres=0;
  while (($ub-$lb>1)) {
    $mb=int(($ub+$lb)/2);
    last if ($mb==$mblast);
    $cmpres=$fcmp->($arr->[$mb],$ask);
    if ($cmpres==0) {
      return $cnt if ($mb>=($cnt-1));
      return ($mb+1);
    }
    if ($cmpres<0) {
      $lb=$mb;
    }
    else {
      $ub=$mb;
    }
    $mblast=$mb;
  }
  return 0 if ($ub<=0);
  return ($ub>=$cnt ? $cnt : $ub);
}

sub inSort {
  my ($ask,$arr,$fcmp)=@_;
  my $cnt=scalar(@{$arr});
  my $ii=0;
  my $idx=nextIdx($ask,$arr,$fcmp);
  for ($ii=$cnt;$ii>$idx;$ii--) {
    $arr->[$ii]=$arr->[$ii-1];
  }
  $arr->[$idx]=$ask;
  return $idx;
}
  
1;
