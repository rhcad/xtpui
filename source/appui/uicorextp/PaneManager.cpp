#include "stdafx.h"
#include "PaneManager.h"
#include "EmbedWnd.h"
#include <xml/IConfigXml.h>
#include <xml/IConfigTransaction.h>

CPaneManager::CPaneManager(CWnd* paneOwner, CXTPDockingPaneManager* manager, 
                           const std::wstring& folderName)
    : m_designMode(true), m_folderName(folderName)
    , m_paneOwner(paneOwner), m_manager(manager)
{
    m_manager->SetTheme(xtpPaneThemeOffice2007);
    m_manager->SetAlphaDockingContext(TRUE);
}

CPaneManager::~CPaneManager()
{
    ASSERT(m_mapView.empty());
}

void CPaneManager::DestroyPanes()
{
    std::map<UINT, CEmbedWnd*>::iterator it = m_mapView.begin();
    for (; it != m_mapView.end(); ++it)
    {
        ASSERT(it->second);
        it->second->DestroyWindow();
        SafeDelete(it->second);
    }
    m_mapView.clear();
}

std::wstring CPaneManager::MakeFileName(const std::wstring& shortName) const
{
    std::wstring filename(getConfigPath());
    if (!m_folderName.empty())
        filename += m_folderName + L"\\";
    return filename + shortName;
}

static void GetPaneIDs(std::vector<UINT>& arrID, CXTPDockingPaneManager* manager)
{
    POSITION pos = manager->GetPaneList().GetHeadPosition();
    while (pos)
    {
        CXTPDockingPane* pPane = manager->GetPaneList().GetNext(pos);
        arrID.push_back(pPane->GetID());
    }
}

bool CPaneManager::CreateDockingPanes()
{
    std::wstring factoryFile(MakeFileName(L"PaneFactory.xml"));
    std::wstring initialFile(MakeFileName(L"PaneInitLayout.xml"));
    std::wstring userFile(MakeFileName(L"PaneLayout.xml"));

    // 根据PaneFactory.xml创建面板
    // DesignMode: 没有PaneInitLayout.xml或根节点的designMode属性为true
    m_designMode = !PathFileExistsW(initialFile.c_str());
    CreatePanes(factoryFile);

    if (m_mapView.empty())
        return false;

    std::vector<UINT> arrID;
    GetPaneIDs(arrID, m_manager);

    if (m_designMode)
    {
        // 删除PaneLayout.xml，以便后续根据PaneInitLayout.xml生成PaneLayout.xml
        ::DeleteFileW(userFile.c_str());

        // 从PaneInitLayout.xml加载面板布局，退出时将保存到PaneInitLayout.xml
        LoadPaneLayout(initialFile);
    }
    else
    {
        if (IsSameTimeStamp(userFile, initialFile))
        {
            // 如果没有PaneLayout.xml，就从PaneInitLayout.xml复制生成
            ::CopyFileW(initialFile.c_str(), userFile.c_str(), TRUE);

            // 从PaneLayout.xml加载面板布局，退出时将保存到PaneLayout.xml
            LoadPaneLayout(userFile);
        }
        else    // PaneInitLayout.xml 发生了改变
        {
            // 从PaneInitLayout.xml加载面板布局，退出时将保存到PaneLayout.xml
            LoadPaneLayout(initialFile);
        }
    }

    std::vector<UINT> arrIDNew;
    GetPaneIDs(arrIDNew, m_manager);

    if (arrIDNew != arrID)
    {
        DestroyPanes();
        m_manager->DestroyAll();
        CreatePanes(factoryFile);
    }

    // 根据配置文件配置面板属性，比如没有标题栏，不能关闭等
    ResetPaneOptions(factoryFile);

    return true;
}

std::wstring GetFileModifyTime(const std::wstring& filename);

bool CPaneManager::IsSameTimeStamp(const std::wstring& userFile, 
                                   const std::wstring& initialFile)
{
    std::wstring wstrTimeInit(GetFileModifyTime(initialFile));

    Object<IConfigXml> pIFXml(clsidXmlFile);
    pIFXml->SetFileName(userFile.c_str());
    pIFXml->SetRootName(L"DockingPaneLayouts");

    ConfigSection secRoot(pIFXml->GetData()->GetSection(L""));
    std::wstring wstrTimeLast(secRoot->GetString(L"TimeStamp"));

    return wstrTimeInit == wstrTimeLast;
}

bool CPaneManager::SaveTimeStamp(const std::wstring& userFile, 
                                 const std::wstring& initialFile)
{
    std::wstring wstrTime(GetFileModifyTime(initialFile));

    Object<IConfigXml> pIFXml(clsidXmlFile);
    pIFXml->SetFileName(userFile.c_str());
    pIFXml->SetRootName(L"DockingPaneLayouts");

    ConfigSection secRoot(pIFXml->GetData()->GetSection(L""));
    secRoot->SetString(L"TimeStamp", wstrTime.c_str());

    return ConfigTransaction(AnyObject(secRoot.p())).Submit();
}

