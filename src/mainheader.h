#ifndef HEADER_H
#define HEADER_H

#include <QWidget>
#include <QHBoxLayout>
#include <QPixmap>
#include <QWidget>
#include <QLabel>

class MainHeader : public QWidget
{
public:
    MainHeader( QWidget *parent = 0 );
private:
    QLabel *m_statusLabel;
    QLabel *m_icon;
};

#endif
