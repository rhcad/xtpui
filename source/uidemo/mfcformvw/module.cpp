#include "stdafx.h"
#include <module/pluginimpl.h>
#include <module/modulemacro.h>

#include <mfc/WndFactory.h>
#include "mfcformvw.h"

const char* const clsidForm = "ca922c30-7d3b-402c-943c-a8ecab127e9f";
const char* const clsidDlg = "32e53d57-6c3f-4a09-b663-ad1cae401929";

XBEGIN_DEFINE_MODULE()
    XDEFINE_CLASSMAP_ENTRY_Wnd(clsidForm, CMyFormView)
    XDEFINE_CLASSMAP_ENTRY_Wnd(clsidDlg, CMyDlgView)
XEND_DEFINE_MODULE_DLL()

OUTAPI bool x3InitializePlugin()
{
    return true;
}

OUTAPI void x3UninitializePlugin()
{
}

CLocalResource::CLocalResource()
{
    m_old = AfxGetResourceHandle();
    AfxSetResourceHandle(x3::getModuleHandle());
}

CLocalResource::~CLocalResource()
{
    AfxSetResourceHandle(m_old);
}
