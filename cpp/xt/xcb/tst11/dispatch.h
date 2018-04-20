
#ifndef _XCBTST11DISPATCH_H_
#define _XCBTST11DISPATCH_H_

#include <xcb/xcb.h>

extern int dispatchEvent(xcb_connection_t *,xcb_generic_event_t *,int);

#endif
