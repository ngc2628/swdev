#!/bin/bash

# statische Pfade
declare -A rvdirs
rvdirs["utm"]="utm"
rvdirs["damxs"]="damxs"
rvdirs["damxu"]="damxu"
rvdirs["damxp"]="damxp"
rvdirs["asyn"]="asyn"
for ll in {a,b,1,2,3,4}; do
  rvdirs["damb${ll}"]="damb${ll}"
done
rvdirs["bin"]="lib/src/target/bin"
rvdirs["libdata"]="jobsteuerung/tools/lib/data"
rvdirs["libhsyro"]="jobsteuerung/tools/lib/hsyro"
rvdirs["libini"]="jobsteuerung/tools/lib/ini"
rvdirs["libpar"]="jobsteuerung/tools/lib/par"
rvdirs["libsql"]="jobsteuerung/tools/lib/sql"
rvdirs["libvlk"]="jobsteuerung/tools/lib/vlk"
rvdirs["tools"]="jobsteuerung/tools/bin"

declare -A avdirs
avdirs["utmprod"]="utm"
avdirs["damxs"]="damxs"
avdirs["damxu"]="damxu"
avdirs["damxp"]="damxp"
avdirs["asyntest"]="asyn"
avdirs["asynprod"]="asyn"
for ll in {a,b,1,2,3,4}; do
  avdirs["damb${ll}"]="damb${ll}"
done
avdirs["bintest"]="bin"
avdirs["binprod"]="bin"
avdirs["libdatatest"]="lib/data"
avdirs["libhsyrotest"]="lib/hsyro"
avdirs["libinitest"]="lib/ini"
avdirs["libpartest"]="lib/par"
avdirs["libsqltest"]="lib/sql"
avdirs["libvlktest"]="lib/vlk"
avdirs["libdataprod"]="lib/data"
avdirs["libhsyroprod"]="lib/hsyro"
avdirs["libiniprod"]="lib/ini"
avdirs["libparprod"]="lib/par"
avdirs["libsqlprod"]="lib/sql"
avdirs["libvlkprod"]="lib/vlk"
avdirs["toolstest"]="tools"
avdirs["toolsprod"]="tools"

avkeys=( ${!rvdirs[@]} )

currdate=$(/usr/bin/date +"%y-%m-%d %H:%M:%S")

