#include "sync_XAlarm.h"

#include <X11/Xlib.h>
/* XExtension */
#include <X11/extensions/sync.h>

#include <string.h>
#include <stdio.h>


static XSyncAlarm alrm;
static XSyncAlarmAttributes al_attr;
static long unsigned int flags = 0x0;

int sync_XAlarm(Display *dpy){
/* XSync */
/*
https://nrk.neocities.org/articles/x11-timeout-with-xsyncalarm
*/
    int sync_event, sync_error, major, minor;
    
    if (!XSyncQueryExtension(dpy, &sync_event, &sync_error))
    {
	fprintf(stderr, "XSync extension not available");
	return 1;
    }
    if (!XSyncInitialize(dpy, &major, &minor))
    {
	fprintf(stderr, "failed to initialize XSync extension");
	return 1;
    }
    printf("XSync major %d minor %d\n", major, minor);

    int ncounter;
    XSyncSystemCounter *counters;
    XSyncCounter servertime = None;
    if ((counters = XSyncListSystemCounters(dpy, &ncounter)) != NULL)
    {
	for (int i = 0; i < ncounter; i++)
	{
	    // fprintf(stderr, "%s\n", counters[i].name);
	    if (strcmp(counters[i].name, "SERVERTIME") == 0)
	    {
		servertime = counters[i].counter;
		break;
	    }
	}
	XSyncFreeSystemCounterList(counters);
    }
    if (servertime == None)
    {
	fprintf(stderr, "SERVERTIME counter not found");
	return 1;
    }
#if 0 
    XSyncAlarmAttributes al_attr;
    long unsigned int flags = 0;
#endif
    al_attr.trigger.counter = servertime;
    flags |= XSyncCACounter;
#ifdef DEBUG
    XSyncIntToValue(&al_attr.trigger.wait_value, 100);
#else
	XSyncIntToValue(&al_attr.trigger.wait_value, 1000);
#endif
    flags |= XSyncCAValue;
    al_attr.trigger.value_type = XSyncRelative;
    flags |= XSyncCAValueType;
    al_attr.trigger.test_type = XSyncPositiveComparison;
    flags |= XSyncCATestType;
/* set the delta to 0, this will deactivate the alarm after firing once. */
    XSyncIntToValue(&al_attr.delta, 0);
    flags |= XSyncCADelta;

    alrm = XSyncCreateAlarm(dpy, flags, &al_attr);

    /*
     * end of alarm
     */
	return sync_event;
}

int reload_XAlarm(Display *dpy){
	/* restart alarm */
	XSyncChangeAlarm(dpy, alrm, flags, &al_attr);
	return 1;
	}
	
int destroy_XAlarm(Display *dpy){
	XSyncDestroyAlarm(dpy, alrm);
	return 1;
}
