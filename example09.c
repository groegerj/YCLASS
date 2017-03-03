/* example9.c -- YCLASS usage example

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
   This is example 8 again, but this time done correctly.

   Like a signal, a YCLASS event is a special kind of method.
   Think of an event as some method that is meant to be called upon
   a "real event", such as a mouse click, and then, depending on
   the situation (the mouse pointer position etc.), call back
   some signal. That signal must be compatible in the sense of
   having the same parameters.

   Technically, an event method is a method with two
   arguments hidden, the first being ysource (as in a signal),
   and the second a pointer to a compatible signal.
*/

yclass(YWin,YClass,
,
  // doclick again, but this time an event as it should be.
  yvirtual_event(void,doclick,int i);

  yvirtual(void,print);
)

ymethod(void,YWin,print)
{
  printf("YWin print\n");
}

// Analogous to ymethod_signal, with two additional parameters:
// ysource as in a signal, and yslot which is the callback signal.
ymethod_event(void,YWin,doclick,int i)
{
  printf("YWin doclick i=%d\n",i);

  // At this place, we want to call the onclick signal of YApp.
  // In the previous example, we did this directly.
  // However, it is much better not to rely on this information.

  // This event was triggered (by ycall_event below) with the
  // onclick signal as a parameter.
  // This pointer is available under the name yslot here.

  // So we could legally write
  // yslot(ysource,ythis(YClass),i);
  // with ysource the object which called this event (here YApp),
  // and ythis is given to that signal as its own ysource parameter.

  // YCLASS provides an abbreviation for this. Equivalent to yslot(...)
  // is the following line:

  ycallback(i);
}

yconstructor(YWin)
{
  ybind(YWin,doclick);
  ybind(YWin,print);
}

ydestructor(YWin)
{
}

yclass(YApp,YMain,
  YWin *win;
,
  yvirtual(void,print);
  yvirtual_signal(void,onclicked,int i);
)
ymain(YApp)

ymethod(void,YApp,print)
{
  printf("YApp print\n");
}

ymethod(int,YApp,main,int argc,char *argv[])
{
  // Call the doclick event with the onclicked signal as
  // its yslot callback parameter.

  ycall_event(ythis->win,YWin,doclick,ythis->onclicked,42);

  // YCLASS's signal/event system is very simple:
  // Instead of adding onclicked to some list of signals
  // to be invoked by the doclick event, there is only
  // one callback signal at a time.
 
  return 0;
}

ymethod_signal(void,YApp,onclicked,int i)
{
  printf("YApp onclicked i=%d\n",i);

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

