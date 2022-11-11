#!/bin/bash

tmpdir="/tmp/$$"

function usage() {

  trap 0 1
  [ $# -gt 0 ] && /usr/bin/echo -e "$1"
  /usr/bin/echo -e "
    usage \"$0\"\n\
    \"--fpath\"\n\
    \"--colfrom\"\n\
    \"--colto\"\n\
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
  /usr/bin/rm -rf "${tmpdir}"
  exit $bailout

}

path_script=$(/usr/bin/dirname $(/usr/bin/readlink -m $0))
path_curr="$(/usr/bin/pwd -P)"

fpath=""
declare -i colfrom=0
declare -i colto=0

while [ $# -gt 0 ]; do
  arg="${1}"
  shift
  if [ "${arg}" == "--fpath" ]; then
    if [ $# -gt 0 ]; then
      fpath="${1}"
      shift
    fi
  elif [ "${arg}" == "--colfrom" ]; then
    if [ $# -gt 0 ]; then
      colfrom=${1}
      shift
    fi
  elif [ "${arg}" == "--colto" ]; then
    if [ $# -gt 0 ]; then
      colto=${1}
      shift
    fi
  elif [ "${arg}" == "--help" -o "${arg}" == "-h" -o "${arg}" == "/?" ]; then
    usage
  else
    /usr/bin/echo "Warnung : unbekannter Parameter \"${arg}\""
  fi
done

[ -z "${fpath}" ] && usage "must provide input file"

umask 0002
traps=(0 1 2 6 11 15)
for tt in ${traps[@]}; do
  trap "inthandler $tt" $tt
done

fpath=$(/usr/bin/readlink -m ${fpath})
fname="$(/usr/bin/basename ${fpath})"
fnameskel="${fname##*.}"
fdir=$(/usr/bin/dirname ${fpath})

/usr/bin/mkdir -p "${tmpdir}" || usage "cannot create transits ${tmpdir}"
/usr/bin/cat <<EOM >"${tmpdir}/vimrc" 
set printoptions+=formfeed:y
set printoptions+=header:0
set printheader=
EOM

/usr/bin/awk -v colfrom=${colfrom} -v colto=${colto} 'BEGIN {FS="";} \
  { ll=(colfrom>0 ? colfrom : 1); rr=(colto>0 ? colto : NF); \
    for (ii=colfrom; ii<=colto; ii++) printf "%s",$ii; printf "\n"; }' \
  ${fpath} > ${tmpdir}/${fnameskel}.asc

#/usr/bin/sed -i 's/^[^1]\(.*\)/\1/; s/^1\(.*\)/\x0c\1/' ${tmpdir}/${fnameskel}.asc

/usr/bin/vim -u ${tmpdir}/vimrc -c ":hardcopy >${tmpdir}/${fnameskel}.ps" -c ":q" \
  ${tmpdir}/${fnameskel}.asc
/usr/bin/ps2pdf ${tmpdir}/${fnameskel}.ps ${tmpdir}/${fnameskel}.pdf
/usr/bin/atril ${tmpdir}/${fnameskel}.pdf


exit 0


