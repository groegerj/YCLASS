/* yclass.h -- single YCLASS header file

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

#ifndef _YCLASS_H
#define _YCLASS_H 1

#include <stdlib.h>
#include <string.h>

#ifdef YCLASS_DEBUG
  #include <stdio.h>
#endif

// --------------------------------------------------------
// miscellaneous
// --------------------------------------------------------

#define _YCLASS_SELECT_MACRO_3(_1,_2,_3,NAME,...) NAME
#define _YCLASS_SELECT_MACRO_4(_1,_2,_3,_4,NAME,...) NAME

#define _YCLASS_CAT4(X,Y,Z,W) X##Y##Z##W
#define _YCLASS_CAT3(X,Y,Z) X##Y##Z
#define _YCLASS_CAT2(X,Y) X##Y
#define _YCLASS_CAT(...)\
_YCLASS_SELECT_MACRO_4(__VA_ARGS__,_YCLASS_CAT4,_YCLASS_CAT3,_YCLASS_CAT2)(__VA_ARGS__)

#define ythis(CLASS) ((CLASS*)ythis)

#ifdef YCLASS_DEBUG
  # define _YCLASS_CLASSNAME_STRLEN 32
#endif

typedef struct YClass YClass;
typedef struct YClass_Methods YClass_Methods;

// --------------------------------------------------------
// standard yclass exception numbers, starting at 32
// equals exit code in standard behaviour
// --------------------------------------------------------

#define YEXCEPTION_BAD_ALLOC 32
#define YEXCEPTION_OUT_OF_RANGE 33

// --------------------------------------------------------
// method macros
// --------------------------------------------------------

#define yvirtual(RETURN,METHOD,...)\
RETURN (*METHOD)(YClass *ythis,##__VA_ARGS__)

#define ymethod(RETURN,CLASS,METHOD,...)\
typedef RETURN (_YCLASS_CAT(*CLASS,_,METHOD,_type))(YClass *ythis,##__VA_ARGS__);\
RETURN _YCLASS_CAT(CLASS,_,METHOD)(CLASS *ythis,##__VA_ARGS__)

// for use in constructor only
#define ybind3(CLASS,ORIGCLASS,METHOD)\
((ORIGCLASS*)ythis)->METHOD=(_YCLASS_CAT(CLASS,_,METHOD,_type))_YCLASS_CAT(CLASS,_,METHOD);\
((_YCLASS_CAT(ORIGCLASS,_Methods*))(&ythis->origmethods))->METHOD=(_YCLASS_CAT(CLASS,_,METHOD,_type))_YCLASS_CAT(CLASS,_,METHOD)

#define ybind2(CLASS,METHOD)\
ybind3(CLASS,CLASS,METHOD)

#define ybind(...)\
_YCLASS_SELECT_MACRO_3(__VA_ARGS__,ybind3,ybind2)(__VA_ARGS__)

#define ycall(OBJECT,ORIGCLASS,METHOD,...)\
((((ORIGCLASS*)OBJECT)->METHOD)?\
((ORIGCLASS*)OBJECT)->METHOD(((YClass*)OBJECT),##__VA_ARGS__):0)

#define ysuper(CLASS,ORIGCLASS,METHOD,...)\
((((_YCLASS_CAT(ORIGCLASS,_Methods*))(&((CLASS*)ythis)->origmethods))->METHOD)?\
((_YCLASS_CAT(ORIGCLASS,_Methods*))(&((CLASS*)ythis)->origmethods))\
->METHOD(((YClass*)ythis),##__VA_ARGS__):0)

// --------------------------------------------------------
// event and signal macros
// --------------------------------------------------------

typedef void (*_yclass_callback_type)(YClass *ythis,YClass *ysource);

#define yvirtual_signal(RETURN,METHOD,...)\
yvirtual(RETURN,METHOD,YClass *ysource,##__VA_ARGS__)

#define yvirtual_event(RETURN,METHOD,...)\
yvirtual(RETURN,METHOD,YClass *ysource,_yclass_callback_type yslot,##__VA_ARGS__)

#define ymethod_signal(RETURN,CLASS,METHOD,...)\
ymethod(RETURN,CLASS,METHOD,YClass* ysource,##__VA_ARGS__)

#define ymethod_event(RETURN,CLASS,METHOD,...)\
typedef RETURN (_YCLASS_CAT(*CLASS,_signal_,METHOD,_type))(YClass *ythis,YClass *ysource,##__VA_ARGS__);\
typedef RETURN (_YCLASS_CAT(*CLASS,_,METHOD,_type))(YClass *ythis,YClass *ysource,_yclass_callback_type yslot,##__VA_ARGS__);\
RETURN _YCLASS_CAT(CLASS,_,METHOD)(CLASS *ythis,YClass *ysource,_YCLASS_CAT(CLASS,_signal_,METHOD,_type) yslot,##__VA_ARGS__)

#define ysuper_signal(CLASS,ORIGCLASS,METHOD,...)\
ysuper(CLASS,ORIGCLASS,METHOD,(YClass*)ysource,##__VA_ARGS__)

#define ysuper_event(CLASS,ORIGCLASS,METHOD,...)\
ysuper(CLASS,ORIGCLASS,METHOD,(YClass*)ysource,(_yclass_callback_type)yslot,##__VA_ARGS__)

#define ycall_signal(OBJECT,ORIGCLASS,METHOD,...)\
ycall(OBJECT,ORIGCLASS,METHOD,(YClass*)ythis,##__VA_ARGS__)

#define ycall_event(OBJECT,ORIGCLASS,METHOD,CBOBJECT,CALLBACK,...)\
ycall(OBJECT,ORIGCLASS,METHOD,(YClass*)CBOBJECT,(_yclass_callback_type)CALLBACK,##__VA_ARGS__)

#define ycallback(...)\
(yslot?yslot(ysource,(YClass*)ythis,##__VA_ARGS__):0)

// --------------------------------------------------------
// exception handling
// --------------------------------------------------------

extern int _yclass_numhandlers;
extern int _yclass_exception;
extern int yexception; // the last thrown exception

#define ytry _yclass_numhandlers++;

#define ycatch \
_yclass_numhandlers--;\
yexception=_yclass_exception;\
_yclass_exception=0;\
if (yexception)

#define ythrow(EXPNUM) \
ycall_signal(_yclass_main_class,YMain,exception,EXPNUM);

// --------------------------------------------------------
// YClass
// --------------------------------------------------------

#ifdef YCLASS_DEBUG
  #define _yclass_debug_calling_new(CLASS) \
  printf("YCLASS: Creating new object of class %s.\n",#CLASS);
  #define _yclass_debug_calling_delete(CLASS) \
  printf("YCLASS: Deleting object of class %s.\n",#CLASS);
  #define _yclass_debug_calling_constructor(CLASS) \
  printf("YCLASS: Calling constructor of class %s.\n",#CLASS);
  #define _yclass_debug_calling_destructor(CLASS) \
  printf("YCLASS: Calling destructor of class %s.\n",#CLASS);
#else
  #define _yclass_debug_calling_new(CLASS)
  #define _yclass_debug_calling_delete(CLASS)
  #define _yclass_debug_calling_constructor(CLASS)
  #define _yclass_debug_calling_destructor(CLASS)
#endif

struct YClass_Methods{};

struct YClass
{
#ifdef YCLASS_DEBUG
  char yclassname[_YCLASS_CLASSNAME_STRLEN];
#endif
  yvirtual(void,ydelete);
  YClass_Methods origmethods;
};

void YClass_constructor(YClass *ythis);
void YClass_parent_constructor(YClass *ythis);
void YClass_destructor(YClass *ythis);
void YClass_parent_destructor(YClass *ythis);

#define yclass4(CLASS,PARENT,NEWVARIABLES,NEWMETHODS)\
typedef struct CLASS CLASS;\
typedef struct _YCLASS_CAT(CLASS,_Methods) _YCLASS_CAT(CLASS,_Methods);\
struct _YCLASS_CAT(CLASS,_Methods)\
{\
  _YCLASS_CAT(PARENT,_Methods) parent;\
  NEWMETHODS\
};\
struct CLASS\
{\
  PARENT parent;\
  NEWVARIABLES\
  NEWMETHODS\
  _YCLASS_CAT(CLASS,_Methods) origmethods;\
};\
void _YCLASS_CAT(CLASS,_delete)(CLASS *ythis);\
CLASS* _YCLASS_CAT(CLASS,_new)(YClass *owner);\
void _YCLASS_CAT(CLASS,_destructor)(CLASS *ythis);\
void _YCLASS_CAT(CLASS,_constructor)(CLASS *ythis);\
inline void _YCLASS_CAT(CLASS,_parent_destructor)(CLASS *ythis)\
{\
  _yclass_debug_calling_destructor(PARENT);\
  _YCLASS_CAT(PARENT,_destructor)((PARENT*)ythis);\
  _YCLASS_CAT(PARENT,_parent_destructor)((PARENT*)ythis);\
}\
inline void _YCLASS_CAT(CLASS,_parent_constructor)(CLASS *ythis)\
{\
  _YCLASS_CAT(PARENT,_parent_constructor)((PARENT*)ythis);\
  memcpy((_YCLASS_CAT(PARENT,_Methods*))(&ythis->origmethods),&((PARENT*)ythis)->origmethods,sizeof(_YCLASS_CAT(PARENT,_Methods)));\
  _yclass_debug_calling_constructor(PARENT);\
  _YCLASS_CAT(PARENT,_constructor)((PARENT*)ythis);\
}

#define yclass3(CLASS,PARENT,NEWVARIABLES)\
yclass4(CLASS,PARENT,NEWVARIABLES,)

#define yclass2(CLASS,PARENT)\
yclass4(CLASS,PARENT,,)

#define yclass1(CLASS)\
yclass4(CLASS,YClass,,)

#define yclass(...)\
_YCLASS_SELECT_MACRO_4(__VA_ARGS__,yclass4,yclass3,yclass2,yclass1)(__VA_ARGS__)

// --------------------------------------------------------
// new and delete
// --------------------------------------------------------

#ifdef YCLASS_DEBUG
  #define _yclass_copy_classname(CLASS) \
    strncpy(((YClass*)ythis)->yclassname,#CLASS,_YCLASS_CLASSNAME_STRLEN-1);\
    ((YClass*)ythis)->yclassname[_YCLASS_CLASSNAME_STRLEN-1]='\0';
#else
  #define _yclass_copy_classname(CLASS)
#endif

#define ynew(CLASS) _YCLASS_CAT(CLASS,_new)((YClass*)ythis)

#define ydelete(OBJECT) \
if (OBJECT) do {((YClass*)OBJECT)->ydelete((YClass*)OBJECT);OBJECT=NULL;} while(0)

#define yconstructor(CLASS)\
extern inline void _YCLASS_CAT(CLASS,_parent_constructor)(CLASS *ythis);\
CLASS* _YCLASS_CAT(CLASS,_new)(YClass *owner)\
{\
  _yclass_debug_calling_new(CLASS) \
  CLASS *ythis=calloc(1,sizeof(CLASS));\
  if (ythis)\
  {\
    ((YClass*)ythis)->ydelete=(void (*)(YClass *ythis))_YCLASS_CAT(CLASS,_delete);\
    _yclass_copy_classname(CLASS) \
    _YCLASS_CAT(CLASS,_parent_constructor)(ythis);\
    _yclass_debug_calling_constructor(CLASS);\
    _YCLASS_CAT(CLASS,_constructor)(ythis);\
  }\
  if(!ythis && owner) ycall(_yclass_main_class,YMain,exception,owner,YEXCEPTION_BAD_ALLOC);\
  return ythis;\
}\
void _YCLASS_CAT(CLASS,_constructor)(CLASS *ythis)

#define ydestructor(CLASS)\
extern inline void _YCLASS_CAT(CLASS,_parent_destructor)(CLASS *ythis);\
void _YCLASS_CAT(CLASS,_delete)(CLASS *ythis)\
{\
  _yclass_debug_calling_delete(CLASS) \
  _yclass_debug_calling_destructor(CLASS);\
  _YCLASS_CAT(CLASS,_destructor)(ythis);\
  _YCLASS_CAT(CLASS,_parent_destructor)(ythis);\
  free(ythis);\
}\
void _YCLASS_CAT(CLASS,_destructor)(CLASS *ythis)

// --------------------------------------------------------
// main
// --------------------------------------------------------

#ifdef YCLASS_DEBUG
  #define _yclass_debug_ymain_exception_handler(SENDER,EXPNUM) \
  printf("YCLASS: Exception number %i thrown in class %s (to be catched).\n",EXPNUM,SENDER->yclassname);
  #define _yclass_debug_ymain_exception_exit(SENDER,EXPNUM) \
  printf("YCLASS: Exception number %i thrown in class %s (terminating program).\n",EXPNUM,SENDER->yclassname);
#else
  #define _yclass_debug_ymain_exception_handler(SENDER,EXPNUM)
  #define _yclass_debug_ymain_exception_exit(SENDER,EXPNUM)
#endif

yclass(YMain,YClass,,
  yvirtual_signal(void,exception,int expnum);
  yvirtual(int,main,int argc, char *argv[]);
)

extern YMain *_yclass_main_class;

// second line in YMain constructor needed in case new fails with exception

#define ymain(CLASS)\
int _yclass_numhandlers=0;\
int _yclass_exception=0;\
int yexception=0;\
YMain *_yclass_main_class=NULL;\
void YClass_constructor(YClass *ythis){}\
void YClass_parent_constructor(YClass *ythis){}\
void YClass_destructor(YClass *ythis){}\
void YClass_parent_destructor(YClass *ythis){}\
ymethod_signal(void,YMain,exception,int expnum)\
{\
  _yclass_exception=expnum;\
  if (_yclass_numhandlers)\
  {\
    _yclass_debug_ymain_exception_handler(ysource,expnum);\
  }\
  else\
  {\
    _yclass_debug_ymain_exception_exit(ysource,expnum);\
    ydelete(ythis);\
    exit(expnum);\
  }\
}\
yconstructor(YMain)\
{\
  _yclass_main_class=ythis;\
  ybind(YMain,exception);\
}\
ydestructor(YMain){}\
int main(int argc, char *argv[])\
{\
  int rval=0;\
  _yclass_main_class=(YMain*)(_YCLASS_CAT(CLASS,_new)(NULL));\
  if (!_yclass_main_class)\
  {\
    printf("YCLASS: Creation of main class %s failed.\n",#CLASS);\
    rval=YEXCEPTION_BAD_ALLOC;\
  }\
  else\
  {\
    rval=ycall(_yclass_main_class,YMain,main,argc,argv);\
    ydelete(_yclass_main_class);\
  }\
  return rval;\
}

#endif // _YCLASS_H

