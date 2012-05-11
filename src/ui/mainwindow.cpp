#include "mainwindow.h"

MainWindow::MainWindow(QObject *parent)
{
	m_info = new QLabel("This installer will install and download packages");
	QVBoxLayout *mainLayout = new QVBoxLayout;
	m_stageWidget = new FirstScreen;
	mainLayout->addWidget(m_info);
	mainLayout->addWidget(m_stageWidget);
	setLayout(mainLayout);
	setWindowTitle("One Click Install");
	show();
}
