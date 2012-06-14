#ifndef FAKEBACKEND_H
#define FAKEBACKEND_H

#include "PackageBackend.h"
#include "repository.h"
#include <QList>
#include <QString>
#include <QWidget>
#include <QMessageBox>
#include <QUrl>


class FakeBackend : public PackageBackend
{
    public:
    FakeBackend( QWidget* main );
    virtual void setToAddRepository( const QUrl& repo  );
    virtual void performInstallation( const QString& pack );
    virtual bool resolveConflicts();
    void installPackages();
    void addRepositories();
    bool m_allTrusted;

    private:
    QWidget *m_main;
    QList< QString > m_repoList;
    QList< QString > m_packageList;
};

#endif
