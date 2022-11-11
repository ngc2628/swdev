#!/bin/bash

[ $# -lt 1 ] && exit 1
fpath="${1}"
fname=$(/usr/bin/basename ${fpath})
fdir=$(/usr/bin/dirname ${fpath})

# stdout console
/usr/bin/awk  'BEGIN {FS="";} {str=""; for (ii=200;ii<210;ii++) str=str$ii; print str;}' ${fpath}

# in-place vim
/usr/bin/cp ${fpath} /tmp
/usr/bin/vim -c ':1,$s/\%>209c.//g' -c '1,$s/\%<200c.//g' -c ':wq!' ${fpath}
/usr/bin/cat ${fpath}
/usr/bin/mv /tmp/${fname} ${fdir}

