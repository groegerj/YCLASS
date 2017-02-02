/* example4.c -- YCLASS usage example

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
  Example 3 continued. This time, we make a class with one variable
  and two methods. Notice the keywords "yvirtual" after the third
  comma within the "yclass" macro.
  Arguments for yvirtual are: return value, method name, arguments (optional)
*/

yclass(YInt,YClass,
  int i;
,
  yvirtual(void,set,int i);
  yvirtual(void,print);
)

// We are going to implement the "set" method here.
// You have encountered the "ymethod" keyword previously.
ymethod(void,YInt,set,int i)
{
  // and yes, ythis refers to the current YInt object here
  ythis->i=i;
}

ymethod(void,YInt,print)
{
  printf("YInt print: i=%d\n",ythis->i);
}

yconstructor(YInt)
{
  /*
    Okay, we have declared the methods set and print in the yclass macro,
    and we have just defined implementations.
    Don't forget, this is still C.
    yvirtual defines a function pointer variable, while ymethod defines
    a function.
    You'll need "ybind" to make that pointer point to the method.
    This must be done here, in the constructor.
    ybind exists with two or three arguments (see below in the YApp constructor).
    To be explained later...
  */
  ybind(YInt,set);
  ybind(YInt,print);
}

ydestructor(YInt)
{
}

yclass(YApp,YMain,
  YInt *a;
)
ymain(YApp)

ymethod(int,YApp,main,int argc, char *argv[])
{
  /*
    "ycall" calls a method...
    arguments are: object pointer (here: our YInt), class, method, arguments
    Care must be taken what to write as class when it comes to
    subclasses. We will come to this point in another example.
  */
  ycall(ythis->a,YInt,print);
  ycall(ythis->a,YInt,set,42);
  ycall(ythis->a,YInt,print);
}

yconstructor(YApp)
{
  ybind(YApp,YMain,main);

  ythis->a=ynew(YInt);
}

ydestructor(YApp)
{
  ydelete(ythis->a);
}

