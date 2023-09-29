#!/bin/bash

function usage() {

  trap 0 1
  [ $# -gt 0 ] && /usr/bin/echo -e "\n$1"
  /usr/bin/echo -e "\n\
    usage \"$0\"\n\
    \"--fpath :  file\"\n\
    \"[--sort :  name|date|size]\"\n\
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

function fpsz() {

  declare -i isz=$1
  shift
  declare -i fac=$1
  shift
  local sz3=$(/usr/bin/bc <<EOI
scale=3
ans=$isz / $fac 
ans
EOI
)
  /usr/bin/echo $sz3

  return 0

}

path_script=$(/usr/bin/dirname $(/usr/bin/readlink -m $0))
path_curr="$(/usr/bin/pwd -P)"

fpathL=()
declare -i sortype=0

while [ $# -gt 0 ]; do
  arg="${1}"
  shift
  if [ "${arg}" == "--fpath" ]; then
    while [ $# -gt 0 -a ! "${1:0:2}" == "--" ]; do
      fpathL+=( "${1}" )
      shift
    done
  elif [ "${arg}" == "--sort" ]; then
    if [ $# -gt 0 -a ! "${1:0:2}" == "--" ]; then
      [ "${1}" == "name" ] && sortype=1
      [ "${1}" == "date" ] && sortype=2
      [ "${1}" == "size" ] && sortype=3
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

[ ${#fpathL} -eq 0 ] && fpathL+=( "${path_curr}" )
[ $sortype -ne 1 -a $sortype -ne 2 -a $sortype -ne 3 ] && sortype=2
fpath=""
if [ ${#fpathL[@]} -eq 1 -a -d "${fpathL[0]}" ]; then
  fpath="$(/usr/bin/readlink -m ${fpathL[0]})"
  export IFS=$'\n'
  fpathL=( $(/usr/bin/ls -1 ${fpath}/) )
  unset IFS
  for ii in ${!fpathL[@]}; do
    fpathL[$ii]="${fpath}/${fpathL[$ii]}"
  done
fi

declare -a suf=(" B" " kB" " MB" " GB")
declare -a sortedL=()
declare -i bsz=0
declare -i sz=0
declare -i fac=1
declare -i loop=0
declare -i ii=0
declare -i tmodutc=0
statout=""
declare -a sortoptL=( -t';' -k${sortype} )
[ $sortype -ne 1 ] && sortoptL+=( -n )

export IFS=$'\n'
for ii in ${!fpathL[@]}; do
  fpath=$(/usr/bin/readlink -m ${fpathL[$ii]})
  [ ! -r "${fpath}" ] && statout="0;0" || statout="$(/usr/bin/stat --format="%Y;%s" ${fpath})"
  fpathL[$ii]="$(/usr/bin/basename ${fpath});${statout}"
done

sortedL=( $(LC_ALL=C /usr/bin/sort ${sortoptL[@]} < <(/usr/bin/echo "${fpathL[*]}")) )

for itm in ${sortedL[@]}; do
  IFS=';' read fpath tmodutc bsz < <(/usr/bin/echo "${itm}")
  statout=$(/usr/bin/date -d "@${tmodutc}" +"%y-%m-%dT%H:%M:%S") 
  sz=$bsz
  fac=1
  loop=0
  while [ $loop -lt 3 ]; do
    if [ $sz -lt 1000 ]; then
      if [ $sortype -eq  1 -o $sortype -eq 2 ]; then
        printf "%-48s  %-17s  %12s\n" "${fpath:0:48}" "${statout}" "$(fpsz $bsz $fac)${suf[$loop]}"
      elif [ $sortype -eq 3 ]; then
        printf "%-48s  %12s  %-17s\n" "${fpath:0:48}" "$(fpsz $bsz $fac)${suf[$loop]}" "${statout}"
      fi
      break
    fi
    fac=$((fac * 1024))
    sz=$((bsz / fac))
    ((loop++))
  done
  if [ $loop -eq 3 ]; then
    if [ $sortype -eq 1 -o $sortype -eq 2 ]; then
      printf "%-48s  %-17s  %12s\n" "${fpath:0:48}" "${statout}" "$(fpsz $bsz $fac)${suf[$loop]}"
    elif [ $sortype -eq 3 ]; then
      printf "%-48s  %12s  %-17s\n" "${fpath:0:48}" "$(fpsz $bsz $fac)${suf[$loop]}" "${statout}"
    fi
  fi
done    

exit 0

