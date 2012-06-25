#include "settings.h"

Settings::Settings( QObject *parent )
{
    //Create Layouts
    QVBoxLayout *mainLayout = new QVBoxLayout;
    QHBoxLayout *closeLayout = new QHBoxLayout;
    //Create Interface Elements
    m_repos = new QLabel( "<b>Repository Sources</b>" );
    m_subscribe = new QCheckBox( "Subscribe to new Repository Sources automatically", this );
    m_trust = new QCheckBox( "Trust and import repository public keys by default", this );
    m_close = new QPushButton( "Close" );

    //Add elements to Layout
    mainLayout->addWidget( m_repos );
    mainLayout->addWidget( m_subscribe );
    mainLayout->addWidget( m_trust );
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
    /*
      Add Code here to save settings
     */
    close();
}
