// x3c - C++ PluginFramework
#ifndef _X3_CONFIGXML_CONFIGXML_H
#define _X3_CONFIGXML_CONFIGXML_H

#include <xml/IConfigData.h>
#include <xml/IConfigXml.h>
#include <xml/IConfigTransaction.h>

BEGIN_NAMESPACE_X3

struct ConfigXmlImpl;

class Cx_ConfigXml
    : public IConfigData
    , public IConfigXml
    , public IConfigTransaction
{
    X3BEGIN_CLASS_DECLARE(Cx_ConfigXml, clsidXmlFile)
        X3DEFINE_INTERFACE_ENTRY(IConfigData)
        X3DEFINE_INTERFACE_ENTRY(IConfigXml)
        X3DEFINE_INTERFACE_ENTRY(IConfigTransaction)
    X3END_CLASS_DECLARE()
protected:
    Cx_ConfigXml();
    virtual ~Cx_ConfigXml();

private:
    // From IConfigXml
    //
    virtual IConfigData* GetData();
    virtual std::wstring GetFileName() const;
    virtual void SetFileName(const wchar_t* filename);
    virtual std::wstring GetRootName() const;
    virtual void SetRootName(const wchar_t* rootName = L"root",
        const wchar_t* encoding = L"UTF-8",
        const wchar_t* nmspace = L"");
    virtual void SetSchemaLocation(const wchar_t* location);
    virtual bool Reload();
    virtual bool Save(const wchar_t* filename = L"") const;
    virtual bool GetXmlContent(std::wstring& content) const;
    virtual bool SetXmlContent(const std::wstring& content);
    virtual bool GetXmlContent(std::wstring& content, IConfigSection* node) const;
    virtual IFileCryptHandler* SetCryptHandler(IFileCryptHandler* handler);

    // From IConfigTransaction
    //
    virtual void BeginTransaction();
    virtual bool EndTransaction();
    virtual ULONG GetModifiedCount();

    // From IConfigData
    //
    virtual AnyObject GetSection(
        const wchar_t* name, bool autoCreate = true);
    virtual AnyObject GetSection(
        IConfigSection* parent, const wchar_t* name,
        const wchar_t* attrName, ULONG attrValue,
        bool autoCreate = true);
    virtual AnyObject GetSection(
        IConfigSection* parent, const wchar_t* name,
        const wchar_t* attrName, const wchar_t* attrValue,
        bool autoCreate = true);
    virtual AnyObject GetSection(
        IConfigSection* parent, const wchar_t* name,
        const wchar_t* attrName, const wchar_t* attrValue,
        const wchar_t* attrName2, const wchar_t* attrValue2,
        bool autoCreate = true);
    virtual AnyObject GetSection(
        IConfigSection* parent, const wchar_t* name,
        const wchar_t* attrName, ULONG attrValue,
        const wchar_t* attrName2, ULONG attrValue2,
        bool autoCreate = true);
    virtual long GetSectionCount(
        IConfigSection* parent, const wchar_t* name);
    virtual AnyObject GetSectionByIndex(
        IConfigSection* parent, const wchar_t* name, long index);
    virtual AnyObject AddSection(
        IConfigSection* parent, const wchar_t* name);
    virtual bool RemoveSection(IConfigSection* sec);
    virtual long RemoveChildren(IConfigSection* parent, const wchar_t* name,
        const wchar_t* attrName = L"", const wchar_t* attrValue = L"");
    virtual long RemoveChildren(IConfigSection* parent, const wchar_t* name,
        const wchar_t* attrName, ULONG attrValue);
    virtual AnyObject GetParentSection(IConfigSection* sec);

private:
    Cx_ConfigXml(const Cx_ConfigXml&);
    void operator=(const Cx_ConfigXml&);

    ConfigXmlImpl*  m_pImpl;
};

END_NAMESPACE_X3
#endif // _X3_CONFIGXML_CONFIGXML_H
