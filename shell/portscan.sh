#!/bin/bash

# simple port scanner

function usage() {

  [ $# -gt 0 ] && usage_msg="${1}" || usage_msg="usage : $0 \n\
    \"--port port (--port may be ommitted if only one arg is given)\" \n\
    \"--help\""
  /usr/bin/echo -e "${usage_msg}"

  exit 1
     
}

declare -i port=0

while [ $# -gt 0 ]; do
  arg="${1}"
  shift
  if [ "${arg}" == "--port" ]; then
    if [ $# -gt 0 ]; then
      port=${1}
      shift
    fi
  elif [ "${arg}" == "--help" ]; then
    usage
  else
    if [ $port -eq 0 ]; then
      port=${arg}
    else
      /usr/bin/echo "unknown argument \"${arg}\""
    fi
  fi
done

[ ${port} -eq 0 ] && usage

# hostlist=( $(/usr/bin/ypcat hosts | /usr/bin/awk '{print $1;}') ) # yellow pages or any hostlist
# hostlist=( $(/usr/bin/cat /etc/hosts | /usr/bin/awk '{print $2;}') ) # yellow pages or any hostlist
declare -a hostlist=()
while read -r line; do
  [[ ! "${line}" =~ ^# ]] && hostlist+=( "$(/usr/bin/echo ${line} | /usr/bin/awk '{print $2;}')" ) 
done < <(/usr/bin/cat /etc/hosts)

msg="${hh} port ${port} is"
for hh in ${hostlist[@]}; do
  [[ "${hh}" =~ localhost ]] && continue
  /usr/bin/echo "nc -4 -z -w 2 ${hh} ${port} 2>&1"
  /usr/bin/nc -4 -z -w 2 ${hh} ${port} 2>&1 
  [ $? -eq 0 ] && msgo="${msg} open" || msgo="${msg} closed"
  /usr/bin/echo "${msgo}"
done

