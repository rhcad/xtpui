#ifndef X3_UICORE_IFRAMEWND_FACTORY_H
#define X3_UICORE_IFRAMEWND_FACTORY_H

#include <objptr.h>

BEGIN_NAMESPACE_X3

const char* const clsidFrameWndFactory = "e13259f0-dabd-48a0-9527-190b11fec69e";

//! Create the main window.
/*! \ingroup _GROUP_APPUI_
    \see clsidFrameWndFactory
*/
class IFrameWndFactory : public x3::IObject
{
    X3DEFINE_IID(IFrameWndFactory);

    //! Check the application instance can run or not.
    /*!
        \param appid empty string or a unique string if the application can has only one instance.
        \return true if the application instance can run.
    */
    virtual bool CheckAppInstance(const wchar_t* appid) = 0;

    //! Create and show the main window.
    /*! The factory file is located in the config path.
        \param factoryFile a XML file name without path.
        \return true if creating successful, otherwise returns false.
    */
    virtual bool CreateFrameWnd(const wchar_t* factoryFile) = 0;

    //! Call it when the application is quiting.
    virtual void OnQuit() = 0;

    //! Call it to update controls' state.
    virtual bool OnIdle(long count) = 0;
};

END_NAMESPACE_X3
#endif