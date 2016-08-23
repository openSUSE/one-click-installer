#ifndef SUMMARY_H
#define SUMMARY_H

#include <QWidget>
#include <QTextBrowser>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QList>
#include <QScrollBar>
#include <cstdlib>
#include "packagebackend.h"
#include "installscreen.h"

class Summary : public QWidget
{
    Q_OBJECT
public:

    /**
     * Construct the object with the filename and backend object
     */
    Summary( PackageBackend *backend, const QString & tmpFileName, QObject *parent = 0 );

private:
    QTextBrowser *m_installationSummary;
    QPushButton *m_continue;
    QPushButton *m_cancel;
    PackageBackend *m_backend;

    QString m_tmpFileName;

private slots:

    /**
     * Show the install screen
     */
    void continueInstallation();

    /**
     * Close the application
     */
    void cancel();

signals:
    void showNextScreen( int );
};

#endif
