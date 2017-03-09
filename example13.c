/* example13.c -- YCLASS usage example

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
  This is the previous example continued. Last time we have seen how to use
  the ytry/ycatch pair to catch an exception and provide some custom reaction
  different from terminating the program.

  In this example, let us define two custom exceptions with numbers 1 and 2,
  corresponding to some index below or above range.
  If an exception is thrown by ythrow, its number is available through the
  variable yexception within the ycatch block (outside, its value is "undefined").
  We will implement different behaviour depending on which exception was thrown.
*/

#define YARR_SIZE 10

// Our custom exceptions. As noted in a previous example, they should be
// in the range 1...31.
#define YEXCEPTION_CUSTOM_BELOW_RANGE 1
#define YEXCEPTION_CUSTOM_ABOVE_RANGE 2

yclass(YArr,YClass,
  int items[YARR_SIZE];
,
  yvirtual(int,get,int pos);
)

ymethod(int,YArr,get,int pos)
{
  ytry
  {
    // We throw different exceptions here depending on whether position
    // is too small or too big.
    if (pos<0)
    {
      ythrow(YEXCEPTION_CUSTOM_BELOW_RANGE);
    }
    else if (pos>YARR_SIZE-1)
    {
      ythrow(YEXCEPTION_CUSTOM_ABOVE_RANGE);
    }
  }
  ycatch
  {
    printf("YArr get: This exception was catched!\n");

    /* Behaviour depends on the yexception variable.
       If the ...BELOW_RANGE exception was thrown, print a message and return -1.

       Otherwise (here: in case of ...ABOVE_RANGE), let us throw another exception,
       which may then be catched somewhere outside provided one has nested ytry/ycatch
       blocks. In this example, that call of ythrow will just lead to the standard
       behaviour: terminate the program. */
    switch (yexception)
    {
      case YEXCEPTION_CUSTOM_BELOW_RANGE: printf("Position too small!\n"); break;
      default: ythrow(yexception);
    }

    return -1;
  }

  return ythis->items[pos];
}

yconstructor(YArr)
{
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
  // Three calls of the get method: valid position, position too small (exception thrown, catched)
  // and position too large (exception thrown, catched, another exception thrown uncatched).

  printf("\nYApp main: array items[2]=%d\n\n",ycall(ythis->a,YArr,get,2));
  printf("\nYApp main: array item[-100]=%d\n\n",ycall(ythis->a,YArr,get,-100));
  printf("\nYApp main: array item[42]=%d\n\n",ycall(ythis->a,YArr,get,42));
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

