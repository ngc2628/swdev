
#include <xt/xcb/tst11/dispatch.h>
#include <xt/xcb/tst11/desktop.h>
#include <stdlib.h>
#include <stdio.h>
#include <xcb/xcb.h>

int xcbmainloop(xcb_connection_t *conn) {
  xcb_generic_event_t *ev=xcb_wait_for_event(conn);
  int ans=0,offtype=(ev->response_type&(~128));
  while (ev) {
    ans=dispatchEvent(conn,ev,offtype);
    free(ev);
    if ((ans&1024)>0)
      break;
    ev=xcb_wait_for_event(conn);
    offtype=(ev->response_type&(~128));
  }
  return 0;
}

int main(int argc,char **argv) {

  int res=1;
  xcb_connection_t *xcbconnection=xcb_connect(0,0);
  if (xcbconnection && setupGui(xcbconnection)>0)
    res=xcbmainloop(xcbconnection);

  return res;
}
