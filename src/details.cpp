#include "details.h"

Details::Details( PackageBackend *backend, QObject *parent )
{
    m_backend = backend;
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setSpacing( 0 );
    m_url = new QLabel( "<b>URL :</b> " );
    m_id = new QLabel( "<b>ID: </b>" );
    m_fingerprint = new QLabel( "<b>Fingerprint: </b>" );
    m_created = new QLabel( "<b>Created: </b>" );
    m_expires = new QLabel( "<b>Expires : </b> ");
    m_url->setStyleSheet( "background-color: rgb(255, 221, 139);" );
    m_id->setStyleSheet( "background-color: rgb(255, 221, 139);" );
    m_fingerprint->setStyleSheet( "background-color: rgb(255, 221, 139)" );
    m_created->setStyleSheet( "background-color: rgb(255, 221, 139)" );
    m_expires->setStyleSheet( "background-color: rgb(255, 221, 139)" );
    mainLayout->addWidget( m_url );
    mainLayout->addWidget( m_id );
    mainLayout->addWidget( m_fingerprint );
    mainLayout->addWidget( m_created );
    mainLayout->addWidget( m_expires );
    setLayout( mainLayout );
    show();
}
