// x3c - C++ PluginFramework
#ifndef _X3_CONFIGXML_XMLSECTION_H
#define _X3_CONFIGXML_XMLSECTION_H

#include <xml/IConfigSection.h>
#include <xml/IConfigSectionXml.h>
#include <xml/IConfigTransaction.h>

#ifdef _MSC_VER
#include "XmlUtil/XmlUtil.h"
#else
#include "TinyXml/XmlUtil.h"
#endif

BEGIN_NAMESPACE_X3

struct ConfigXmlImpl;

const char* const clsidXmlSection = "04875843-8a6d-43d5-af58-e5e588f04b92";

class Cx_XmlSection
    : public IConfigSection
    , public IConfigSectionXml
    , public IConfigTransaction
{
    X3BEGIN_CLASS_DECLARE(Cx_XmlSection, clsidXmlSection)
        X3DEFINE_INTERFACE_ENTRY(IConfigSection)
        X3DEFINE_INTERFACE_ENTRY(IConfigSectionXml)
        X3DEFINE_INTERFACE_ENTRY(IConfigTransaction)
    X3END_CLASS_DECLARE()
protected:
    Cx_XmlSection();
    virtual ~Cx_XmlSection();

public:
    ConfigXmlImpl*      m_pData;
    XMLDOMElementPtr    m_xmlParent;
    XMLDOMElementPtr    m_xmlNode;
    bool                m_bSubElement;

private:
    // From IConfigSectionXml
    //
    virtual void UseSubElement(bool element);
    virtual std::wstring GetText();
    virtual bool SetText(const wchar_t* value, bool cdata = false);
#ifdef _OLE2_H_
    virtual IUnknown* GetDOMElement(bool addRef = false);
    virtual IUnknown* GetDOMDocument(bool addRef = false);
#endif

    // From IConfigTransaction
    //
    virtual void BeginTransaction();
    virtual bool EndTransaction();
    virtual ULONG GetModifiedCount();

    // From IConfigSection
    //
    virtual IConfigData* GetData() const;
    virtual bool IsValid() const;
    virtual std::wstring GetString(const wchar_t* name, const wchar_t* defValue = L"");
    virtual bool SetString(const wchar_t* name, const wchar_t* value);
    virtual int GetInt(const wchar_t* name, int defValue = 0);
    virtual bool SetInt(const wchar_t* name, int value);
    virtual long GetInt32(const wchar_t* name, long defValue = 0);
    virtual bool SetInt32(const wchar_t* name, long value);
    virtual ULONG GetUInt32(const wchar_t* name, ULONG defValue = 0);
    virtual bool SetUInt32(const wchar_t* name, ULONG value);
    virtual short GetInt16(const wchar_t* name, short defValue = 0);
    virtual bool SetInt16(const wchar_t* name, short value);
    virtual WORD GetUInt16(const wchar_t* name, WORD defValue = 0);
    virtual bool SetUInt16(const wchar_t* name, WORD value);
    virtual bool GetBool(const wchar_t* name, int defValue);
    virtual bool SetBool(const wchar_t* name, int value);
    virtual double GetDouble(const wchar_t* name, double defValue = 0);
    virtual bool SetDouble(const wchar_t* name, double value);
    virtual float GetFloat(const wchar_t* name, float defValue = 0);
    virtual bool SetFloat(const wchar_t* name, float value);
    virtual DWORD GetRGB(const wchar_t* name, DWORD defValue);
    virtual bool SetRGB(const wchar_t* name, DWORD value);
    virtual bool GetCMYK(const wchar_t* name, WORD& c, WORD& m, WORD& y, WORD& k);
    virtual bool SetCMYK(const wchar_t* name, WORD c, WORD m, WORD y, WORD k);
    virtual bool GetDate(const wchar_t* name, int& year, int& month, int& day);
    virtual bool SetDate(const wchar_t* name, int year, int month, int day);
    virtual bool GetDateTime(const wchar_t* name, int& year, int& month, int& day,
        int& hour, int& minute, int& second);
    virtual bool SetDateTime(const wchar_t* name, int year, int month, int day,
        int hour, int minute, int second);
    virtual long GetDoubleArray(const wchar_t* name, double* items, long count);
    virtual bool SetDoubleArray(const wchar_t* name, const double* items, long count);
    virtual long GetIntArray(const wchar_t* name, long* items, long count);
    virtual bool SetIntArray(const wchar_t* name, const long* items, long count);
    virtual long GetBinary(const wchar_t* name, void* buf, long size);
    virtual bool SetBinary(const wchar_t* name, const void* buf, long size);

private:
    Cx_XmlSection(const Cx_XmlSection&);
    void operator=(const Cx_XmlSection&);
};

END_NAMESPACE_X3
#endif // _X3_CONFIGXML_XMLSECTION_H
