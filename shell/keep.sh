#!/bin/bash 

function usage() {
   
  msg="usage $0 \n\
    directory \n\
    [keep-name] \n\
    [keep-name] \n\
    [...] \n\
    [--verbose 1|2|4] \n\ 
    [--help|-h|/? these lines]" 
  [ $# -gt 0 ] && msg="${1}"
  /usr/bin/echo -e "${msg}"
  exit 1

}

sourcedir=""
declare -a keepL=()
declare -i verbose=0

while [ $# -gt 0 ]; do
  arg="${1}"
  shift
  if [ "${arg}" == "--verbose" ]; then
    verbose=1
    if [ $# -gt 0 ]; then
      verbose=${1}
      shift
    fi
  fi
  [ "${arg}" == "--help" -o "${arg}" == "-h" -o "${arg}" == "/?" ] && usage
  [ -z "${sourcedir}" ] && sourcedir="${arg}" || keepL+=( "${arg}" )
done

[ ! -d ${sourcedir} -o ! -w ${sourcedir} ] && usage  "source-dir \"${sourcedir}\" does not exist or is not writable"

contentlist=( $(/usr/bin/ls -1 ${sourcedir} 2>/dev/null) )
declare -i dokeep=0
for itm in ${contentlist[@]}; do
  dokeep=0
  for keep in ${keepL[@]}; do
    if [ "${itm}" == "${keep}" ]; then
      dokeep=1
      break
    fi
  done
  [ ${dokeep} -eq 0 ] && rm -rf "${sourcedir}/${itm}" 2>/dev/null
done

exit 0

