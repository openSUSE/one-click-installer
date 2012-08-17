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
     * @brief updateDetails : update package size
     * @param size : size of package
     */
    void updateDetails( QString size );

public slots:

    /**
     * @brief changeStatusLabel : update repo and package counts
     * @param repoCount : repository count
     * @param packageCount : package counts
     */
    void changeStatusLabel( int repoCount, int packageCount );

private:
    QLabel *m_statusLabel;
    QLabel *m_icon;

    int m_repoCount;
    int m_packageCount;
};

#endif
