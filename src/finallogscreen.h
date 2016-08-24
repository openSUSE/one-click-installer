#ifndef FINAL_LOG_SCREEN
#define FINAL_LOG_SCREEN

#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QString>
#include <QFile>

class FinalLogScreen : public QWidget
{
  Q_OBJECT
public:
    FinalLogScreen();
public slots:
    void close();
    void processInstalledPackages( const QStringList &, bool );
signals:
    void finalLogHeader( int, bool, bool );
private:
    QGridLayout* installedWidget( const QString & );
    QLabel* repoWidget( const QString & );
    QPixmap image( bool );
    QHBoxLayout* packageStatus( bool );    
private:
    QVBoxLayout *m_mainLayout;
    QWidget *m_logWidget;
    QPushButton *m_close;
    QFile m_logFile;
    bool m_packageInstalled;
    unsigned m_numOfPackagesInstalled;
};
#endif
