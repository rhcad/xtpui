#ifndef X3_UICORE_IUIOPTIONS_H
#define X3_UICORE_IUIOPTIONS_H

#include <xml/ConfigSection.h>

BEGIN_NAMESPACE_X3

const char* const clsidUIOptions = "37461df0-eafd-48e9-a5c4-818911f59ac4";

class IUIOptions : public x3::IObject
{
    X3DEFINE_IID(IUIOptions);

    //! Return a config node object of the specified name.
    virtual ConfigSection getSection(const wchar_t* name = L"") = 0;

    //! Save UI config data if modified.
    virtual bool save() = 0;
};

inline ConfigSection getUIOptions()
{
    return Object<IUIOptions>(clsidUIOptions)->getSection();
}

END_NAMESPACE_X3
#endif