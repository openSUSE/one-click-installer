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
protected:

    /**
     *  Default Show Event. To start the timer
     */
    void showEvent( QShowEvent *s );

private slots:

    /**
     * Invoke Backend Helper
     */
    void callBackend();

private:
    QLabel *m_installStatus;
    QLabel *m_sources;

    QPushButton *m_cancel;
    PackageBackend *m_backend;

    QString *m_tmpFileName;
};

#endif
