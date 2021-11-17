#!/bin/bash

function usage() {

  [ $# -gt 0 ] && msg="$1" || msg="usage $0 \"-t to\" \"[-s subject]\""
  /usr/bin/echo -e "${msg}"
  exit 1

}

bindir=$(/usr/bin/dirname "$(/usr/bin/readlink -m $0)")
[ ! -x "${bindir}/send.speedbone.sh" ] && usage "send.speedbone.sh not found in \"${bindir}\""
[ ! -x "${bindir}/pun" ] && usage "password rejected"

to=""
subject=""
while [ $# -gt 0 ]; do
  arg="$1"
  shift
  if [ "${arg}" == "-t" ]; then
    if [ $# -gt 0 ]; then
      to="$1"
      shift
    fi
  elif [ "${arg}" == "-s" ]; then
    if [ $# -gt 0 ]; then
      subject="$1"
      shift
    fi
  else
    /usr/bin/echo "unknown argument \"${arg}\""
  fi
done
    
[ -z "${to}" ] && read -p "to : " to
[ -z "${to}" ] && usage "recipient missing"
[ -z "${subject}" ] && read -p "subject : " subject
[ -z "${subject}" ] && subject="no subject"

pass=$(${bindir}/pun -d B:jy,5{ING | /usr/bin/awk -F"=" '{print $2;}')

exec ${bindir}/send.speedbone.sh -f "martin@kloss.berlin" -t "${to}" -s "${subject}" -p "${pass}"



