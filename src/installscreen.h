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
#include <QStackedLayout>
#include <iostream>
#include "packagebackend.h"
#include "fakebackend.h"

class InstallScreen : public QWidget
{
    Q_OBJECT
public:
    InstallScreen( PackageBackend *backend, QStackedLayout *screenStack, QString *tmpFileName,  QObject *parent = 0 );
    void showEvent( QShowEvent *s );
private:
    QLabel *m_installStatus;
    QLabel *m_sources;

    QPushButton *m_cancel;
    PackageBackend *m_backend;

    QString *m_tmpFileName;

    QStackedLayout *m_screenStack;
};

#endif
