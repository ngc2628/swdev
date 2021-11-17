#!/bin/bash

# statische Pfade
declare -A rvdirs
rvdirs["utm"]="utm"
rvdirs["damxs"]="damxs"
rvdirs["damxu"]="damxu"
rvdirs["damxp"]="damxp"
rvdirs["asyn"]="asyn"
rvdirs["damba"]="damba"
rvdirs["dambb"]="dambb"
rvdirs["toolsbin"]="jobsteuerung/tools/bin"
rvdirs["toolslibdata"]="jobsteuerung/tools/lib/data"
rvdirs["toolslibhsyro"]="jobsteuerung/tools/lib/hsyro"
rvdirs["toolslibini"]="jobsteuerung/tools/lib/ini"
rvdirs["toolslibpar"]="jobsteuerung/tools/lib/par"
rvdirs["toolslibsql"]="jobsteuerung/tools/lib/sql"
rvdirs["toolslibvlk"]="jobsteuerung/tools/lib/vlk"
rvdirs["bin"]="lib/src/target/bin"
rvdirs["src"]="sourcen"

declare -A tstdirs
tstdirs["utm"]="utm"
tstdirs["damxs"]="damxs"
tstdirs["damxu"]="damxu"
tstdirs["damxp"]="damxp"
tstdirs["asyn"]="asyn"
tstdirs["damba"]="damba"
tstdirs["dambb"]="dambb"
tstdirs["toolsbin"]="jobsteuerung/tools/bin"
tstdirs["toolslibdata"]="jobsteuerung/tools/lib/data"
tstdirs["toolslibhsyro"]="jobsteuerung/tools/lib/hsyro"
tstdirs["toolslibini"]="jobsteuerung/tools/lib/ini"
tstdirs["toolslibpar"]="jobsteuerung/tools/lib/par"
tstdirs["toolslibsql"]="jobsteuerung/tools/lib/sql"
tstdirs["toolslibvlk"]="jobsteuerung/tools/lib/vlk"
tstdirs["bin"]="jobsteuerung/tools/lib/src/target/bin"
tstdirs["src"]="sourcen"

tstkeys=( ${!tstdirs[@]} )

currdate=$(/usr/bin/date +"%y-%m-%d %H:%M:%S")

