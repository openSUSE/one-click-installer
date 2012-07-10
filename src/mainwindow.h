#ifndef MAINWINDOW_H
#define MAINWINDIW_H

#include <QDialog>
#include <QLabel>
#include <QVBoxLayout>
#include <QList>
#include <QPixmap>
#include "firstscreen.h"
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
    QLabel *m_info;
    QWidget *m_stageWidget;	//This will load the corresponding widget to the layout depending on the stage of the installation
    PackageBackend *m_backend;
    FirstScreen *m_firstScreen;
    QString *m_tmpFileName;
};

#endif
