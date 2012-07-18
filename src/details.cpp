#include "details.h"

Details::Details(PackageBackend *backend, OCI::Repository *repo, QObject *parent )
{
    m_backend = backend;
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setSpacing( 0 );

    m_url = new QLabel( QString( "<b>URL :</b> %1" ).arg( repo->url() ) );
    m_summary = new QLabel( QString( "<b>Summary :</b> %1" ).arg( repo->summary() ) );
    m_id = new QLabel( "<b>ID: </b>" );
    m_fingerprint = new QLabel( "<b>Fingerprint: </b>" );
    m_created = new QLabel( "<b>Created: </b>" );
    m_expires = new QLabel( "<b>Expires : </b> ");
    m_url->setStyleSheet( "background-color: rgb(254, 250, 210); padding-left : 10px; padding-top : 10px; padding-bottom : 10px;" );
    m_id->setStyleSheet( "background-color: rgb(254, 250, 210); padding-left : 10px; padding-top : 10px; padding-bottom : 10px;" );
    m_fingerprint->setStyleSheet( "background-color: rgb(254, 250, 210); padding-left : 10px; padding-top : 10px; padding-bottom : 10px;" );
    m_created->setStyleSheet( "background-color: rgb(254, 250, 210); padding-left : 10px; padding-top : 10px; padding-bottom : 10px;" );
    m_expires->setStyleSheet( "background-color: rgb(254, 250, 210); padding-left : 10px; padding-top : 10px; padding-bottom : 10px;" );
    m_summary->setStyleSheet( "background-color: rgb(254, 250, 210); padding-left : 10px; padding-top : 10px; padding-bottom : 10px;" );

    mainLayout->addWidget( m_summary );
    mainLayout->addWidget( m_url );
    mainLayout->addWidget( m_id );
    mainLayout->addWidget( m_fingerprint );
    mainLayout->addWidget( m_created );
    mainLayout->addWidget( m_expires );
    setLayout( mainLayout );
}
