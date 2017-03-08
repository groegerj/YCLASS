/* example11.c -- YCLASS usage example

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

#define YCLASS_DEBUG
#include <yclass.h>
#include <stdio.h>

/*
  This is the first example demonstrating the use of exceptions.
  As usual, it is not meant to be realistic. Imaging you want to
  implement an int array class, call it YArr. To make the example
  as simple as possible, let this array be static, and implement
  only one method, get, returning the item at a given position.

  If the position is within bounds, let us do so.
  Otherwise, we shall throw an exception.
  The standard behaviour (no ytry/ycatch block to be covered in
  a later example), is as follows.
  Delete the main class object (which is handled internally by YCLASS,
  here of class YApp) by the usual means (invoke destructor etc.)
  such that in the end everything is tidied up, and then exit the program.

  An exception in YCLASS comes with a number. yclass.h defines some
  standard exceptions such as
  #define YEXCEPTION_BAD_ALLOC 32
  #define YEXCEPTION_OUT_OF_RANGE 33
  with the first e.g. thrown at a failing ynew command,
  and the second used here.

  Custom exceptions should be in the range 1...31.
  When shutting down the program upon an exception,
  YCLASS uses the exception number
  also as exit code of the program.
*/

#define YARR_SIZE 10

yclass(YArr,YClass,
  int items[YARR_SIZE];
,
  yvirtual(int,get,int pos);
)

ymethod(int,YArr,get,int pos)
{
  if (pos<0 || pos>YARR_SIZE-1)
  {
    // The macro for throwing an exception is ythrow(exception_number)
    // with exception number as explained above.
    ythrow(YEXCEPTION_OUT_OF_RANGE);
  }
  else
  {
    // The position is in bounds, proceed normally.
    return ythis->items[pos];
  }
}

yconstructor(YArr)
{
  // Some not very useful initialisation.
  int i;
  for(i=0;i<YARR_SIZE;i++) ythis->items[i]=1000+i;

  ybind(YArr,get);
}

ydestructor(YArr)
{
}

yclass(YApp,YMain,
  YArr *a;
)
ymain(YApp)

ymethod(int,YApp,main,int argc, char *argv[])
{
  // Here, we invoke the get method twice.
  // First time with a meaningful position.
  printf("\nYApp main: array items[2]=%d\n\n",ycall(ythis->a,YArr,get,2));

  // Second time out of range, this raises an exception and
  // terminates the program.
  int j=ycall(ythis->a,YArr,get,42);
}

yconstructor(YApp)
{
  ybind(YApp,YMain,main);

  ythis->a=ynew(YArr);
}

ydestructor(YApp)
{
  ydelete(ythis->a);
}

