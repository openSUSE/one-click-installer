#ifndef FAKEBACKEND_H
#define FAKEBACKEND_H

#include "packagebackend.h"
#include "repository.h"
#include <QList>
#include <QString>
#include <QWidget>
#include <QMessageBox>
#include <QUrl>


class FakeBackend : public PackageBackend
{
    public:
    /**
    	Default Constructor
     */
    FakeBackend( QWidget* main );

    /**
    	Fake implementation of the install function
     */
    void install();

    private:
    QWidget *m_main;
};

#endif
