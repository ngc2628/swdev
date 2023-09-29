#!/bin/bash

function usage() {

   trap 0 1
   [ $# -gt 0 ] && /usr/bin/echo -e "$1"
   /usr/bin/echo
   /usr/bin/echo -e "infloop\n\
     \"usage \"$0\" \n\
     \"[--timeout (3)]\" \n\
     \"[--verbose 1|2|4]\" \n\
     \"[--help|-h|/?]\""
   /usr/bin/echo
   exit 1

}

function inthandler() {

  trap 0 1
  [ $1 -gt 1 ] && msg="Programmende durch Signal <$(/usr/bin/kill -l $1)>"
  exit 0

}

/usr/bin/echo

path_script=$(/usr/bin/dirname $(/usr/bin/readlink -m $0))
path_curr="$(/usr/bin/pwd -P)"
declare -i verbose=0
declare -i timeout=0

while [ $# -gt 0 ]; do
  par="${1}"
  shift
  if [ "${par}" == "--timeout" ]; then
    if [ $# -gt 0 ]; then
      timeout="${1}"
      shift
    fi
  elif [ "${par}" == "--verbose" ]; then
    if [ $# -gt 0 ]; then
      verbose=${1}
      shift
    fi
  elif [ "${par}" == "--help" -o "${par}" == "-h" -o "${par}" == "/?" ]; then
    usagex
  else
    usage "Warnung : unbekannter Parameter \"${par}\""
  fi
done

umask 0002
traps=(0 1 2 6 11 15)
for tt in ${traps[@]}; do
  trap "inthandler $tt" $tt
done

[ ${timeout} -lt 1 ] && timeout=3

declare -i cnt=0
while [ 1 -eq 1 ]; do
  sleep $timeout
  [ $((cnt%10)) -eq 0 ] && /usr/bin/echo "[$$] $(/usr/bin/date +'%y-%m-%dT%H:%M:%S')"
  ((cnt++))
done 

exit 0
