#!/bin/bash

function usage() {

  [ $# -gt 0 ] && msg="$1" || msg="usage $0 -p \"pass\""
  /usr/bin/echo -e "${msg}"
  exit 1

}

bindir=$(/usr/bin/dirname "$(/usr/bin/readlink -m $0)")
[ ! -x "${bindir}/pun" ] && usage "password failed"

pass1=$(${bindir}/pun -d <<"EOM"
B:jy,5{ING 
EOM
)
pass1=$(/usr/bin/echo "${pass1}" | /usr/bin/awk -F"=" '{print $2;}')
/usr/bin/sed -i "/martin@kloss.berlin/s/passwd/${pass1}/" ~siberia/.fetchmailrc
/usr/bin/sed -i "/martin.kloss@fuzzball.eu/s/passwd/${pass1}/" ~siberia/.fetchmailrc
/usr/bin/sed -i "/gt@fuzzball.eu/s/passwd/${pass1}/" ~siberia/.fetchmailrc
pass2=$(${bindir}/pun -d <<"EOM"
B:j1,5{ING
EOM
)
pass2=$(/usr/bin/echo "${pass2}" | /usr/bin/awk -F"=" '{print $2;}')
/usr/bin/sed -i "/mail@kloss.berlin/s/passwd/${pass2}/" ~siberia/.fetchmailrc
pass3=$(${bindir}/pun -d <<"EOM"
*+c?r-GSz'''
EOM
)
pass3=$(/usr/bin/echo "${pass3}" | /usr/bin/awk -F"=" '{print $2;}')
/usr/bin/sed -i "/biz@fuzzball.eu/s/passwd/${pass3}/" ~siberia/.fetchmailrc
/usr/bin/fetchmail -v
/usr/bin/sed -i "/martin@kloss.berlin/s/${pass1}/passwd/" ~siberia/.fetchmailrc
/usr/bin/sed -i "/mail@kloss.berlin/s/${pass2}/passwd/" ~siberia/.fetchmailrc
/usr/bin/sed -i "/martin.kloss@fuzzball.eu/s/${pass1}/passwd/" ~siberia/.fetchmailrc
/usr/bin/sed -i "/gt@fuzzball.eu/s/${pass1}/passwd/" ~siberia/.fetchmailrc
/usr/bin/sed -i "/biz@fuzzball.eu/s/${pass3}/passwd/" ~siberia/.fetchmailrc


