#ifndef MAINWINDOW_H
#define MAINWINDIW_H

#include <QDialog>
#include <QLabel>
#include <QVBoxLayout>
#include <QList>
#include <QPixmap>
#include <QStackedLayout>
#include "mainheader.h"
#include "firstscreen.h"
#include "summary.h"
#include "installscreen.h"
#include "fakebackend.h"
#include "packagebackend.h"
#include "backend.h"

class MainWindow : public QDialog
{
public:
    /**
        Default Constructor taking the YMP file as argument
    */
    MainWindow( const QString& filename, QString tmpFileName, bool fakeRequested, QObject *parent = 0 );
private:
    PackageBackend *m_backend;
    FirstScreen *m_firstScreen;
    QString *m_tmpFileName;
    MainHeader *m_header;
    QStackedLayout *m_screenStack;

    bool m_fakeRequested;
};

#endif
