#!/bin/bash

function usage() {

  trap 0 1
  [ $# -gt 0 ] && /usr/bin/echo -e "$1"
  /usr/bin/echo -e "
    usage \"$0\"\n\
    \"[--signal number]\"\n\
    \"[--verbose number]\"\n\
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

function isinList() {

  [ $# -lt 2 ] && return 0
  local key="${1}"
  shift
  local -a keyL=( "$@" )
  declare -i idx=0
  
  for chk in ${keyL[@]}; do
    if [ "${chk}" == "${key}" ]; then
      /usr/bin/echo $idx
      return 1
    fi
    ((idx++))
  done

  return 0

}

function getParents() {

  declare -i chkpid=${1}
  shift

  declare -i procid=0
  declare -i pprocid=0

  for pstr in ${pstrL[@]}; do
    procid=$(/usr/bin/echo ${pstr} | /usr/bin/awk '{print $1;}')
    if [ $chkpid -eq $procid ]; then
      pprocid=$(/usr/bin/echo ${pstr} | /usr/bin/awk '{print $2;}')
      if [ $pprocid -gt 1 ]; then
        isinList $pprocid ${presL[@]} >/dev/null
        [ $? -eq 0 ] && presL+=( $pprocid )
        getParents $pprocid
      fi 
    fi
  done

  return 0

}

function getChildren() {

  declare -i chkpid=${1}
  shift

  declare -i procid=0
  declare -i pprocid=0

  for pstr in ${pstrL[@]}; do
    pprocid=$(/usr/bin/echo ${pstr} | /usr/bin/awk '{print $2;}')
    if [ $chkpid -eq $pprocid ]; then
      procid=$(/usr/bin/echo ${pstr} | /usr/bin/awk '{print $1;}')
      isinList $procid ${presL[@]} >/dev/null
      [ $? -eq 0 ] && presL+=( $procid )
      getChildren $procid 
    fi
  done

  return 0

}

function rmProcesses() {

  declare -i procid=0
  declare -i fl=1
  local ans=""
  local parg=""

  for pstr in ${pstrL[@]}; do
    procid=$(/usr/bin/echo ${pstr} | /usr/bin/awk '{print $1;}')
    isinList $procid ${presL[@]} >/dev/null
    if [ $? -eq 0 ]; then
      ans=$(/usr/bin/kill -${ksig} $procid 2>&1)
      fl=$?
      if [ $verbose -gt 0 ]; then
        parg=$(/usr/bin/echo ${pstr} | /usr/bin/awk '{$1=$2=$3=""; print $0;}')
        /usr/bin/echo "kill -${ksig} $procid $parg [$fl]"
        [ $verbose -gt 1 -a $fl -ne 0 ] && /usr/bin/echo ${ans}
      fi
    fi
  done

  return 0

}

path_script=$(/usr/bin/dirname $(/usr/bin/readlink -m $0))
path_curr="$(/usr/bin/pwd -P)"

declare -i verbose=0
declare -i ksig=15

while [ $# -gt 0 ]; do
  arg="${1}"
  shift
  if [ "${arg}" == "--verbose" ]; then
    if [ $# -gt 0 ]; then
      verbose=${1}
      shift
    fi
  elif [ "${arg}" == "--signal" ]; then
    if [ $# -gt 0 ]; then
      ksig=${1}
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

me="$(/usr/bin/id -nu)"
declare -i myprocid=$$
declare -a presL=( $myprocid )
export IFS=$'\n'
declare -a pstrL=( $(/usr/bin/ps -ax -o pid,ppid,euser,args | /usr/bin/awk -v me=${me} '{if($3==me) print $0;}' | /usr/bin/grep ${me}) )
if [ $verbose -gt 2 ]; then
  for pstr in ${pstrL[@]}; do
    /usr/bin/echo "${pstr}"
  done
  /usr/bin/echo
fi

getChildren $myprocid
declare -a currL=( "${presL[@]}" )
for procid in ${currL[@]}; do
  getParents $procid
done
currL=( "${presL[@]}" )
for procid in ${currL[@]}; do
  getChildren $procid
done

rmProcesses

exit 0

