#ifndef INSTALLSCREEN_H
#define INSTALLSCREEN_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QProgressBar>
#include <QList>
#include <QUrl>
#include <QDebug>
#include "PackageBackend.h"
#include "FakeBackend.h"

class InstallScreen : public QWidget
{
	Q_OBJECT
	public:
		InstallScreen( PackageBackend *backend,  QObject *parent = 0 );
	private:
		QLabel *m_installStatus;
		QLabel *m_sources;
		QPushButton *m_cancel;
		PackageBackend *m_backend;
};

#endif
