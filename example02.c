/* example2.c -- YCLASS usage example

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

/* ------------------------------------------------------------------

Same example as example1.c, but with YCLASS_DEBUG defined.
Look for the additional debugging output of the program.

------------------------------------------------------------------ */

yclass(YApp,YMain)
ymain(YApp)

ymethod(int,YApp,main,int argc, char *argv[])
{
  printf("This is main\n");
}

yconstructor(YApp)
{
  ybind(YApp,YMain,main);
}

ydestructor(YApp)
{
}


