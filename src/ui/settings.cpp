#include "settings.h"

Settings::Settings(QObject *parent)
{
	//Create Layouts
	m_mainLayout = new QVBoxLayout;

	//Create Interface Elements
	m_repos = new QLabel("Repository Sources");
	m_subscribe = new QCheckBox("Subscribe to new Repository Sources automatically",this);
	m_trust = new QCheckBox("Trust and import repository public keys by default",this);
	m_close = new QPushButton("Close");

	//Add elements to Layout
	m_mainLayout->addWidget(m_repos);
	m_mainLayout->addWidget(m_subscribe);
	m_mainLayout->addWidget(m_trust);
	m_mainLayout->addWidget(m_close);
	
	this->setLayout(m_mainLayout);
	this->setWindowTitle("One Click Install Settings");
	this->show();
}
