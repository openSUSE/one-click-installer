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
#include "installscreen.h"

class MainWindow;

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
                QLabel *m_moreDetails;
                QWidget *m_stageWidget;
                bool m_detailsVisible;

                public:
		/**
			Default constructor taking the backend, and the filename as argument
		 */
                FirstScreen( PackageBackend *backend, QWidget* stageWidget, const QString& filename, QObject *parent = 0 );
	private slots:
		/**
		 	Shows the Settings dialog
		 */
		void showSettings();

		/**
		 	Triggers the installation when the Install software button is clicked
		 */
		void performInstallation();

                /**
                        Trusts the repositories, and enables the Install Button
                */
                void trust();

                /**
                        More Details on untrusted repositories
                */
                void untrusedRepoDetails( QString link);
};


#endif
