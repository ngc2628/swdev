
#include <xt/xcb/tst11/dispatch.h>
#include <xt/xcb/tst11/desktop.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int dispatchEvent(xcb_connection_t *conn,xcb_generic_event_t *ev,int type) {

  int ii=0,ans=0;
  xcb_drawable_t win=0;
  xcb_void_cookie_t cookievoid;
  xcb_intern_atom_cookie_t cookieatom;
  xcb_intern_atom_reply_t *ansatom=0;
  xcb_get_property_cookie_t cookieprop;
  xcb_get_property_reply_t *ansprop=0;
  xcb_generic_error_t *err=0;
  unsigned int *hints=(unsigned int*)malloc(20);
  memset(hints,0,20);
  hints[0]=2; // MWM_HINTS_DECORATIONS;
  hints[2]=0;
  uint32_t configvalues[1]={0};

  if (type==XCB_KEY_RELEASE) {
    xcb_key_press_event_t *kev=(xcb_key_press_event_t *)ev;
    win=kev->event;
    if (kev->detail==9) // escape
      ans|=1024;
    else if (kev->detail==10) {
      configvalues[0]=XCB_STACK_MODE_BELOW;
      xcb_configure_window(conn,win,XCB_CONFIG_WINDOW_STACK_MODE,configvalues);
      ans|=1;
    }
    else if (kev->detail==11) {
      cookieatom=xcb_intern_atom(conn,0,15,"_MOTIF_WM_HINTS");
      ansatom=xcb_intern_atom_reply(conn,cookieatom,&err);
      cookievoid=xcb_change_property(
        conn,XCB_PROP_MODE_REPLACE,win,
        ansatom->atom,ansatom->atom,32,5,(const void*)hints
      );
      ans|=5;
    }
    else if (kev->detail==12) {
      cookieatom=xcb_intern_atom(conn,0,7,"WM_NAME");
      ansatom=xcb_intern_atom_reply(conn,cookieatom,0);
      cookievoid=xcb_change_property(
        conn,XCB_PROP_MODE_REPLACE,win,
        ansatom->atom,XCB_ATOM_STRING,8,13,(const void*)"hello martin"
      );
      ans|=1;
    }
    else if (kev->detail==13) {
      ans|=1;
    }
//fprintf(stderr,"%d  [%d,%d,%d]\n",__LINE__,type,kev->detail,ans);
  }
  else if (type==XCB_BUTTON_RELEASE) {
    xcb_button_release_event_t *mev=(xcb_button_release_event_t *)ev;
    win=mev->event;
    if (win==xcbdesktop[0].xcbmainwindowbutton) {
      configvalues[0]=1500;
      xcb_configure_window(conn,xcbdesktop[0].xcbmainwindow,XCB_CONFIG_WINDOW_WIDTH,configvalues);
      ans|=1;
    }
  }

  if ((ans&1)>0) {
    if (win) {
      if ((ans&4)>0) {
        xcb_unmap_window(conn,win);
        xcb_flush(conn);
        xcb_map_window(conn,win);
      }
      xcb_flush(conn);
      cookieatom=xcb_intern_atom(conn,0,15,"_MOTIF_WM_HINTS");
      ansatom=xcb_intern_atom_reply(conn,cookieatom,0);
      cookieprop=xcb_get_property(conn,0,win,ansatom->atom,ansatom->atom,0,20);
      ansprop=xcb_get_property_reply(conn,cookieprop,0);
      unsigned int *iansprop=(unsigned int*)xcb_get_property_value(ansprop);
      fprintf(stderr,"%d win [%u] ",__LINE__,win);
      for (ii=0;ii<5;ii++) {
        fprintf(stderr,"[%u] ",*iansprop);
        iansprop++;
      }
      fprintf(stderr,"\n");
      cookieatom=xcb_intern_atom(conn,0,7,"WM_NAME");
      ansatom=xcb_intern_atom_reply(conn,cookieatom,0);
      cookieprop=xcb_get_property(conn,0,win,ansatom->atom,XCB_ATOM_STRING,0,32);
      ansprop=xcb_get_property_reply(conn,cookieprop,0);
      fprintf(stderr,"%d [%s]\n",__LINE__,(char*)xcb_get_property_value(ansprop));
    }
  }

  return ans;
}
