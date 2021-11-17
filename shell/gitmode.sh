#/bin/bash

function usage() {

  msg="usage $0 [git directory]"
  [ $# -gt 0 ] && msg="$1"
  /usr/bin/echo -e "${msg}"
  exit 1  

}

gitdir="."
[ $# -gt 0 ] && gitdir="$1"
[ ! -d "${gitdir}" ] && usage "${gitdir} directory invalid"
[ ! -d "${gitdir}/.git" ] && usage "${gitdir} ist not a git directory"
pushd "${gitdir}" >/dev/null

modL=( $(/usr/bin/git status --short | /usr/bin/awk '{if ($1 ~ "M") print $2;}') )
for ff in ${modL[@]}; do
  /usr/bin/git diff -p -- ${ff} | \
  /usr/bin/grep -E '^(diff|old mode|new mode)' | \
  /usr/bin/sed -e 's/^old/NEW/;s/^new/old/;s/^NEW/new/' | \
  /usr/bin/git apply
done

popd >/dev/null
