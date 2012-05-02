/*! \file IConfigTransaction.h
 *  \brief Define configure transaction helper class: ConfigTransaction
 *  \author Zhang Yungui, X3 C++ PluginFramework
 *  \date   2011.06.30
 */
#ifndef X3_XML_CONFIGTRANSACTION_H_
#define X3_XML_CONFIGTRANSACTION_H_

#include <objptr.h>

BEGIN_NAMESPACE_X3

//! Configure transaction interface.
/*! IConfigSection and IConfigData can be casted to this interface.
    \see ConfigTransaction
    \see IConfigSection, IConfigData
*/
class IConfigTransaction : public IObject
{
public:
    X3DEFINE_IID(IConfigTransaction);

    //! Begin to change configure data.
    virtual void BeginTransaction() = 0;

    //! End to change configure data.
    /*! The configure data will be saved when transaction counter becomes to zero.
        \return false if error occurred, otherwise returns true.
    */
    virtual bool EndTransaction() = 0;

    //! Return the total count of configure data changes.
    virtual ULONG GetModifiedCount() = 0;
};

//! Configure transaction helper class.
/*! Use this class to declare local variable in functions,
    so EndTransaction() will be automatic called to save configure data.
    \ingroup _GROUP_PLUGIN_XML_
    \see IConfigSection, IConfigData
*/
class ConfigTransaction
{
public:
    //! Begin to change configure data.
    ConfigTransaction(const AnyObject& obj) : m_trans(obj)
    {
        if (m_trans)
        {
            m_trans->BeginTransaction();
        }
    }

#ifdef X3_XML_ICONFIGDATA_H_
    //! Begin to change configure data.
    explicit ConfigTransaction(const Object<IConfigData>& p) : m_trans(p)
    {
        if (m_trans)
        {
            m_trans->BeginTransaction();
        }
    }
    //! Begin to change configure data.
    explicit ConfigTransaction(IConfigData* p) : m_trans(p)
    {
        if (m_trans)
        {
            m_trans->BeginTransaction();
        }
    }
#endif

    //! End to change configure data.
    ~ConfigTransaction()
    {
        Submit();
    }

    //! End to change configure data.
    bool Submit()
    {
        bool ret = false;

        if (m_trans)
        {
            ret = m_trans->EndTransaction();
            m_trans.release();
        }

        return ret;
    }

    //! Return true if the configure data has changed.
    bool IsModified() const
    {
        return m_trans && m_trans->GetModifiedCount() > 0;
    }

private:
    Object<IConfigTransaction>  m_trans;
};

END_NAMESPACE_X3
#endif // X3_XML_CONFIGTRANSACTION_H_
