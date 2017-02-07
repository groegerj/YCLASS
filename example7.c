/* example7.c -- YCLASS usage example

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
   This example is rather a non-example. It shows two things.
   First, a forgotten ybind make a method effectively purely virtual.

   Second, in a child class you should never introduce a new variable or
   method with a name that already exists for a variable or method,
   respectively, in an ancestor class.
   Let's say this is forbidden, but if you try anyway, the behaviour
   is undefined. Of course, something happens with the current implementation
   of YCLASS, and this is to be demonstrated here.   
*/

yclass(YInt,YClass,
  int i;
,
  yvirtual(void,set,int i);
  yvirtual(void,print);
)

ymethod(void,YInt,set,int i)
{
  ythis->i=i;
}

ymethod(void,YInt,print)
{
  printf("YInt print: i=%d\n",ythis->i);
}

yconstructor(YInt)
{
  ybind(YInt,set);

  // Say we have simply forgotten to write
  // ybind(YInt,print);
  // This has the effect that the above definition for "print"
  // cannot be used.
}

ydestructor(YInt)
{
}

/*
   Never do the following, this is a non-example...
   which compiles fine in the current implementation of YCLASS
   but must be understood as _undefined_.

   First, we introduce a new variable "i". The parent class
   already contains "i" so, morally, nothing should happen here.
   In fact, something happens: YInt2 simply has two integer variables.
   The old one is accessed through "ythis(YInt)->i", while the
   new one is "ythis(YInt2)->i" or, equivalently in YInt2-methods,
   "ythis->i".

   Second, worse, we do a similar thing with the "set" and "print" methods,
   but in two different ways...
*/

yclass(YInt2,YInt,
  int i; // another integer...
,
  yvirtual(void,set,int i);
  yvirtual(void,print);
)

/*
   We are going to state new definitions for "print" and "set"...
*/

ymethod(void,YInt2,print)
{
  ysuper(YInt,YInt,print);
  printf("YInt2 print: i(YInt)=%d, i(YInt2)=%d\n",ythis(YInt)->i,ythis->i);
}

ymethod(void,YInt2,set,int i)
{
  ysuper(YInt,YInt,set,i); // pay attention to ysuper
  ythis(YInt)->i+=100;
  ythis->i=9900+i;
}

/*
   Consider the first statement in the following constructor:
   ybind(YInt2,YInt,print);
   This is how to overwrite YInt's print method.

   There is just one strange thing: The declaration
   yvirtual(void,print);
   in yclass(YInt2,...
   Without this yvirtual call, it would be correct.

   In the present form, it introduces another function pointer variable
   (besides YInt's print variable), just like there are two integer variables
   "i". It is just that this new pointer is not at all used.
*/

yconstructor(YInt2)
{
  // different behaviour due to different second arguments...
  ybind(YInt2,YInt,print);
  ybind(YInt2,YInt2,set);
}

/*
   As for "set"..., we understand by now, that there are also two function
   pointer variables, corresponding to the yvirtual(...set...) calls
   in YInt and YInt2, respectively.

   The first is bound to YInt's definition by ybind(...set) in YInt's
   constructor, while the second one is bound to
   ymethod(void,YInt2,set,int i)
   from above.

   This is not method overwriting, this is introducing another method
   besides the original method. In a way, both methods have the same name
   but live in different namespaces.

   Still there is some interference... the call of ysuper seems to work,
   see below. I repeat: This is a non-example!
*/

ydestructor(YInt2)
{
}

yclass(YApp,YMain,
  YInt *a;
  YInt2 *b;
)
ymain(YApp)

ymethod(int,YApp,main,int argc, char *argv[])
{
  // The print method will not be called due to the missing ybind statement
  // in YInt's constructor (nothing happens).
  // As noted in the previous example, future implementations of YCLASS
  // may raise an exception here.
  ycall(ythis->a,YInt,set,1);
  ycall(ythis->a,YInt,print);

  // Calls the original YInt set method
  ycall(ythis->b,YInt,set,23);
  ycall(ythis->b,YInt,print);

  // Calls the new YInt2 set method,
  // which is not the old one overwritten but another method...
  // However, the ysuper statement in that methods calls YInt's set method.
  ycall(ythis->b,YInt2,set,42);
  ycall(ythis->b,YInt,print);
}

yconstructor(YApp)
{
  ybind(YApp,YMain,main);

  ythis->a=ynew(YInt);
  ythis->b=ynew(YInt2);
}

ydestructor(YApp)
{
  ydelete(ythis->a);
  ydelete(ythis->b);
}

