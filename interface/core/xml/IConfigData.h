/*! \file IConfigData.h
 *  \brief Define the configure data interface: IConfigData
 *  \author Zhang Yungui, X3 C++ PluginFramework
 *  \date   2011.06.30
 */
#ifndef X3_XML_ICONFIGDATA_H_
#define X3_XML_ICONFIGDATA_H_

#include "IConfigSection.h"

BEGIN_NAMESPACE_X3

//! The configure data interface.
/*! This interface object is represented as all data nodes as hierarchical structure.\n
    ConfigSection object is represented as one node.\n
    This interface can be casted to IConfigTransaction so can used to save immediately.
    \interface IConfigData
    \ingroup _GROUP_PLUGIN_XML_
    \see ConfigTransaction, ConfigSection
*/
class IConfigData : public IObject
{
public:
    X3DEFINE_IID(IConfigData);

    //! Get a child node of the root node.
    /*!
        \return the child node (ConfigSection). The node will be created when not exist and autoCreate is true.
        \param name The child node name.\n
            The name may contains slash chars for multiple levels.\n
            Empty string is represented as getting the root node.
        \param autoCreate true if enable to auto create new node.
    */
    virtual AnyObject GetSection(
        const wchar_t* name, bool autoCreate = true) = 0;
    
    //! Returns a child node by node name and integer property value.
    /*!
        \return the child node (ConfigSection). The node will be created when not exist and autoCreate is true.
        \param parent the parent node. Empty string is represented as the root node.
        \param name the child node name. the name may contains slash chars for multiple levels.
        \param attrName a field name of the child node.
        \param attrValue the field value of the child node.
        \param autoCreate true if enable to auto create new node.
    */
    virtual AnyObject GetSection(
        IConfigSection* parent, const wchar_t* name, 
        const wchar_t* attrName, ULONG attrValue, 
        bool autoCreate = true) = 0;

    //! Returns a child node by node name and string property value.
    /*!
        \return the child node (ConfigSection). The node will be created when not exist and autoCreate is true.
        \param parent the parent node. Empty string is represented as the root node.
        \param name the child node name. the name may contains slash chars for multiple levels.
        \param attrName a field name of the child node.
        \param attrValue the field value of the child node.
        \param autoCreate true if enable to auto create new node.
    */
    virtual AnyObject GetSection(
        IConfigSection* parent, const wchar_t* name, 
        const wchar_t* attrName, const wchar_t* attrValue, 
        bool autoCreate = true) = 0;

    //! Returns a child node by node name and string property value.
    /*!
        \return the child node (ConfigSection). The node will be created when not exist and autoCreate is true.
        \param parent the parent node. Empty string is represented as the root node.
        \param name the child node name. the name may contains slash chars for multiple levels.
        \param attrName a field name of the child node.
        \param attrValue the field value of the child node.
        \param attrName2 another field name of the child node.
        \param attrValue2 the field value of attrName2.
        \param autoCreate true if enable to auto create new node.
    */
    virtual AnyObject GetSection(
        IConfigSection* parent, const wchar_t* name, 
        const wchar_t* attrName, const wchar_t* attrValue, 
        const wchar_t* attrName2, const wchar_t* attrValue2, 
        bool autoCreate = true) = 0;

    //! Returns a child node by node name and integer property value.
    /*!
        \return the child node (ConfigSection). The node will be created when not exist and autoCreate is true.
        \param parent the parent node. Empty string is represented as the root node.
        \param name the child node name. the name may contains slash chars for multiple levels.
        \param attrName a field name of the child node.
        \param attrValue the field value of the child node.
        \param attrName2 another field name of the child node.
        \param attrValue2 the field value of attrName2.
        \param autoCreate true if enable to auto create new node.
    */
    virtual AnyObject GetSection(
        IConfigSection* parent, const wchar_t* name, 
        const wchar_t* attrName, ULONG attrValue, 
        const wchar_t* attrName2, ULONG attrValue2, 
        bool autoCreate = true) = 0;

    //! Returns the child count by child node name.
    /*!
        \param parent the parent node. Empty string is represented as the root node.
        \param name the child node name. the name may contains slash chars for multiple levels.
        \return the child count.
        \see GetSectionByIndex
    */
    virtual long GetSectionCount(
        IConfigSection* parent, const wchar_t* name) = 0;

    //! Returns the child node of the specified order.
    /*!
        \return the child node (ConfigSection) even not exist actually.\n
             I/O operations will be ignored when the internal object is not exist.\n
        \param parent the parent node. Empty string is represented as the root node.
        \param name the child node name. the name may contains slash chars for multiple levels.
        \param index the child index (0 to GetSectionCount()).
        \see GetSectionCount, IConfigSection::IsValid().
    */
    virtual AnyObject GetSectionByIndex(
        IConfigSection* parent, const wchar_t* name, long index) = 0;
    
    //! Add a child node.
    /*!
        \return the new node (ConfigSection).
        \param parent the parent node. Empty string is represented as the root node.
        \param name the child node name. the name may contains slash chars for multiple levels.
    */
    virtual AnyObject AddSection(
        IConfigSection* parent, const wchar_t* name) = 0;

    //! Removes a child nodes.
    /*!
        \param sec the node whose internal object will be free.
        \return true if successful.
    */
    virtual bool RemoveSection(IConfigSection* sec) = 0;
    
    //! Removes all child nodes of the specified node.
    /*!
        \param parent the specified parent node.
        \param name the child node name that can not contains slash chars.
        \param attrName the optional field name of the child node.
        \param attrValue the field value valid if attrName is not empty.
        \return the count of nodes removed.
    */
    virtual long RemoveChildren(IConfigSection* parent, const wchar_t* name, 
        const wchar_t* attrName = L"", const wchar_t* attrValue = L"") = 0;

    //! Removes all child nodes of the specified node.
    /*!
        \param parent the specified parent node.
        \param name the child node name that can not contains slash chars.
        \param attrName the field name of the child node, not empty.
        \param attrValue the field value of attrName.
        \return the count of nodes removed.
    */
    virtual long RemoveChildren(IConfigSection* parent, const wchar_t* name, 
        const wchar_t* attrName, ULONG attrValue) = 0;

    //! Returns the parent node of the specified node.
    /*!
        \return the parent node. The result node will be invalid
            (IConfigSection::IsValid()==false) if sec is root node.
        \param sec the specified node.
    */
    virtual AnyObject GetParentSection(IConfigSection* sec) = 0;
};

END_NAMESPACE_X3
#endif // X3_XML_ICONFIGDATA_H_
