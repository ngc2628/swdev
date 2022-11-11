#!/bin/bash

function usage() {

  trap 0 1
  [ $# -gt 0 ] && /usr/bin/echo -e "$1"
  /usr/bin/echo -e "
    usage \"$0\" \"ip-address\"\n\
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

ip=""

while [ $# -gt 0 ]; do
  arg="${1}"
  shift
  if [ "${arg}" == "--help" -o "${arg}" == "-h" -o "${arg}" == "/?" ]; then
    usage
  else
    if [ -z "${ip}" ]; then
      ip="${arg}"
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

[ -z "${ip}" ] && usage "no ip address provided"
declare -a ipL=( $(/usr/bin/echo ${ip} | /usr/bin/awk -F"." '{print $1,$2,$3,$4;}') )
[ ${#ipL[@]} -ne 4 ] && usage "\"${ip}\" does not seem to look like an ip address"

nip=$(($((${ipL[0]}<<24))+$((${ipL[1]}<<16))+$((${ipL[2]}<<8))+${ipL[3]}))
/usr/bin/echo $nip

exit 0

