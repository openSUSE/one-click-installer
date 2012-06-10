#ifndef PACKAGE_H
#define PACKAGE_H

#include <QString>
#include <zypp/ZYppFactory.h>
#include <zypp/ZYppCommit.h>
#include <zypp/misc/DefaultLoadSystem.h>
#include <zypp/ResPool.h>
#include <zypp/ResStatus.h>
#include <zypp/ResFilters.h>
#include <QDebug>

namespace OCI{
class Package
{
	public:
		Package( QString name, QString summary, QString description );
		Package();
		QString name() const;
		QString summary() const;
		QString description() const;
		void setName( const QString& name );
		void setSummary( const QString& summary );
		void setDescription( const QString& description );
                void installPackage( const zypp::ZYpp::Ptr& zypp_pointer );
                bool isInstalled();
	private:
		QString m_name;
		QString m_summary;
		QString m_description;
                bool m_status;
};
}

#endif
