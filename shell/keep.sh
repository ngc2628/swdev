#!/bin/bash

if [ $# -lt 1 ]; then
  echo "usage  : keep source-dir files-and-directories"
  exit 0
fi

sourcedir=$1
shift

if [ ! -d ${sourcedir} -o ! -w ${sourcedir} ]; then
  echo "source-dir ${sourcedir} does not exist or is not writable"
  exit 0
fi

keeplist=()
while [ $# -gt 0 ]
do
  keeplist=( ${keeplist} $1 )
  shift
done

contentlist=( `ls -1 ${sourcedir} 2>/dev/null` )
dokeep=0
for itm in ${contentlist[@]}
do
  dokeep=0
  for keep in ${keeplist[@]}
  do
    if [ "${itm}" == "${keep}" ]; then
      dokeep=1
      break
    fi
  done
  if [ ${dokeep} -eq 0 ]; then
    rm -rf "${sourcedir}/${itm}" 2>/dev/null
  fi
done

