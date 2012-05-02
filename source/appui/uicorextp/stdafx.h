#pragma once

#include <module/plugininc.h>

//#define _XTP_EXCLUDE_COMMON
//#define _XTP_EXCLUDE_TABMANAGER
#define _XTP_EXCLUDE_GRAPHICLIBRARY
//#define _XTP_EXCLUDE_CONTROLS
//#define _XTP_EXCLUDE_COMMANDBARS
//#define _XTP_EXCLUDE_DOCKINGPANE
#define _XTP_EXCLUDE_PROPERTYGRID
#define _XTP_EXCLUDE_REPORTCONTROL
#define _XTP_EXCLUDE_CALENDAR
#define _XTP_EXCLUDE_TASKPANEL
#define _XTP_EXCLUDE_SHORTCUTBAR
#define _XTP_EXCLUDE_SKINFRAMEWORK
//#define _XTP_EXCLUDE_RIBBON
#define _XTP_EXCLUDE_SYNTAXEDIT
#define _XTP_EXCLUDE_MARKUP
#define _XTP_EXCLUDE_CHART
#define _XTP_EXCLUDE_FLOWGRAPH
#include <XTToolkitPro.h>    // Xtreme Toolkit Pro components

#include <xml/ConfigSection.h>
#include <manager/iworkpath.h>
#include <utilfunc/convstr.h>
using namespace x3;

inline std::wstring GetStringValue(LPCWSTR,LPCWSTR name,LPCWSTR defvalue=L"")
{
    return defvalue && *defvalue ? defvalue : name;
}
