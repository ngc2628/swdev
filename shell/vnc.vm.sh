#!/bin/bash

function usage() {

  trap 0 1
  [ $# -gt 0 ] && /usr/bin/echo -e "$1"
  /usr/bin/echo -e "
    usage \"$0\"\n\
    \"--vm\"\n\
    \"--local-port\"\n\
    \"--vnc-display\"\n\
    \"[--log-path]\"\n\
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

vm=""
declare -i localport=0
declare -i vncdisp=0
logpath=""

while [ $# -gt 0 ]; do
  arg="${1}"
  shift
  if [ "${arg}" == "--vm" ]; then
    if [ $# -gt 0 ]; then
      vm="${1}"
      shift
    fi
  elif [ "${arg}" == "--local-port" ]; then
    if [ $# -gt 0 ]; then
      localport=${1}
      shift
    fi
  elif [ "${arg}" == "--vnc-display" ]; then
    if [ $# -gt 0 ]; then
      vncdisp=${1}
      shift
    fi
  elif [ "${arg}" == "--log-path" ]; then
    if [ $# -gt 0 ]; then
      logpath=${1}
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

[ -z "${vm}" ] && usage "cannot continue without target host supplied"
[ $localport -lt 1025 ] && usage "local port cannot be a system port"
[ $vncdisp -lt 1 -o $vncdisp -gt 99 ] && usage "remote vnc display connot be beyond 1 or 99"
[ -z "${logpath}" ] && logpath="${HOME}/vnc.${vm}.log"  

/usr/bin/echo -e "\n----------------------\n$(/usr/bin/date +%y-%m-%dT%H:%M:%S)" >> ${logpath}
/usr/bin/ssh ${vm} "vncserver :${vncdisp} -securitytypes=vncauth,tlsvnc -geometry=1680x1050" >> ${logpath} 2>&1
/usr/bin/ssh -f -L ${localport}:localhost:59${vncdisp} ${vm} "sleep 15"
/usr/bin/vncviewer localhost:${localport} >> ${logpath} 2>&1
/usr/bin/ssh ${vm} "vncserver -kill :${vncdisp}" >> ${logpath} 2>&1

exit 0

