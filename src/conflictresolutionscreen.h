#ifndef CONFLICTRESOLUTIONSCREEN_H
#define CONFLICTRESOLUTIONSCREEN_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include "oci_helper_interface.h"

class ConflictResolutionScreen : public QWidget
{
    Q_OBJECT
    Q_CLASSINFO( "D-Bus Interface", "org.opensuse.oneclickinstaller" )
    
public:
    ConflictResolutionScreen();
    
private:
    QVBoxLayout *m_mainLayout;
    QPushButton *m_cancelInstallation;
    QPushButton *m_continueInstallation;
    QWidget *m_solutionWidget;
    org::opensuse::OCIHelper *m_ociHelper;
    
private slots:
    void problemSolutionWidget( QString problem, QString solutions );
    void cancelInstallation();
    void sendSolutionToOCIHelper();

Q_SIGNALS:
    void solutionNumber( int );
};

#endif
