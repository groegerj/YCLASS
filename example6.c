/* example6.c -- YCLASS usage example

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
#include <yclass.h>
#include <stdio.h>

/*
   This is example5 slightly modified. We will see abstract classes
   and learn more about ysuper.

   Have a look at our new YInt class. As before, it comes with two methods
   but, this time, only "print" is defined (via ymethod and ybind),
   while "set" is purely virtual.
   In other words, YInt is sort of an abstract class.
   Yet, there may be objects, and we can even attempt to call "set",
   see below for what will happen then.
*/

yclass(YInt,YClass,
  int i;
,
  yvirtual(void,set,int i);
  yvirtual(void,print);
)

ymethod(void,YInt,print)
{
  printf("YInt print: i=%d\n",ythis->i);
}

yconstructor(YInt)
{
  ybind(YInt,print);
}

ydestructor(YInt)
{
}

/*
   YInt2: We overwrite "print" and implement "set".
*/

yclass(YInt2,YInt)

ymethod(void,YInt2,print)
{
  ysuper(YInt,YInt,print);
  printf("YInt2 print: i=%d\n",ythis(YInt)->i);
}

ymethod(void,YInt2,set,int i)
{
  ythis(YInt)->i=i;
}

yconstructor(YInt2)
{
  ybind(YInt2,YInt,set);
  ybind(YInt2,YInt,print);
}

ydestructor(YInt2)
{
}

/*
   Let us derive one more time and overwrite "print" again.
*/

yclass(YInt3,YInt2)

ymethod(void,YInt3,print)
{
  // ysuper in this form calls the grandparent's print function.
  printf("YInt3 print: call print method as YInt:\n");
  ysuper(YInt,YInt,print);

  // This usage of ysuper is more common:
  // Call of the parent's method using YInt2 (direct parent)
  // as the first argument.
  // Through ysuper in YInt2's print method, YInt's print
  // will be called again here.
  printf("YInt3 print: call print method as YInt2:\n");
  ysuper(YInt2,YInt,print);

  printf("YInt3 print: i=%d\n",ythis(YInt)->i);
}

yconstructor(YInt3)
{
  ybind(YInt3,YInt,print);
}

ydestructor(YInt3)
{
}

/*
   YApp with two objects of types YInt and YInt3, respectively.
*/

yclass(YApp,YMain,
  YInt *a;
  YInt3 *c;
)
ymain(YApp)

ymethod(int,YApp,main,int argc, char *argv[])
{
  // Let us attempt to call YInt's virtual "set" method,
  // which is not defined.
  // Current implementation of YCLASS is: Silently ignore this, do nothing.
  // It might be better to raise some exception,
  // if you think so, please let me know.
  printf("main method: invoking YInt a's methods\n\n");
  ycall(ythis->a,YInt,set,7);
  ycall(ythis->a,YInt,print);
  printf("\n");

  // YInt3 is derived from YInt2, where "set" is defined.
  // In the "ycall" macro, we still need "YInt" since "set" is declared there.
  // Finally, "print" is called.
  // Compare the output with the implementation for YInt3 above.
  printf("main method: invoking YInt3 c's methods\n\n");
  ycall(ythis->c,YInt,set,7);
  ycall(ythis->c,YInt,print);
}

yconstructor(YApp)
{
  ybind(YApp,YMain,main);

  ythis->a=ynew(YInt);
  ythis->c=ynew(YInt3);
}

ydestructor(YApp)
{
  ydelete(ythis->a);
  ydelete(ythis->c);
}

