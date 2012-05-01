#ifndef FIRSTSCREEN_H
#define FIRSTSCREEN_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QComboBox>
#include <QHBoxLayout>

#include "settings.h"

class FirstScreen : public QWidget
{
	Q_OBJECT
	private:
		QLabel *m_warning;
		QPushButton *m_trust;
		QVBoxLayout *m_warningLayout;
		QVBoxLayout *m_installLayout;
		QHBoxLayout *m_buttonLayout;
		QVBoxLayout *m_mainLayout;
		QPushButton *m_settings;
		QPushButton *m_cancel;
		QPushButton *m_install;
	public:
		FirstScreen(QObject *parent = 0);
	private slots:
		void showSettings();

};


#endif
