#!/bin/bash

function usage() {

  trap 0 1
  [ $# -gt 0 ] && /usr/bin/echo -e "$1"
  /usr/bin/echo
  /usr/bin/echo -e "skeleton\n\
    \"usage \"$0\" \n\
    \"name /path/to/outfile.sh\" \n\
    \"[--par parname str|int|arr comment] [--par parname str|int|arr comment] ...\" \n\
    \"[--help|-h|/?]\""
  /usr/bin/echo
  exit 1

}

function inthandler() {

  trap 0 1
  [ $1 -gt 1 ] && /usr/bin/echo "Programmende durch Signal <$(/usr/bin/kill -l $1)>"
  exit 0

}

/usr/bin/echo
path_script=$(/usr/bin/dirname $(/usr/bin/readlink -m $0))
path_curr=$(/usr/bin/pwd -P)
fout=""
declare -A parL

while [ $# -gt 0 ]; do
  arg="${1}"
  shift
  if [ "${arg}" == "--par" ]; then 
    parname=""
    partype=""
    parcomm=""
    while [ $# -gt 0 ]; do
      [ "${1:0:2}" == "--" ] && break
      if [ -z "${parname}" ]; then
        parname="${1}"
      elif [ -z "${partype}" ]; then
        partype="${1}"
      else
        parcomm="${parcomm} ${1}"
      fi
      shift
    done
    [ -n "${parname}" ] && parL[${parname}]="${partype} ${parcomm}"
  elif [ "${arg}" == "--help" -o "${arg}" == "-h" -o "${arg}" == "/?" ]; then
    usage
  else
    [ -z "${fout}" ] && fout="${arg}" || /usr/bin/echo "Warnung : unbekannter Parameter \"${arg}\""
  fi
done

[ -z "${fout}" ] && usage "outfile missing"

umask 0002
traps=(0 1 2 6 11 15)
for tt in ${traps[@]}; do
  trap "inthandler $tt" $tt
done

fdir=$(/usr/bin/dirname ${fout})
fname=$(/usr/bin/basename ${fout})
fpath="${fname}.sh"
[ -n "${fdir}" ] && fpath="${fdir}/${fpath}"

/usr/bin/echo '#!/bin/bash' >${fpath}
/usr/bin/echo >>${fpath}
/usr/bin/echo 'function usage() {' >>${fpath}
/usr/bin/echo >>${fpath}
/usr/bin/echo '  trap 0 1' >>${fpath}
/usr/bin/echo '  [ $# -gt 0 ] && /usr/bin/echo -e "$1"' >>${fpath}   
/usr/bin/echo '  /usr/bin/echo -e "' >>${fpath}  
/usr/bin/echo '  usage \"$0\"\n\' >>${fpath} 

for pkey in ${!parL[@]}; do
  /usr/bin/echo -n '  \"--' >>${fpath}
  /usr/bin/echo -n "${pkey}" >>${fpath}
  pvalL=( ${parL[$pkey]} )
  if [ ${#pvalL[@]} -gt 1 ]; then
    /usr/bin/echo -n " :" >>${fpath}
    for pval in ${pvalL[@]:1}; do
      /usr/bin/echo -n "  ${pval}" >>${fpath}
    done
  fi
  /usr/bin/echo '\"\n\' >>${fpath}
done

/usr/bin/echo '  \"[--help|-h|/?]\""' >>${fpath}
/usr/bin/echo "  /usr/bin/echo" >>${fpath}
/usr/bin/echo "  exit 1" >>${fpath}
/usr/bin/echo >>${fpath}
/usr/bin/echo "}" >>${fpath}
/usr/bin/echo >>${fpath}
/usr/bin/echo "function inthandler() {" >>${fpath}
/usr/bin/echo >>${fpath}
/usr/bin/echo "  trap 0 1" >>${fpath}
/usr/bin/echo '  [ $1 -gt 1 ] && /usr/bin/echo "Programmende durch Signal <$(/usr/bin/kill -l $1)>"' >>${fpath}
/usr/bin/echo "  exit 0" >>${fpath}
/usr/bin/echo >>${fpath}
/usr/bin/echo "}" >>${fpath}
/usr/bin/echo >>${fpath}
/usr/bin/echo 'path_script=$(/usr/bin/dirname $(/usr/bin/readlink -m $0))' >>${fpath}
/usr/bin/echo 'path_curr="$(/usr/bin/pwd -P)"' >>${fpath}
/usr/bin/echo >>${fpath}

for pkey in ${!parL[@]}; do
  pval=( ${parL[$pkey]} )
  if [ ${#pval[@]} -eq 0 ]; then
    /usr/bin/echo "declare -i ${pkey}=0" >>${fpath}
  else
    if [ "${pval[0]}" == "str" ]; then
      /usr/bin/echo "${pkey}=\"\"" >>${fpath}
    elif [ "${pval[0]}" == "int" ]; then
      /usr/bin/echo "declare -i ${pkey}=0" >>${fpath}
    elif [ "${pval[0]}" == "arr" ]; then
      /usr/bin/echo "declare -a ${pkey}=()" >>${fpath}
    else
      usage "unknown type definition \"${pval[0]}\""
    fi
  fi
done
/usr/bin/echo >>${fpath}

/usr/bin/echo 'while [ $# -gt 0 ]; do' >>${fpath}
/usr/bin/echo '  arg="${1}"' >>${fpath}
/usr/bin/echo "  shift" >>${fpath}
declare -i parcnt=0
for pkey in ${!parL[@]}; do
  [ $parcnt -eq 0 ] && /usr/bin/echo -n "  if " >>${fpath} || /usr/bin/echo -n "  elif " >>${fpath}
  ((parcnt++))
  /usr/bin/echo -n '[ "${arg}" == "--' >>${fpath}
  /usr/bin/echo -n "${pkey}" >>${fpath}
  /usr/bin/echo '" ]; then' >>${fpath}
  pval=( ${parL[$pkey]} )
  if [ -z "${pval}" ]; then
    /usr/bin/echo "    ${pkey}=1" >>${fpath}
  else
    /usr/bin/echo '    if [ $# -gt 0 ]; then' >>${fpath}
    if [ "${pval}" == "arr" ]; then
      /usr/bin/echo -n "      ${pkey}+=( " >>${fpath}
      /usr/bin/echo '"${1}" )' >>${fpath}
    elif [ "${pval}" == "int" ]; then
      /usr/bin/echo -n "      ${pkey}=" >>${fpath}
      /usr/bin/echo '${1}' >>${fpath}
    else
      /usr/bin/echo -n "      ${pkey}=" >>${fpath}
      /usr/bin/echo '"${1}"' >>${fpath}
    fi
    /usr/bin/echo "      shift" >>${fpath}
    /usr/bin/echo "    fi" >>${fpath} 
  fi
done
[ $parcnt -eq 0 ] && /usr/bin/echo -n "  if " >>${fpath} || /usr/bin/echo -n "  elif " >>${fpath}
/usr/bin/echo '[ "${arg}" == "--help" -o "${arg}" == "-h" -o "${arg}" == "/?" ]; then' >>${fpath}
/usr/bin/echo "    usage" >>${fpath}
/usr/bin/echo "  else" >>${fpath} 
/usr/bin/echo '    /usr/bin/echo "Warnung : unbekannter Parameter \"${arg}\""' >>${fpath} 
/usr/bin/echo "  fi" >>${fpath} 
/usr/bin/echo "done" >>${fpath}
/usr/bin/echo >>${fpath}  
/usr/bin/echo "umask 0002" >>${fpath}
/usr/bin/echo "traps=(0 1 2 6 11 15)" >>${fpath}
/usr/bin/echo 'for tt in ${traps[@]}; do' >>${fpath}
/usr/bin/echo '  trap "inthandler $tt" $tt' >>${fpath}
/usr/bin/echo "done" >>${fpath}
/usr/bin/echo >>${fpath}
/usr/bin/echo >>${fpath}
/usr/bin/echo "# some payload goes here" >>${fpath}
/usr/bin/echo >>${fpath}
/usr/bin/echo >>${fpath}
/usr/bin/echo "exit 0" >>${fpath}
/usr/bin/echo >>${fpath}

/usr/bin/chmod 775 "${fpath}"
/usr/bin/echo "written : \"${fpath}\""
/usr/bin/echo

exit 0


