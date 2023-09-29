#!/bin/bash

function usage() {

  trap 0 1
  [ $# -gt 0 ] && /usr/bin/echo -e "\n$1"
  /usr/bin/echo -e "
    usage \"$0\" \"ip-address\"\n\
    \"--(host|network)\"\n\
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
bo=-1

while [ $# -gt 0 ]; do
  arg="${1}"
  shift
  if [ "${arg}" == "--help" -o "${arg}" == "-h" -o "${arg}" == "/?" ]; then
    usage
  elif [ "${arg}" == "--host" ]; then
    bo=0
  elif [ "${arg}" == "--network" ]; then
    bo=1
  else
    if [ -z "${ip}" ]; then
      ip="${arg}"
    else
      /usr/bin/echo "Warnung : unbekannter Parameter \"${arg}\""
    fi
  fi
done

[ -z "${ip}" ] && usage "no ip address provided"
[ $bo -lt 0 ] && usage "byte-order needed"

umask 0002
traps=(0 1 2 6 11 15)
for tt in ${traps[@]}; do
  trap "inthandler $tt" $tt
done

declare -a ipL=( $(/usr/bin/echo ${ip} | /usr/bin/awk -F"." '{print $1,$2,$3,$4;}') )
[ ${#ipL[@]} -ne 4 ] && usage "\"${ip}\" does not seem to look like an ip address"

[ $bo -eq 0 ] && \
  nip=$(($((${ipL[0]}<<24))+$((${ipL[1]}<<16))+$((${ipL[2]}<<8))+${ipL[3]})) || \
  nip=$(($((${ipL[3]}<<24))+$((${ipL[2]}<<16))+$((${ipL[1]}<<8))+${ipL[0]}))

/usr/bin/echo $nip

exit 0

