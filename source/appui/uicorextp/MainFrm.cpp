#include "stdafx.h"
#include "MainFrm.h"
#include <utilfunc/lockcount.h>
#include <IUIOptions.h>
#include "PaneManager.h"
#include <FireCmdMsg.h>

ConfigSection    g_factoryRoot;

#if _XTPLIB_VERSION_MAJOR <= 13
inline CXTPOffice2007Images* XTPResourceImages() { return XTPOffice2007Images(); }
#endif

CMainFrame::CMainFrame()
    : m_id(0), m_enableCustomize(false)
    , m_paneLayout(NULL), m_mainwnd(NULL), m_cmdbars(NULL)
{
}

CMainFrame::~CMainFrame()
{
    SafeDelete(m_paneLayout);
    XTPResourceManager()->SetResourceFile(_T(""));
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
    cs.style &= ~FWS_ADDTOTITLE;
    cs.lpszClass = m_appid.empty() ? _T("XTPMainFrame") : m_appid.c_str();
    cs.lpszName = m_appid.c_str();
    CXTPDrawHelpers::RegisterWndClass(AfxGetInstanceHandle(), cs.lpszClass, 
        CS_DBLCLKS, AfxGetApp()->LoadIcon(m_id));

    return TRUE;
}

BOOL CMainFrame::BeforeLoadFrame(CFrameWnd* pMainWnd)
{
    m_mainwnd = pMainWnd;
    m_frameNode = g_factoryRoot.GetSection(L"mainframe");
    m_appid = g_factoryRoot->GetString(L"_appid");
    m_appname = g_factoryRoot->GetString(L"appname");
    m_id = GetNodeID(m_frameNode, L"id");
    ASSERT(!m_appname.empty());

    m_barsfile = getConfigPath() + g_factoryRoot->GetString(L"cmdbarsFile", L"cmdbars.xml");

    return TRUE;
}

BOOL CMainFrame::AfterLoadFrame()
{
    CString title;
    VERIFY(title.LoadString(m_id));
    m_mainwnd->SetWindowText(title);

    m_frameNode.release();
    g_factoryRoot.release();
    AfxGetApp()->m_pMainWnd = m_mainwnd;

    return TRUE;
}

BOOL CMainFrame::InitRibbonBars(CXTPCommandBars* pCommandBars)
{
    m_cmdbars = pCommandBars;

    m_cmdbars->GetCommandBarsOptions()->ShowKeyboardCues(xtpKeyboardCuesShowWindowsDefault);
    m_cmdbars->GetCommandBarsOptions()->bToolBarAccelTips = TRUE;
    m_cmdbars->GetCommandBarsOptions()->bShowKeyboardTips = TRUE;
    m_cmdbars->GetShortcutManager()->SetAccelerators(m_id);

    if (InitRibbonTheme() && !CreateRibbonBar())
    {
        TRACE0("Failed to create ribbon\n");
    }

    if (!CreateStatusBar())
        return FALSE;

    CXTPToolTipContext* pToolTipContext = m_cmdbars->GetToolTipContext();
    pToolTipContext->SetStyle((XTPToolTipStyle)(xtpToolTipLuna + 1));
    pToolTipContext->ShowTitleAndDescription();
    pToolTipContext->SetMargin(CRect(2, 2, 2, 2));
    pToolTipContext->SetMaxTipWidth(180);

    m_enableCustomize = m_frameNode->GetBool(L"enableCustomize", !HasRibbonBar());
    LoadCommandBars();

    CreateDockingPanes();

    return TRUE;
}

