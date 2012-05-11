#include "settings.h"

Settings::Settings(QObject *parent)
{
	//Create Layouts
	QVBoxLayout *mainLayout = new QVBoxLayout;

	//Create Interface Elements
	m_repos = new QLabel("Repository Sources");
	m_subscribe = new QCheckBox("Subscribe to new Repository Sources automatically",this);
	m_trust = new QCheckBox("Trust and import repository public keys by default",this);
	m_close = new QPushButton("Close");

	//Add elements to Layout
	mainLayout->addWidget(m_repos);
	mainLayout->addWidget(m_subscribe);
	mainLayout->addWidget(m_trust);
	mainLayout->addWidget(m_close);
	
	setLayout(mainLayout);
	setWindowTitle("One Click Install Settings");
	show();
}
