#ifndef FIRSTSCREEN_H
#define FIRSTSCREEN_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>

class FirstScreen : public QWidget
{
	private:
		QLabel *m_warning;
		QPushButton *m_trust;
		QVBoxLayout *m_warningLayout;
	public:
		FirstScreen(QObject *parent = 0);
};


#endif
