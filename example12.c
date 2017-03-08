/* example12.c -- YCLASS usage example

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
  This is the previous example continued. Last time, an out of range
  position in the get method had thrown an exception which, by the
  default implementation, made the program terminate.

  This time, we are going to catch the exception by use of the
  new keyword (aka macro) pair ytry/ycatch.
  
  These two macros must always come in pairs, even if the block
  following either one is empty.
  ytry increases an internal counter, which is initially zero.
  Similarly, ycatch decreases that counter.
  The behaviour of ythrow is accordingly: standard behaviour
  (program termination) on counter zero, and more or less nothing
  if counter is positive.

  Nesting is allowed: A ytry/ycatch block may be placed inside another
  ytry or inside another ycatch block. See another example.
*/

#define YARR_SIZE 10

yclass(YArr,YClass,
  int items[YARR_SIZE];
,
  yvirtual(int,get,int pos);
)

ymethod(int,YArr,get,int pos)
{
  ytry
  {
    if (pos<0 || pos>YARR_SIZE-1)
    {
      ythrow(YEXCEPTION_OUT_OF_RANGE);
      // We are in a ytry block, so the program is not terminated here.
    }

    // A return statement is a ytry block must strictly be avoided
    // since the aforementioned counter should be decreased again.
  }
  ycatch
  {
    // The following code is executed in case an exception was
    // thrown in the previous ytry block.

    printf("YArr get: This exception was catched!\n");

    // For the sake of an example, imaging our array contains
    // only nonzero integers, while a -1 indicates that an item
    // does not exist.
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
  // Again, we invoke the get method with a valid and a non-valid position.

  printf("\nYApp main: array items[2]=%d\n\n",ycall(ythis->a,YArr,get,2));

  printf("YApp main: array item[42]=%d\n\n",ycall(ythis->a,YArr,get,42));
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

