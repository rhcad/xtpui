#include <module/plugininc.h>
#include <module/pluginimpl.h>
#include <module/modulemacro.h>

#include <mfc/WndFactory.h>
#include "mfcwnd.h"

const char* const clsidWnd = "56f4880e-5658-4354-952a-3b01b8e6b485";

XBEGIN_DEFINE_MODULE()
    XDEFINE_CLASSMAP_ENTRY_Wnd(clsidWnd, CSimpleWnd)
XEND_DEFINE_MODULE_DLL()

OUTAPI bool x3InitializePlugin()
{
    return true;
}

OUTAPI void x3UninitializePlugin()
{
}
