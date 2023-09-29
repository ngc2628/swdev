#!/bin/bash

function usage() {

  trap 0 1
  [ $# -gt 0 ] && /usr/bin/echo -e "\n$1"
  /usr/bin/echo -e "\n\
    usage \"$0\"\n\
    \"--ecol :  end  column\"\n\
    \"--scol :  start  column\"\n\
    \"--filepath\" filepath\n\
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

declare -i ecol=-1
declare -i scol=0
fpath=""

while [ $# -gt 0 ]; do
  arg="${1}"
  shift
  if [ "${arg}" == "--ecol" ]; then
    if [ $# -gt 0 ]; then
      ecol=${1}
      shift
    fi
  elif [ "${arg}" == "--scol" ]; then
    if [ $# -gt 0 ]; then
      scol=${1}
      shift
    fi
  elif [ "${arg}" == "--filepath" ]; then
    if [ $# -gt 0 ]; then
      fpath="${1}"
      shift
    fi
  elif [ "${arg}" == "--help" -o "${arg}" == "-h" -o "${arg}" == "/?" ]; then
    usage
  else
    /usr/bin/echo "Warnung : unbekannter Parameter \"${arg}\""
  fi
done

umask 0002
traps=(0 1 2 6 11 15)
for tt in ${traps[@]}; do
  trap "inthandler $tt" $tt
done

[ -z "${fpath}" ] && usage "filepath \"\" invalid"
[ $ecol -lt $scol ] && usage "cannot extract no column, start must be lowerequal end \"$scol\" <= \"$ecol\"" 

fname=$(/usr/bin/basename ${fpath})
fdir=$(/usr/bin/dirname ${fpath})

/usr/bin/awk -v scol=$scol -v ecol=$ecol '
  BEGIN {FS="";} 
  {
    str=""; 
    for (ii=scol;ii<=ecol;ii++) 
      str=str$ii; 
    print str;
  }
' ${fpath}

# in-place vim
#/usr/bin/cp ${fpath} /tmp
#/usr/bin/vim -c ':1,$s/\%>209c.//g' -c '1,$s/\%<200c.//g' -c ':wq!' ${fpath}
#/usr/bin/cat /tmp/${fname}
# /usr/bin/mv /tmp/${fname} ${fdir}

exit 0

