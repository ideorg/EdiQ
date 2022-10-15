//
// Created by Andrzej Borucki on 2022-08-17
//
#include "EdiException.h"
#include "XRaise.h"


bool XRaise::tryGetCurrentWorkspace(const char *prop_name, uint &result) {
    Atom property = XInternAtom(display, prop_name, false);
    Atom req_type = XA_CARDINAL;
    long long_offset = 0;
    long long_length = 1; //size property type XA_CARDINAL in 32-bit words

    Atom actual_type_return;
    int actual_format_return;
    ulong nitems_return;
    ulong bytes_after_return;
    unsigned char *prop_return;
    if (XGetWindowProperty(display, root, property, long_offset, long_length, false,
                           req_type, &actual_type_return, &actual_format_return,
                           &nitems_return, &bytes_after_return, &prop_return) != Success) {
        return false;
    }
    if (actual_type_return != req_type) {
        XFree(prop_return);
        return false;
    }
    result = *prop_return;
    XFree(prop_return);
    return true;
}

void XRaise::xclientSend(const char *msg, long data) {
    /* mask is very important, enables true bringing to front */
    long mask = SubstructureNotifyMask | SubstructureRedirectMask;
    XEvent event;
    event.xclient.type = ClientMessage;
    event.xclient.serial = 0;
    event.xclient.send_event = true;
    event.xclient.message_type = XInternAtom(display, msg, false);
    event.xclient.window = w;
    event.xclient.format = 32;
    event.xclient.data.l[0] = data;
    event.xclient.data.l[1] = 0;
    event.xclient.data.l[2] = 0;
    event.xclient.data.l[3] = 0;
    event.xclient.data.l[4] = 0;

    if (!XSendEvent(display, root, false, mask, &event))
        throw EdiException("Can't send event");
}

XRaise::XRaise(Window w) : w(w) {
    if (!(display = XOpenDisplay(nullptr)))
        throw EdiException("Can't open display");
    root = DefaultRootWindow(display);
}

XRaise::~XRaise() {
    XCloseDisplay(display);
}

uint XRaise::getCurrentWorkspace() {
    uint currentWorkspace = 0;
    if (!tryGetCurrentWorkspace("_NET_CURRENT_DESKTOP", currentWorkspace))
        if (!tryGetCurrentWorkspace("_WIN_WORKSPACE", currentWorkspace))
            throw EdiException("can't get current workspace\n");
    return currentWorkspace;
}

/* fetch window to current workspace */
void XRaise::fetch() {
    xclientSend("_NET_WM_DESKTOP", getCurrentWorkspace());
}

void XRaise::activate() {
    /* this true activates window */
    xclientSend("_NET_ACTIVE_WINDOW", 0);
    /* just in case */
    XMapRaised(display, w);
}