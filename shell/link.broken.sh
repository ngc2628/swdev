#!/bin/bash

function usage() {

  trap 0 1
  [ $# -gt 0 ] && /usr/bin/echo -e "$1"
  /usr/bin/echo -e "
    usage \"$0\"\n\
    \"--path\" root-folder\n\
    \"--rm\"\n\
    \"[--verbose]\" 0|1|2\n\
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

path_script=$(/usr/bin/dirname $(/usr/bin/readlink -m $0))
path_curr="$(/usr/bin/pwd -P)"

findpath=""
declare -i dorm=0
declare -i verbose=0

while [ $# -gt 0 ]; do
  arg="${1}"
  shift
  if [ "${arg}" == "--path" ]; then
    if [ $# -gt 0 ]; then
      findpath="${1}"
      shift
    fi
  elif [ "${arg}" == "--rm" ]; then
    dorm=1
  elif [ "${arg}" == "--verbose" ]; then
    if [ $# -gt 0 ]; then
      verbose=$1
      shift
    else
      verbose=1
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

[ -z "${findpath}" ] && findpath="${path_curr}"

/usr/bin/echo -e "\ngathering symlinks ... please wait ..."
export LANG=""
export LANGUAGE=""
export LC_ALL=C
export IFS=$'\n'
declare -a symL=( $(/usr/bin/find ${findpath} -type l -exec /usr/bin/file {} \; 2>/dev/null) )
[ $verbose -gt 0 ] && /usr/bin/echo "found ${#symL[@]} symlinks in \"${findpath}\""

declare -i deadcnt=0
declare -i rmcnt=0
for ff in ${symL[@]}; do 
  fpath="${ff%:*}"
  finfo="${ff##*:}"
  /usr/bin/echo "${finfo}" | /usr/bin/grep "broken symbolic link" >/dev/null 2>&1
  if [ $? -eq 0 ]; then
# echo "<$fpath> <$finfo>"
    ((deadcnt++))
    [ $verbose -gt 1 ] && /usr/bin/echo -n "dead link \"${fpath}\" "
    if [ $dorm -gt 0 ]; then
      /usr/bin/rm -f "${fpath}" >/dev/null 2>&1
      if [ $? -eq 0 ]; then 
        [ $verbose -gt 1 ] && /usr/bin/echo "removed"
        ((rmcnt++))
      else
        [ $verbose -gt 1 ] && /usr/bin/echo "cannot be removed"
      fi
    else
      [ $verbose -gt 1 ] && /usr/bin/echo 
    fi
  fi
done

msg="found $deadcnt broken symlinks"
[ $dorm -gt 0 -a $deadcnt -gt 0 ] && msg+=" - $rmcnt removed"
/usr/bin/echo ${msg}

exit 0

