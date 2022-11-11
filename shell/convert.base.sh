#!/bin/bash

# convert integer number "num" from base "ibase" to base "obase"
# base range 2...37 (2,3,...,a,b,...z)
# e.g. convert.base.sh --num 44027 --ibase 10 --obase 36  =  XYZ
#      convert.base.sh --num ffee --ibase 16 --obase 2    =  1111111111101110  (65518)
#

declare -i maxint=9223372036854775807
declare -i maxbase=37
declare -a baseascii=( 48 49 50 51 52 53 54 55 56 57 65 66 67 68 69 70 71 72 73 74 75 76 77 78 79 80 81 82 83 84 85 86 87 88 89 90 95 48 ) 
declare -a basechar=( "0" "1" "2" "3" "4" "5" "6" "7" "8" "9" "A" "B" "C" "D" "E" "F" "G" "H" "I" "J" "K" "L" "M" "N" "O" "P" "Q" "R" "S" "T" "U" "V" "W" "X" "Y" "Z" "_" "0" )

function usage() {

  trap 0 1
  [ $# -gt 0 ] && /usr/bin/echo -e "\n$1"
  /usr/bin/echo -e "
    usage \"$0\"\n\
    \"--num :  input ^[0-9a-z_]+$\"\n\
    \"--obase : 2...37\"\n\
    \"--ibase : 2...37\"\n\
    \"[--help|-h|/?]\""
  /usr/bin/echo
  exit 1

}

function inthandler() {

  declare -i bailout=$1
  trap 0 1
  if [ $bailout -gt 1 ]; then
    /usr/bin/echo "Programmende durch Signal <$(/usr/bin/kill -l $bailout)>"
    bailout=1
  fi
  exit $bailout

}

path_script=$(/usr/bin/dirname $(/usr/bin/readlink -m $0))
path_curr="$(/usr/bin/pwd -P)"

declare -u inum=""
declare -i obase=0
declare -i ibase=0

while [ $# -gt 0 ]; do
  arg="${1}"
  shift
  if [ "${arg}" == "--num" ]; then
    if [ $# -gt 0 ]; then
      inum="${1}"
      shift
    fi
  elif [ "${arg}" == "--obase" ]; then
    if [ $# -gt 0 ]; then
      obase=$1
      shift
    fi
  elif [ "${arg}" == "--ibase" ]; then
    if [ $# -gt 0 ]; then
      ibase=$1
      shift
    fi
  elif [ "${arg}" == "--help" -o "${arg}" == "-h" -o "${arg}" == "/?" ]; then
    usage
  else
    /usr/bin/echo "Warnung : unbekannter Parameter \"${arg}\""
  fi
done

[ -z "${inum}" ] && usage "\number\" is empty"
[ $ibase -lt 2 -o $ibase -gt $maxbase ] && usage "ibase $maxbase>$ibase>1"
[ $obase -lt 2 -o $obase -gt $maxbase ] && usage "obase $maxbase>$obase>1"

umask 0002
traps=(0 1 2 6 11 15)
for tt in ${traps[@]}; do
  trap "inthandler $tt" $tt
done

declare -i cc=0
declare -i idx=0
declare -i mult=1
declare -i ll=${#inum}
declare -i inum10=0

for ((ii=$ll-1;ii>-1;ii--)); do
  idx=-1
  cc=$(LC_CTYPE=C printf "%d" \'${inum:$ii:1})
  for ((jj=0;jj<$ibase;jj++)); do
    if [ $cc -eq ${baseascii[$jj]} ]; then
      idx=$jj
      break
    fi
  done
  [ $idx -lt 0 ] && usage "cannot parse \"$inum\" as base \"$ibase\"" 
  ((inum10+=idx*mult))
  [ $inum10 -lt 0 -o $inum10 -eq $maxint ] && usage "number \"$inum\" too large"
  ((mult*=ibase))    
done

[ $inum10 -lt 0 ] && usage "\"$inum\" too large"

declare -u onum=""
idx=-1

while [ $inum10 -gt 0 ]; do
  idx=$((inum10%obase))
  [ $idx -lt 0 -o $idx -ge $obase ] && usage "cannot convert \"$inum\" to base \"$obase\""
  onum+="${basechar[$idx]}"
  ((inum10/=obase))
done
[ -z "${onum}" ] && onum="0"

/usr/bin/echo "${onum}" | /usr/bin/rev

exit 0

