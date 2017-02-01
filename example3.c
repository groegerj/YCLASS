/* example3.c -- YCLASS usage example

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

// uncomment for debugging information:
//#define YCLASS_DEBUG

#include <yclass.h>
#include <stdio.h>

/*
  Let's make an "integer class"...
  (this example does not make much sense but still might be instructive)

  As said before, the base class is YClass.
  The yclass macro takes up to four arguments, separated by a comma:
  1) class name, 2) parent class, 3) variables, 4) methods
  default (if not specified) is: 2) YClass, 3) none, 4) none

  Variables are all public, there is no such thing as protected or private.
  Use an underscore "_variablename" to indicate that something
  should not considered public.

  In this example, we shall directly access the variable i.
  This is bad style and better done through access methods...
  See the next example...
*/

yclass(YInt,YClass,
  int i;
  // int j; more variables here
)

yconstructor(YInt)
{
}

ydestructor(YInt)
{
}

yclass(YApp,YMain,
  YInt *a; // a YInt variable
)
ymain(YApp)

// The following is just a declaration,
// necessary if we want to define the constructor
// before the main method.
ymethod(int,YApp,main,int argc, char *argv[]);

yconstructor(YApp)
{
  ybind(YApp,YMain,main);

  ythis->a=ynew(YInt);
/*
  ynew(YInt):
  Return a new object, i.e. reserve memory, call constructors etc.
  YCLASS guarantees that everything is initialised as 0,
  so there is no need e.g. for initialising a->i with 0.

  If ynew fails, an exception is thrown (to be explained later...),
  default behaviour is controlled program exit.

  ythis:
  Another pseudokeyword which refers to the current object
  (here YApp object that was created by means of the "ymain(YApp)" command).

  ythis->a:
  should be obvious...
*/
}

ymethod(int,YApp,main,int argc, char *argv[])
{
  // prints numbers 0 and 42...
  printf("Number %i\n",ythis->a->i);
  ythis->a->i=42;
  printf("Number %i\n",ythis->a->i);

  // as said before, it is in general not a good idea to directly access
  // object variables, see the next example...
}

ydestructor(YApp)
{
  // call destructors and free the memory
  ydelete(ythis->a);
}

