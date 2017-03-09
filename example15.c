/* example15.c -- YCLASS usage example

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
   We continue our examples on exceptions, by demonstrating how ytry/ycatch
   blocks can be nested.

   As before, our get method shall throw an exception if the position is
   out of range. Let us introduce some indirection, for the sake of example.
   We catch that exception, print some debugging messages and then throw
   the same exception again to the outside world (here: the main method).

   Now, that second exception may be catched there or not.
   If not, of course leading to program termination.
*/

#define YARR_SIZE 10

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
    // On exception, print some degugging messages.
    printf("YArr get: This exception was catched!\n");
    switch (yexception)
    {
      case YEXCEPTION_CUSTOM_BELOW_RANGE: printf("Position too small!\n"); break;
      case YEXCEPTION_CUSTOM_ABOVE_RANGE: printf("Position too big!\n"); break;
    }

    // Throw the same exception again.
    ythrow(yexception);

    // If the exception of the previous line is catched (here: in the main method),
    // then we will reach the following line.
    // Otherwise, the program has terminated before.
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
  // position valid
  printf("\nYApp main: array items[2]=%d\n\n",ycall(ythis->a,YArr,get,2));

  // position too small, catch that exception
  ytry
  {
    printf("\nYApp main: array item[-100]=%d\n\n",ycall(ythis->a,YArr,get,-100));
  }
  ycatch
  {
    printf("YApp main: This exception was catched!\n\n");
  }

  // position too big, do not catch exception but terminate the program
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

