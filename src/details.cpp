#include "details.h"

Details::Details(PackageBackend *backend, int i, OCI::Repository *repo, QObject *parent )
{
    m_settings.sync();

    m_backend = backend;

    QVBoxLayout *mainLayout = new QVBoxLayout;
    QHBoxLayout *sourceLayout = new QHBoxLayout;
    mainLayout->setSpacing( 0 );

    m_name = new QLabel( QString( "Source: <b>%1</b>" ).arg( repo->name() ) );
    m_showDetails = new QLabel( QString( "<a href = %1>Show Details</a>" ).arg( i ) );

    m_name->setContentsMargins( 10, 10, 10, 10 );
    m_showDetails->setContentsMargins( 10, 10, 10, 10 );

    m_name->setStyleSheet( "background-color: rgb(254, 250, 210); border-bottom : 1px solid rgb(252,233,79); border-left : 1px solid rgb(196,181,147); border-top : 1px solid rgb(196,181,147);" );
    m_showDetails->setStyleSheet( "background-color: rgb(254, 250, 210); border-bottom : 1px solid rgb(252,233,79); border-right : 1px solid rgb(196,181,147); border-top : 1px solid rgb(196,181,147);" );

    sourceLayout->addWidget( m_name );
    sourceLayout->addWidget( m_showDetails );

    mainLayout->addLayout( sourceLayout );

    m_detailsVisible = false;

    QObject::connect( m_showDetails, SIGNAL( linkActivated( QString ) ), this, SLOT( showDetails(QString ) ) );

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

    setLayout( mainLayout );

    if( m_settings.value( "showdetails", 1 ).toInt() == 1 ) {
        showDetails( QString( "%1" ).arg( i ) );
    }
}

void Details::showDetails( QString link )
{
    int linkNo = link.toInt();
    if( m_detailsVisible ) {
        m_url->hide();
        m_fingerprint->hide();
        m_summary->hide();
        m_created->hide();
        m_expires->hide();
        m_id->hide();

        m_detailsVisible = false;

        m_showDetails->setText( QString( "<a href = %1>Show Details</a>" ).arg( linkNo ) );
    } else {
        this->layout()->addWidget( m_summary );
        this->layout()->addWidget( m_url );
        this->layout()->addWidget( m_id );
        this->layout()->addWidget( m_fingerprint );
        this->layout()->addWidget( m_created );
        this->layout()->addWidget( m_expires );

        m_summary->show();
        m_url->show();
        m_fingerprint->show();
        m_created->show();
        m_expires->show();

        m_detailsVisible = true;

        m_showDetails->setText( QString( "<a href = %1>Hide Details</a>" ).arg( linkNo ) );
    }
}
