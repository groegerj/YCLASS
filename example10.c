/* example10.c -- YCLASS usage example

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
   This example is an extension of example 9.
   We will see that the signal/event mechanism is compatible with
   inheritance of classes.
*/

// YWin is as in the previous example
yclass(YWin,YClass,
,
  yvirtual_event(void,doclick,int i);
  yvirtual(void,print);
)

ymethod(void,YWin,print)
{
  printf("YWin print\n");
}

ymethod_event(void,YWin,doclick,int i)
{
  printf("YWin doclick i=%d\n",i);

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

// From YWin, we derive a class YWin2, which overwrites the doclick event.

yclass(YWin2,YWin)

ymethod_event(void,YWin2,doclick,int i)
{
  printf("YWin2 doclick i=%d\n",i);

  // Call parent's doclick event, including the hidden arguments.
  ysuper_event(YWin,YWin,doclick,i);
}

yconstructor(YWin2)
{
  ybind(YWin2,YWin,doclick);
}

ydestructor(YWin2)
{
}

// The class YApp as in the previous example, with two differences:
// First, YWin *win is replaced by YWin2 *win (for no other reason than
// to have an example for the ysuper_event macro above.
// Second, YApp is no longer the main class, but the derived class
// YBetterApp below.

yclass(YApp,YMain,
  YWin2 *win;
,
  yvirtual(void,print);
  yvirtual_signal(void,onclicked,int i);
)

ymethod(void,YApp,print)
{
  printf("YApp print\n");
}

ymethod(int,YApp,main,int argc,char *argv[])
{
  // It is safe to assume that ythis->onclicked is the
  // "correct" method also in child classes.
  ycall_event(ythis->win,YWin,doclick,ythis->onclicked,42);

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

  ythis->win=ynew(YWin2);
}

ydestructor(YApp)
{
  ydelete(ythis->win);
}

// YBetterApp, a child class of YApp, is going to be our main class.
// We leave the main method as in YApp, but overwrite the onclicked signal.
// This overwritten onclicked is then, correctly, passed to the doclick
// event in YApp's main method.

yclass(YBetterApp,YApp)
ymain(YBetterApp)

ymethod_signal(void,YBetterApp,onclicked,int i)
{
  printf("YBetterApp onclicked i=%d\n",i);

  ysuper_signal(YApp,YApp,onclicked,i);
}

yconstructor(YBetterApp)
{
  ybind(YBetterApp,YApp,onclicked);
}

ydestructor(YBetterApp)
{
}

