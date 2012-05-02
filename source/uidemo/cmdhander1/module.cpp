#include <module/plugininc.h>
#include <module/pluginimpl.h>
#include <module/modulemacro.h>
#include <observer/observerimpl.h>

#include "cmdhander1.h"

XBEGIN_DEFINE_MODULE()
XEND_DEFINE_MODULE_DLL()

OUTAPI bool x3InitializePlugin()
{
    return CmdHandler::Register();
}

OUTAPI void x3UninitializePlugin()
{
}
