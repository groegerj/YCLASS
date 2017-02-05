/* example5.c -- YCLASS usage example

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
  In this example, we will derive a child class from example4's YInt,
  which introduces a new method and overwrites an old one.
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
  ybind(YInt,print);
}

ydestructor(YInt)
{
}

/*
  YInt2 is a class derived from YInt.
  We do not introduce new variables here, therefore the two
  commas in the yclass macro.
  In this child class, we add a method "get" to obtain the variable i,
  since we have forgotten this method in YInt...
  makes not much sense, but shows how to derive classes.
*/

yclass(YInt2,YInt,,
  yvirtual(int,get);
)

ymethod(int,YInt2,get)
{
  /* The variable i belongs to YInt.
     ythis(YInt) casts the ythis pointer (of type YInt2) to a YInt pointer.
     After all, a class here is just some struct, and a child class is
     another struct which contains the parent class struct.
     What else did you expect?
  */
  return ythis(YInt)->i;
}

// This method will be overwritten
ymethod(void,YInt2,print)
{
  /*
     The "print" method will be overwritten, i.e. the function pointer
     will point to this new method,
     through the ybind macro in the constructor below.

     ysuper calls some ancestor incarnation of a method.
     Second and third arguments: We mean the method "print" which was
     originally declared in YInt.
     First argument: We call this method as if being of class YInt.
     You will see more examples of this later.
  */
  ysuper(YInt,YInt,print);
  printf("YInt2 print: i=%d\n",ythis(YInt)->i);
}

yconstructor(YInt2)
{
  /*
    ybind comes in two versions: Two arguments and three arguments.
    The one with two arguments is an abbreviation of the one with three with the first argument repeated.
    
    Three argument form:
    Argument 1: This class, arguments 3 and 2: method along with original class as declared through yvirtual.
  */
  ybind(YInt2,get);
  ybind(YInt2,YInt,print);

  // some new initialisation
  ythis(YInt)->i=23;
}

ydestructor(YInt2)
{
}


yclass(YApp,YMain,
  YInt2 *b;
)
ymain(YApp)

ymethod(int,YApp,main,int argc, char *argv[])
{
  // Notice the use of "YInt2" versus "YInt" in the "ycall" macro calls below.
  // By now, you should know when to use which in this situation.

  printf("Variable equals %d\n",ycall(ythis->b,YInt2,get));
  ycall(ythis->b,YInt,set,42);
  ycall(ythis->b,YInt,print);
}

yconstructor(YApp)
{
  ybind(YApp,YMain,main);

  ythis->b=ynew(YInt2);
}

ydestructor(YApp)
{
  ydelete(ythis->b);
}

