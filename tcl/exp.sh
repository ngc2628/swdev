#!/bin/bash

dir="/data/scm/misc"
secret="@penSesam5"

/usr/bin/expect <(cat <<EOM
  log_user 0
  spawn ssh hal9008 /bin/bash --norc -c 'test -d ${dir} && echo ok || echo ko'
  expect "password: "
  send "${secret}\r"
  #expect -re {[0-9]+}
  #puts \$expect_out(0,string)
  interact
EOM
)