static XTPDockingPaneDirection ToDirection(const std::wstring& value)
{
    LPCWSTR names[] = { L"left", L"right", L"top", L"bottom" };
    int i = _countof(names);

    while (--i >= 0 && StrCmpIW(value.c_str(), names[i]) != 0) ;
    ASSERT_MESSAGE(i >= 0, "Invalid direction value");

    return (XTPDockingPaneDirection)max(0, i);
}

void CPaneManager::CreatePanes(const std::wstring& filename)
{
    Object<IConfigXml> pIFXmlFile(clsidXmlFile);
    pIFXmlFile->SetFileName(filename.c_str());

    ConfigSection root(pIFXmlFile->GetData()->GetSection(L""));
    ConfigSection secList(root.GetSection(L"DockingPanes"));

    m_designMode = m_designMode || root->GetBool(L"designMode", false);

    for (long i = 0; i < 40; i++)
    {
        ConfigSection secPane(secList.GetSectionByIndex(L"Pane", i));

        std::wstring wstrClsid(secPane->GetString(L"clsid"));
        if (wstrClsid.empty())
            break;
        const std::string clsidWnd(x3::w2a(wstrClsid).c_str());

        UINT id = secPane->GetUInt32(L"id");
        ASSERT_MESSAGE(id > 0 && m_mapView.find(id) == m_mapView.end(), "Invalid pane id.");

        CXTPDockingPane* pPane = m_manager->CreatePane(
            id, CRect(0, 0, 100, 100), 
            ToDirection(secPane->GetString(L"direction")));
        ASSERT(pPane != NULL);

        std::wstring wstrCaption(secPane->GetString(L"caption"));
        pPane->SetTitle(wstrCaption.c_str());

        CEmbedWnd* pWnd = new CEmbedWnd(clsidWnd.c_str());
        m_mapView[id] = pWnd;

        pWnd->SetCaption(wstrCaption.c_str());
        pWnd->SetContextMenu(secPane->GetInt(L"contextMenuIndex", -1));

        DWORD dwExStyle = secPane->GetBool(L"sunken", true) ? WS_EX_STATICEDGE : 0;
        if (!pWnd->CreateWnd(m_paneOwner, id, WS_CHILD, dwExStyle))
        {
            X3LOG_WARNING2(L"@uicorextp:IDS_CREATEVIEW_FAIL", id);
        }
        else if (!pWnd->CreateSubWnd(clsidWnd.c_str()))
        {
            X3LOG_WARNING2(L"@uicorextp:IDS_CREATEVIEW_FAIL", x3::a2w(clsidWnd));
        }

        CView* pView = DYNAMIC_DOWNCAST(CView, 
            CWnd::FromHandle(pWnd->GetChildWnd()));
        CView* pOwnerView = DYNAMIC_DOWNCAST(CView, m_paneOwner);

        if (pView && pOwnerView)
        {
            pOwnerView->GetDocument()->AddView(pView);
        }
    }

    m_manager->EnableKeyboardNavigate();
    m_manager->SetAlphaDockingContext(TRUE);
    m_manager->SetShowDockingContextStickers(TRUE);
}

static void SetPaneOptions(CXTPDockingPane* pPane, 
                           const ConfigSection& secPane, bool bInnerPane)
{
    DWORD dwOptions = 0;

    if (!secPane->GetBool(L"canClose", true))
    {
        dwOptions |= xtpPaneNoCloseable;
    }
    if (!secPane->GetBool(L"canHide", true))
    {
        dwOptions |= xtpPaneNoHideable;
    }
    if (!secPane->GetBool(L"canFloat", true) || bInnerPane)
    {
        dwOptions |= xtpPaneNoFloatable;
    }
    if (!secPane->GetBool(L"canDock", true))
    {
        dwOptions |= xtpPaneNoDockable;
    }
    if (!secPane->GetBool(L"hasCaption", true))
    {
        dwOptions |= xtpPaneNoCaption;
    }
    if (secPane->GetBool(L"hasButton", true))
    {
        dwOptions |= xtpPaneHasMenuButton;
    }

    pPane->SetOptions(dwOptions);
}

void CPaneManager::ResetPaneOptions(const std::wstring& filename)
{
    Object<IConfigXml> pIFXmlFile(clsidXmlFile);
    pIFXmlFile->SetFileName(filename.c_str());

    ConfigSection secList(pIFXmlFile->GetData()->GetSection(L"DockingPanes"));

    std::map<UINT, CEmbedWnd*>::iterator it = m_mapView.begin();
    for (; it != m_mapView.end(); ++it)
    {
        UINT id = it->first;
        ConfigSection secPane(secList.GetSection(L"Pane", L"id", id, false));
        ASSERT(!secPane->GetString(L"clsid").empty());

        CXTPDockingPane* pPane = m_manager->FindPane(id);
        if (NULL == pPane)
        {
            X3LOG_WARNING2(L"@uicorextp:IDS_PANE_NOTEXIST", id);
        }
        else
        {
            pPane->SetTitle(secPane->GetString(L"caption").c_str());

            if (!m_designMode)
            {
                SetPaneOptions(pPane, secPane, 
                    m_paneOwner->IsKindOf(RUNTIME_CLASS(CMDIChildWnd))
                    || m_paneOwner->IsKindOf(RUNTIME_CLASS(CView)));
            }
        }
    }
}

