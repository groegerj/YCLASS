/* example14.c -- YCLASS usage example

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
  This example differs from the previous example 13 only in two lines.
  Instead of one exception thrown on an invalid position, we shall throw two
  exceptions one after another.

  This will show a severe limitation of YCLASS: Only the last exception thrown
  in a ytry block is available through the variable yexception within a ycatch
  block. In other words, YCLASS does not keep track of exceptions.

  Keep this limitation in mind, for otherwise the program will show unintended
  behaviour as shown here.
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
      // throw two exceptions
      ythrow(YEXCEPTION_CUSTOM_BELOW_RANGE);
      ythrow(YEXCEPTION_OUT_OF_RANGE);
    }
    else if (pos>YARR_SIZE-1)
    {
      // also here, throw two exceptions
      ythrow(YEXCEPTION_CUSTOM_ABOVE_RANGE);
      ythrow(YEXCEPTION_OUT_OF_RANGE);
    }
  }
  ycatch
  {
    printf("YArr get: This exception was catched!\n");

    // At this place, only the last exception thrown is available through yexception.
    // In the case at hand, this is YEXCEPTION_OUT_OF_RANGE.
    // As a result, the first case in the following switch statement is never reached
    // but always the default throwing another exception which then terminates the program.

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
  // As before. This time, the second ycall will terminate the program.
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

