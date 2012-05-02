/*! \file WndFactory.h
 *  \brief Define template class (CWndFactory) to implement IWndFactory.
 *  \author Zhang Yungui, X3 C++ PluginFramework
 *  \date   2011.10.11
 */
#ifndef X3_VIEW_CREATEWND_MFCIMPL_H_
#define X3_VIEW_CREATEWND_MFCIMPL_H_

#ifdef _MSC_VER
#pragma warning(disable:4097)   // typedef-name used as synonym
#include <afxpriv.h>
#endif

#include "IWndFactory.h"
#include <objptr.h>

//! The macro to register window class in XBEGIN_DEFINE_MODULE() group.
/*!
    \ingroup _GROUP_APPUI_
    \param clsid window class unique id (ANSI string).
    \param cls window class deriving from CWnd or another window class.
    \see CWndFactory, XDEFINE_CLASSMAP_ENTRY
*/
#if defined(XDEFINE_CLASSMAP_ENTRY) && !defined(XDEFINE_CLASSMAP_ENTRY_Wnd)
#define XDEFINE_CLASSMAP_ENTRY_Wnd(clsid, cls) \
    x3::ClassEntry(1, "NormalObject<CWndFactory<" #cls ">>", clsid,  \
        (x3::ObjectCreator)(&x3::NormalObject<CWndFactory<cls> >::create), \
        (x3::HASIID)(&x3::NormalObject<CWndFactory<cls> >::hasInterface)),
#endif

#ifdef __AFXWIN_H__
class IWndFactoryMfc : public x3::IObject
{
public:
    X3DEFINE_IID(IWndFactoryMfc);
    virtual BOOL OnCmdMsg(UINT id, int nCode, void* pExtra, void* pHandlerInfo) = 0;
};
#endif

//! Template class to implement IWndFactory.
/*! BASEWND is a window class which need to implement the following functions:
    \code
    BOOL CreateWnd(CWnd* pParentWnd, UINT id);
    void Refresh();
    \endcode
    \ingroup _GROUP_APPUI_
    \see XDEFINE_CLASSMAP_ENTRY_Wnd
*/
template <class BASEWND>
class CWndFactory : public x3::IWndFactory
#ifdef __AFXWIN_H__
    , public IWndFactoryMfc
