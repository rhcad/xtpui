#ifndef X3_UICORE_IUIOPTIONS_IMPL_H
#define X3_UICORE_IUIOPTIONS_IMPL_H

#include <IUIOptions.h>

class IUIOptionsInit : public x3::IObject
{
    X3DEFINE_IID(IUIOptionsInit);
    virtual void setFileName(const wchar_t* filename) = 0;
};

class CUIOptions : public IUIOptions, public IUIOptionsInit
{
    X3BEGIN_CLASS_DECLARE(CUIOptions, clsidUIOptions)
        X3DEFINE_INTERFACE_ENTRY(IUIOptionsInit)
        X3DEFINE_INTERFACE_ENTRY(IUIOptions)
    X3END_CLASS_DECLARE()
protected:
    CUIOptions();
    virtual ~CUIOptions();

private:
    virtual void setFileName(const wchar_t* filename);
    virtual ConfigSection getSection(const wchar_t* name = L"");
    virtual bool save();

private:
    AnyObject       m_xmlfile;
    ConfigSection   m_root;
    ConfigSection   m_options;
};

#endif