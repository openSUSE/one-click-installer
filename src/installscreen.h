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
private slots:
    void cancelInstallation();
    void newResolvableInAction( QString );
    void newProgressInAction( QString );
    void updateCurrentResolvableStatusUponCompletion( QString, bool );
    void updateCurrentProgressStatusUponCompletion( QString, bool );
private:
    QWidget* horizontalLine();
    QLabel* loadingAnimation(); 
private:
    QVBoxLayout *m_mainLayout;
    QProgressBar *m_progressBar;
    QPushButton *m_cancelButton;
    QLabel *m_currentPackageStatusLabel;
    QTextBrowser *m_statusWidget;
    QFile m_logFile;
    QTextStream *m_outData;
    
    /* OCIHelper proxies */
    org::opensuse::OCIHelper *m_ociHelper;
    org::opensuse::MediaCallbacks *m_mediaCallbacks;
};
#endif