BOOL CMainFrame::InitRibbonTheme()
{
    std::wstring themeName(getUIOptions()->GetString(L"themeName", L"OFFICE2010SILVER.INI"));
    std::wstring themeDll(getUIOptions()->GetString(L"themeDll", L"Office2010.dll"));

    getUIOptions()->SetString(L"themeDll", themeDll.c_str());
    getUIOptions()->SetString(L"themeName", themeName.c_str());

    themeDll = getTranslationsPath((L"../" + themeDll).c_str());
    BOOL ret = XTPResourceImages()->SetHandle(themeDll.c_str(), themeName.c_str());

    if (!ret)
    {
        std::wstring dllfile(getTranslationsPath(L"../Office2007.dll"));
        ret = XTPResourceImages()->SetHandle(dllfile.c_str(), L"OFFICE2007BLUE.INI");
        if (ret)
        {
            getUIOptions()->SetString(L"themeDll", L"Office2007.dll");
            getUIOptions()->SetString(L"themeName", L"OFFICE2007BLUE.INI");
        }
    }

    if (ret)
    {
        XTPPaintManager()->SetTheme(xtpThemeRibbon);
        m_cmdbars->SetTheme(xtpThemeRibbon);
    }
    else
    {
        CString msg;
        msg.Format(L"Need Office2010.dll or Office2007.dll in the translations folder: %s.\n", getTranslationsPath(L"..").c_str());
        ASSERT_MESSAGE(0, w2a(msg).c_str());
    }

    return ret;
}

BOOL CMainFrame::CreateRibbonBar()
{
    std::wstring xcbfile(getConfigPath() + g_factoryRoot->GetString(L"xcbfile"));
    CFile file(xcbfile.c_str(), CFile::modeRead);
    CArchive ar(&file, CArchive::load);
    
    m_cmdbars->LoadDesignerBars(ar);

    CXTPRibbonBar* pRibbonBar = DYNAMIC_DOWNCAST(
        CXTPRibbonBar, m_cmdbars->GetMenuBar());
    if (NULL == pRibbonBar)
        return FALSE;

    SetRibbonFont();

    std::wstring themeName(getUIOptions()->GetString(L"themeName"));
    if (themeName.find(L"WINDOWS7") != std::wstring::npos
        || themeName.find(L"OFFICE201") != std::wstring::npos)
    {
        pRibbonBar->GetSystemButton()->SetStyle(xtpButtonCaption);
    }
    pRibbonBar->EnableFrameTheme();

    return TRUE;
}

void CMainFrame::SetRibbonFont()
{
    CXTPOffice2007Theme* pOfficeTheme = (CXTPOffice2007Theme*)(m_cmdbars->GetPaintManager());
    CString fontname, sizestr;

    std::wstring fontcfg(GetStringValue(m_appname.c_str(), L"RibbonFont", L"."));
    if (fontcfg.size() < 2)
    {
        fontcfg = GetStringValue(L"uicorextp", L"RibbonFont", 
            getUIOptions()->GetString(L"ribbonFont", L"Tahoma,12").c_str());
    }

    AfxExtractSubString(fontname, fontcfg.c_str(), 0, ',');
    AfxExtractSubString(sizestr, fontcfg.c_str(), 1, ',');
    long fontsize = _wtol(sizestr);

    if (!fontname.IsEmpty() && pOfficeTheme->FontExists(fontname))
    {
        pOfficeTheme->m_strOfficeFont = fontname;
        XTPPaintManager()->m_strOfficeFont = fontname;
    }
    if (fontsize != 0)
    {
#if _XTPLIB_VERSION_MAJOR > 13
        pOfficeTheme->SetFontHeight(fontsize);
#else
        CXTPRibbonBar* pRibbonBar = DYNAMIC_DOWNCAST(
            CXTPRibbonBar, m_cmdbars->GetMenuBar());
        pRibbonBar->SetFontHeight(fontsize);
#endif
    }
    else
    {
        pOfficeTheme->UpdateFonts();
    }

    m_cmdbars->GetToolTipContext()->SetFont(m_cmdbars->GetPaintManager()->GetRegularFont());
}

BOOL CMainFrame::HasRibbonBar() const
{
    return !!DYNAMIC_DOWNCAST(CXTPRibbonBar, m_cmdbars->GetMenuBar());
}

