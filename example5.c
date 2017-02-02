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

#define YCLASS_DEBUG
#include <yclass.h>
#include <stdio.h>

// -----------------------------------------------------------------------

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

// -----------------------------------------------------------------------

yclass(YInt2,YInt)

ymethod(void,YInt2,print)
{
  ysuper(YInt,YInt,print);
  printf("YInt2 print: i=%d\n",ythis(YInt)->i);
}

yconstructor(YInt2)
{
  ybind(YInt2,YInt,print);

  ythis(YInt)->i=23;
}

ydestructor(YInt2)
{
}

// -----------------------------------------------------------------------

yclass(YInt3,YInt2)

ymethod(void,YInt3,print)
{
  ysuper(YInt2,YInt,print);
  printf("YInt3 print: i=%d\n",ythis(YInt)->i);
}

yconstructor(YInt3)
{
  ybind(YInt3,YInt,print);

  ythis(YInt)->i=42;
}

ydestructor(YInt3)
{
}

// -------------------------------------------

yclass(YApp,YMain,
  YInt *a;
  YInt3 *c;
)
ymain(YApp)

ymethod(int,YApp,main,int argc, char *argv[]);

yconstructor(YApp)
{
  ybind(YApp,YMain,main);

  ythis->a=ynew(YInt);
  ythis->c=ynew(YInt3);
}

ymethod(int,YApp,main,int argc, char *argv[])
{
  printf("This is main\n");

  printf("-------------------\n");
  
  ycall(ythis->c,YInt,print);
  ycall(ythis->c,YInt,set,1);
  ycall(ythis->c,YInt,print);

  printf("-------------------\n");
}

ydestructor(YApp)
{
  ydelete(ythis->a);
  ydelete(ythis->c);
}


