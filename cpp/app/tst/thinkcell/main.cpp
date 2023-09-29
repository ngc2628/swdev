
#include <stdio.h>
#include <stdlib.h>
#include <mkbase/mkmath.h>
#include <mkbase/mkconv.h>
#include <mkbase/mkla.h>
#include <mkbase/mkutil.h>
#include <app/tst/thinkcell/intervalmap.h>

int usage() {

  printf ("\n");
  return 0;

}

int main(int argc,char **argv) {

  std::string ss("lowest");

  interval_map<int,std::string> intervalmap(ss);
  intervalmap.assign(4,10,"martin");
  intervalmap.assign(15,19,"kloss");
  intervalmap.assign(9,17,"tiger");

return 0;

  std::string res=intervalmap[0];
  printf("%d [%s]\n",__LINE__,res.c_str());
  res=intervalmap[7];
  printf("%d [%s]\n",__LINE__,res.c_str());
  res=intervalmap[17];
  printf("%d [%s]\n",__LINE__,res.c_str());

  return 0;

}

