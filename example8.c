/* example8.c -- YCLASS usage example

   Copyright (C) 2016-2017 Josua Groeger.

   YCLASS is an extension of the C programming language with
   object oriented elements, mostly by means of the C preprocessor.


   LICENSE

   YCLASS is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   YCLASS is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with YCLASS.  If not, see <http://www.gnu.org/licenses/>.
*/

//#define YCLASS_DEBUG
#include<yclass.h>
#include<stdio.h>

/*
   A signal, in YCLASS terminology, is just a method which has
   (YClass*)ysource as its first argument, which is meant to
   be the calling object.

   This example is supposed to model some graphical user interface
   in which the main method contains the program loop which, upon
   mouse click, invokes a method of some window.

   The next example shall introduce a related concept, YCLASS events,
   which are not yet introduced here for pedagogical reasons.
   Note, however, that using signals without events enforces
   a bad rather style of programming.
   See the next example for a good version of this one.
*/

// YWin: Imagine some window or widget of some GUI such as GTK.
yclass(YWin,YClass,
,
  yvirtual_signal(void,doclick,int i);
  // equivalent to
  // yvirtual(void,doclick,YClass* ysource,int i);

  yvirtual(void,print);
)

yclass(YApp,YMain,
  YWin *win;
,
  yvirtual(void,print);
  yvirtual_signal(void,onclicked,int i);
)
ymain(YApp)

ymethod(void,YWin,print)
{
  printf("YWin print\n");
}

// To be invoked by the main program loop:
// A (physical) mouse click is translated into calling this method.
ymethod_signal(void,YWin,doclick,int i)
{
  printf("YWin doclick i=%d\n",i);

  // ysource is the calling object, in this case the main class.
  ycall_signal(ysource,YApp,onclicked,i);
  // ycall_signal calls a signal with the current object as ysource,
  // i.e. equivalent to
  // ycall(ysource,YApp,onclicked,(YClass*)ythis,i);

  // This is not good: The current YWin class needs to know about
  // details of YApp for this to work (has an onclicked signal).
  // The preferred way is to make doclick an event, see the next example.
}

yconstructor(YWin)
{
  // nothing new here; no need for a ybind_signal macro
  ybind(YWin,doclick);
  ybind(YWin,print);
}

ydestructor(YWin)
{
}

ymethod(void,YApp,print)
{
  printf("YApp print\n");
}

ymethod(int,YApp,main,int argc,char *argv[])
{
  // The main program loop.
  // OK,in fact there is no loop here, but imagine that we wait for
  // user input and, upon mouse click, call the following method,
  // then wait again for next input.
  ycall_signal(ythis->win,YWin,doclick,42);
  // Imagine 42 has the meaning of mouse pointer position.

  return 0;
}

// YWin's doclick is meant to process the click event,
// e.g. check whether the click was in some valid region of the window and,
// if so, call the following signal which should contain some
// logic specific to the current program.
ymethod_signal(void,YApp,onclicked,int i)
{
  printf("YApp onclicked i=%d\n",i);

  // Invoke the print methods of the window and the main class
  ycall(ysource,YWin,print);
  ycall(ythis,YApp,print);
}

yconstructor(YApp)
{
  ybind(YApp,YMain,main);
  ybind(YApp,print);
  ybind(YApp,onclicked);

  ythis->win=ynew(YWin);
}

ydestructor(YApp)
{
  ydelete(ythis->win);
}

