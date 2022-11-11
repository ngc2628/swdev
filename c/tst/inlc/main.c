

#include <time.h>
#include <stdio.h>

int main(int argc, char **argv) {

  long int i1=11,i2=12,isum=0;
    
  /*  
    contraints x86 :  
      "m" : memory
      "r" : (g)eneral (p)urpose (r)egister 
        (a)ccumulator
        (b)ase
        (c)ounter
        (d)data
        (S)ource [i]ndex
        (D)estination [i]ndex
        (s)tack (p)ointer
        (b)ase (p)ointer
      "a-d,S,D" : named register 
  */
 
  __asm__ __volatile__ 
  (
    "movq %1,%%rax\n" 
    "add %2,%%rax\n"
    "movq %%rax,%0" 
    : "=b"(isum) 
    : "c"(i1), "d"(i2)
  );
    
  printf ("%d i1=%ld i2=%ld isum=%ld\n",
    __LINE__,i1,i2,isum);
  
  double d1=1.e-33, d2=2.e-33, dsum=.0;
  
  __asm__ __volatile__
  (
    "fldl %1\n"
    "fldl %2\n"
    "faddp\n"
    "fstl %0" 
    : "=m" (dsum) 
    : "m" (d1), "m" (d2)
  );
      
  printf ("%d d1=%g d2=%g dsum=%g\n",
    __LINE__,d1,d2,dsum);
  
  time_t tt;
  time(&tt);
  printf ("%s\n",ctime(&tt));
  
	return (0);

}
