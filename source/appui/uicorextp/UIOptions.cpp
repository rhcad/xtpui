#include "stdafx.h"
#include "UIOptions.h"
#include <xml/IConfigTransaction.h>
#include <xml/IConfigXml.h>

CUIOptions::CUIOptions()
{
}

CUIOptions::~CUIOptions()
{
}

void CUIOptions::setFileName(const wchar_t* filename)
{
    Object<IConfigXml> xmlfile(m_xmlfile);

    if (!xmlfile)
    {
        VERIFY(xmlfile.create(clsidXmlFile));
        m_xmlfile = xmlfile;
    }

    xmlfile->SetFileName(filename);
    m_root = xmlfile->GetData()->GetSection(L"");
    m_options = m_root.GetSection(L"options");
}

ConfigSection CUIOptions::getSection(const wchar_t* name)
{
    return name && *name ? ConfigSection(m_root.GetSection(name)) : m_options;
}

bool CUIOptions::save()
{
    ConfigTransaction autosave(m_xmlfile);
    return autosave.Submit();
}
