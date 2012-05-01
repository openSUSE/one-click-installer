#include "firstscreen.h"

FirstScreen::FirstScreen(QObject *parent)
{
	m_warningLayout = new QVBoxLayout;
	m_warning = new QLabel("This is a warning Message");
	m_trust = new QPushButton("Trust");
	m_warningLayout->addWidget(m_warning);
	m_warningLayout->addWidget(m_trust);
	this->setLayout(m_warningLayout);
	this->show();
}
