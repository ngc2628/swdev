#!/bin/bash

/usr/bin/perl -e "$(/usr/bin/cat <<'EOM'
  print "the quick striped tiger\n";
  print "jumps over the lazy fox\n";
EOM
)"

[ $# -le 0 ] && exit 0;

/usr/bin/perl -e "$(/usr/bin/cat <<'EOM'
  $ff=$ARGV[0];
  open (FH, "-|", "/usr/bin/readlink -m $ff");
  $fname=<FH>;
  chop $fname;
  close (FH);
  print "file : " . $fname . "\n";
  open(FH,"<","$fname") || die &usage ("error :","  cannot open \"$fname\""); 
  while (<FH>) {
    $str=$_;
    chomp($str);
    # print __LINE__ . "<" . $str . ">\n";
    print $str . "\n" if ($str=~s/^(?!\s*#)\s*xxx\s*=\s*(\w+).*/$1/);
  }
EOM
)" $1

exit 0

