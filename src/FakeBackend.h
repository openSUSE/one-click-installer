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
    void install();

    private:
    QWidget *m_main;
};

#endif
