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
    MainHeader( QWidget *parent = 0 );
public slots:
    void changeStatusLabel( int repoCount, int packageCount );
private:
    QLabel *m_statusLabel;
    QLabel *m_icon;
};

#endif
