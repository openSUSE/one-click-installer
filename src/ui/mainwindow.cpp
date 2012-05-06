#include "mainwindow.h"

MainWindow::MainWindow(QObject *parent)
{
	m_info = new QLabel("This installer will install and download packages");
	m_layout = new QVBoxLayout;
	m_stageWidget = new FirstScreen;
	m_layout->addWidget(m_info);
	m_layout->addWidget(m_stageWidget);
	this->setLayout(m_layout);
	this->setWindowTitle("One Click Install");
	this->show();
}