#endif
{
    X3BEGIN_CLASS_DECLARE0(CWndFactory)
        X3DEFINE_INTERFACE_ENTRY(x3::IWndFactory)
#ifdef __AFXWIN_H__
        X3DEFINE_INTERFACE_ENTRY(IWndFactoryMfc)
#endif
    X3END_CLASS_DECLARE()
protected:
    typedef CWndFactory<BASEWND> BaseWnd;
    static const char* _getClassID() { return ""; }
    static const char* _getClassName() { return "CWndFactory(Mfc)"; }

    CWndFactory()
    {
#if defined(_USRDLL) && !defined(_AFXEXT)
        AFX_MANAGE_STATE(AfxGetStaticModuleState());
#endif
        m_wnd = new BASEWND();
    }

    virtual ~CWndFactory()
    {
        ASSERT(NULL == m_wnd);
    }

private:
    virtual HWND GetWindow() const
    {
        return m_wnd->GetSafeHwnd();
    }

    // BASEWND class must has function: BOOL CreateWnd(CWnd* pParentWnd, UINT id)
    virtual bool CreateWnd(HWND hwndParent, int id)
    {
#if defined(_USRDLL) && !defined(_AFXEXT)
        AFX_MANAGE_STATE(AfxGetStaticModuleState());
#endif
        return m_wnd && m_wnd->CreateWnd(CWnd::FromHandle(hwndParent), id);
    }

    virtual void DestroyWnd()
    {
#if defined(_USRDLL) && !defined(_AFXEXT)
        AFX_MANAGE_STATE(AfxGetStaticModuleState());
#endif
        if (m_wnd != NULL)
        {
            BOOL delself = m_wnd->IsKindOf(RUNTIME_CLASS(CView));
            m_wnd->DestroyWindow();
            if (!delself)
                delete m_wnd;
            m_wnd = NULL;
        }
    }

    virtual bool OnCommand(int id, bool test = false)
    {
#if defined(_USRDLL) && !defined(_AFXEXT)
        AFX_MANAGE_STATE(AfxGetStaticModuleState());
#endif
#ifdef __AFXWIN_H__
        AFX_CMDHANDLERINFO info = { NULL };
        return !!m_wnd->OnCmdMsg(id, CN_COMMAND, NULL, test ? &info: NULL);
#else
        return false;
#endif
    }

    virtual bool OnUpdateUI(int id, bool& enabled, bool& checked, std::wstring& text)
    {
#ifdef _USRDLL
        AFX_MANAGE_STATE(AfxGetStaticModuleState());
#endif
#ifdef __AFXWIN_H__
        class CTempCmdUI : public CCmdUI
        {
        public:
            CTempCmdUI(bool& enabled, bool& checked, std::wstring& text)
                : m_enabled(enabled), m_checked(checked), m_text(text)
            {
            }

            virtual void Enable(BOOL on) { m_enabled = !!on; }
            virtual void SetCheck(int checked) { m_checked = (1 == checked); }
            virtual void SetRadio(BOOL on) { on; }
            virtual void SetText(LPCTSTR text) { m_text = text ? text : L""; }

            bool& m_enabled;
            bool& m_checked;
            std::wstring& m_text;
        };

        CTempCmdUI cmdui(enabled, checked, text);
        return !!m_wnd->OnCmdMsg(id, CN_UPDATE_COMMAND_UI, &cmdui, NULL);
#else
        return false;
#endif
    }

#ifdef __AFXWIN_H__
    virtual BOOL OnCmdMsg(UINT id, int nCode, void* pExtra, void* pHandlerInfo)
    {
#if defined(_USRDLL) && !defined(_AFXEXT)
        AFX_MANAGE_STATE(AfxGetStaticModuleState());
#endif
        return m_wnd->OnCmdMsg(id, nCode, pExtra, 
            static_cast<AFX_CMDHANDLERINFO*>(pHandlerInfo));
    }
#endif

    // BASEWND class may has function: void Refresh()
    virtual void Refresh()
    {
#if defined(_USRDLL) && !defined(_AFXEXT)
        AFX_MANAGE_STATE(AfxGetStaticModuleState());
#endif
#if defined(_MSC_VER) && _MSC_VER > 1300
        __if_exists(BASEWND::Refresh)
#endif
        {
            m_wnd->Refresh();
        }
    }

protected:
    BASEWND*    m_wnd;
};

#ifdef __AFXWIN_H__
namespace x3 {
static inline BOOL DoCmdMsgHelper(IWndFactory* pWnd, UINT id, int nCode, 
                                  void* pExtra, void* pHandlerInfo)
{
    Object<IWndFactoryMfc> pIFMfc(pWnd);
    if (pIFMfc)
    {
        return pIFMfc->OnCmdMsg(id, nCode, pExtra, pHandlerInfo);
    }

    if (CN_UPDATE_COMMAND_UI == nCode)
    {
        CCmdUI *pUI = static_cast<CCmdUI*>(pExtra);
        bool enabled = true;
        bool checked = false;
        std::wstring text(L"@#@$");

        bool ret = pWnd->OnUpdateUI(id, enabled, checked, text);

        if (ret)
        {
            pUI->Enable(ret && enabled);
            pUI->SetCheck(checked ? 1 : 0);
            if (text != L"@#@$")
                pUI->SetText(text.c_str());
        }
    }

    if (CN_COMMAND == nCode)
    {
        return pWnd->OnCommand(id, pHandlerInfo != NULL);
    }

    return FALSE;
}
} // x3
#endif

#endif // X3_VIEW_CREATEWND_MFCIMPL_H_
