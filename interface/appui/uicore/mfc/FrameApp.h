#ifndef X3_UICORE_FRAMEAPP_H
#define X3_UICORE_FRAMEAPP_H

#include "IFrmWndFactory.h"

//! Base class for MFC application using the uicorextp plugin.
/*! \ingroup _GROUP_APPUI_
*/
class CFrameApp : public CWinApp
{
public:
    CFrameApp();
    virtual ~CFrameApp();

public: // Overrides from CWinApp
    virtual BOOL InitInstance();
    virtual int ExitInstance();
    virtual BOOL OnIdle(LONG lCount);

protected:
    //! Returns a unique string if the application can has only one instance.
    virtual LPCWSTR GetSingletonAppUID() const { return L""; }

    //! Returns the file used to create the main window.
    /*! This file is located in the config path.
        \return a XML file name without path.
    */
    virtual LPCWSTR GetUIFactoryFile() const { return L"uifactory.xml"; }

    //! Load the uicorextp plugin and some important plugins.
    virtual bool LoadCorePlugins();

    //! Check the application instance can run or not.
    virtual bool CheckAppInstance();

    //! Create and show the main window.
    virtual bool CreateFrameWnd();

protected:
    x3::Object<x3::IFrameWndFactory>    m_factory;

protected:
    DECLARE_MESSAGE_MAP()
};

#endif