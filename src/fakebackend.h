#ifndef FAKEBACKEND_H
#define FAKEBACKEND_H

#include <QList>
#include <QString>
#include <QWidget>
#include <QMessageBox>
#include <QDebug>
#include <QUrl>
#include "packagebackend.h"
#include "repository.h"

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

    /**
        Fake Call to the Backend Helper
     */
    void callBackendHelper();

    /**
        Fake Implementation of Exists function
     */
    bool exists( const QString& url );

    private:
    QWidget *m_main;
};

#endif
