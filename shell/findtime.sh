#!/bin/bash 

where="."
what="*"

while [ $# -gt 0 ]; do
  if [ "$1" == "-name" ]; then
    shift
    if [ $# -gt 0 ]; then
      what="$1"
      shift
    fi
  else
    where="$1"
    shift
  fi
done

/usr/bin/find ${where} -name "${what}" -printf "%TY%Tm%Td%TH%TM\t%p\n" 2>/dev/null | sort
res=$?
exit $res

