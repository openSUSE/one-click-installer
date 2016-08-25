#ifndef INSTALLSCREEN_H
#define INSTALLSCREEN_H

#include <QApplication>
#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>
#include <QProgressBar>
#include <QMovie>
#include <QDebug>
#include <QTextBrowser>
#include <QFile>
#include <QTextStream>
#include "oci_helper_interface.h"
#include "media_callbacks_interface.h"

class InstallScreen : public QWidget
{
    Q_OBJECT
public:
    /**
     *  Default Constructor
     */
    InstallScreen();
public slots:
    void initDBusServices();
    void closeLogFile();
    void setTotalProgressMax( int );
private slots:
    void cancelInstallation();
    void newResolvableInAction( const QString & );
    void newProgressInAction( const QString & );
    void updateCurrentResolvableStatusUponCompletion( const QString &, bool );
    void updateCurrentProgressStatusUponCompletion( const QString &, bool );
private:
    QWidget* horizontalLine();
    QLabel* loadingAnimation(); 
private:
    QVBoxLayout *m_mainLayout;
    QProgressBar *m_progressBar;
    QProgressBar *m_totalProgressBar;
    QPushButton *m_cancelButton;
    QLabel *m_currentPackageStatusLabel;
    QLabel *m_totProgBarStatusLabel;
    QTextBrowser *m_statusWidget;
    QFile m_logFile;
    QTextStream *m_outData;
    unsigned m_currentPackage;
    
    /* OCIHelper proxies */
    org::opensuse::OCIHelper *m_ociHelper;
    org::opensuse::MediaCallbacks *m_mediaCallbacks;
};
#endif
