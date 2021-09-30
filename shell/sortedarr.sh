#!/bin/bash

function cmpi() {
  declare -i cmp1=${1}
  declare -i cmp2=${2}
  if [ $cmp1 -lt $cmp2 ]; then
    return 2
  elif [ $cmp2 -lt $cmp1 ]; then
    return 1
  fi
  return 0
}

function cmps() {
  if [ "${1}" \< "${2}" ]; then
    return 2
  elif [ "${2}" \< "${1}" ]; then
    return 1
  fi
  return 0
}

function hsort() {
  local fn=$1
  shift
  local arr=$1
  shift
  declare -i ii=0
  declare -i jj=0
  declare -i cnt=0
  eval "cnt=\${#${arr}[@]}"
  declare -i ub=$((cnt-1))
  declare -i mb=$((cnt/2))
  local tmp=""
  local cmp1=""
  local cmp2=""  
  while [ $mb -gt -1 ]; do
    if [ $mb -eq 0 ]; then
      eval "tmp=\${${arr}[\$ub]}"
      eval "${arr}[\$ub]=\${${arr}[0]}"
      ((ub--))
      if [ $ub -eq 0 ]; then
        eval "${arr}[0]=\${tmp}"
        break
      fi
    else
      ((mb--))
      eval "tmp=\${${arr}[\$mb]}"
    fi    
    ii=$mb
    jj=$((ii+ii+1))
    while [ $jj -le $ub ]; do
      if [ $jj -lt $ub ]; then
        eval "cmp1=\${${arr}[\$jj]}"
        eval "cmp2=\${${arr}[\$((jj+1))]}"
        ${fn} ${cmp1} ${cmp2}
        if [ $? -eq 2 ]; then
          ((jj++))
        fi
      fi
      cmp1=${tmp}
      eval "cmp2=\${${arr}[\$jj]}"
      ${fn} ${cmp1} ${cmp2}
      if [ $? -eq 2 ]; then
        eval "${arr}[\$ii]=\${${arr}[\$jj]}"
        ii=$jj
        jj=$((jj+ii+1))
      else
        break
      fi
    done
    eval "${arr}[\$ii]=\${tmp}"
  done
}

 uarr=( "the" "quick" "quack" "brown" "fox" "jumps" "over" "the" "lazy" "dog" )
# uarr=( "t" "c" "o" "x" "a" "7" "b" "f" "d" )
# uarr=( 11 121 2 6 -111 5 -5 4 10 564 9 8 24 1 )

for itm in ${uarr[@]}; do
  echo -n "$itm "
done
echo

hsort "cmps" "uarr" 

for itm in ${uarr[@]}; do
  echo -n "$itm "
done
echo

function bsearch() {

  local itm=$1
  shift
  local fn=$1
  shift
  local arr=$1
  shift
  if [ $# -gt 0 ]; then
    declare -n ans=$1
    shift
  fi
  declare -i lb=-1
  declare -i mb=0
  declare -i ub=0
  eval "ub=\${#${arr}[@]}"
  declare -i df=$((ub-lb))
  local tmp=""
  while [ $df -gt 1 ]; do
    mb=$(((ub+lb)/2))
    eval "tmp=\${${arr}[\$mb]}"
    ${fn} ${tmp} ${itm}
    res=$?
    if [ $res -eq 0 ]; then
      if [ -n "${ans:-}" ]; then
        ans=$mb
        return 0
      fi
      return $mb
    fi
    if [ $res -eq 2 ]; then
      lb=$mb
    else
      ub=$mb
    fi
    df=$((ub-lb))
  done

  if [ -n "${ans:-}" ]; then
    ans=-1
    return 1
  fi
  return 255

}

lookfor="quick"
echo "find \"$lookfor\""
declare -i res=-1
bsearch $lookfor "cmps" "uarr" res
echo "$? $res"














