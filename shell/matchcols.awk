#!/usr/bin/awk -f

BEGIN {
  if (ARGC<4) {
    print "usage " ENVIRON["_"] " beg-col " " text " " file"
    exit 1;
  }
  begcol=ARGV[1];
  text=ARGV[2] ;
  ARGV[1]=ARGV[3];
  delete ARGV[3];
  delete ARGV[2]; 
  if (begcol<0)
    begcol=0;
  FS="";
}

{
  endcol=begcol+length(text);
  if (endcol<NF) {
    smatch="";
    for (ii=begcol;ii<endcol;ii++)
      smatch=smatch$ii;
    if (smatch == text) {
      print;    
    }
  }
}

END {
  
}