BOOL CMainFrame::CreateStatusBar()
{
    std::vector<UINT> indicators(1, ID_SEPARATOR);
    ConfigSection items(m_frameNode.GetSection(L"statusbar/indicators"));

    for (int i = 0; ; i++)
    {
        ConfigSection item(items.GetSectionByIndex(L"item", i));
        UINT id = GetNodeID(item, L"id");
        if (0 == id)
            break;
        indicators.push_back(id);
        if (ID_SEPARATOR == id && indicators.front() == id)
            indicators.erase(indicators.begin());
    }

    VERIFY(m_statusBar.Create(m_mainwnd)
        && m_statusBar.SetIndicators(&indicators[0], x3::GetSize(indicators)));

    m_statusBar.SetCommandBars(m_cmdbars);
    m_statusBar.SetDrawDisabledText(FALSE);
    m_statusBar.GetStatusBarCtrl().SetMinHeight(22);
    m_statusBar.GetPane(0)->SetMargins(8, 1, 2, 1);

    return TRUE;
}

void CMainFrame::ExchangeLayout(CXTPPropExchange* px)
{
	XTP_COMMANDBARS_PROPEXCHANGE_PARAM param;
	param.bSerializeControls = TRUE;
    param.bSaveOriginalControls = FALSE;

	CXTPPropExchangeSection pxCommandBars(px->GetSection(_T("CommandBars")));
	m_cmdbars->DoPropExchange(&pxCommandBars, &param);

	CXTPPropExchangeSection secShortcuts(px->GetSection(_T("Shortcuts")));
	m_cmdbars->GetShortcutManager()->DoPropExchange(&secShortcuts);

	CXTPPropExchangeSection secOptions(px->GetSection(_T("Options")));	
	m_cmdbars->GetCommandBarsOptions()->DoPropExchange(&secOptions, TRUE);
}

void CMainFrame::LoadCommandBars()
{
    if (m_enableCustomize)
	{
		CXTPPropExchangeXMLNode px(TRUE, NULL, _T("Settings"));

		if (px.LoadFromFile(m_barsfile.c_str()))
		{
			CXTPPropExchangeSection pxLayout(px.GetSection(_T("NormalLayout")));
			ExchangeLayout(&pxLayout);
		}
	}
}

void CMainFrame::SaveCommandBars()
{
    if (m_enableCustomize)
	{
		CXTPPropExchangeXMLNode px(FALSE, NULL, _T("Settings"));

		if (px.OnBeforeExchange())
		{
			CXTPPropExchangeSection pxLayout(px.GetSection(_T("NormalLayout")));
			ExchangeLayout(&pxLayout);

			px.SaveToFile(m_barsfile.c_str());
		}
	}
    if (m_mainwnd->IsWindowVisible())
    {
        getUIOptions()->SetBool(L"showMaximized", m_mainwnd->IsZoomed());
    }
}

void CMainFrame::OnClose()
{
    SaveCommandBars();
    SafeCall(m_paneLayout, SaveDockingPanes());
}

void CMainFrame::OnDestroy()
{
    SafeCall(m_paneLayout, DestroyPanes());

    Object<IUIOptions>(clsidUIOptions)->save();
}

BOOL CMainFrame::OnCmdMsg(UINT id, int nCode, void* pExtra, 
                          AFX_CMDHANDLERINFO* pInfo)
{
    static long s_nLocker = 0;
    LockCount locker (&s_nLocker);

    if (1 == s_nLocker)
    {
        ///if (CN_COMMAND == nCode && pInfo && FindGalleryItems(id))
        //    return TRUE;
        if (FireCmdMsgEvent(id, nCode, pExtra, pInfo))
            return TRUE;
        if (m_paneLayout && m_paneLayout->DoCmdMsg(id, nCode, pExtra, pInfo))
		    return TRUE;
    }

    return FALSE;
}

