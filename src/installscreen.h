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
#include <QTimer>
#include <iostream>
#include <QHash>
#include <QFileSystemWatcher>
#include <QFile>
#include "packagebackend.h"
#include "fakebackend.h"

class InstallScreen : public QWidget
{
    Q_OBJECT
public:

    /**
     *  Default Constructor
     */
    InstallScreen( PackageBackend *backend, QString *tmpFileName,  QObject *parent = 0 );

private slots:
    /**
        Hide Progressbars and show status for each package
    */
    void showCompletionStatus();

    void logFileChanged( QString path );

private:
    QLabel *m_installStatus;
    QLabel *m_sources;

    QPushButton *m_cancel;
    PackageBackend *m_backend;

    QString *m_tmpFileName;

    QHash< int, QHBoxLayout* > m_packageLayouts;
    QProgressBar *m_progressBar;

    QFileSystemWatcher *m_watcher;

signals:
    void installationCompleted();
};

#endif
