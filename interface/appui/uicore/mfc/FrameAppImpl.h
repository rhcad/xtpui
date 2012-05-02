#include "FrameApp.h"
#include <nonplugin/useplugins.h>
#include <nonplugin/scanplugins.h>

BEGIN_MESSAGE_MAP(CFrameApp, CWinApp)
    ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
END_MESSAGE_MAP()

CFrameApp::CFrameApp()
{
}

CFrameApp::~CFrameApp()
{
}

BOOL CFrameApp::InitInstance()
{
    // Enable visual styles on Windows XP or later
    INITCOMMONCONTROLSEX initCtrls;
    initCtrls.dwSize = sizeof(initCtrls);
    initCtrls.dwICC = ICC_WIN95_CLASSES | ICC_DATE_CLASSES;
    InitCommonControlsEx(&initCtrls);

    CWinApp::InitInstance();
    VERIFY(AfxOleInit());

    return LoadCorePlugins()
        && CheckAppInstance()
        && CreateFrameWnd();
}

bool CFrameApp::LoadCorePlugins()
{
    const char* plugins[] = { "x3manager.pln", "configxml.pln", "uicorextp.pln", NULL };
    x3::loadPlugins(plugins, "plugins");

    if (!m_factory.create(x3::clsidFrameWndFactory))
    {
        AfxMessageBox(L"Need plugins/ToolkitPro*vc*.dll to load the uicorextp plugin.\n"
            L"Fail to create IFrameWndFactory (uicorextp) instance.", MB_ICONERROR);
        return false;
    }

    return true;
}

int CFrameApp::ExitInstance()
{
    SafeCall(m_factory, OnQuit());
    m_factory.release();

    x3::unloadScanPlugins();
    x3::unloadPlugins();

    return CWinApp::ExitInstance();
}

bool CFrameApp::CheckAppInstance()
{
    return 0 == *GetSingletonAppUID()
        || m_factory->CheckAppInstance(GetSingletonAppUID());
}

bool CFrameApp::CreateFrameWnd()
{
    x3::loadScanPlugins("plugins");
    return m_factory->CreateFrameWnd(GetUIFactoryFile());
}

BOOL CFrameApp::OnIdle(LONG lCount)
{
    BOOL more = CWinApp::OnIdle(lCount);
    return m_factory->OnIdle(lCount) || more;
}


#if _MSC_VER > 1200     // not VC6
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif
