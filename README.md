YCLASS
=====================================================================

Copyright (C) 2016-2017 Josua Groeger.
groegerj at thp.uni-koeln dot de

YCLASS is an extension of the C programming language with
object oriented elements, mostly by means of the C preprocessor.

LICENSE
=====================================================================

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

Design Decisions, Status, Advantages and Disadvantages, and Bugs
=====================================================================

While C itself is not an object oriented programming language,
it can be used as such. This can be done by utilising structs and
functions in a way that models classes and functions, i.e. to use
C in its original form but use the existing language elements in a
specific form. Another possibility is to make up some new syntax
that is translated to C by some preprocessor.
The first version (or predecessor) of C++ was implemented that way.

The objective of YCLASS is to have object oriented C by means
of C's preprocessor, by inclusion of a single header file.
I have read somewhere that the preprocessor is not meant to introduce
new language elements. But this is exactly what YCLASS does.

Advantages and status:

*  Easy to use.
*  No additional software needed besides a C compiler.
*  Lightweight.
*  Everything seems to work now (classes, constructors, destructors,
   inheritance, virtual methods, signals and events, exceptions).
*  Besides the disadvantages (see below), there are no known bugs.

Disadvantages and suggestions for improvement:

*  Usage of the C preprocessor restricts the possibilities
   and does not prevent the user from using YCLASS in a way
   that is not intended.
*  Compiler errors become really hard to understand.
   The situation might become slightly better by using
   the #error directive in yclass.h.
*  So far tested only with GCC. Not everything might be portable,
   e.g. I am unsure about "__VA_ARGS__".
*  So far tested only with examples consisting of a single .c file.
   While I have tried to forsee and circumvent linking errors,
   I have not tested it so far.
*  YCLASS messes up the namespace. The macros declare functions
   and variables, do typedefs etc., with names that are invisible
   to the YCLASS user and only used internally.
   It is possible to accidentally run into such a name, which
   gives an error. More care should be taken in the implementation
   of YCLASS, e.g. by using some common prefix.
*  In the current implementation, the method table is part of each
   object of one class. That means, if you have two objects of the
   same class, the method table for that class exists twice, certainly
   a waste of memory. It is thus not a good idea e.g. to define an
   "integer class" and to have a large array of it. This certainly needs
   improvement. The information concerning the class should
   better be kept separately.
*  The system of signals and events is very simple, for some purposes
   maybe too simple. The same statement applies even more to the
   current form of exception handling.
*  Documentation (see below) still needs significant improvement.
*  There is no real world program written with YCLASS yet.

Usage
=====================================================================

To use YCLASS, it suffices to include the header file yclass.h
in your program.

    #include <yclass.h>

Debug information is printed if YCLASS_DEBUG is defined:

    #define YCLASS_DEBUG
    #include <yclass.h>

For usage examples, see the files

    example1.c
    example2.c
    example3.c
    example4.c
    example5.c
    example6.c
    example7.c
    
along with the comments therein.

These examples are meant to be read, compiled and run
in ascending order. They should give you a good idea of the
basic usage.
Signals and events and exceptions are not yet documented or
covered by examples.

