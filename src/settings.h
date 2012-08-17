#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>
#include <QPushButton>
#include <QLabel>
#include <QCheckBox>
#include <QVBoxLayout>
#include <QSettings>

class Settings : public QDialog
{
    Q_OBJECT
public:

    /**
     * @brief Settings
     * @param settings : QSettings object
     * @param parent
     */
    Settings(QSettings *settings, QObject *parent = 0 );

private:
    QPushButton *m_confirm;
    QCheckBox *m_showDetails;
    QCheckBox *m_proposal;
    QPushButton *m_close;
    QLabel *m_repos;

    QSettings *m_settings;
private slots:

    /**
     * Save Settings and Close the dialog
     */
    void saveSettingsAndClose();
};

#endif
