#include "firstscreen.h"

FirstScreen::FirstScreen(QObject *parent)
{
	//Create Layouts
	m_warningLayout = new QVBoxLayout;
	m_installLayout = new QVBoxLayout;
	m_buttonLayout = new QHBoxLayout;
	m_mainLayout = new QVBoxLayout;

	//Create Interface Elemenets
	m_warning = new	QLabel("This is a warning Message");	//This should be done only if repositories to be added need to be trusted
	m_trust = new QPushButton("Trust");		// Same as above

	
	m_settings = new QPushButton("Settings");
	m_cancel = new QPushButton("Cancel");
	m_install = new QPushButton("Install");

	//Add Elements to corresponding Layouts;
	m_warningLayout->addWidget(m_warning);
	m_warningLayout->addWidget(m_trust);
	m_buttonLayout->addWidget(m_settings);
	m_buttonLayout->addWidget(m_cancel);
	m_buttonLayout->addWidget(m_install);

	m_mainLayout->addLayout(m_warningLayout);
	m_mainLayout->addLayout(m_installLayout);
	m_mainLayout->addLayout(m_buttonLayout);

	this->setLayout(m_mainLayout);
	this->show();
}
