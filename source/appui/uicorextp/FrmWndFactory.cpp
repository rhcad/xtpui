#include "stdafx.h"
#include "FrmWndFactory.h"
#include <xml/IConfigXml.h>

#include "DummyDoc.h"
#include "ChildFrm.h"
#include "EmbedView.h"
#include "ViewItem.h"
#include "SDIFrameWnd.h"
#include "MDIFrameWnd.h"
#include "sinstance.h"
#include "UIOptions.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern ConfigSection        g_factoryRoot;
static CInstanceChecker*    s_checker = NULL;
static std::vector<ViewItem>    s_views;

CFrameWndFactory::CFrameWndFactory()
{
}

CFrameWndFactory::~CFrameWndFactory()
{
    delete s_checker;
}

bool CFrameWndFactory::CheckAppInstance(LPCWSTR appid)
{
    m_appid = appid;

    ASSERT(NULL == s_checker);
    s_checker = new CInstanceChecker(appid);

    s_checker->ActivateChecker();
    if (s_checker->PreviousInstanceRunning())
    {
        CCommandLineInfo cmdInfo;
        AfxGetApp()->ParseCommandLine(cmdInfo);
        s_checker->ActivatePreviousInstance(cmdInfo.m_strFileName);

        return false;
    }

    return true;
}

bool CFrameWndFactory::CreateFrameWnd(LPCWSTR factoryFile)
{
    CXTPWinDwmWrapper().SetProcessDPIAware();   // Support high DPI on Vista or above.

    std::wstring xtpfile(getTranslationsPath(L"ToolkitPro.Resource.xml"));
    if (PathFileExistsW(xtpfile.c_str()))
        XTPResourceManager()->SetResourceFile(xtpfile.c_str());
    else
        TRACE1("Warning: no translation file '%s'\n", xtpfile.c_str());

    Object<IConfigXml> xmlfile(x3::clsidXmlFile);
    ASSERT_MESSAGE(xmlfile, "Need the configxml plugin.");
    xmlfile->SetFileName((getConfigPath() + factoryFile).c_str());
    ASSERT_MESSAGE(xmlfile->Reload(), "No xml file or need to setup MSXML4.");

    ConfigSection root(xmlfile->GetData()->GetSection(L""));
    ConfigSection mainframe(root.GetSection(L"mainframe"));
    bool mdi = root->GetBool(L"mdi", false);

    g_factoryRoot = root;
    g_factoryRoot->SetString(L"_appid", m_appid.c_str());

    Object<IUIOptionsInit> initOptions(clsidUIOptions);
    initOptions->setFileName((getConfigPath() + 
        root->GetString(L"optionsFile", L"uioptions.xml")).c_str());

    RegisterDocTemplate(mdi, mainframe, root.GetSection(L"views"));
    if (mdi)
    {
        CMainMDIFrame* pFrame = new CMainMDIFrame;
        if (!pFrame->LoadFrame(0))
            return FALSE;

        std::vector<ViewItem>::const_iterator it = s_views.begin();
        for (; it != s_views.end(); ++it)
        {
            if (!it->caption.empty())
            {
                CDocument* pDoc = it->pTemplate->OpenDocumentFile(NULL);
                pDoc->SetTitle(it->caption.c_str());
            }
        }
    }

    return ProcessShellCommand();
}

ViewItem* FindViewItem(CDocTemplate* pTemplate)
{
    std::vector<ViewItem>::iterator it = s_views.begin();
    for (; it != s_views.end(); ++it)
    {
        if (it->pTemplate == pTemplate)
            return &*it;
    }
    return NULL;
}

void CFrameWndFactory::RegisterDocTemplate(bool mdi, const ConfigSection& frame, 
                                           const ConfigSection& views)
{
    UINT id = frame->GetUInt32(L"id");
    ASSERT(id != 0);
    ViewItem item;

    if (mdi)
    {
        for (int i = 0; i < 20; i++)
        {
            ConfigSection view(views.GetSectionByIndex(L"view", i));

            if (!view->IsValid())
                break;
            
            item.frameID = view->GetUInt32(L"id", id);
            item.layout = view->GetString(L"layout");
            item.caption = view->GetString(L"caption");
            item.clsid = view->GetString(L"clsid");

            item.pTemplate = new CMultiDocTemplate(
                item.frameID,
                RUNTIME_CLASS(CDummyDoc),
                RUNTIME_CLASS(CChildFrame),
                RUNTIME_CLASS(CEmbedView));
            AfxGetApp()->AddDocTemplate(item.pTemplate);
            s_views.push_back(item);
        }
    }
    else
    {
        item.pTemplate = new CSingleDocTemplate(id,
            RUNTIME_CLASS(CDummyDoc),
            RUNTIME_CLASS(CMainSDIFrame),
            RUNTIME_CLASS(CEmbedView));
        AfxGetApp()->AddDocTemplate(item.pTemplate);

        item.clsid = frame->GetString(L"viewClsid");
        if (!item.clsid.empty())
            s_views.push_back(item);
    }
}

bool CFrameWndFactory::ProcessShellCommand()
{
    CCommandLineInfo cmdInfo;
    AfxGetApp()->ParseCommandLine(cmdInfo);

    if (cmdInfo.m_nShellCommand == CCommandLineInfo::FileNew && !s_views.empty())
        cmdInfo.m_nShellCommand = CCommandLineInfo::FileNothing;
    if (!AfxGetApp()->ProcessShellCommand(cmdInfo))
        return false;

    if (AfxGetApp()->m_nCmdShow == SW_NORMAL
        && getUIOptions()->GetBool(L"showMaximized", true))
    {
        AfxGetApp()->m_nCmdShow = SW_SHOWMAXIMIZED;
    }
    AfxGetMainWnd()->ShowWindow(AfxGetApp()->m_nCmdShow);
    AfxGetMainWnd()->UpdateWindow();

    // The first instance of our app is running
	SafeCall(s_checker, TrackFirstInstanceRunning());

    return true;
}

void CFrameWndFactory::OnQuit()
{
}

bool CFrameWndFactory::OnIdle(long)
{
    bool more = false;

    return more;
}
