#!/bin/bash

function usage() {

  trap 0 1
  [ $# -gt 0 ] && /usr/bin/echo -e "$1"
  /usr/bin/echo -e "
    usage \"$0\" \"network-address\"\n\
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

declare -i nip=0

[ $# -eq 0 ] && usage "no address provided"

while [ $# -gt 0 ]; do
  arg="${1}"
  shift
  if [ "${arg}" == "--help" -o "${arg}" == "-h" -o "${arg}" == "/?" ]; then
    usage
  else
    if [ $nip -eq 0 ]; then
      nip=$arg
    else
      /usr/bin/echo "Warnung : unbekannter Parameter \"${arg}\""
    fi
  fi
done

umask 0002
traps=(0 1 2 6 11 15)
for tt in ${traps[@]}; do
  trap "inthandler $tt" $tt
done

[ $nip -lt 0 ] && nip=0
ip="$(($nip>>24&255)).$(($nip>>16&255)).$(($nip>>8&255)).$(($nip&255))"

/usr/bin/echo ${ip}

exit 0

