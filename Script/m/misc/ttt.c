
#include <time.h>
#include <sys/time.h>
#include <sys/timeb.h>
#include <dlfcn.h>

/* gcc -D_GNU_SOURCE -Wall -fPIC -shared -o ttc.so ttt.c -ldl */

static int (*orgettimeofday)(struct timeval *, struct timezone *)=0;
static int (*ortime)(time_t *)=0;
static int (*orftime)(struct timeb *)=0;

int gettimeofday(struct timeval *tv, struct timezone *tz) {

  if (!orgettimeofday)
    orgettimeofday = dlsym((void*)-1L, "gettimeofday");
  
  int res=(*orgettimeofday)(tv,tz);
  tv->tv_sec-=8640000;
  return res; 

}

time_t time (time_t *tt) {

  if (!ortime)
    ortime = dlsym((void*)-1L, "time");
  time_t ttt=0;  
  (*ortime)(&ttt);
  ttt-=8640000;
  if (tt)
    *tt=ttt;
  return ttt;
  
}

int ftime(struct timeb *tb) {

  if (!orftime)
    orftime = dlsym((void*)-1L, "ftime");

  int res=(*orftime)(tb);
  tb->time-=8640000;
  return res;

}

