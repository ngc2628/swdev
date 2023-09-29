#!/usr/bin/perl

use feature 'say';
use Cwd;
use sigtrap 'handler',\&inthandler,'normal-signals';
use sigtrap 'handler',\&inthandler,'error-signals';
use Config;

sub usage {

  my $nargs=@_;
  say $_[0] if ($nargs>0);
  print "\n$0 :
    usage \"$0\"
    \"name /path/to/outfile\"
    \"[--par parname str|int|arr comment] [--par parname str|int|arr comment] ...\"
    \"[--help|-h|/?]\"\n\n";

  exit 1;

}

sub inthandler {

  my $bailout=0;
  $SIG{'ZERO'}='DEFAULT';
  $SIG{'HUP'}='DEFAULT';
  $bailout=$static_sigL{$_[0]} if (@_>0);
  if ($bailout>1) {
    say "emergency exit <$bailout>";
    $bailout=1;
  }

  exit $bailout;
  
}

sub arridx {
  
  my $idx=0;
  for ($idx=1;$idx<=scalar @_;$idx++) {
    return $idx if ($_[0] eq $_[$idx]);
  }
  return -1;

}

sub makesigL {

  my @signames=split(" ",$Config{sig_name});
  my @signums=split(" ",$Config{sig_num});
  for ($ii=0;$ii<=$#signames;$ii++) {
    $static_sigL{$signames[$ii]}=$signums[$ii];
  }

  return 0;

}

say;
umask 002;
%static_sigL;
&makesigL;
@static_partypes=("str","int","arr");
$path_script=Cwd::realpath($0);
$path_curr=Cwd::getcwd();
$fout;
%parL;
@parkeyL;
$parvalsref;
$par;
$arg;
$parname;
$partype;
$parcnt=0;
$ii=0;

while ($#ARGV>=0) {
  $arg=$ARGV[0];
  shift;
  if ($arg eq "--par") { 
    $parname="";
    $partype="";
    while ($#ARGV>=0) {
      last if (substr($ARGV[0],0,2) eq "--");
      if ($parname eq "") {
        $parname=$ARGV[0];
        $parL{$parname}=[];
      }
      elsif ($partype eq "") {
        $partype=$ARGV[0];
        usage "unknown parameter type" if (&arridx($partype,@static_partypes)<0);
        $parvalsref=$parL{$parname};
        push (@$parvalsref,$partype);
      }
      else {
        $parvalsref=$parL{$parname};
        push (@$parvalsref,$ARGV[0]);
      }
      shift;
    }
  }
  elsif ($arg eq "--help" || $arg eq "-h" || $arg eq "/?") {
    usage();
  }
  else {
    if(defined $fout) {
      say "Warning : unknown parameter \"$arg\"";
    }
    else {
      $fout=$arg;
    }
  }
}

