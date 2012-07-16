#include "settings.h"

Settings::Settings( QSettings* settings, QObject *parent )
{
    m_settings = settings;
    m_settings->sync();

    //Create Layouts
    QVBoxLayout *mainLayout = new QVBoxLayout;
    QHBoxLayout *closeLayout = new QHBoxLayout;

    //Create Interface Elements
    m_repos = new QLabel( "<b>Repository Sources</b>" );
    m_showDetails = new QCheckBox( "Show repository details by default", this );
    m_proposal = new QCheckBox( "Always display an installation proposal", this );
    m_close = new QPushButton( "Close" );

    if( m_settings->childKeys().count() != 0 ) {
        if( m_settings->value( "proposal", 1 ).toInt() == 1 ) {
            m_proposal->setChecked( true );
        } else {
            m_proposal->setChecked( false );
        }

        if ( m_settings->value( "showdetails", 1 ).toInt() ) {
            m_showDetails->setChecked( true );
        } else {
            m_showDetails->setChecked( false );
        }
    }

    //Add elements to Layout
    mainLayout->addWidget( m_repos );
    mainLayout->addWidget( m_showDetails );
    mainLayout->addWidget( m_proposal );
    closeLayout->addSpacing(150);
    closeLayout->addWidget( m_close );
    mainLayout->addLayout( closeLayout );
    setLayout( mainLayout );
    setWindowTitle( "One Click Install Settings" );

    QObject::connect( m_close, SIGNAL(clicked()), this, SLOT( saveSettingsAndClose()) );
    show();
}

void Settings::saveSettingsAndClose()
{
    if( m_proposal->isChecked() ) {
        m_settings->setValue( "proposal", 1 );
    } else {
        m_settings->setValue( "proposal", 0 );
    }

    if( m_showDetails->isChecked() ) {
        m_settings->setValue( "showdetails", 1 );
    } else {
        m_settings->setValue( "showdetails", 0 );
    }

    m_settings->sync();

    close();
}
