/* example1.c -- YCLASS usage example

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

#include <yclass.h>
#include <stdio.h>

/* ------------------------------------------------------------------

The basic example without much use ("hello world").
The base class is called YClass.
YMain is derived from YClass.
Here, we derive another class (YApp) which implements the main method.

------------------------------------------------------------------ */

// declare class YApp as derived from class YMain
// in this form with no additional variables or methods
yclass(YApp,YMain)

// YApp is to be the main class which implements the main method
// this macro involves the standard C main function
ymain(YApp)

// the main method
// arguments are: return value, class name, method name, arguments
// after all, this is going to be a plain C function
// see ybind below...
ymethod(int,YApp,main,int argc, char *argv[])
{
  printf("This is main\n");
}

// the constructor of class YApp
// even if it does nothing, it is required for every class
yconstructor(YApp)
{
  // YMain has "main" as a virtual method,
  // ybind sets the according function pointer to ymethod(...)
  // from above.
  ybind(YApp,YMain,main);
}

// the destructor, similar comments as for the constructor
ydestructor(YApp)
{
}


