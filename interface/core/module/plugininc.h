#ifndef X3_CORE_PLUGIN_INC_H
#define X3_CORE_PLUGIN_INC_H

#include <portability/x3port.h>

#ifndef NO_VECFUNC
#include <utilfunc/vecfunc.h>
#endif
#ifndef NO_SAFECALL
#include <utilfunc/safecall.h>
#endif
#if !defined(NO_DEBUGR) && (defined(_USRDLL) || defined(_AFXEXT))
#include <log/DebugR.h>
#endif
#ifndef NO_LOGHELPER
#include <log/LogHelper.h>
#endif
#ifndef NO_CLASSMACRO
#include <module/classmacro.h>
#endif

#endif