
/*

Platform neutral runtime library.

To be statically linked with an appropriate gxruntime driver.

*/

#ifndef BBRUNTIME_H
#define BBRUNTIME_H

#include "../gxruntime/gxruntime.h"

#include "bbblitz3d.h"
#include "bbgraphics.h"
#include "../blitz3d/blitz3d.h"
#include "../blitz3d/world.h"
#include "../blitz3d/texture.h"
#include "../blitz3d/brush.h"
#include "../blitz3d/camera.h"
#include "../blitz3d/sprite.h"
#include "../blitz3d/meshmodel.h"
#include "../blitz3d/loader_x.h"
#include "../blitz3d/loader_3ds.h"
#include "../blitz3d/loader_b3d.h"
#include "../blitz3d/md2model.h"
#include "../blitz3d/q3bspmodel.h"
#include "../blitz3d/meshutil.h"
#include "../blitz3d/pivot.h"
#include "../blitz3d/planemodel.h"
#include "../blitz3d/terrain.h"
#include "../blitz3d/listener.h"
#include "../blitz3d/cachedtexture.h"

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
