#ifndef REPOSITORY_H
#define REPOSITORY_H

#include <QString>
#include <zypp/RepoInfo.h>
#include <zypp/RepoManager.h>

namespace OCI{

class Repository
{
	public:
		/**
			Default Consructor
		 */
		Repository();

		/**
			Returns the default name of the repository
		 */
		QString name() const;

		/**
			Returns the description of the repository
		 */
		QString description() const;

		/**
			Returns the summary of the repository
		 */
		QString summary() const;

		/**
			Returns the URL of the repository as a String
		 */
		QString url() const;

		/**
			Returns whether the repository is recommended or not
		 */
		QString recommended() const;

		/**
			Sets the value of Recommended for the repository
		 */
		void setRecommended( const QString& recommmended );

		/**
		 	Sets the name for the repository
		 */
		void setName( const QString& name );

		/**
			Sets the description for the repository
		 */
		void setDescription( const QString& description );

		/**
		 	Sets the summary for the repository
		 */
		void setSummary( const QString& summary );

		/**
			Set the URL for the repository
		 */
		void setUrl( const QString& url );
		void addRepository();
	private:
		QString m_recommended;
		QString m_name;
		QString m_description;
		QString m_summary;
		QString m_url;
};
}
#endif
