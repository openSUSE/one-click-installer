#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>
#include <QPushButton>
#include <QLabel>
#include <QCheckBox>
#include <QVBoxLayout>

class Settings : public QDialog
{
    Q_OBJECT
public:
    /**
    Constructor for the Settings dialog
    */
    Settings( QObject *parent = 0 );
private:
    QPushButton *m_confirm;
    QCheckBox *m_subscribe;
    QCheckBox *m_trust;
    QPushButton *m_close;
    QLabel *m_repos;
private slots:
    void saveSettingsAndClose();
};

#endif
