
/*

Platform neutral runtime library.

To be statically linked with an appropriate gxruntime driver.

*/

#ifndef BBRUNTIME_H
#define BBRUNTIME_H

#include "../gxruntime/gxruntime.h"

#include <vector>

void bbruntime_link( void (*rtSym)( const char *sym,void *pc ) );

const char *bbruntime_run( gxRuntime *runtime,void (*pc)(),bool debug );

void bbruntime_panic( const char *err );

class ErrorCheck
{
public:
  static int Size;
  static std::string* Content;
};

static std::vector<const char*> ErrorLog;

#endif