bool CPaneManager::OnDockingPaneNotify(WPARAM wParam, LPARAM lParam)
{
    if (XTP_DPN_SHOWWINDOW == wParam)
    {
        CXTPDockingPane* pPane = (CXTPDockingPane*)lParam;
        UINT id = pPane->GetID();
        std::map<UINT, CEmbedWnd*>::iterator it = m_mapView.find(id);

        if (it == m_mapView.end() || !::IsWindow(it->second->GetSafeHwnd()))
        {
            return false;
        }

        if (!pPane->IsValid())
        {
            pPane->Attach(it->second);
        }
        else
        {
            it->second->Refresh();
        }

        return true;
    }

    return false;
}

void CPaneManager::LoadPaneLayout(const std::wstring& filename)
{
    if (m_mapView.empty())
        return;

    CXTPDockingPaneLayout layoutNormal(m_manager);

    if (layoutNormal.LoadFromFile(filename.c_str(), _T("NormalLayout")))
    {
        m_manager->SetLayout(&layoutNormal);
    }
}

void CPaneManager::SaveDockingPanes()
{
    if (m_mapView.empty())
        return;

    std::wstring filename(MakeFileName(
        m_designMode ? L"PaneInitLayout.xml" : L"PaneLayout.xml"));
    DWORD dwFileAttr = ::GetFileAttributesW(filename.c_str());

    if ((dwFileAttr == (DWORD)-1) || (dwFileAttr & FILE_ATTRIBUTE_READONLY) == 0)
    {
        CXTPDockingPaneLayout layoutNormal(m_manager);
        m_manager->GetLayout(&layoutNormal);
        layoutNormal.SaveToFile(filename.c_str(), _T("NormalLayout"));

        if (!m_designMode)
        {
            SaveTimeStamp(filename, MakeFileName(L"PaneInitLayout.xml"));
        }
    }
}

BOOL CPaneManager::DoCmdMsg(UINT id, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pInfo)
{
    CWnd* pFocusWnd = CWnd::GetFocus();

    std::map<UINT, CEmbedWnd*>::iterator it = m_mapView.begin();
    for (; it != m_mapView.end(); ++it)
    {
        CEmbedWnd* pWnd = it->second;
        if (!IsWindow(pWnd->GetSafeHwnd()))
            continue;

        if (pFocusWnd->GetSafeHwnd() == pWnd->GetSafeHwnd())
        {
            if (pFocusWnd->OnCmdMsg(id, nCode, pExtra, pInfo))
                return TRUE;
            break;
        }
        else if (pFocusWnd != NULL && pWnd->IsChild(pFocusWnd))
        {
            if (pFocusWnd->OnCmdMsg(id, nCode, pExtra, pInfo)
                || pWnd->OnCmdMsg(id, nCode, pExtra, pInfo))
                return TRUE;
            pFocusWnd = pWnd;
            break;
        }
    }

    return FALSE;
}

AnyObject CPaneManager::GetViewObject(CWnd* pWnd) const
{
    CEmbedWnd* pEmbedWnd = DYNAMIC_DOWNCAST(CEmbedWnd, pWnd);
    return pEmbedWnd ? pEmbedWnd->GetWndObj() : AnyObject();
}

long CPaneManager::GetViewCount() const
{
    return GetSize(m_mapView);
}

CWnd* CPaneManager::GetView(long index) const
{
    std::map<UINT, CEmbedWnd*>::const_iterator it = m_mapView.begin();
    for (long i = 0; it != m_mapView.end(); ++it, ++i)
    {
        if (i == index)
        {
            return it->second;
        }
    }

    return NULL;
}

CWnd* CPaneManager::FindPaneWnd(const char* clsidView, UINT* pID) const
{
    std::map<UINT, CEmbedWnd*>::const_iterator it = m_mapView.begin();
    for (; it != m_mapView.end(); ++it)
    {
        if (it->second->GetClsid() == clsidView)
        {
            if (pID)
            {
                *pID = it->first;
            }
            return it->second;
        }
    }

    return NULL;
}

bool CPaneManager::ShowPane(UINT id, bool activate)
{
    CXTPDockingPane* pPane = m_manager->FindPane(id);
    if (pPane != NULL)
    {
        m_manager->ShowPane(pPane, activate);
        return true;
    }

    return false;
}

bool CPaneManager::HidePane(UINT id)
{
    CXTPDockingPane* pPane = m_manager->FindPane(id);
    if (pPane != NULL)
    {
        m_manager->HidePane(pPane);
        return true;
    }

    return false;
}

bool CPaneManager::SwitchPaneLayout(const std::wstring&)
{
    ASSERT_MESSAGE(0, "TODO: CPaneManager::SwitchPaneLayout()");
    return false;
}
