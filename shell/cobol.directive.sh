#!/bin/bash

# 07.08.2020 Martin Kloss
# fuegt Cobol Quell Dateien ggf. fehlende benoetigte Compiler Direktiven am Anfang der Datei hinzu  
#

# wrkdir=${1:-$(pwd)}

if [ $# -lt 1 ]; then
  /usr/bin/echo "usage : \"$0 path-to-directory\""
  exit 1
fi

fileL=( $(/usr/bin/find "$1" -regex ".*/.+\.\(pco\|cbl\)" -print0 | xargs -0 grep -L -i "\$set use") ) 

line1='      $set use(config/direktiven/common.dir)'
line2='      $set use(config/direktiven/default.dir)'
line3='      $set use(config/direktiven/notrunc.dir)'
line4='      $set use(config/direktiven/sql.dir)'
# line2='      $set use(config/direktiven/nossrange.dir)'

filecnt=${#fileL[@]}
/usr/bin/echo "$filecnt files"

ii=0
jj=0
kk=0
while (( $ii < $filecnt )); do

  fname="${fileL[$ii]}"
  ((ii++))
  if [ ! -f $fname ]; then
    continue
  fi

  nl=$(/usr/bin/cat $fname | /usr/bin/wc -l 2>/dev/null)

  /usr/bin/echo  "processing $fname ($nl lines) ... "

  if [ "${fname##*.}" == "pco" ]; then
    l2="NN"
    l3="$line3"
    l4="$line4"
  else
    l2="$line2"
    l3="NN"
    l4="NN"
  fi

  /usr/bin/perl -e '
    open(FH,"<",$ARGV[0]);
    @lines=();
    while ( <FH> ) {
      push @lines,$_;
    }
    close(FH);
    open(FH,">",$ARGV[0]);
    print FH $ARGV[1] . "\n";
    if ($ARGV[2] ne "NN") {
      print FH $ARGV[2] . "\n";
    }
    if ($ARGV[3] ne "NN") {
      print FH $ARGV[3] . "\n";
    }
    if ($ARGV[4] ne "NN") {
      print FH $ARGV[4] . "\n";
    }
    print FH @lines;
    close(FH);
  ' "$fname" "$line1" "$l2" "$l3" "$l4" 1>/dev/null 2>&1

  if [ $? -eq 0 ]; then
    ((kk++))
    continue
  fi

  /usr/bin/echo -n "reading "

  start=$(/usr/bin/date +"%s")

  ffL=()
  IFS=''
  while read line; do
    ffL=( ${ffL[@]} "$line" )
    jj=${#ffL[@]}
    if [ $((jj%100)) -eq 0 -o $jj -eq $nl ]; then
      echo -n "$jj "
    fi
  done < <(/usr/bin/cat $fname)
  unset IFS


  if [ ${#ffL[@]} -ne $nl ]; then
    echo "conversion error ${#ffL[@]} <-> $nl"
    continue
  fi

  errmsg=$(/usr/bin/echo "$line1" 2>&1 > $fname)
  if [ $? -ne 0 ]; then
    echo "$LINENO : $errmsg"
    continue
  fi 
  if [ ! "$l2" == "NN" ]; then
    /usr/bin/echo "$l2" >> $fname
  fi
  if [ ! "$l3" == "NN" ]; then
    /usr/bin/echo "$l3" >> $fname
  fi
  if [ ! "$l4" == "NN" ]; then
    /usr/bin/echo "$l4" >> $fname
  fi

  echo -n " ... writing ... "
  
  jj=0
  while [ $jj -lt $nl ]; do
    /usr/bin/echo "${ffL[$jj]}"  >> $fname
    ((jj++))
  done  

  end=$(/usr/bin/date +"%s")
  min=$(( (end-start)/60 ))
  sec=$(( (end-start)-min*60 ))

  /usr/bin/echo "patched (${min}min${sec}sec)"
  ((kk++))

done

/usr/bin/echo "$kk files converted"

