
#include <xt/xcb/tst11/desktop.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

xcb_connection_t *xcbconnection=0;
struct xcbdesktop_t xcbdesktop[]={
{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},
{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0}
};

int findScreens(xcb_connection_t *conn) {

  const xcb_setup_t *xcbsetup=xcb_get_setup(conn);
  int nscreens=0;
  xcb_screen_iterator_t itscreen=xcb_setup_roots_iterator(xcbsetup);
  while (itscreen.rem>0) {
    xcbdesktop[nscreens++].xcbscreen=itscreen.data;
    xcb_screen_next(&itscreen);
  }
  return nscreens;
}

xcb_drawable_t createSubwindow(xcb_connection_t *conn,struct xcbdesktop_t *desk) {

  // configure
  uint32_t mask=(XCB_CW_BACK_PIXEL|XCB_CW_BORDER_PIXEL|XCB_CW_EVENT_MASK);
  uint32_t values[3]={16000000,14000000,0};
  values[2]|=XCB_EVENT_MASK_EXPOSURE;
  values[2]|=XCB_EVENT_MASK_BUTTON_PRESS;
  values[2]|=XCB_EVENT_MASK_BUTTON_RELEASE;
  int xx=0,yy=0,ww=20,hh=20,bh=2;

  desk->xcbmainwindowbutton=xcb_generate_id(conn);
  xcb_create_window (
    conn,XCB_COPY_FROM_PARENT,desk->xcbmainwindowbutton,desk->xcbmainwindow,
    xx,yy,ww,hh,bh,XCB_WINDOW_CLASS_INPUT_OUTPUT,desk->xcbscreen->root_visual,mask,values
  );

  return desk->xcbmainwindowbutton;
}

xcb_drawable_t spawnDesktop(xcb_connection_t *conn,struct xcbdesktop_t *desk) {

  // configure
  uint32_t mask=(XCB_CW_BACK_PIXEL|XCB_CW_EVENT_MASK);
  uint32_t values[2]={15000000,0};
  values[1]|=XCB_EVENT_MASK_EXPOSURE;
  values[1]|=XCB_EVENT_MASK_KEY_RELEASE;
  //values[1]|=XCB_EVENT_MASK_BUTTON_RELEASE;
  values[1]|=XCB_EVENT_MASK_VISIBILITY_CHANGE;
  values[1]|=XCB_EVENT_MASK_STRUCTURE_NOTIFY;
  values[1]|=XCB_EVENT_MASK_SUBSTRUCTURE_NOTIFY;
  values[1]|=XCB_EVENT_MASK_PROPERTY_CHANGE;
  xcb_get_geometry_cookie_t cookiegeom;
  xcb_get_geometry_reply_t *ansgeom=0;
  int xx=0,yy=0,ww=0,hh=0,bh=0;
  cookiegeom=xcb_get_geometry(conn,desk->xcbscreen->root);
  ansgeom=xcb_get_geometry_reply(conn,cookiegeom,0);
  xx=2000;//ansgeom->x;
  yy=200;//ansgeom->y;
  ww=800;//ansgeom->width;
  hh=600;//ansgeom->height;

  desk->xcbmainwindow=xcb_generate_id(conn);
  xcb_create_window (
    conn,desk->xcbscreen->root_depth,desk->xcbmainwindow,desk->xcbscreen->root,xx,yy,ww,hh,bh,
    XCB_WINDOW_CLASS_INPUT_OUTPUT,desk->xcbscreen->root_visual,mask,values
  );
  return desk->xcbmainwindow;
}

int setupGui(xcb_connection_t *conn) {

  int ii=0,jj=0,nscreens=findScreens(conn);
fprintf(stderr,"%d  %d screens\n",__LINE__,nscreens);
  if (nscreens==0)
    return 0;
  for (ii=0;ii<nscreens;ii++) {
    spawnDesktop(conn,&(xcbdesktop[ii]));
    createSubwindow(conn,&(xcbdesktop[ii]));
  }

  for (ii=0;ii<nscreens;ii++) {
    xcb_map_window(conn,xcbdesktop[ii].xcbmainwindow);
    xcb_map_subwindows(conn,xcbdesktop[ii].xcbmainwindow);
  }

  xcb_flush(conn);

  return nscreens;

}