function usage() {

  trap 0 1
  [ $# -gt 0 ] && /usr/bin/echo -e "$1"
  /usr/bin/echo
  /usr/bin/echo -e "Installation mit rsync - Doppelpunkt trennt vm vom Pfad - mindestens eine Seite (Quelle oder Ziel) muss lokal sein\n\
    \"usage \"$0\" \n\
    \"--path_test /Pfad/zu/Test (Installationsverzeichnis - z.B. s25b0032:/opt/rvdialog)\" \n\
    \"--path_pmvw /Pfad/zu/pmvw (Programmverwaltung z.B. s25b0036:/opt/rvdialog)\" \n\
    \"--version Versions-Nummer 2.1nn[.n{1-3}][.n{1-3}] (z.B. 2.106.9 - die neute Korrektursammlung (z.B. Infomails) zum Aenderungsbrief 2.106)\" \n\
    \"[--dir Verzeichnis] [--dir Verzeichnis] ... nur einzelne Verzeichnisse verteilen \n\
        damxs damxu asyn damba dambb toolsbin toolslibdata toolslibhsyro \n\
        toolslibini toolslibpar toolslibsql toolslibvlk bin\" \n\
    \"[--group Gruppe] Gruppenzugehoerigkeit erstellter Objecte\" \n\
    \"[--debug v|vv|vvv]\""
  /usr/bin/echo
  exit 1
}

# --------------------------------------------------------------------------

/usr/bin/echo
path_script=$(/usr/bin/dirname $(/usr/bin/readlink -m $0))
[ ! -r "${path_script}/dist.func.sh" ] && usage "kann \"${path_script}/dist_func.sh\" nicht finden"
source "${path_script}/dist.func.sh"
path_pmvw=""
path_test=""
sender=""
receiver=""
aebf=""
dirL=()
grp=""
fldbg=""

while [ $# -gt 0 ]; do
  arg="${1}"
  shift
  if [ "${arg}" == "--version" ]; then
    if [ $# -gt 0 ]; then
      aebf="$1"
      shift
    fi
  elif [ "${arg}" == "--path_pmvw" ]; then
    if [ $# -gt 0 ]; then
      path_pmvw="$1"
      shift
    fi
  elif [ "${arg}" == "--path_test" ]; then
    if [ $# -gt 0 ]; then
      path_test="$1"
      shift
    fi
  elif [ "${arg}" == "--debug" ]; then
    if [ $# -gt 0 ]; then
      fldbg="--debug -${1}"
      shift
    fi
  elif [ "${arg}" == "--dir" ]; then
    if [ $# -gt 0 ]; then
      dirL+=( "${1}" )
      shift
    fi
  elif [ "${arg}" == "--group" ]; then
    if [ $# -gt 0 ]; then
      grp="${1}"
      shift
    fi
  elif [ "${arg}" == "--help" -o "$1" == "-h" -o "$1" == "/?" ]; then
    usage
  else
    /usr/bin/echo "Warnung : unbekannter Parameter \"${1}\""
  fi
done

[ -z "${path_pmvw}" ] && usage "der Pfad zu pmvw \"\" ist ungueltig"
[ -z "${path_test}" ] && usage "der Zielpfad \"\" ist ungueltig" 
[ -z "${aebf}" ] && usage "Version \"\" ist ungueltig"
[[ ! "${aebf}" =~ ^2\.1[0-9][0-9][\.0-9]?[0-9]?[0-9]?[\.0-9]?[0-9]?[0-9]?$ ]] && usage "Version \"${aebf}\" ist ungueltig"

pathvm=$(/usr/bin/echo ${path_pmvw} | /usr/bin/awk -F: '{print $2;}')
if [ -n "${pathvm}" ]; then
  sender=$(/usr/bin/echo ${path_pmvw} | /usr/bin/awk -F: '{print $1;}')
  path_pmvw=$(/usr/bin/echo ${path_pmvw} | /usr/bin/awk -F: '{print $2;}')
fi
pathvm=$(/usr/bin/echo ${path_test} | /usr/bin/awk -F: '{print $2;}')
if [ -n "${pathvm}" ]; then
  receiver=$(/usr/bin/echo ${path_test} | /usr/bin/awk -F: '{print $1;}')
  path_test=$(/usr/bin/echo ${path_test} | /usr/bin/awk -F: '{print $2;}')
fi
[ -n "${sender}" -a -n "${receiver}" ] && usage "entweder Sender oder Empfaenger muss lokal verfuegbar sein"

expect_login "${sender}"
ans=$?
[ $ans -eq 1 ] && usage "Start abgebrochen"
[ $ans -eq 2 ] && usage "Login auf \"${sender}\" nicht moeglich"
expect_login "${receiver}"
ans=$?
[ $ans -eq 1 ] && usage "Start abgebrochen"
[ $ans -eq 2 ] && usage "Login auf \"${receiver}\" nicht moeglich"

path_aebf="${path_pmvw}/${aebf}"
check_path "${path_aebf}" "${sender}"
[ $? -gt 0 ] && usage "der Pfad \"${sender}:${path_aebf}\" ist ungueltig"

parvm=()
if [ -n "${receiver}" ]; then
  parvm+=( "--vm" )
  parvm+=( "${receiver}" )
fi
pargrp=()
if [ -n "${grp}" ]; then
  pargrp+=( "--group" )
  pargrp+=( "${grp}" )
fi

make_path "--path" "${path_test}" ${pargrp[@]} ${parvm[@]}
check_path "${path_test}" "${receiver}"
[ $? -gt 0 ] && usage "der Pfad \"${receiver}:${path_test}\" ist ungueltig"

for dd in ${!rvdirs[@]}; do
  [ "${dd}" == "utm" ] && pre="${path_aebf}/.." || pre="${path_aebf}"
  rvdirs["${dd}"]="${pre}/${rvdirs["${dd}"]}"
done
for dd in ${!tstdirs[@]}; do
  tstdirs["${dd}"]="${path_test}/${tstdirs["${dd}"]}"
done

nn=$(( ${#dirL[@]}-1 )) 
for ii in $(/usr/bin/seq $nn -1 0); do
  isinkeyL "${dirL[$ii]}" ${tstkeys[@]}
  [ $? -eq 0 ] && unset dirL[$ii]
done
if [ ${#dirL[@]} -gt 0 ]; then
  tstkeys=( ${dirL[@]} )
else
  nn=$(( ${#tstkeys[@]}-1 ))
  for ii in $(/usr/bin/seq $nn -1 0); do
    [ "${tstkeys[$ii]}" == "utm" ] && unset tstkeys[$ii]
  done
fi

traps=(0 1 2 6 11 15)
for tt in ${traps[@]}; do
  trap "inthandler $tt" $tt
done

/usr/bin/echo
read -p "Zusammenstellen der Daten aus \"${sender}:${path_aebf}\" nach \"${receiver}:${path_test}\" (j/${xbx}n${xnx}) ? " ans
[ ! "${ans}" == "j" ] && cleanup "Start abgebrochen"

[ -n "${sender}" ] && vms="${sender}:" || vms=""
[ -n "${receiver}" ] && vmr="${receiver}:" || vmr=""

remove_file "${path_test}/Test.*" ${receiver} 

isinkeyL "utm" ${tstkeys[@]}
[ $? -gt 0 ] && sync_path "--src" "${vms}${rvdirs["utm"]}" "--dest" "${vmr}${tstdirs["utm"]}" ${pargrp[@]} ${fldgb} '--exclude' '/**.git'
isinkeyL "toolsbin" ${tstkeys[@]}
[ $? -gt 0 ] && sync_path "--src" "${vms}${rvdirs["toolsbin"]}" "--dest" "${vmr}${tstdirs["toolsbin"]}" ${pargrp[@]} ${fldgb}
isinkeyL "toolslibdata" ${tstkeys[@]}
[ $? -gt 0 ] && sync_path "--src" "${vms}${rvdirs["toolslibdata"]}" "--dest" "${vmr}${tstdirs["toolslibdata"]}" ${pargrp[@]} ${fldgb} '--norm'
isinkeyL "toolslibhsysro" ${tstkeys[@]}
[ $? -gt 0 ] && sync_path "--src" "${vms}${rvdirs["toolslibhsyro"]}" "--dest" "${vmr}${tstdirs["toolslibhsyro"]}" ${pargrp[@]} ${fldgb}
isinkeyL "toolslibini" ${tstkeys[@]}
[ $? -gt 0 ] && sync_path "--src" "${vms}${rvdirs["toolslibini"]}" "--dest" "${vmr}${tstdirs["toolslibini"]}" ${pargrp[@]} ${fldgb}
isinkeyL "toolslibpar" ${tstkeys[@]}
if [ $? -gt 0 ]; then
  includeL=()
  if [ -z "${vms}" ]; then
    declare -a parL
    local_00425 ${rvdirs["toolslibpar"]} parL
    for ff in ${parL[@]}; do
      includeL+=( '--include' )
      includeL+=( "${ff}" )
    done
  fi 
  sync_path "--src" "${vms}${rvdirs["toolslibpar"]}" "--dest" "${vmr}${tstdirs["toolslibpar"]}" ${pargrp[@]} ${fldgb} '--exclude' '00_*' ${includeL[@]}
fi
isinkeyL "toolslibsql" ${tstkeys[@]}
[ $? -gt 0 ] && sync_path "--src" "${vms}${rvdirs["toolslibsql"]}" "--dest" "${vmr}${tstdirs["toolslibsql"]}" ${pargrp[@]} ${fldgb}
isinkeyL "toolslibvlk" ${tstkeys[@]}
if [ $? -gt 0 ]; then
  includeL=()
  if [ -z "${vms}" ]; then
    declare -a vlkL
    local_00425 ${rvdirs["toolslibvlk"]} vlkL
    for ff in ${vlkL[@]}; do
      includeL+=( '--include' )
      includeL+=( "${ff}" )
    done
  fi 
  sync_path "--src" "${vms}${rvdirs["toolslibvlk"]}" "--dest" "${vmr}${tstdirs["toolslibvlk"]}" ${pargrp[@]} ${fldgb} '--exclude' '*_old' '--exclude' '00_*' ${includeL[@]}
fi
isinkeyL "bin" ${tstkeys[@]}
[ $? -gt 0 ] && sync_path "--src" "${vms}${rvdirs["bin"]}" "--dest" "${vmr}${tstdirs["bin"]}" ${pargrp[@]} ${fldgb} '--exclude' '/**.int' '--exclude' '/**.o' '--exclude' 'TPRO.bin' '--exclude' 'libtpro.so' '--exclude' 'ASYN.bin' '--exclude' 'libasyn.so'
isinkeyL "damxs" ${tstkeys[@]}
[ $? -gt 0 ] && sync_path "--src" "${vms}${rvdirs["damxs"]}" "--dest" "${vmr}${tstdirs["damxs"]}" ${pargrp[@]} ${fldgb} '--exclude' '/**.log' '--exclude' '/**.log.*' '--exclude' '/**.c' '--exclude' '/**.o'
isinkeyL "damxu" ${tstkeys[@]}
[ $? -gt 0 ] && sync_path "--src" "${vms}${rvdirs["damxu"]}" "--dest" "${vmr}${tstdirs["damxu"]}" ${pargrp[@]} ${fldgb} '--exclude' '/**.log' '--exclude' '/**.log.*' '--exclude' '/**.c'
isinkeyL "asyn" ${tstkeys[@]}
[ $? -gt 0 ] && sync_path "--src" "${vms}${rvdirs["asyn"]}" "--dest" "${vmr}${tstdirs["asyn"]}" ${pargrp[@]} ${fldgb} '--exclude' '/**.log' '--exclude' '/**.log.*' '--exclude' '/**.c' '--exclude' '/**.o'
isinkeyL "damba" ${tstkeys[@]}
[ $? -gt 0 ] && sync_path "--src" "${vms}${rvdirs["damba"]}" "--dest" "${vmr}${tstdirs["damba"]}" ${pargrp[@]} ${fldgb} '--exclude' '/**.log' '--exclude' '/**.log.*' '--exclude' '/**.c' '--exclude' '/**.o'
isinkeyL "dambb" ${tstkeys[@]}
[ $? -gt 0 ] && sync_path "--src" "${vms}${rvdirs["dambb"]}" "--dest" "${vmr}${tstdirs["dambb"]}" ${pargrp[@]} ${fldgb} '--exclude' '/**.log' '--exclude' '/**.log.*' '--exclude' '/**.c' '--exclude' '/**.o'

create_file "--path" "${path_test}/Test.${aebf}" "--content" "Versionsstand ${aebf} von ${currdate}" ${pargrp[@]} ${parvm[@]}

rmL=()

exit 0 