usage ("outfile missing") if (($fout // "") eq "");

$path_out=Cwd::realpath($fout);
usage "cannot write to \"$path_out" if (-d $path_out || (-e $path_out && ! -w $path_out));

$parL{"fin"}=["str","input file"];
$parL{"fout"}=["str","output file"];
@parkeyL=keys %parL;
open (FH,">",$path_out);
print FH "#!/usr/bin/perl\n\n";
print FH "# mods : say (print \\n), cwd (real paths) sigtrap+config (signal handling)\n\n";
print FH "use feature \'say\';\n";
print FH "use Scalar::Util \'refaddr\',\'reftype\';\n";
print FH "use bigint;\n";
print FH "use Cwd;\n";
print FH "use sigtrap \'handler\', \\&inthandler, \'normal-signals\';\n";
print FH "use sigtrap \'handler\', \\&inthandler, \'error-signals\';\n";
print FH "use Config;\n";
print FH "use File::Basename;\n";
print FH "use File::Which;\n\n";

print FH "# util subs\n\n";
print FH "sub usage {\n\n";
print FH "  my \$nargs=\@_;\n";
print FH "  say \$_[0] if (\$nargs>0);\n";
print FH "  say \"\$0 :\";\n";
print FH "  say \"usage \$path_script\";\n";
for $parname (@parkeyL) {
  print FH "  print \"  [--$parname : ";
  $parvalsref=$parL{$parname};
  for ($ii=1;$ii<=$#$parvalsref;$ii++) {
#    print FH "$parvalsref->[$ii] ";
    print FH "$$parvalsref[$ii] ";
  }
  print FH "]\\n\";\n";
}
print FH "  say \"  [--help|-h|/?]\";\n\n";
print FH "  exit 1;\n\n";
print FH "}\n\n";
print FH "sub inthandler {\n\n";
print FH "  my \$bailout=0;\n";
print FH "  \$SIG{\'ZERO\'}=\'DEFAULT\';\n";
print FH "  \$SIG{\'HUP\'}=\'DEFAULT\';\n";
print FH "  \$bailout=\$static_sigL{\$_[0]} if (\@_>0);\n";
print FH "  if (\$bailout>1) {\n";
print FH "    say \"emergency exit \$bailout\";\n";
print FH "    \$bailout=1;\n  }\n\n";
print FH "  exit \$bailout;\n\n}\n\n";
print FH "sub arridx {\n\n";
print FH "  my \$idx=0;\n";
print FH "  for (\$idx=1;\$idx<=scalar \@_;\$idx++) {\n";
print FH "    return \$idx if (\$_[0] eq \$_[\$idx]);\n  }\n\n  return -1\n\n}\n\n";
print FH "sub makesigL {\n\n";
print FH "  my \@signames=split(\" \",\$Config{sig_name});\n";
print FH "  my \@signums=split(\" \",\$Config{sig_num});\n";
print FH "  for (\$ii=0;\$ii<=$#signames;\$ii++) {\n";
print FH "    \$static_sigL{\$signames[\$ii]}=\$signums[\$ii];\n  }\n\n  return 0;\n\n}\n\n";

print FH "# declarations for asked parameters from cmdline args + basics + temporaries\n\n";
print FH "\$path_script=Cwd::realpath(\$0);\n";
print FH "\$path_curr=Cwd::getcwd();\n";
for $parname (@parkeyL) {
  $parvalsref=$parL{$parname};
  if ($#$parvalsref>=0) {
    ($$parvalsref[0] eq "arr") ? print FH "\@$parname=();\n" : 
      ($$parvalsref[0] eq "int") ? print FH "\$$parname=0;\n" : 
        print FH "\$$parname=\"\";\n";
  }
}
print FH "\$arg=\"\";\n";
print FH "\$parcnt=0;\n";
print FH "\%static_sigL;\n";
print FH "\&makesigL;\n\n";
print FH "# parse and set cmdline args\n\n";
print FH "while (\$#ARGV>=0) {\n";
print FH "  \$arg=\$ARGV[0];\n";
print FH "  shift;\n";
print FH "  usage if (\$arg eq \"--help\" || \$arg eq \"-h\" || \$arg eq \"/?\");\n";
for $parname (@parkeyL) {
  ($parcnt++==0) ? print FH "  if (" : print FH "  elsif (";
  print FH "\$arg eq \"--$parname\") {\n"; 
  print FH "    while (\$#ARGV>0 && substr(\$ARGV[0],0,2) ne \"--\") {\n";
  $parvalsref=$parL{$parname};
  ($#$parvalsref>=0 && $$parvalsref[0] eq "arr") ? 
    print FH "      push (\@$parname,\$ARGV[0]);\n" : 
      print FH "      \$$parname=\$ARGV[0];\n";  
  print FH "      shift;\n    }\n  }\n";
}
print FH "  else {\n";
print FH "    say \"warning , unknown parameter \$arg\";\n  }\n";
print FH "}\n\n";
print FH "umask 002; # collaboration\n\n";

print FH "# mostly operate on data files ...\n";
print FH "# e.g. can read and write all when file is small ...\n\n"; 

print FH "sub read_input {\n\n";
print FH "  my (\$fname,\$lnref)=\@_;\n";
print FH "  print __LINE__ . \" \" . reftype(\$lnref) . \"-\" . ";
print FH "(refaddr(\$lnref)+0)->as_hex() . \"\\n\";\n";
print FH "  my \$lncnt=0;\n";
print FH "  return \$lncnt if (\$fname eq \"\");\n";
print FH "  \$fname=Cwd::realpath(\$fname);\n";
print FH "  return \$lncnt if (! -e \$fname || ! -f \$fname || ! -r \$fname);\n";
print FH "  return \$lncnt if (\! defined \$lnref);\n";
print FH "  open (FH,\"<\",\$fname);\n";
print FH "  while (<FH>) {\n";
print FH "    chomp(\$_);\n";
print FH "    push(\@\$lines,\$_);\n";
print FH "    \$lncnt++;\n  }\n";
print FH "  close(FH);\n\n";
print FH "  return \$lncnt;\n\n}\n\n";

print FH "sub process_input {\n\n";
print FH "  my \$lnref=\$_[0];\n";
print FH "  print __LINE__ . \" \" . reftype(\$lnref) . \"-\" . ";
print FH "(refaddr(\$lnref)+0)->as_hex() . \"\\n\";\n";
print FH "  my \$lncnt=0;\n";
print FH "  return \$lncnt if (\! defined \$lnref);\n";
print FH "  my \$line=\"\";\n";
print FH "  my \$ii=0;\n";
print FH "  for (\$ii=\$#\$lnref;\$ii>-1;\$ii--) {\n";
print FH "    if (\$\$lnref[$ii]=~/^\s*\$/) { # skip and remove empty line\n";
print FH "      splice(\@\$lnref,\$ii,1);\n";
print FH "      next;\n    }\n";
print FH "    \$line=\$_; # do something with input line \n";
print FH "    \$lncnt++;\n  }\n";
print FH "  return \$lncnt;\n\n}\n\n";

print FH "sub write_output {\n\n";
print FH "  my (\$fname,\$lnref)=\@_;\n";
print FH "  print __LINE__ . \" \" . reftype(\$lnref) . \"-\" . ";
print FH "(refaddr(\$lnref)+0)->as_hex() . \"\\n\";\n";
print FH "  my \$lncnt=0;\n";
print FH "  return \$lncnt if (\$fname eq \"\");\n";
print FH "  \$fname=Cwd::realpath(\$fname);\n";
print FH "  return \$lncnt if (-e \$fname && (! -f \$fname || ! -w \$fname));\n";
print FH "  return \$lncnt if (! -w dirname(\$fname));\n";
print FH "  return \$lncnt if (\! defined \$lnref);\n";
print FH "  open (FH,\">\",\$fname);\n";
print FH "  for (\@\$lnref) {\n";
print FH "    print FH \$_ . \"\\n\";\n";
print FH "    \$lncnt++;\n  }\n";
print FH "  close(FH);\n\n";
print FH "  return \$lncnt;\n\n}\n\n";

print FH "\@lines=();\n";
print FH "\$cntln=0;\n";
print FH "\$cntln=\&read_input(\$fin,\\\@lines); # returns #lines read and put into array\n";
print FH "\$cntln=\&process_input(\\\@lines); # returns #lines remain in array\n";
print FH "\$cntln=\&write_output(\$fout,\\\@lines); # returns #lines written;\n\n";

print FH "# put some command output into array\n\n";
print FH "sub do_command {\n\n";
print FH "  my (\$cmd,\$outL)=\@_;\n";
print FH "  my \$ans=\$cmd . \" : command not found\";\n";
print FH "  my \@cmdL=split(\" \",\$cmd);\n";
print FH "  return \$ans if (\$#cmdL==0);\n";
print FH "  my \@cmdpath=File::Which::which(\$cmdL[0]);\n";
print FH "  return \$ans if (\$#cmdpath==0 || \$cmdpath[0] eq \"\");\n";
print FH "  \$ans=\"\";\n";
print FH "  open (FH,\"-|\",\$cmd);\n";
print FH "  while (<FH>) {\n";
print FH "    chomp(\$_);\n";
print FH "    push (\@\$out,\$_) if (defined \$outL);\n  }\n";
print FH "  close (FH);\n\n";
print FH "  return \$ans;\n\n}\n\n";

print FH "\@lines=();\n";
print FH "\$ans=\&do_command(\"some_cmd -opt -opt\",\\\@lines);\n\n";

print FH "exit 0;\n\n";
close(FH);
chmod 0775,$path_out;

exit 0;
