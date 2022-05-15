#!/bin/bash

xbx=$(/usr/bin/tput bold)
xnx=$(/usr/bin/tput sgr0)
secret=""
declare -a rmL

# interrupt handler
function inthandler() {

  trap 0 1
  [ $1 -gt 1 ] && msg="Programmende durch Signal <$(/usr/bin/kill -l $1)>"
  cleanup "$msg"
  exit 0

}

# remove local directories created
function cleanup() {

  trap 0 1
  [ $# -gt 0 ] && /usr/bin/echo "$1"
  declare -i nn=${#rmL[@]}
  for (( ii=$nn-1; ii>=0; ii-- )); do
    /usr/bin/echo "entferne ${rmL[$ii]}"
    /usr/bin/rm -rf ${rmL[$ii]}
  done
  /usr/bin/echo "$0 beendet"
  exit 0

}

# check file local or remote 
# check_file "path" "[vm]"
# return 0 : available 
function check_file() {

  [ $# -lt 1 ] && return 1
  local path="${1}"
  shift
  local vm=""
  if [ $# -gt 0 ]; then 
    vm="${1}"
    shift
  fi

  if [ -n "${vm}" ]; then 
    if [ -n "${secret}" ]; then
      nln=$(/usr/bin/expect 2>/dev/null <(/usr/bin/cat <<EOM
log_user 0;
spawn /usr/bin/ssh ${vm} "/bin/bash --norc -c '/usr/bin/stat --format=\"%h\" ${path} 2>/dev/null'";
expect "Password: " { send "${secret}\r" };
expect -re {\r\n(\d+)\r\n};
set nln \$expect_out(1,string);
puts \$nln
interact;
EOM
          ))
    else
      nln=$(/usr/bin/ssh ${vm} "/bin/bash --norc -c '/usr/bin/stat --format=\"%h\" ${path} 2>/dev/null'")
    fi
    nln=$((nln+0))
    [ $nln -ne 1 ] && ans=1 || ans=0
  else
    [ ! -r "${path}" ] && ans=1 || ans=0
  fi

  return $ans

}

# check path local or remote 
# check_path "path" "[vm]"
# return 0 : available 
function check_path() {

  [ $# -lt 1 ] && return 1
  local path="${1}"
  shift
  local vm=""
  if [ $# -gt 0 ]; then 
    vm="${1}"
    shift
  fi

  if [ -n "${vm}" ]; then 
    if [ -n "${secret}" ]; then
      nln=$(/usr/bin/expect 2>/dev/null <(/usr/bin/cat <<EOM
log_user 0;
spawn /usr/bin/ssh ${vm} "/bin/bash --norc -c '/usr/bin/stat --format=\"%h\" ${path} 2>/dev/null'";
expect "Password: " { send "${secret}\r" };
expect -re {\r\n(\d+)\r\n};
set nln \$expect_out(1,string);
puts \$nln
interact;
EOM
          ))
    else
      nln=$(/usr/bin/ssh ${vm} "/bin/bash --norc -c '/usr/bin/stat --format=\"%h\" ${path} 2>/dev/null'")
    fi
    nln=$((nln+0))
    [ $nln -lt 2 ] && ans=1 || ans=0
  else
    [ ! -d "${path}" ] && ans=1 || ans=0
  fi

  return $ans

}

# make local directory path and register every portion
# mkdir_follow "path"
# return 0 : success
function mkdir_follow() {

  local path="$1"
  local dirL=( $(/usr/bin/echo ${path} | /usr/bin/awk -F/ '{for (ii=2;ii<=NF;ii++) print $ii;}') )

  declare -i ans=0
  local dir=""
  for dd in ${dirL[@]}; do
    dir+="/${dd}"
    if [ ! -d "${dir}" ]; then
      /usr/bin/mkdir "${dir}" >/dev/null 2>&1
      ans=$(($ans+$?))
      [ -d "${dir}" ] && rmL+=( "${dir}" )
    fi
  done

  return $ans

}

# create path local or remote
# make_path "--path" "path" ["--group" "group"] ["--vm" "vm"]
# return 0 : success
function make_path() {

  local path=""
  local grp=""
  local vm=""

  while [ $# -gt 0 ]; do
    arg="${1}"
    shift
    if [ "${arg}" == "--path" ]; then
      if [ $# -gt 0 ]; then
        path="${1}"
        shift
      fi
    elif [ "${arg}" == "--group" ]; then
      if [ $# -gt 0 ]; then
        grp="${1}"
        shift
      fi
    elif [ "${arg}" == "--vm" ]; then
      if [ $# -gt 0 ]; then
        vm="${1}"
        shift
      fi
    else
      /usr/bin/echo "$LINENO unbekannter Parameter \"${arg}\""
    fi
  done

  declare -i ans=0

  declare -a pathL=( $(/usr/bin/echo ${path} | /usr/bin/awk -F"/" '{ for (ii=1;ii<=NF;ii++) print $ii;}') )
  local mkp=""

  for ii in $(/usr/bin/seq 0 ${#pathL[@]}); do
  
    mkp="${mkp}/${pathL[$ii]}"
    
    if [ -n "${vm}" ]; then
      if [ -n "${secret}" ]; then
        /usr/bin/expect 2>/dev/null <(/usr/bin/cat <<EOM
log_user 0;
spawn /usr/bin/ssh ${vm} "/bin/bash --norc -c 'umask 0002; /usr/bin/mkdir -p ${mkp}'";
expect "Password: " { send "${secret}\r" };
interact;
EOM
        )
        ans=$((ans+$?))
        if [ -n "${grp}" ]; then
          /usr/bin/expect 2>/dev/null <(/usr/bin/cat <<EOM
log_user 0;
spawn /usr/bin/ssh ${vm} "/bin/bash --norc -c '/usr/bin/chgrp ${grp} ${mkp} 2>/dev/null'";
expect "Password: " { send "${secret}\r" };
interact;
EOM
          )
        fi
      else
        /usr/bin/ssh ${vm} "/bin/bash --norc -c 'umask 0002; /usr/bin/mkdir -p ${mkp}'";
        ans=$((ans+$?))
        [ -n "${grp}" ] && /usr/bin/ssh ${vm} "/bin/bash --norc -c '/usr/bin/chgrp ${grp} ${mkp} 2>/dev/null'";
      fi
    else
      if [ ! -d "${mkp}" ]; then
        mkdir_follow ${mkp}
        ans=$((ans+$?))
        [ -n "${grp}" ] && /usr/bin/chgrp ${grp} ${mkp} 2>/dev/null;
        eval ${grpcmd}
      fi
    fi

  done

  return $ans

}

# create file local or remote
# create_file "--path" "file abs path" "--content" "content" ["--group" "group"] ["--vm" "vm"] 
# return 0 : success
function create_file() {

  local fpath=""
  local grp=""
  local content=" "
  local vm=""

  while [ $# -gt 0 ]; do
    arg="${1}"
    shift
    if [ "${arg}" == "--path" ]; then
      if [ $# -gt 0 ]; then
        fpath="${1}"
        shift
      fi
    elif [ "${arg}" == "--group" ]; then
      if [ $# -gt 0 ]; then
        grp="${1}"
        shift
      fi
    elif [ "${arg}" == "--content" ]; then
      if [ $# -gt 0 ]; then
        content="${1}"
        shift
      fi
    elif [ "${arg}" == "--vm" ]; then
      if [ $# -gt 0 ]; then
        vm="${1}"
        shift
      fi
    else
      /usr/bin/echo "$LINENO unbekannter Parameter \"${arg}\""
    fi
  done

  local grpcmd=":"
  [ -n "${grp}" ] && grpcmd=" /usr/bin/chgrp ${grp} ${fpath}" 

  local ans=0

  if [ -n "${vm}" ]; then
    if [ -n "${secret}" ]; then
      /usr/bin/expect 2>/dev/null <(/usr/bin/cat <<EOM
log_user 0;
spawn /usr/bin/ssh ${vm} "/bin/bash --norc -c 'umask 0002; /usr/bin/echo ${content} > ${fpath} 2>/dev/null; ${grpcmd} 2>/dev/null'";
expect "Password: " { send "${secret}\r" };
interact;
EOM
          )
      ans=$?
    else
      /usr/bin/ssh ${vm} "/bin/bash --norc -c 'umask 0002; /usr/bin/echo ${content} > ${fpath} 2>/dev/null; ${grpcmd} 2>/dev/null'"
      ans=$?
    fi
  else
    /usr/bin/echo ${content} > ${fpath}
    ans=$?
    ${grpcmd} 2>/dev/null
  fi

  return $ans

}

# insert into db using pm se71
# se71_insert "--pdb" "(ORAP|ORAT)" "--pumgebung" "(prod|test)" "--ftbsef" "/path/to/file" ["--path_se71" "/path/to/se71.sh"] ["--vm" "vm"]
# return 0 : success
function se71_insert() {

  local se71sh="/opt/rvdialog/jobsteuerung/tools/bin/sh/se71.sh"
  local pktan="--pktan=25"
  local pumgebung=""
  local pbestand="--pbestand=P"
  local plfnr="--plfnr=SE71"
  local pdb=""
  local phausint="--phausint=J"
  local przbetr="--przbetr=N" 
  local ftbsef=""
  local path_ftbsef=""
  local vm=""
  
  while [ $# -gt 0 ]; do
    arg="${1}"
    shift
    if [ "${arg}" == "--pdb" ]; then
      if [ $# -gt 0 ]; then
        arg="${1}"
        shift
        [ "${arg}" == "ORAP" -o "${arg}" == "ORAT" ] && pdb="--pdb=${arg}"  
      fi
    elif [ "${arg}" == "--pumgebung" ]; then
      if [ $# -gt 0 ]; then
        arg="${1}"
        shift
        [ "${arg}" == "prod" -o "${arg}" == "test" ] && pumgebung="--pumgebung=${arg}"  
      fi
    elif [ "${arg}" == "--ftbsef" ]; then
      if [ $# -gt 0 ]; then
        arg="${1}"
        shift
        ftbsef="--ftbsef=${arg}"
        path_ftbsef="${arg}"
      fi
    elif [ "${arg}" == "--path_se71" ]; then
      if [ $# -gt 0 ]; then
        arg="${1}"
        shift
        se71sh="${arg}"
      fi
    elif [ "${arg}" == "--vm" ]; then
      if [ $# -gt 0 ]; then
        arg="${1}"
        shift
        vm="${arg}"
      fi
    else
      /usr/bin/echo "$LINENO unbekannter Parameter \"${arg}\""
    fi
  done
  
  local ans=0

  [ -z "${pdb}" -o -z "${pumgebung}" -o -z "${ftbsef}" ] && return 1

  [ -n "${vm}" ] && vmstr="${vm}" || vmstr="$(/usr/bin/hostname)"
  /usr/bin/echo
  /usr/bin/echo -e "  Das Kommando\n  ${se71sh} ${pktan} ${pumgebung} ${pbestand} ${plfnr} ${pdb} ${phausint} ${przbetr} ${ftbsef}\n"
  read -p "  wird auf ${vmstr} ausgefuehrt (j/${xbx}n${xnx}) ? " ans
  [ ! "${ans}" == "j" -a ! "${ans}" == "J" ] && return 1  

  if [ -n "${vm}" ]; then
    if [ -n "${secret}" ]; then
      /usr/bin/expect 2>/dev/null <(/usr/bin/cat <<EOM
log_user 0;
spawn /usr/bin/ssh ${vm} "/bin/bash --norc -c '/usr/bin/ls ${path_ftbsef} >/dev/null 2>&1 ; /usr/bin/ls ${se71sh} >/dev/null 2>&1'";
expect "Password: " { send "${secret}\r" };
interact;
EOM
          )
      [ $? -ne 0 ] && return 1
      /usr/bin/expect 2>/dev/null <(/usr/bin/cat <<EOM
log_user 0;
spawn /usr/bin/ssh ${vm} "/bin/bash --norc -c '${se71sh} ${pktan} ${pumgebung} ${pbestand} ${plfnr} ${pdb} ${phausint} ${przbetr} ${ftbsef} >/dev/null 2>&1'";
expect "Password: " { send "${secret}\r" };
interact;
EOM
          )
      ans=$?
    else
      /usr/bin/ssh ${vm} "/bin/bash --norc -c '/usr/bin/ls ${path_ftbsef} >/dev/null 2>&1 ; /usr/bin/ls ${se71sh} >/dev/null 2>&1'"
      [ $? -ne 0 ] && return 1
      /usr/bin/ssh ${vm} "/bin/bash --norc -c '${se71sh} ${pktan} ${pumgebung} ${pbestand} ${plfnr} ${pdb} ${phausint} ${przbetr} ${ftbsef} >/dev/null 2>&1'"
      ans=$?
    fi
  else
    [ ! -r "${path_ftbsef}" -o ! -x "${se71sh}" ] && return 1
    ${se71sh} ${pktan} ${pumgebung} ${pbestand} ${plfnr} ${pdb} ${phausint} ${przbetr} ${ftbsef} >/dev/null 2>&1
    ans=$?
  fi

  return $ans

}

# remove local or remote
# remove "file abs path" "[vm]" 
# return 0 : success
function remove_file() {

  [ $# -lt 1 ] && return 1
  local fpath="${1}"
  shift
  local vm=""
  if [ $# -gt 0 ]; then 
    vm="${1}"
    shift
  fi
  local ans=0

  if [ -n "${vm}" ]; then
    if [ -n "${secret}" ]; then
      /usr/bin/expect 2>/dev/null <(/usr/bin/cat <<EOM
log_user 0;
spawn /usr/bin/ssh ${vm} "/bin/bash --norc -c '/usr/bin/rm -rf ${fpath}  2>/dev/null'";
expect "Password: " { send "${secret}\r" };
interact;
EOM
          )
      ans=$?
    else
      /usr/bin/ssh ${vm} "/bin/bash --norc -c '/usr/bin/rm -rf ${fpath} 2>/dev/null'"
      ans=$?
    fi
  else
    /usr/bin/rm -rf ${fpath}
    ans=$?
  fi

  return $ans

}

# chgrp local or remote
# mod_group --path "path" ["--group" "group"] ["--vm" "vm"] 
# return 0 : success
function mod_group() {

  local fpath=""
  local grp=""
  local vm=""

  while [ $# -gt 0 ]; do
    arg="${1}"
    shift
    if [ "${arg}" == "--path" ]; then
      if [ $# -gt 0 ]; then
        path="${1}"
        shift
      fi
    elif [ "${arg}" == "--group" ]; then
      if [ $# -gt 0 ]; then
        grp="${1}"
        shift
      fi
    elif [ "${arg}" == "--vm" ]; then
      if [ $# -gt 0 ]; then
        vm="${1}"
        shift
      fi
    else
      /usr/bin/echo "$LINENO unbekannter Parameter \"${arg}\""
    fi
  done

  local ans=0

  if [ -n "${vm}" ]; then
    if [ -n "${secret}" ]; then
      /usr/bin/expect 2>/dev/null <(/usr/bin/cat <<EOM
log_user 0;
spawn /usr/bin/ssh ${vm} "/bin/bash --norc -c '/usr/bin/chgrp -R ${grp} ${fpath}  2>/dev/null'";
expect "Password: " { send "${secret}\r" };
interact;
EOM
          )
      ans=$?
    else
      /usr/bin/ssh ${vm} "/bin/bash --norc -c '/usr/bin/chgrp -R ${grp} ${fpath} 2>/dev/null'"
      ans=$?
    fi
  else
    /usr/bin/chgrp -R ${grp} ${fpath}
    ans=$?
  fi

  return $ans

}

# check login to remote host
# expect_login "[vm]" , set secret in case
# return : 0 - success , 1 - user cancel , 2 - rejected
function expect_login {

  [ $# -eq 0 ] && return 0
  local vm="${1}"
  shift
  [ -z "${vm}" ] && return 0

  vmuser=$(/usr/bin/echo ${vm} | /usr/bin/awk -F"@" '{print $1;}')
  [ ! "${vmuser}" == "${vm}" ] && vm=$(/usr/bin/echo ${vm} | /usr/bin/awk -F"@" '{print $2;}')

  local ans=0

  /usr/bin/nc -z "${vm}" 22
  if [ $? -gt 0 ]; then
    /usr/bin/echo "${vm} ist nicht per ssh erreichbar"
    return 2 
  fi
  [ -n "${vmuser}" ] && vmstr="${vmuser}@${vm}" || vmstr="${vm}"
  /usr/bin/ssh -o "BatchMode yes" "${vmstr}" /usr/bin/kill -0 \$\$ 2>/dev/null
  if [ $? -gt 0 ]; then
    read -p "${vmstr} : Login per ssh-key nicht möglich, mit Passwort Eingabe versuchen ? (j/${xbx}n${xnx}) " ans
    [ ! "${ans}" == "j" ] && return 1
    read -s -p "Passwort: " secret
    /usr/bin/echo
    ans=$(/usr/bin/expect 2>/dev/null <(/usr/bin/cat <<EOM
log_user 0;
spawn /usr/bin/ssh -o StrictHostKeyChecking=no ${vmstr} "/bin/bash --norc -c '/usr/bin/date --iso-8601 2>/dev/null'";
expect "Password: " { send "${secret}\r" };
expect -re {\r\n(\d\d\d\d\-\d\d\-\d\d)\r\n};
puts "Success\n";
interact;
EOM
    ))
    [ ! "${ans}" == "Success" ] && return 2
  fi

  return 0

}

# synchronize source files -> dest directory
# sync_files "--src" "[vm:]src" "--src" "[vm:]src" ... "--dest" "[vm:]dest" ["--group" "group"]
# return 0 : Success
function sync_files() {

  local srcL=()
  local dest=""
  local grp=""
  while [ $# -gt 0 ]; do
    arg="${1}"
    shift
    if [ "${arg}" == "--src" ]; then
      if [ $# -gt 0 ]; then
        srcL+=( "${1}" )
        shift
      fi
    elif [ "${arg}" == "--dest" ]; then
      if [ $# -gt 0 ]; then
        dest="${1}"
        shift
      fi
    elif [ "${arg}" == "--group" ]; then
      if [ $# -gt 0 ]; then
        grp="${1}"
        shift
      fi
    else
      /usr/bin/echo "$LINENO unbekannter Parameter \"${arg}\""
    fi    
  done

  [ ${#srcL[@]} -eq 0 -o -z "${dest}" ] && return 1
  local sender=""
  local receiver=""
  local src_path=""
  local dest_path=$(/usr/bin/echo ${dest} | /usr/bin/awk -F: '{print $2;}')
  [ -z "${dest_path}" ] && \
    dest_path=$(/usr/bin/echo ${dest} || /usr/bin/awk -F: '{print $1;}') || \
    receiver=$(/usr/bin/echo ${dest} | /usr/bin/awk -F: '{print $1;}')
  [ -z "${receiver}" ] && pushd ${dest_path} >/dev/null

  local rsync_opts=" -ltp -h "
  [ -n "${grp}" ] && rsync_opts=" -lptog --chown=$(/usr/bin/id -u):${grp} -h " 
  declare -a pargrp=()
  if [ -n "${grp}" ]; then
    pargrp+=( "--group" )
    pargrp+=( "${grp}" )
  fi
  declare -a parvm=()
  if [ -n "${receiver}" ]; then
    parvm+=( "--vm" )
    parvm+=( "${receiver}" )
  fi
  /usr/bin/echo

  for src in ${srcL[@]}; do
    src_path=$(/usr/bin/echo ${src} | /usr/bin/awk -F: '{print $2;}')
    [ -z "${src_path}" ] && \
      src_path=$(/usr/bin/echo ${src} || /usr/bin/awk -F: '{print $1;}') || \
      sender=$(/usr/bin/echo ${src} | /usr/bin/awk -F: '{print $1;}')
    if [ -n "${sender}" -a -n "${receiver}" ]; then
      /usr/bin/echo "$LINENO: Warnung : kopieren remote <-> remote ist mit rsync nicht moeglich"
      continue
    fi
    /usr/bin/echo "$LINENO: ${sender}:${src_path} -> ${receiver}:${dest_path} ----------"

    check_file "${src_path}" "${sender}"
    if [ $? -gt 0 ]; then
      /usr/bin/echo "$LINENO: Warnung : konnte ${sender}:${src_path} nicht finden"
      continue
    fi

    make_path "--path" ${dest_path} ${pargrp[@]} ${parvm[@]} 
    if [ $? -gt 0 ]; then
      /usr/bin/echo "$LINENO: Warnung : konnte ${receiver}:${dest_path} nicht erstellen"
      return 1
    fi

    [ -n "${sender}" ] && src_par="${sender}:${src_path}" || src_par="${src_path}"
    [ -n "${receiver}" ] && dest_par="${receiver}:${dest_path}" || dest_par="${dest_path}"
    [ -n "${sender}" -o -n "${receiver}" ] && sec="${secret}" || sec=""
  
    if [ -n "${sec}" ]; then
      /usr/bin/expect <(/usr/bin/cat <<EOM
log_user 0;
spawn /usr/bin/rsync ${rsync_opts} ${src_par} ${dest_par};
expect "Password: " { send "${sec}\r" };
interact;
EOM
      )
      ans=$?
    else
      /usr/bin/rsync ${rsync_opts} ${src_par} ${dest_par}
      ans=$?
    fi
    /usr/bin/echo "$LINENO: done ${src_par} -> ${dest_par} ----------"
  done

  /usr/bin/echo
  [ -z "${receiver}" ] && popd >/dev/null
  return $ans

}

# synchronize : src-dir <-> dest-dir ... rsync options selection
# sync_path "--src" "[vm:]src" "--dest" "[vm:]dest" ["--group" "group"] ["--exclude" "rsync-regexp" ...] 
#  ["--include" "rsync-regexp" ...] ["--debug"] ["--norm"] "["--opt" "extra-opt"]
# return 0 : success
function sync_path() {

  local rsync_opt_std=" -rltp -h "
  local rsync_opt_chown=" -rltpog -h --chown=$(/usr/bin/id -u):"
  local rsync_opt_dirtimes=" --omit-dir-times "
  local rsync_opt_del=" --delete-before --delete-excluded "
  local rsync_opt_ndbg=" --info=STATS2 "
  local rsync_opts=""
  local src=""
  local dest=""
  local sender=""
  local receiver=""
  local dbg=""
  local grp=""
  local norm=""
  local -a cmdL
  local -a exclL
  local -a inclL
  local -a optL

  while [ $# -gt 0 ]; do
    arg="${1}"
    shift
    if [ "${arg}" == "--src" ]; then
      if [ $# -gt 0 ]; then
        src="${1}"
        shift
      fi
    elif [ "${arg}" == "--dest" ]; then
      if [ $# -gt 0 ]; then
        dest="${1}"
        shift
      fi
    elif [ "${arg}" == "--group" ]; then
      if [ $# -gt 0 ]; then
        grp="${1}"
        shift
      fi
    elif [ "${arg}" == "--exclude" ]; then
      if [ $# -gt 0 ]; then
        exclL+=("${arg} ")
        exclL+=("${1} ")
        shift
      fi
    elif [ "${arg}" == "--include" ]; then
      if [ $# -gt 0 ]; then
        inclL+=("${arg} ")
        inclL+=("${1} ")
        shift
      fi
    elif [ "${arg}" == "--debug" ]; then
      if [ $# -gt 0 ]; then
        dbg="${1}"
        shift
      fi
    elif [ "${arg}" == "--norm" ]; then
      norm="norm"
    elif [ "${arg}" == "--opt" ]; then
      optL+=( "${arg}" )
    else
      /usr/bin/echo "$LINENO unbekannter Parameter \"${arg}\""
    fi    
  done

  local src_path=$(/usr/bin/echo ${src} | /usr/bin/awk -F: '{print $2;}')
  [ -z "${src_path}" ] && \
    src_path=$(/usr/bin/echo ${src} || /usr/bin/awk -F: '{print $1;}') || \
    sender=$(/usr/bin/echo ${src} | /usr/bin/awk -F: '{print $1;}')
  local dest_path=$(/usr/bin/echo ${dest} | /usr/bin/awk -F: '{print $2;}')
  [ -z "${dest_path}" ] && \
    dest_path=$(/usr/bin/echo ${dest} || /usr/bin/awk -F: '{print $1;}') || \
    receiver=$(/usr/bin/echo ${dest} | /usr/bin/awk -F: '{print $1;}')
  [ -n "${sender}" -a -n "${receiver}" ] && return 1

  declare -a pargrp=()
  if [ -n "${grp}" ]; then
    pargrp+=( "--group" )
    pargrp+=( "${grp}" )
    rsync_opts="${rsync_opt_chown}${grp} "
  else
    rsync_opts="${rsync_opt_std}"
  fi
  [ -n "${dbg}" ] && rsync_opts="${rsync_opts} ${dbg} " || rsync_opts="${rsync_opts} ${rsync_opt_ndbg} "
  [ -z "${norm}" ] && rsync_opts="${rsync_opts} ${rsync_opt_del} "
  rsync_opts="${rsync_opts} ${rsync_opt_dirtimes} "

  /usr/bin/echo
  /usr/bin/echo "$LINENO: ${sender}:${src_path}/ -> ${receiver}:${dest_path} ----------"

  check_path "${src_path}" "${sender}"
  if [ $? -gt 0 ]; then
    /usr/bin/echo "$LINENO: Warnung : konnte ${sender}:${src_path} nicht finden"
    return 1
  fi

  declare -a parvm=()
  if [ -n "${receiver}" ]; then
    parvm+=( "--vm" )
    parvm+=( "${receiver}" )
  fi
  make_path "--path" ${dest_path} ${pargrp[@]} ${parvm[@]} 
  if [ $? -gt 0 ]; then
    /usr/bin/echo "$LINENO: Warnung : konnte ${receiver}:${dest_path} nicht erstellen"
    return 1
  fi

  [ -z "${receiver}" ] && pushd ${dest_path} >/dev/null
  [ -n "${sender}" ] && src_par="${sender}:${src_path}" || src_par="${src_path}"
  [ -n "${receiver}" ] && dest_par="${receiver}:${dest_path}" || dest_par="${dest_path}"
  [ -n "${sender}" -o -n "${receiver}" ] && sec="${secret}" || sec=""
  
  declare -i ans=0

  if [ -n "${sec}" ]; then
    /usr/bin/expect <(/usr/bin/cat <<EOM
log_user 0;
spawn /usr/bin/rsync ${rsync_opts} ${inclL[@]} ${exclL[@]} ${optL[@]} ${src_par}/ ${dest_par};
expect "Password: " { send "${sec}\r" };
interact;
EOM
      )
    ans=$?
  else
    /usr/bin/rsync ${rsync_opts} \
      --include-from=<(for ((ii=1;ii<=${#inclL[@]};ii+=2)); do echo ${inclL[$ii]}; done) \
      --exclude-from=<(for ((ii=1;ii<=${#exclL[@]};ii+=2)); do echo ${exclL[$ii]}; done) \
      ${optL[@]} ${src_par}/ ${dest_par}
    ans=$?
  fi
  /usr/bin/echo "$LINENO: done ${src_par}/ -> ${dest_par} ----------"
  /usr/bin/echo

  [ -z "${receiver}" ] && popd >/dev/null
  return $ans

}


# check for key in array
# isinkeyL "key" "keylist"
# return 0 : no , 1 : yes
function isinkeyL() {

  [ $# -lt 2 ] && return 0
  local key="${1}"
  shift
  local -a keyL=( "$@" )

  for chk in ${keyL[@]}; do
    [ "${chk}" == "${key}" ] && return 1
  done

  return 0

}

function local_00425() {

  [ $# -lt 2 ] && return 1
  local folder=$1
  shift
  local -n arr00=$1
  shift
  [ ! -d "${folder}" ] && return 1
  pushd "${folder}" >/dev/null
  f00L=( $(/usr/bin/ls -1 00_*) )
  f25L=( $(/usr/bin/ls -1 25_*) )
  for ff in ${f00L[@]}; do
    name25="${ff/00_/25_}"
    isinkeyL "${name25}" ${f25L[@]}
    [ $? -eq 0 ] && arr00+=( "${ff}" )
  done
  popd >/dev/null

  return 0
}

# strip symbols from .bin and .so and other modules
# strip_path "/path/to/dir" "module" "module" ...
# additional module in path
function strip_path() {

  local stripath=""
  [ $# -gt 0 ] && stripath="${1}"
  [ -z "${stripath}" -o ! -d "${stripath}" ] && return 1
  shift
  local -a modL=()
  while [ $# -gt 0 ]; do
    modL+=( "${stripath}/${1}" )
    shift
  done
  for ff in $(/usr/bin/ls -1 ${stripath}/*.so 2>/dev/null); do
    modL+=( "${ff}" )
  done
  for ff in $(/usr/bin/ls -1 ${stripath}/*.bin 2>/dev/null); do
    modL+=( "${ff}" )
  done

  /usr/bin/strip -s ${modL[@]}

  return 0                                                     

}

# determine best aebf no
function aebf_dir() {

  [ $# -lt 1 -o ! -d "$1" ] && return 1
  local aebfdir="${1}"

  pushd "${aebfdir}" >/dev/null
  local -a list1 list2
  for dd in $(/usr/bin/ls -1 "$1"); do
    if [ ! -d "$dd" ]; then
      continue
    fi
    if [[ "$dd" =~ ^aebf\.[0-9]\.[0-9][0-9][0-9]\.?[0-9]?$ ]]; then
      list1=( ${list1[@]:-} "${dd:5}" )
    elif [[ "$dd" =~ ^[0-9]\.[0-9][0-9][0-9]\.?[0-9]?$ ]]; then
      list1=( ${list1[@]:-} "${dd}" )
    fi
  done
  local lcnt=1
  while [ ${#list1[@]} -gt 1 -a $lcnt -lt 10 ]; do
    str=$(
      for dd in ${list1[@]}; do
        /usr/bin/echo "$dd"
      done | /usr/bin/sort -r -n -t "." -k $lcnt
    )
    list1=()
    list2=( $(/usr/bin/echo $str) )
    cnt=${#list2[@]}
    ii=0
    while [[ $ii -lt $cnt ]]; do
      list1=( ${list1[@]:-} ${list2[$ii]} )
      v1=$(/usr/bin/echo ${list2[$ii]} | /usr/bin/awk -v lcnt=$lcnt 'BEGIN {FS="."} {print $lcnt;}')
      [ $ii -eq $((cnt-1)) ] && jj=$ii || jj=$(($ii+1))
      v2=$(/usr/bin/echo ${list2[$jj]} | /usr/bin/awk -v lcnt=$lcnt 'BEGIN {FS="."} {print $lcnt;}')
# read -p "<$lcnt> <$ii><${list2[$ii]}><$v1> <$jj><${list2[$jj]}><$v2>"
      [ -z "$v1" ] && v1=0
      [ -z "$v2" ] && v2=0
      if [ $v2 -lt $v1 ]; then
        break
      fi
    ((ii++))
    done
    if [ ${#list1[@]} -le 1 ]; then
      break
    fi
    ((lcnt++))
  done

  local maxver=${list1}
# read -p "<$maxver>"
  popd >/dev/null
  /usr/bin/echo $maxver
  [ -n "$maxver" ] && return 0 || return 1

}

function file_size_meg() {

  [ $# -lt 1 -o ! -r "$1" -o ! -f "$1" ] && return 1
  local file="${1}"
  shift
  local bsz=$(/usr/bin/stat --format="%s" ${file})
  local msz=0
msz=$(/usr/bin/bc <<EOI
scale=3
ans=$bsz / ( 1024 * 1024 )
ans
EOI
)
  /usr/bin/echo $msz
  return 0

} 

