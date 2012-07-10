#ifndef SUMMARY_H
#define SUMMARY_H

#include <QWidget>
#include <QTextBrowser>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QList>
#include <QProcess>
#include <QScrollBar>
#include <cstdlib>
#include "packagebackend.h"
#include "installscreen.h"

class Summary : public QWidget
{
    Q_OBJECT
public:
    Summary( PackageBackend *backend, bool fakeRequested, QString *tmpFileName, QWidget *stageWidget, QObject *parent = 0 );
private:
    QTextBrowser *m_installationSummary;
    QPushButton *m_continue;
    QPushButton *m_cancel;
    PackageBackend *m_backend;
    QWidget *m_stageWidget;
    QProcess backendProcess;

    bool m_fakeRequested;

    QString *m_tmpFileName;

private slots:
    void continueInstallation();
    void cancel();
};

#endif
