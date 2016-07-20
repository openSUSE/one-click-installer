#ifndef HEADER_H
#define HEADER_H

#include <QWidget>
#include <QHBoxLayout>
#include <QPixmap>
#include <QWidget>
#include <QLabel>
#include <QDebug>

class MainHeader : public QWidget
{
    Q_OBJECT
public:

    /**
     *  Default Constructor for Header
     */
    MainHeader( QWidget *parent = 0 );

    /**
     *  Update the size of the packages
     */
    void updateDetails( const QString& size );

public slots:

    /**
     *  Recieve the Repository and Package Counts, and set them in the respective labels
     */
    void changeStatusLabel( int repoCount, int packageCount );

    /**
     * Show installation status in header
     */
    void installationStarted();

    /**
     * Show success/failure message in header
     */
    void installationCompleted();
    
    /**
     * Show Check Conflict header
     */
    void showCheckForConflictsHeader();

private:
    QLabel *m_statusLabel;
    QLabel *m_icon;
    float m_totalSize;
    int m_repoCount;
    int m_packageCount;
};

#endif
