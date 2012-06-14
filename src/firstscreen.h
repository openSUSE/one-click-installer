#ifndef FIRSTSCREEN_H
#define FIRSTSCREEN_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QCheckBox>
#include <QHBoxLayout>

#include "FakeBackend.h"
#include "PackageBackend.h"
#include "YmpParser.h"
#include "settings.h"
#include "package.h"
#include "repository.h"

class FirstScreen : public QWidget
{
	Q_OBJECT
	private:
		QLabel *m_warning;
		QPushButton *m_trust;
		QPushButton *m_settings;
		QPushButton *m_cancel;
		QPushButton *m_install;
		PackageBackend *m_backend;
	public:
		FirstScreen( PackageBackend *backend , const QString& filename, QObject *parent = 0 );
	private slots:
		void showSettings();
		void performInstallation();
};


#endif