void CMainFrame::OnUpdateRibbonTab(CCmdUI* pCmdUI)
{
    CXTPRibbonControlTab* pControl = DYNAMIC_DOWNCAST(
        CXTPRibbonControlTab, CXTPControl::FromUI(pCmdUI));
    if (!pControl)
        return;

    //CXTPRibbonTab* pTab = pControl->FindTab(ID_TAB_EDIT);
    //pTab->SetVisible(MDIGetActive() != NULL);
}

void CMainFrame::ShowCustomizeDialog(int nSelectedPage)
{
    CXTPCustomizeSheet cs(m_cmdbars);
    CXTPRibbonCustomizeQuickAccessPage pageQuickAccess(&cs);

    if (HasRibbonBar())
    {
        cs.AddPage(&pageQuickAccess);
        pageQuickAccess.AddCategories(m_cmdbars->m_pDesignerControls);
    }

    CXTPCustomizeKeyboardPage pageKeyboard(&cs);
    cs.AddPage(&pageKeyboard);
    pageKeyboard.AddCategories(m_cmdbars->m_pDesignerControls);

    CXTPCustomizeOptionsPage pageOptions(&cs);
    cs.AddPage(&pageOptions);

	cs.GetCommandsPage()->AddCategories(m_cmdbars->m_pDesignerControls);
	cs.GetCommandsPage()->InsertAllCommandsCategory();

    cs.SetActivePage(nSelectedPage);
    cs.SetResizable();

    if (m_enableCustomize)
    {
        cs.DoModal();
    }
}

void CMainFrame::OnCustomize()
{
    ShowCustomizeDialog(0);
}

void CMainFrame::OnCustomizeQuickAccess()
{
    ShowCustomizeDialog(2);
}

UINT CMainFrame::GetNodeID(const ConfigSection& node, LPCWSTR name)
{
    UINT id = node->GetUInt32(name);
    std::wstring strname(node->GetString(name));
    size_t pos = strname.find(L' ');

    if (id != 0 && pos != strname.npos)
    {
        strname = strname.substr(pos + 1);

        TRACE2("String ID: %d, %s\n", id, strname.c_str());
        ASSERT(strname.find(L' ') == strname.npos && !strname.empty());

        std::map<std::wstring, UINT>::iterator it = m_idnames.find(strname);
        ASSERT_MESSAGE(it == m_idnames.end() || it->second == id,
            "A string name must has only one ID value in the factory xml.");

        m_idnames[strname] = id;
    }

    return id;
}

UINT CMainFrame::FindID(const std::wstring& name) const
{
    std::map<std::wstring, UINT>::const_iterator it = m_idnames.find(name);
    return it != m_idnames.end() ? it->second : 0;
}

std::wstring CMainFrame::GetLocalizationString(const std::wstring& name) const
{
    std::wstring text(GetStringValue(m_appname.c_str(), name.c_str()));

    if (text.empty())
    {
        size_t pos = name.rfind(L'|');
        text = (pos != name.npos) ? name.substr(pos + 1) : name;
    }

    return text;
}

int CMainFrame::OnCreateControl(LPCREATECONTROLSTRUCT lpCreateControl)
{
    return FALSE;
}

void CMainFrame::CreateDockingPanes()
{
    // Initialize the docking pane manager and set the
    // initial them for the docking panes.  Do this only after all
    // control bars objects have been created and docked.
    m_paneManager.InstallDockingPanes(m_mainwnd);

    m_paneLayout = new CPaneManager(m_mainwnd, &m_paneManager, 
        m_frameNode->GetString(L"layout"));
    m_paneLayout->CreateDockingPanes();
}

LRESULT CMainFrame::OnDockingPaneNotify(WPARAM wParam, LPARAM lParam)
{
	return m_paneLayout && m_paneLayout->OnDockingPaneNotify(wParam, lParam);
}
