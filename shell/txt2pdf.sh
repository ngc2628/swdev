#!/bin/bash

tmpdir="/tmp/$$"

function usage() {

   trap 0 1
   [ $# -gt 0 ] && /usr/bin/echo -e "$1"
   /usr/bin/echo
   /usr/bin/echo -e "Text-Datei -> pdf-Datei\n\
     \"usage \"$0\" \n\
     \"--path_in /Pfad/zur/Text-Datei\" \n\
     \"--path_out /Pfad/zur/pdf-Datei\" \n\
     \"[--verbose 1|2|4]\" \n\
     \"[--help|-h|/?]\""
   /usr/bin/echo
   exit 1

}

function inthandler() {

  trap 0 1
  [ $1 -gt 1 ] && msg="Programmende durch Signal <$(/usr/bin/kill -l $1)>"
  /usr/bin/rm -rf "${tmpdir}"
  exit 0

}

/usr/bin/echo

path_script=$(/usr/bin/dirname $(/usr/bin/readlink -m $0))
path_curr="$(/usr/bin/pwd -P)"
path_in=""
path_out=""
declare -i verbose=0

while [ $# -gt 0 ]; do
  par="${1}"
  shift
  if [ "${par}" == "--path_in" ]; then
    if [ $# -gt 0 ]; then
      path_in="${1}"
      shift
    fi
  elif [ "${par}" == "--path_out" ]; then
    if [ $# -gt 0 ]; then
      path_out="${1}"
      shift
    fi
  elif [ "${par}" == "--verbose" ]; then
    if [ $# -gt 0 ]; then
      verbose=${1}
      shift
    fi
  elif [ "${par}" == "--help" -o "${par}" == "-h" -o "${par}" == "/?" ]; then
    usagex
  else
    usage "Warnung : unbekannter Parameter \"${par}\""
  fi
done

[ -z "${path_in}" -o ! -f "${path_in}" -o ! -r "${path_in}" ] && usage "kann \"${path_in}\" nicht lesen"
[ -z "${path_out}" ] && path_out="${path_in%.*}.pdf"

umask 0002
traps=(0 1 2 6 11 15)
for tt in ${traps[@]}; do
  trap "inthandler $tt" $tt
done

isrel=$(/usr/bin/echo ${path_out} | /usr/bin/awk -F"/" '{print $1;}')
[ -n "${isrel}" ] && path_out="${path_curr}/${path_out}"
path_d=$(/usr/bin/dirname ${path_out})
path_f=$(/usr/bin/basename ${path_out})  
if [ ! -d "${path_d}" ]; then
  /usr/bin/mkdir -p "${path_d}" || usage "kann \${path_d}\" nicht anlegen"
fi
[ ! -w "${path_d}" ] && usage "kann \"${path_d}\" nicht schreiben" 
[ -f "${path_out}" -a ! -w "${path_out}" ] && usage "kann \"${path_out}\" nicht schreiben"

[ ! -x "/usr/bin/vim" ] && usage "Programm \"/usr/bin/vim\" nicht gefunden"
[ ! -x "/usr/bin/ps2pdf" ] && usage "Programm \"/usr/bin/ps2pdf\" nicht gefunden"

[ ${verbose} -gt 1 ] && /usr/bin/echo "/usr/bin/mkdir -p ${tmpdir}"
/usr/bin/mkdir -p "${tmpdir}"

[ ${verbose} -gt 0 ] && /usr/bin/echo "/usr/bin/vim -c \":hardcopy > ${tmpdir}/${path_f%.*}.ps\" -c \":quit\" ${path_in}"
/usr/bin/vim -c ":hardcopy > ${tmpdir}/${path_f%.*}.ps" -c ":quit" ${path_in}
[ ${verbose} -gt 0 ] && /usr/bin/echo "/usr/bin/ps2pdf ${tmpdir}/${path_f%.*}.ps ${path_out}"
/usr/bin/ps2pdf ${tmpdir}/${path_f%.*}.ps ${path_out}

exit 0
