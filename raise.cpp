//
// Created by andrzej on 8/15/22.
//
#include "raise.h"
#include <cstdlib>
#include <cstdio>
#include <string>
#include <iostream>
#ifndef _WIN32
#include "XRaise.h"
#include <X11/Xatom.h>
#include <X11/Xlib.h>
#include <csignal>

bool searchHelper(Display* display, Window w, Atom& atomPID, unsigned long pid, Window& result)
{
    bool ret = false;

    Atom atomType;
    int format;
    unsigned long nItems;
    unsigned long bytesAfter;
    unsigned char* propPID = 0;
    if (Success == XGetWindowProperty(display,w,atomPID,0,1,False,XA_CARDINAL,&atomType,&format,&nItems,&bytesAfter,&propPID))
    {
        if (propPID != 0)
        {
            if (pid == *((unsigned long *)propPID))
            {
                result = w;
                ret = true;
            }
            XFree(propPID);
        }
    }

    if (ret)
        return result; //we found we can stop

//check the children of the window
    Window wRoot;
    Window wParent;
    Window *wChild=NULL;
    unsigned nChildren=0;
    if (XQueryTree(display, w, &wRoot, &wParent, &wChild, &nChildren) != 0 )
    {
        for (unsigned i=0; i<nChildren; ++i)
        {   //<------------always last found ret?
            ret = searchHelper(display, wChild[i], atomPID, pid, result);
        }
    }
    return result;
}

int pid2wid(int pid) {
    Display *display = XOpenDisplay(0);
    Window window = XDefaultRootWindow(display);
    Atom atomPID = XInternAtom(display, "_NET_WM_PID", true);
    if (atomPID == None)
    {
        return 1;
    }
    Window result;
    searchHelper(display, window, atomPID, pid, result);
    return result;
}

void raisePid(int pid) {
    int wid = pid2wid(pid);
    XRaise win(wid);
    win.fetch();
    win.activate();
}
#endif

void raiseThis() {
#ifdef _WIN32
    activateWindow();
    raise();
#else
    int pid = getpid();
    raisePid(pid);
#endif
}