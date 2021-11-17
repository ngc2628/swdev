/* 
  Test auf ein anzusprechendes X11 Display als Argument oder default Display (null)
  X11 Development Kit ist nicht verfuegbar -> dynamic load/resolve
  02.03.2021  martin.kloss@drv-berlin-brandenburg.de 
*/

#include <stdio.h>
#include <string.h>
#include <dlfcn.h>

typedef void* (*f_xopendisplay)();

int main (int args, char **argv) {

  char dispname[65];
  memset(&dispname[0],0,65);
  if (args>1 && strlen(argv[1])>0)
    strncpy(dispname,argv[1],64);

  void *libx11=dlopen("libX11.so.6",RTLD_NOW|RTLD_LOCAL);
  if (libx11) {
    f_xopendisplay xopendisplay=(f_xopendisplay)dlsym(libx11,"XOpenDisplay");
    if (xopendisplay) {
      void *disp=(*xopendisplay)(strlen(dispname)>0 ? &dispname[0] : 0);
      if (disp)
        return 0;
    }
  }

  return 1;

}
