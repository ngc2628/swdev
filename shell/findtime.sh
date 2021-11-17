#!/bin/bash 

function usage() {
   
  msg="usage $0 \n\
    [root-directory] \n\
    [-name file-pattern]"
  [ $# -gt 0 ] && msg="${1}"
  /usr/bin/echo -e "${msg}"
  exit 1

}

where="."
what="*"

while [ $# -gt 0 ]; do
  arg="${1}"
  shift
  if [ "${arg}" == "-name" ]; then
    if [ $# -gt 0 ]; then
      what="${1}"
      shift
    fi
  else
    where="${arg}"
  fi
done

# /usr/bin/find ${where} -name "${what}" -printf "%TY%Tm%Td%TH%TM\t%p\n" 2>/dev/null | /usr/bin/sort
/usr/bin/find ${where} -name "${what}" -printf "%TY%Tm%Td%TH%TM%TS\t%TY-%Tm-%TdT%TH:%TM:\t%TS\t%p\n" 2>/dev/null | \
  /usr/bin/sort -k1 | /usr/bin/awk '{ printf "%s%04.2f %s\n",$2,$3,$4; }'

res=$?


exit $res

