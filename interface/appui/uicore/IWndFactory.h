/*! \file IWndFactory.h
 *  \brief Define window creator interface: IWndFactory
 *  \author Zhang Yungui, X3 C++ PluginFramework
 *  \date   2011.10.11
 */
#ifndef X3_VIEW_ICREATEWND_H_
#define X3_VIEW_ICREATEWND_H_

#include <iobject.h>

BEGIN_NAMESPACE_X3

//! Window creator interface
/*! Use CWndFactory<YourWndClass> to implement this interface.
    Parent window class need not to known child window class or implement detail.
    \interface IWndFactory
    \ingroup _GROUP_APPUI_
    \see CWndFactory
*/
class IWndFactory : public x3::IObject
{
    X3DEFINE_IID(IWndFactory);

    //! Return the window handle.
    virtual HWND GetWindow() const = 0;

    //! Create window with a specified control id.
    virtual bool CreateWnd(HWND hwndParent, int id) = 0;

    //! Destroy window and object.
    virtual void DestroyWnd() = 0;

    //! Dispatch a command message.
    virtual bool OnCommand(int id, bool test = false) = 0;

    //! Dispatch the update notify of a command UI object.
    virtual bool OnUpdateUI(int id, bool& enabled, bool& checked, std::wstring& text) = 0;

    //! Refresh display.
    virtual void Refresh() = 0;
};

END_NAMESPACE_X3
#endif