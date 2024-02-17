//
// Created by andrzej on 8/15/22.
//
#include "raise.h"
#include <cstdlib>
#include <cstdio>
#include <string>
#include <iostream>
#include <vector>
#ifndef _WIN32
#include "XRaise.h"
#include <X11/Xatom.h>
#include <X11/Xlib.h>
#include <csignal>

static long windowProperty(Display *dpy, Window focuswin, const char *property) {
  Atom nameAtom = XInternAtom(dpy, property, 1);
  Atom type;
  int format, status;
  long pid = -1;
  unsigned long nitems, after;
  unsigned char *data;

  status = XGetWindowProperty(dpy, focuswin, nameAtom, 0, 1024, 0,
                              XA_CARDINAL, &type, &format, &nitems, &after, &data);
  if (status == Success && data) {
    pid = *((long *) data);
    XFree(data);
  } else;
  return pid;
}

static long windowPid(Display *dpy, Window focuswin) {
  return windowProperty(dpy, focuswin, "_NET_WM_PID");
}

std::vector<Window> searchHelper(Display *display, int w, unsigned long pid) {
  std::vector<Window> ret;
  unsigned long retpid = windowPid(display, w);
  if (retpid == pid)
    ret.push_back(w);

//check the children of the window
  Window wRoot;
  Window wParent;
  Window *wChild = NULL;
  unsigned nChildren = 0;
  if (XQueryTree(display, w, &wRoot, &wParent, &wChild, &nChildren) != 0) {
    for (unsigned i = 0; i < nChildren; ++i) {
      auto ret1 = searchHelper(display, wChild[i], pid);
      ret.insert(std::end(ret), std::begin(ret1), std::end(ret1));
    }
  }
  return ret;
}

int area(Display *display, Window w) {
  XWindowAttributes attr;
  if (XGetWindowAttributes(display, w, &attr))
    return attr.height * attr.width;
  else
    return 0;
}

Window searchmaxArea(Display *display, std::vector<Window> &winlist) {
  Window result = 0;
  int maxArea = 0;
  for (auto w : winlist) {
    int a = area(display, w);
    if (a > maxArea) {
      maxArea = a;
      result = w;
    }
  }
  return result;
}

int pid2wid(int pid) {
  Display *display = XOpenDisplay(0);
  Window window = XDefaultRootWindow(display);
  std::vector<Window> result = searchHelper(display, window, pid);
  return searchmaxArea(display, result);
}

void raisePid(int pid) {
  int wid = pid2wid(pid);
  if (wid) {
    XRaise win(wid);
    win.fetch();
    win.activate();
  }
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