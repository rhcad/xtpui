/*! \file IConfigSectionXml.h
 *  \brief Define the xml node interface: IConfigSectionXml
 *  \author Zhang Yungui, X3 C++ PluginFramework
 *  \date   2011.06.30
 */
#ifndef X3_XML_ICONFIGSECTIONXML_H_
#define X3_XML_ICONFIGSECTIONXML_H_

#include "IObject.h"

BEGIN_NAMESPACE_X3

//! The xml node interface like ConfigSection.
/*! This interface can be casted to IConfigSection.
    \interface IConfigSectionXml
    \ingroup _GROUP_PLUGIN_XML_
    \see IConfigSection
*/
class IConfigSectionXml : public IObject
{
public:
    X3DEFINE_IID(IConfigSectionXml);

    //! Set the fields of this element node are xml element nodes or xml property nodes.
    /*! This switch value is used in IConfigSection::GetString() and so on.\n
        The default value is false, thus each field is a xml property node.
        \param element true: each field is a xml element node.
            false: each field is a xml property node.
    */
    virtual void UseSubElement(bool element) = 0;

    //! Get the xml element text (may be CDATA text) of this element node.
    /*! The result content doesn't contain the field property values.
        \return the xml element text (may be CDATA text).
    */
    virtual std::wstring GetText() = 0;

    //! Set the xml element text of this element node.
    /*! The content doesn't contain the field property values.
        \param value the xml element text.
        \param cdata saves as normal element text (false) or CDATA text (true).
        \return true if the element text is changed.
    */
    virtual bool SetText(const wchar_t* value, bool cdata = false) = 0;

#ifdef _OLE2_H_
    //! Return the internal DOM element object (IXMLDOMElement).
    virtual IUnknown* GetDOMElement(bool addRef = false) = 0;

    //! Return the internal DOM document object (IXMLDOMDocument).
    virtual IUnknown* GetDOMDocument(bool addRef = false) = 0;
#endif // _OLE2_H_
};

END_NAMESPACE_X3
#endif // X3_XML_ICONFIGSECTIONXML_H_