function usage() {

  trap 0 1
  [ $# -gt 0 ] && /usr/bin/echo -e "$1"
  /usr/bin/echo
  /usr/bin/echo -e "Installation mit rsync - Doppelpunkt trennt vm vom Pfad - mindestens eine Seite (Quelle oder Ziel) muss lokal sein\n\
    \"usage \"$0\" \n\
    \"--path_av /Pfad/zu/AV (Installationsverzeichnis - z.B. s25b0032:/opt/rvdialog/av)\" \n\
    \"--path_pmvw /Pfad/zu/pmvw (Programmverwaltung z.B. s25b0036:/opt/rvdialog)\" \n\
    \"--version Versions-Nummer (z.B. 2.106.9 - die neunte Korrektur (gesammelte Infomails + mehr) zum Aenderungsbrief 2.106)\" \n\
    \"[--test] in das Unterverzeichnis test verteilen \n\
    \"[--prod] in das Unterverzeichnis prod verteilen \n\
    \"[--dir Verzeichnis] [--dir Verzeichnis] ... nur einzelne Verzeichnisse verteilen \n\
     damxp asyn bin damb1..4 libdata libhsyro libini libpar libsql libvlk tools\" \n\
    \"[--group] Gruppe Gruppenzugehoerigkeit erstellter Objekte\n\
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
path_av=""
sender=""
receiver=""
version=""
fldbg=""
dirL=()
grp=""
declare -i prodtest=0

while [ $# -gt 0 ]; do
  par="${1}"
  shift
  if [ "${par}" == "--version" ]; then
    if [ $# -gt 0 ]; then
      version="$1"
      shift
    fi
  elif [ "${par}" == "--path_pmvw" ]; then
    if [ $# -gt 0 ]; then
      path_pmvw="$1"
      shift
    fi
  elif [ "${par}" == "--path_av" ]; then
    if [ $# -gt 0 ]; then
      path_av="$1"
      shift
    fi
  elif [ "${par}" == "--test" ]; then
    prodtest=$((prodtest | 1))
  elif [ "${par}" == "--prod" ]; then
    prodtest=$((prodtest | 2))
  elif [ "${par}" == "--dir" ]; then
    if [ $# -gt 0 ]; then
      dirL+=( "${1}" )
      shift
    fi
  elif [ "${par}" == "--group" ]; then
    if [ $# -gt 0 ]; then
      grp="$1"
      shift
    fi
  elif [ "${par}" == "--debug" ]; then
    if [ $# -gt 0 ]; then
      fldbg="--debug -${1}"
      shift
    fi
  elif [ "${par}" == "--help" -o "${par}" == "-h" -o "${par}" == "/?" ]; then
    usagex
  else
    usage "Warnung : unbekannter Parameter \"${par}\""
  fi
done

[ -z "${path_pmvw}" ] && usage "der Pfad zu pmvw ist ungueltig"
[ -z "${path_av}" ] && usage "der Zielpfad \"\" ist ungueltig" 
if [ -z "${version}" ]; then
  version=$(version_dir ${path_pmvw}) 
  read -p "soll ${version} verwendet werden ? (j/${xbx}n${xbn}) " ans
  [ ! "${ans}" == "j" -a ! "${ans}" == "J" ] && usage "bitte version Nr. angeben"
fi 
[[ ! "${version}" =~ ^2\.1[0-9][0-9][\.0-9]?[0-9]?[0-9]?[\.0-9]?[0-9]?[0-9]?$ ]] && usage "Version \"${version}\" ist ungueltig"
aebf=$(/usr/bin/echo ${version} | /usr/bin/sed -n 's/^\(2\.1[0-9][0-9]\).*/\1/p')

pathvm=$(/usr/bin/echo ${path_pmvw} | /usr/bin/awk -F: '{print $2;}')
if [ -n "${pathvm}" ]; then
  sender=$(/usr/bin/echo ${path_pmvw} | /usr/bin/awk -F: '{print $1;}')
  path_pmvw=$(/usr/bin/echo ${path_pmvw} | /usr/bin/awk -F: '{print $2;}')
fi
pathvm=$(/usr/bin/echo ${path_av} | /usr/bin/awk -F: '{print $2;}')
if [ -n "${pathvm}" ]; then
  receiver=$(/usr/bin/echo ${path_av} | /usr/bin/awk -F: '{print $1;}')
  path_av=$(/usr/bin/echo ${path_av} | /usr/bin/awk -F: '{print $2;}')
fi
[ -n "${sender}" -a -n "${receiver}" ] && usage "entweder Sender oder Empfaenger muss lokal verfuegbar sein"

[ $prodtest -eq 0 ] && prodtest=3

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

expect_login "${sender}"
ans=$?
[ $ans -eq 1 ] && usage "Start abgebrochen"
[ $ans -eq 2 ] && usage "Login auf \"${sender}\" nicht moeglich"
expect_login "${receiver}"
ans=$?
[ $ans -eq 1 ] && usage "Start abgebrochen"
[ $ans -eq 2 ] && usage "Login auf \"${receiver}\" nicht moeglich"

path_version="${path_pmvw}/${version}"
check_path "${path_version}" "${sender}"
[ $? -gt 0 ] && usage "${sender}:${path_version} ist ungueltig"

path_avversion="${path_av}/${version}"
make_path "--path" "${path_avversion}" ${parvm[@]} ${pargrp[@]}
check_path "${path_avversion}" "${receiver}"
[ $? -gt 0 ] && usage "${receiver}:${path_avversion} ist ungueltig"

for dd in ${!rvdirs[@]}; do
  [ "${dd}" == "utm" ] && pre="${path_version}/.." || pre="${path_version}"
  rvdirs["${dd}"]="${pre}/${rvdirs["${dd}"]}"
done
avdirs["utmprod"]="${path_avversion}/../${avdirs["utmprod"]}"
avdirs["damxs"]="${path_avversion}/test/dam/${avdirs["damxs"]}"
avdirs["damxu"]="${path_avversion}/test/dam/${avdirs["damxu"]}"
avdirs["damxp"]="${path_avversion}/prod/dam/${avdirs["damxp"]}"
avdirs["asyntest"]="${path_avversion}/test/dam/${avdirs["asyntest"]}"
avdirs["asynprod"]="${path_avversion}/prod/dam/${avdirs["asynprod"]}"
avdirs["damba"]="${path_avversion}/test/dam/${avdirs["damba"]}"
avdirs["dambb"]="${path_avversion}/test/dam/${avdirs["dambb"]}"
for ll in {1,2,3,4}; do
  avdirs["damb${ll}"]="${path_avversion}/prod/dam/${avdirs["damb${ll}"]}"
done
avdirs["bintest"]="${path_avversion}/test/${avdirs["bintest"]}"
avdirs["binprod"]="${path_avversion}/prod/${avdirs["binprod"]}"
avdirs["libdatatest"]="${path_avversion}/test/${avdirs["libdatatest"]}"
avdirs["libhsyrotest"]="${path_avversion}/test/${avdirs["libhsyrotest"]}"
avdirs["libinitest"]="${path_avversion}/test/${avdirs["libinitest"]}"
avdirs["libpartest"]="${path_avversion}/test/${avdirs["libpartest"]}"
avdirs["libsqltest"]="${path_avversion}/test/${avdirs["libsqltest"]}"
avdirs["libvlktest"]="${path_avversion}/test/${avdirs["libvlktest"]}"
avdirs["libdataprod"]="${path_avversion}/prod/${avdirs["libdataprod"]}"
avdirs["libhsyroprod"]="${path_avversion}/prod/${avdirs["libhsyroprod"]}"
avdirs["libiniprod"]="${path_avversion}/prod/${avdirs["libiniprod"]}"
avdirs["libparprod"]="${path_avversion}/prod/${avdirs["libparprod"]}"
avdirs["libsqlprod"]="${path_avversion}/prod/${avdirs["libsqlprod"]}"
avdirs["libvlkprod"]="${path_avversion}/prod/${avdirs["libvlkprod"]}"
avdirs["toolstest"]="${path_avversion}/test/${avdirs["toolstest"]}"
avdirs["toolsprod"]="${path_avversion}/prod/${avdirs["toolsprod"]}"

nn=$(( ${#dirL[@]}-1 ))
for ii in $(/usr/bin/seq $nn -1 0); do
  isinkeyL "${dirL[$ii]}" ${avkeys[@]}
  [ $? -eq 0 ] && unset dirL[$ii]
done
if [ ${#dirL[@]} -gt 0 ]; then
  avkeys=( ${dirL[@]} )
else
  nn=$(( ${#avkeys[@]}-1 ))
  for ii in $(/usr/bin/seq $nn -1 0); do
    [ "${avkeys[$ii]}" == "utm" ] && unset avkeys[$ii]
  done  
fi

traps=(0 1 2 6 11 15)
for tt in ${traps[@]}; do
  trap "inthandler $tt" $tt
done

/usr/bin/echo -e "\n${currdate}\n"
read -p "Zusammenstellen der Daten aus \"${sender}:${path_version}\" nach \"${receiver}:${path_avversion}\" (j/${xbx}n${xnx}) ? " ans
[ ! "${ans}" == "j" ] && cleanup "Start abgebrochen"

[ -n "${sender}" ] && vms="${sender}:" || vms=""
[ -n "${receiver}" ] && vmr="${receiver}:" || vmr=""

isinkeyL "utm" ${avkeys[@]}
if [ $? -gt 0 ]; then 
  sync_path "--src" "${vms}${rvdirs["utm"]}" "--dest" "${vmr}${avdirs["utmprod"]}" ${pargrp[@]} ${fldbg} '--exclude' '/**.git' '--include' '/**PROD/cfg' '--exclude' '/**cfg' '--exclude' '/**skripte/*gen*' '--exclude' '/**skripte/*utmwrk*'
  /usr/bin/date +"%y-%m-%d %H:%M:%S"
fi
isinkeyL "bin" ${avkeys[@]}
if [ $? -gt 0 ]; then
  [ $((prodtest & 1)) -gt 0 ] && \
    sync_path "--src" "${vms}${rvdirs["bin"]}" "--dest" "${vmr}${avdirs["bintest"]}" ${pargrp[@]} ${fldbg} '--exclude' '/**.int' '--exclude' '/**.o' '--exclude' 'TPRO.bin' '--exclude' 'ASYN.bin' 
  if [ $((prodtest & 2)) -gt 0 ]; then
    sync_path "--src" "${vms}${rvdirs["bin"]}" "--dest" "${vmr}${avdirs["binprod"]}" ${pargrp[@]} ${fldbg} '--exclude' '/**.int' '--exclude' '/**.o' '--exclude' '/**.idy' '--exclude' 'TPRO.bin' '--exclude' 'libtpro.so' '--exclude' 'ASYN.bin' '--exclude' 'libasyn.so' 
    [ -z "${vmr}" ] && strip_path "${avdirs["binprod"]}"
  fi
  /usr/bin/date +"%y-%m-%d %H:%M:%S"
fi
isinkeyL "libdata" ${avkeys[@]}
if [ $? -gt 0 ]; then
  [ $((prodtest & 1)) -gt 0 ] && sync_path "--src" "${vms}${rvdirs["libdata"]}" "--dest" "${vmr}${avdirs["libdatatest"]}" ${pargrp[@]} ${fldbg} 
  [ $((prodtest & 2)) -gt 0 ] && sync_path "--src" "${vms}${rvdirs["libdata"]}" "--dest" "${vmr}${avdirs["libdataprod"]}" ${pargrp[@]} ${fldbg} 
fi
isinkeyL "libhsyro" ${avkeys[@]}
if [ $? -gt 0 ]; then
  [ $((prodtest & 1)) -gt 0 ] && sync_path "--src" "${vms}${rvdirs["libhsyro"]}" "--dest" "${vmr}${avdirs["libhsyrotest"]}" ${pargrp[@]} ${fldbg} 
  [ $((prodtest & 2)) -gt 0 ] && sync_path "--src" "${vms}${rvdirs["libhsyro"]}" "--dest" "${vmr}${avdirs["libhsyroprod"]}" ${pargrp[@]} ${fldbg} 
fi
isinkeyL "libini" ${avkeys[@]}
if [ $? -gt 0 ]; then
  [ $((prodtest & 1)) -gt 0 ] && sync_path "--src" "${vms}${rvdirs["libini"]}" "--dest" "${vmr}${avdirs["libinitest"]}" ${pargrp[@]} ${fldbg} 
  [ $((prodtest & 2)) -gt 0 ] && sync_path "--src" "${vms}${rvdirs["libini"]}" "--dest" "${vmr}${avdirs["libiniprod"]}" ${pargrp[@]} ${fldbg} 
fi
isinkeyL "libpar" ${avkeys[@]}
if [ $? -gt 0 ]; then
  includeL=()
  if [ -z "${vms}" ]; then
    declare -a parL
    local_00425 ${rvdirs["libpar"]} parL
    for ff in ${parL[@]}; do
      includeL+=( '--include' )
      includeL+=( "${ff}" )
    done
  fi
  [ $((prodtest & 1)) -gt 0 ] && sync_path "--src" "${vms}${rvdirs["libpar"]}" "--dest" "${vmr}${avdirs["libpartest"]}" ${pargrp[@]} ${fldbg} '--exclude' '/00_*' ${includeL[@]}
  [ $((prodtest & 2)) -gt 0 ] && sync_path "--src" "${vms}${rvdirs["libpar"]}" "--dest" "${vmr}${avdirs["libparprod"]}" ${pargrp[@]} ${fldbg} '--exclude' '/00_*' ${includeL[@]}
fi
isinkeyL "libsql" ${avkeys[@]}
if [ $? -gt 0 ]; then
  [ $((prodtest & 1)) -gt 0 ] && sync_path "--src" "${vms}${rvdirs["libsql"]}" "--dest" "${vmr}${avdirs["libsqltest"]}" ${pargrp[@]} ${fldbg} 
  [ $((prodtest & 2)) -gt 0 ] && sync_path "--src" "${vms}${rvdirs["libsql"]}" "--dest" "${vmr}${avdirs["libsqlprod"]}" ${pargrp[@]} ${fldbg} 
fi
isinkeyL "libvlk" ${avkeys[@]}
if [ $? -gt 0 ]; then
  includeL=()
  if [ -z "${vms}" ]; then
    declare -a vlkL
    local_00425 ${rvdirs["libvlk"]} vlkL
    for ff in ${vlkL[@]}; do
      includeL+=( '--include' )
      includeL+=( "${ff}" )
    done
  fi
  [ $((prodtest & 1)) -gt 0 ] && sync_path "--src" "${vms}${rvdirs["libvlk"]}" "--dest" "${vmr}${avdirs["libvlktest"]}" ${pargrp[@]} ${fldbg} '--exclude' '/00_*' ${includeL[@]}
  [ $((prodtest & 2)) -gt 0 ] && sync_path "--src" "${vms}${rvdirs["libvlk"]}" "--dest" "${vmr}${avdirs["libvlkprod"]}" ${pargrp[@]} ${fldbg} '--exclude' '/00_*' ${includeL[@]}
fi
isinkeyL "tools" ${avkeys[@]}
if [ $? -gt 0 ]; then 
  [ $((prodtest & 1)) -gt 0 ] && sync_path "--src" "${vms}${rvdirs["tools"]}" "--dest" "${vmr}${avdirs["toolstest"]}" ${pargrp[@]} ${fldbg} 
  [ $((prodtest & 2)) -gt 0 ] && sync_path "--src" "${vms}${rvdirs["tools"]}" "--dest" "${vmr}${avdirs["toolsprod"]}" ${pargrp[@]} ${fldbg} 
fi
/usr/bin/date +"%y-%m-%d %H:%M:%S"
isinkeyL "asyn" ${avkeys[@]}
if [ $? -gt 0 ]; then
  [ $((prodtest & 1)) -gt 0 ] && sync_path "--src" "${vms}${rvdirs["asyn"]}" "--dest" "${vmr}${avdirs["asyntest"]}" ${pargrp[@]} ${fldbg} '--exclude' '/**.log' '--exclude' '/**.log.*' '--exclude' '/**.txt' '--exclude' '/**.c'
  [ $((prodtest & 2)) -gt 0 ] && sync_path "--src" "${vms}${rvdirs["asyn"]}" "--dest" "${vmr}${avdirs["asynprod"]}" ${pargrp[@]} ${fldbg} '--exclude' '/**.log' '--exclude' '/**.log.*' '--exclude' '/**.c' '--exclude' '/**.o'
  [ -z "${vmr}" ] && strip_path "${avdirs["asynprod"]}"  "rtsora64" "utmwork"
fi
isinkeyL "damxs" ${avkeys[@]}
[ $? -gt 0 -a $((prodtest & 1)) -gt 0 ] && sync_path "--src" "${vms}${rvdirs["damxs"]}" "--dest" "${vmr}${avdirs["damxs"]}" ${pargrp[@]} ${fldbg} '--exclude' '/**.log' '--exclude' '/**.log.*' '--exclude' '/**.txt' '--exclude' '/**.c'  
isinkeyL "damxu" ${avkeys[@]}
[ $? -gt 0 -a $((prodtest & 1)) -gt 0 ] && sync_path "--src" "${vms}${rvdirs["damxu"]}" "--dest" "${vmr}${avdirs["damxu"]}" ${pargrp[@]} ${fldbg} '--exclude' '/**.log' '--exclude' '/**.log.*' '--exclude' '/**.txt' '--exclude' '/**.c'
isinkeyL "damba" ${avkeys[@]}
[ $? -gt 0 -a $((prodtest & 1)) -gt 0 ] && sync_path "--src" "${vms}${rvdirs["damba"]}" "--dest" "${vmr}${avdirs["damba"]}" ${pargrp[@]} ${fldbg} '--exclude' '/**.log' '--exclude' '/**.log.*' '--exclude' '/**.txt' '--exclude' '/**.c' 
isinkeyL "dambb" ${avkeys[@]}
[ $? -gt 0 -a $((prodtest & 1)) -gt 0 ] && sync_path "--src" "${vms}${rvdirs["dambb"]}" "--dest" "${vmr}${avdirs["dambb"]}" ${pargrp[@]} ${fldbg} '--exclude' '/**.log' '--exclude' '/**.log.*' '--exclude' '/**.txt' '--exclude' '/**.c' 
isinkeyL "damxp" ${avkeys[@]}
if [ $? -gt 0 -a $((prodtest & 2)) -gt 0 ]; then
  sync_path "--src" "${vms}${rvdirs["damxp"]}" "--dest" "${vmr}${avdirs["damxp"]}" ${pargrp[@]} ${fldbg} '--exclude' '/**.log' '--exclude' '/**.log.*' '--exclude' '/**.c' '--exclude' '/**.o' '--exclude' '/**.txt' '--exclude' 'genlog'
  [ -z "${vmr}" ] && strip_path "${avdirs["damxp"]}/dialog"  "rtsora64" "utmwork"
fi
for ll in {1,2,3,4}; do
  isinkeyL "damb${ll}" ${avkeys[@]}
  if [ $? -gt 0 -a $((prodtest & 2)) -gt 0 ]; then
    sync_path "--src" "${vms}${rvdirs["damb${ll}"]}" "--dest" "${vmr}${avdirs["damb${ll}"]}" ${pargrp[@]} ${fldbg} '--exclude' '/**.log' '--exclude' '/**.log.*' '--exclude' '/**.c' '--exclude' '/**.o' '--exclude' '/**.txt' '--exclude' 'genlog'
    [ -z "${vmr}" ] && strip_path "${avdirs["damb${ll}"]}" "rtsora64" "utmwork"
  fi
done
/usr/bin/date +"%y-%m-%d %H:%M:%S"

function tbsfile() {

  declare -i cnt=0
  local path_src="${1}"
  shift
  local path_dst="${1}"
  shift
  local name="${1}"
  shift
  local fcnt=$(/usr/bin/find ${path_src} -name \*.tbs 2>/dev/null | /usr/bin/wc -l)
  /usr/bin/echo -e "$(/usr/bin/date +"%y-%m-%d %H:%M:%S")\n${fcnt} Dateien"
  while IFS=  read -r -d $'\0' ff; do
    /usr/bin/cat ${ff} >> ${path_dst}/${name}
    ((cnt++))
    /usr/bin/echo -ne "\r$cnt Dateien verarbeitet"
  done < <(/usr/bin/find ${path_src} -name \*.tbs -print0 2>/dev/null)
  fsz=$(file_size_meg "${path_dst}/${name}")
  /usr/bin/echo -e "$(/usr/bin/date +"%y-%m-%d %H:%M:%S")\nfertig - ${name} : ${fsz} Megabyte"

  return 0

}

read -p "Ladedatei fuer Textbausteine erstellen (${xbx}n${xnx}/j) " ans
if [ "${ans}" == "j" -o "${ans}" == "J" ]; then
  /usr/bin/date +"%y-%m-%d %H:%M:%S"
  pathtbs=""
  read -p "Pfad zum tbs Verzeichnis (z.B. /opt/rvdialog/tbs) : " pathtbs
  pathtbs=$(/usr/bin/readlink -m ${pathtbs} 2>/dev/null)
  [ ! -d "${pathtbs}" -o ! -x "${pathtbs}" ] && pathtbs=""
  if [ -n "${pathtbs}" ]; then
    declare -i cnt=0
    path_tmp="/tmp/$$"
    mkdir_follow "${path_tmp}"
    read -p "Name der tbs Ladedatei (leer=\"ftbsef.dat\") : " ans
    [ -n "${ans}" ] && fname="${ans}" || fname="ftbsef.dat"
    if [ -d "${pathtbs}/.git" ]; then  # is repo
      pushd "${pathtbs}" >/dev/null
      currbranch=$(/usr/bin/git rev-parse --abbrev-ref HEAD)
      refL=( $(/usr/bin/git for-each-ref --format="%(refname)" refs/remotes) )
      typeL=()
      [ $((prodtest & 2)) -gt 0 ] && typeL+=( "prod" )
      [ $((prodtest & 1)) -gt 0 ] && typeL+=( "test" )
      for br in ${typeL[@]}; do
        for ref in ${refL[@]}; do 
          lref=$(/usr/bin/echo ${ref} | /usr/bin/awk -F"/" '{print $3"/"$4"/"$5;}')
          reltype=$(/usr/bin/echo ${lref} | /usr/bin/awk -F"/" '{print $2;}')
          tbsaebf=$(/usr/bin/echo ${lref} | /usr/bin/awk -F"/" '{print $3;}')
          if [ "${reltype}" == "${br}" ]; then
            if [ -n "${tbsaebf}" -a "${tbsaebf}" == "${aebf}" ]; then
              /usr/bin/git checkout -b tmpbr$$ ${lref} 
              if [ $? -eq 0 ]; then
                tbsfile "${pathtbs}" "${path_tmp}" "${fname}"
                if [ $? -eq 0 ]; then
                  sync_files "--src" "${path_tmp}/${fname}" "--dest" "${vmr}${path_avversion}/${br}" ${pargrp[@]}
                fi
                /usr/bin/rm -f "${path_tmp}/${fname}"
                /usr/bin/git checkout "${currbranch}"
                /usr/bin/git branch -D tmpbr$$ 
              fi
            fi
          fi
        done
        /usr/bin/date +"%y-%m-%d %H:%M:%S"
      done
    fi
    /usr/bin/rm -rf "${path_tmp}"
  fi
fi

[ -n "${vmr}" ] && exit 0

if [ ${#avkeys[@]} -eq 1 -a "${avkeys[0]}" == "utm" ]; then
  rmL=()
  exit 0
fi

pushd "${path_avversion}" >/dev/null

if [ $((prodtest & 1)) -gt 0 ]; then
  remove_file "${path_avversion}/test/Test.*" ${receiver} 
  create_file "--path" "${path_avversion}/test/Test.${version}" "--content" "Versionsstand ${aebf} vom ${currdate}" ${pargrp[@]} ${parvm[@]} 
  logfile="test.log"
  /usr/bin/echo "schreibe ${logfile}"
  create_file "--path" "${logfile}" ${pargrp[@]}
  /usr/bin/find test -type f -not -name "KDCA" -not -name "PAR" -not -name "utmwork" -exec /usr/bin/md5sum {} \; | /usr/bin/sort -k2 >${logfile} 2>/dev/null
  create_file "--path" "${logfile%.log}.md5" ${pargrp[@]}
  while read -r ff; do
    /usr/bin/md5sum "${ff}"
  done < <(/usr/bin/cat "${logfile}" | /usr/bin/awk '{print $2;}') | /usr/bin/md5sum > "${logfile%.log}.md5" 2>/dev/null
fi

if [ $((prodtest & 2)) -gt 0 ]; then
  remove_file "${path_avversion}/prod/Prod.*" ${receiver} 
  create_file "--path" "${path_avversion}/prod/Prod.${version}" "--content" "Versionsstand ${aebf} vom ${currdate}" ${pargrp[@]} ${parvm[@]} 
  logfile="prod.log"
  /usr/bin/echo "schreibe ${logfile}"
  create_file "--path" "${logfile}" ${pargrp[@]}
  /usr/bin/find prod -type f -not -name "KDCA" -not -name "PAR" -not -name "utmwork" -exec /usr/bin/md5sum {} \; | /usr/bin/sort -k2 >${logfile} 2>/dev/null
  create_file "--path" "${logfile%.log}.md5" ${pargrp[@]}
  while read -r ff; do
    /usr/bin/md5sum "${ff}"
  done < <(/usr/bin/cat "${logfile}" | /usr/bin/awk '{print $2;}') | /usr/bin/md5sum >"${logfile%.log}.md5" 2>/dev/null
fi

popd >/dev/null

rmL=()

exit 0 
