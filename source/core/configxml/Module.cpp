// x3c - C++ PluginFramework
#include <module/plugininc.h>
#include <module/modulemacro.h>
#include <module/pluginimpl.h>

#include "Cx_ConfigXml.h"
#include "Cx_XmlSection.h"

#ifdef _XMLDOM_HELPERS_H
#include "XmlUtil/XmlUtil.cpp"
#else
#include "TinyXml/XmlUtil.cpp"
#endif

XBEGIN_DEFINE_MODULE()
    XDEFINE_CLASSMAP_ENTRY(x3::Cx_ConfigXml)
    XDEFINE_CLASSMAP_ENTRY(x3::Cx_XmlSection)
XEND_DEFINE_MODULE_DLL()

OUTAPI bool x3InitializePlugin()
{
    return true;
}

OUTAPI void x3UninitializePlugin()
{
}
