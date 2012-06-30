#ifndef SUMMARY_H
#define SUMMARY_H

#include <QWidget>
#include <QTextBrowser>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QList>
#include "PackageBackend.h"

class Summary : public QWidget
{
public:
    Summary( PackageBackend *backend, QObject *parent = 0 );
private:
    QTextBrowser *m_installationSummary;
    QPushButton *m_continue;
    QPushButton *m_cancel;
    PackageBackend *m_backend;
};

#endif
