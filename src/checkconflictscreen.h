#ifndef CHECKCONFLICTSCREEN_H
#define CHECKCONFLICTSCREEN_H

#include <QWidget>
#include <QLabel>
#include <QMovie>
#include <QDebug>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <klocalizedstring.h>

class CheckConflictScreen : public QWidget
{
public:
    CheckConflictScreen() {
	m_layout = new QVBoxLayout;
	m_animationHolder = new QLabel();
	m_conflictAnimation.setFileName( "/usr/share/one-click-installer/res/conflictCheck.gif" );
    
	if ( m_conflictAnimation.isValid() ) {
	    m_animationHolder->setMovie( &m_conflictAnimation );
	    m_conflictAnimation.start();
	}    
	
	m_quitButton = new QPushButton( i18n("Quit Installation") );
	QHBoxLayout *buttonLayout = new QHBoxLayout();
	buttonLayout->addSpacing( 400 );
	buttonLayout->addWidget( m_quitButton );
	QObject::connect(m_quitButton, SIGNAL( clicked() ), QApplication::instance(), SLOT( quit() ) );
	
	m_layout->addWidget( m_animationHolder );
	m_layout->addLayout( buttonLayout );
	setLayout( m_layout );
    }
private:
    QVBoxLayout *m_layout;
    QLabel *m_animationHolder;
    QMovie m_conflictAnimation;
    QPushButton *m_quitButton;    
}; 

#endif
