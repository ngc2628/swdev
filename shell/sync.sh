#!/bin/bash

function usage() {
 
  /usr/bin/echo
  [ $# -gt 0 ] && /usr/bin/echo "${1}"
  /usr/bin/echo
  /usr/bin/echo "usage \"${0}\" \"--path_src {host:path}\" \"--path_dest {local path to be generated}\""
  /usr/bin/echo
  exit 1

}

function excludes() {

  while [ $# -gt 0 ]; do
    /usr/bin/echo "${1}"
    shift 
  done
  
  return 0

}

transdir_1="damba"
path_src=""
path_dest=""
host_src=""
declare -a optionL

while [ $# -gt 0 ]; do
  arg="${1}"
  shift
  if [ "${arg}" == "--path_src" ]; then
    if [ $# -gt 0 ]; then
      host_src="${1}"
      shift
    fi  
  elif [ "${arg}" == "--path_dest" ]; then
    if [ $# -gt 0 ]; then
      path_dest="${1}"
      shift
    fi
  else
    optionL+=( "${arg}" )
  fi
done

[ -z "${host_src}" -o -z "${path_dest}" ] && usage "path_src or path_dest invalid"

path_src=$(/usr/bin/echo ${host_src} | /usr/bin/awk -F: '{print $2;}')
if [ -z "${path_src}" ]; then
  path_src="${host_src}"
  host_src=""
else 
  host_src=$(/usr/bin/echo ${host_src} | /usr/bin/awk -F: '{print $1;}')
fi
[ ! "${path_src:0:1}" == "/" ] && usage "remote src-dir cannot be relative"
path_dest=$(/usr/bin/readlink -m ${path_dest})

[ -z "{host_src}" ] && remhost="" || remhost="${host_src}:"

options=" -vvv -a --protect-args --stats "
for opt in ${optionL[@]}; do
  options+=" --exclude ${opt} "
done
addr="  ${remhost}${path_src}/${transdir_1}/ ${path_dest}/${transdir_1}"

/usr/bin/rsync ${options} ${addr} 

#/usr/bin/rsync ${options} --exclude-from=<(excludes ${optionL[@]}) ${addr} 







