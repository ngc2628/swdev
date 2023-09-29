#!/bin/bash 

function usage() {
   
  msg="\nusage $0 \n\
    [root-directory] \n\
    [-name file-pattern] \n\
    [-sort size|time] \n\
    [-showpath] \n\
    [-help|-h|/?]"
  [ $# -gt 0 ] && msg="\n${1}\n${msg}\n"
  /usr/bin/echo -e "${msg}"
  exit 1

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
  /usr/bin/echo -n $sz3

  return 0

}

function fsize() {

  if [ $# -le 0 ]; then
    /usr/bin/echo -n "0B"
    return 1
  fi
  declare -i bsz=$1
  shift
  declare -a suf=("B" "kB" "MB" "GB")
  declare -i sz=$bsz
  declare -i fac=1
  declare -i loop=0
  while [ $loop -lt 3 ]; do
    if [ $sz -lt 1000 ]; then
      printf "$(fpsz $bsz $fac)${suf[$loop]}"
      break
    fi
    fac=$((fac * 1024))
    sz=$((bsz / fac))
    ((loop++))
  done
  [ $loop -eq 3 ] && printf "$(fpsz $bsz $fac)${suf[$loop]}"

  return 0

}

where="$(/usr/bin/pwd -P)"
what="*"
sorttype="time"
showpath="f"

while [ $# -gt 0 ]; do
  arg="${1}"
  shift
  if [ "${arg}" == "-name" ]; then
    if [ $# -gt 0 ]; then
      what="${1}"
      shift
    fi
  elif [ "${arg}" == "-sort" ]; then
    if [ $# -gt 0 ]; then
      sorttype="${1}"
      shift
    fi
  elif [ "${arg}" == "-showpath" ]; then
    showpath="p"
  elif [ "${arg}" == "-help" -o "${arg}" == "--help" -o "${arg}" == "-h" -o "${arg}" == "/?" ]; then
    usage
  else
    where="${arg}"
  fi
done

[ ! "${sorttype}" == "time" -a ! "${sorttype}" == "size" ] && sorttype="time"
[ ! -e "${where}" ] && usage "root search folder \"${where}\" not found"
[ -h "${where}" ] && where="$(/usr/bin/readlink -m ${where})"
if [ -f "${where}" ]; then
  /usr/bin/echo -n "modifying root search folder from \"${where}\" to "
  where="$(/usr/bin/dirname ${where})"
  /usr/bin/echo "\"${where}\""
fi
[ ! -d "${where}" -o ! -r "${where}" ] && usage "cannot read \"${where}\""

declare -a fL=()
declare -a outL=()
ss=""
tt=""
export IFS=$'\n'
if [ "${sorttype}" == "time" ]; then
  fL=( $(/usr/bin/find ${where} -name "${what}" \
    -printf "%TY%Tm%Td%TH%TM%TS;%TY-%Tm-%TdT%TH:%TM:%TS;%s;%${showpath}\n" 2>/dev/null | /usr/bin/sort -t";" -k1 ) )
  printf "%-19s  %12s  %s\n" "TIME" "SIZE" "NAME"
  for ff in ${fL[@]}; do
    outL[0]=$(/usr/bin/echo -n "${ff}" | /usr/bin/awk -F";" '{ printf "%s",$2; }')
    outL[1]=$(/usr/bin/echo -n "${ff}" | /usr/bin/awk -F";" '{ printf "%s",$3; }')
    outL[2]=$(/usr/bin/echo -n "${ff}" | /usr/bin/awk -F";" '{ printf "%s",$4; }')
    ss=$(fsize ${outL[1]})
    tt=${outL[0]}
    printf "%-19s  %12s  %s\n" ${tt:0:19} ${ss} ${outL[2]}
  done
elif [ "${sorttype}" == "size" ]; then
  fL=( $(/usr/bin/find ${where} -name "${what}" \
    -printf "%TY-%Tm-%TdT%TH:%TM:%TS;%s;%${showpath}\n" 2>/dev/null | /usr/bin/sort -t";" -k2 -n) )
  printf "%12s  %-19s  %s\n" "SIZE" "TIME" "NAME"
  for ff in ${fL[@]}; do
    outL[0]=$(/usr/bin/echo -n "${ff}" | /usr/bin/awk -F";" '{ printf "%s",$2; }')
    outL[1]=$(/usr/bin/echo -n "${ff}" | /usr/bin/awk -F";" '{ printf "%s",$1; }')
    outL[2]=$(/usr/bin/echo -n "${ff}" | /usr/bin/awk -F";" '{ printf "%s",$3; }')
    ss=$(fsize ${outL[0]})
    tt=${outL[1]}
    printf "%12s  %-19s  %s\n" ${ss} ${tt:0:19} ${outL[2]}
  done
fi

res=$?

exit $res


