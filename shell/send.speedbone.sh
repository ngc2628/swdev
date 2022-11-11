#!/bin/bash

function usage() {

  /usr/bin/echo "usage $0 \"-f from-addr\" \"-t to addr\" \"-p pass\" \"[-s subject]\""
  exit 1

}

from=""
to=""
subject=""
pass=""

while [ $# -gt 0 ]; do
  arg="$1"
  shift
  if [ "${arg}" == "-f" ]; then
    if [ $# -gt 0 ]; then
      from="$1"
      shift
    fi
  elif [ "${arg}" == "-t" ]; then
    if [ $# -gt 0 ]; then
      to="$1"
      shift
    fi
  elif [ "${arg}" == "-s" ]; then
    if [ $# -gt 0 ]; then
      subject="$1"
      shift
    fi
  elif [ "${arg}" == "-p" ]; then
    if [ $# -gt 0 ]; then
      pass="$1"
      shift
    fi
  else
    /usr/bin/echo "unknown arg \"${arg}\""
  fi
done

[ -z "${subject}" ] && subject="no subject"
[ -z "${from}" -o -z "${to}" -o -z "${pass}" ] && usage

/usr/bin/mailx -v \
  -s "${subject}" \
  -S smtp="smtp.worldserver.net" \
  -S smtp-auth="login" \
  -S smtp-auth-user="${from}" \
  -S smtp-auth-password="${pass}" \
  -S from="${from}" \
  ${to} 

