/*! \file ConfigSection.h
 *  \brief the configure data node class: ConfigSection
 *  \author Zhang Yungui, X3 C++ PluginFramework
 *  \date   2010.10.22
 */
#ifndef X3_XML_CONFIGIOSECTION_H_
#define X3_XML_CONFIGIOSECTION_H_

#include "IConfigData.h"

BEGIN_NAMESPACE_X3

//! The configure data node wrapper class.
/*! This class object is represented as a data node in hierarchical structure nodes.\n
    IConfigData object is represented as all nodes.
    \ingroup _GROUP_PLUGIN_XML_
*/
class ConfigSection
{
public:
    Object<IConfigSection> sec;

    ConfigSection() {}
    ConfigSection(const ConfigSection& src) : sec(src.sec) {}
    ConfigSection(const AnyObject& src) : sec(src.p()) {}

    ConfigSection& operator=(const ConfigSection& src)
    {
        sec = src.sec;
        return *this;
    }

    ConfigSection& operator=(const AnyObject& src)
    {
        sec = src.p();
        return *this;
    }

    IConfigSection* p() const { return sec.p(); }
    IConfigSection* operator->() const { return sec.p(); }
    void release() { sec.release(); }

    //! Returns a child node by node name.
    /*!
        \return the child node (ConfigSection). The node will be created when not exist and autoCreate is true.
        \param name the child node name. the name may contains slash chars for multiple levels.
        \param autoCreate true if enable to auto create new node.
    */
    AnyObject GetSection(
        const wchar_t* name, bool autoCreate = true) const
    {
        IConfigData* pIFData = p()->GetData();
        return pIFData->GetSection(p(), name, L"", L"", autoCreate);
    }

    //! Returns a child node by node name and integer property value.
    /*!
        \return the child node (ConfigSection). The node will be created when not exist and autoCreate is true.
        \param name the child node name. the name may contains slash chars for multiple levels.
        \param attrName a field name of the child node.
        \param attrValue the field value of the child node.
        \param autoCreate true if enable to auto create new node.
    */
    AnyObject GetSection(const wchar_t* name,
        const wchar_t* attrName, ULONG attrValue, bool autoCreate = true) const
    {
        IConfigData* pIFData = p()->GetData();
        return pIFData->GetSection(p(), name, attrName, attrValue, autoCreate);
    }

    //! Returns a child node by node name and string property value.
    /*!
        \return the child node (ConfigSection). The node will be created when not exist and autoCreate is true.
        \param name the child node name. the name may contains slash chars for multiple levels.
        \param attrName a field name of the child node.
        \param attrValue the field value of the child node.
        \param autoCreate true if enable to auto create new node.
    */
    AnyObject GetSection(const wchar_t* name,
        const wchar_t* attrName, const wchar_t* attrValue,
        bool autoCreate = true) const
    {
        IConfigData* pIFData = p()->GetData();
        return pIFData->GetSection(p(), name, attrName, attrValue, autoCreate);
    }

    //! Returns a child node by node name and string property value.
    /*!
        \return the child node (ConfigSection). The node will be created when not exist and autoCreate is true.
        \param name the child node name. the name may contains slash chars for multiple levels.
        \param attrName a field name of the child node.
        \param attrValue the field value of the child node.
        \param attrName2 another field name of the child node.
        \param attrValue2 the field value of attrName2.
        \param autoCreate true if enable to auto create new node.
    */
    AnyObject GetSection(const wchar_t* name,
        const wchar_t* attrName, const wchar_t* attrValue,
        const wchar_t* attrName2, const wchar_t* attrValue2,
        bool autoCreate = true) const
    {
        IConfigData* pIFData = p()->GetData();
        return pIFData->GetSection(p(), name, attrName, attrValue,
            attrName2, attrValue2, autoCreate);
    }

    //! Returns a child node by node name and integer property value.
    /*!
        \return the child node (ConfigSection). The node will be created when not exist and autoCreate is true.
        \param name the child node name. the name may contains slash chars for multiple levels.
        \param attrName a field name of the child node.
        \param attrValue the field value of the child node.
        \param attrName2 another field name of the child node.
        \param attrValue2 the field value of attrName2.
        \param autoCreate true if enable to auto create new node.
    */
    AnyObject GetSection(const wchar_t* name,
        const wchar_t* attrName, ULONG attrValue,
        const wchar_t* attrName2, ULONG attrValue2,
        bool autoCreate = true)
    {
        IConfigData* pIFData = p()->GetData();
        return pIFData->GetSection(p(), name, attrName, attrValue,
            attrName2, attrValue2, autoCreate);
    }

    //! Returns the child count by child node name.
    /*!
        \param name the child node name. the name may contains slash chars for multiple levels.
        \return the child count.
        \see GetSectionByIndex
    */
    long GetSectionCount(const wchar_t* name) const
    {
        IConfigData* pIFData = p()->GetData();
        return pIFData->GetSectionCount(p(), name);
    }

    //! Returns the child node of the specified order.
    /*!
        \return the child node (ConfigSection) even not exist actually.\n
             I/O operations will be ignored when the internal object is not exist.\n
        \param name the child node name. the name may contains slash chars for multiple levels.
        \param index the child index (0 to GetSectionCount()).
        \see GetSectionCount, IConfigSection::IsValid().
    */
    AnyObject GetSectionByIndex(const wchar_t* name, long index) const
    {
        IConfigData* pIFData = p()->GetData();
        return pIFData->GetSectionByIndex(p(), name, index);
    }

    //! Add a child node.
    /*!
        \return the new node (ConfigSection).
        \param name the child node name. the name may contains slash chars for multiple levels.
    */
    AnyObject AddSection(const wchar_t* name)
    {
        IConfigData* pIFData = p()->GetData();
        return pIFData->AddSection(p(), name);
    }

    //! Removes all child nodes.
    /*!
        \param name the child node name that can not contains slash chars.
        \param attrName the optional field name of the child node.
        \param attrValue the field value valid if attrName is not empty.
        \return the count of nodes removed.
    */
    long RemoveChildren(const wchar_t* name, const wchar_t* attrName = L"", const wchar_t* attrValue = L"")
    {
        IConfigData* pIFData = p()->GetData();
        return pIFData->RemoveChildren(p(), name, attrName, attrValue);
    }

    //! Removes all child nodes.
    /*!
        \param name the child node name that can not contains slash chars.
        \param attrName the field name of the child node, not empty.
        \param attrValue the field value of attrName.
        \return the count of nodes removed.
    */
    long RemoveChildren(const wchar_t* name, const wchar_t* attrName, ULONG attrValue)
    {
        IConfigData* pIFData = p()->GetData();
        return pIFData->RemoveChildren(p(), name, attrName, attrValue);
    }

    //! Returns the parent node.
    /*!
        \return the parent node. The result node will be invalid
            (IConfigSection::IsValid()==false) if sec is root node.
    */
    AnyObject GetParentSection()
    {
        IConfigData* pIFData = p()->GetData();
        return pIFData->GetParentSection(p());
    }
};

END_NAMESPACE_X3
#endif // X3_XML_CONFIGIOSECTION_H_
