#include "package.h"

OCI::Package::Package()
{
}

QString OCI::Package::name() const
{
    return m_name;
}

QString OCI::Package::summary() const
{
    return m_summary;
}

QString OCI::Package::description() const
{
    return m_description;
}

void OCI::Package::setName( const QString& name )
{
    m_name = name;
}

void OCI::Package::setSummary( const QString& summary )
{
    m_summary = summary;
}

void OCI::Package::setDescription( const QString& description )
{
    m_description = description;
}

void OCI::Package::installPackage( const zypp::ZYpp::Ptr& zypp_pointer )
{
    bool rres;
    m_status = false;
    zypp::ui::Selectable::Ptr s = zypp::ui::Selectable::get( this->name().toStdString() );
    if ( !s ) {
        qDebug()<<"Not initialised";
        return;
    }
    zypp::PoolItem p = s->highestAvailableVersionObj();
    s->setCandidate( p );
    zypp::ResPool pool = zypp::ResPool::instance();
    p.status().setToBeInstalled( zypp::ResStatus::USER );
    rres = zypp::ResPool::instance().resolver().resolvePool();
    if ( !rres ) {
        qDebug()<<"Failed to Resolve Pool";
        return;
    } else {
        qDebug()<<"Pool Resolved";
    }
    zypp::ZYppCommitPolicy policy;
    policy.restrictToMedia( 0 );
    policy.downloadMode( zypp::DownloadInHeaps );
    policy.syncPoolAfterCommit( true );
    zypp::ZYppCommitResult result = zypp_pointer->commit( policy );
    if ( result.allDone() ) {
        qDebug()<<"Installation Completed";
        m_status = true;
    } else {
        qDebug()<<"Installation Failed";
    }
}

bool OCI::Package::isInstalled()
{
    sreturn m_status;
}
