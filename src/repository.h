#ifndef REPOSITORY_H
#define REPOSITORY_H

#include <QString>

namespace OCI {

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
     * @brief setRecommended : set recommended value
     * @param recommmended : recommended value
     */
    void setRecommended( const QString& recommmended );

    /**
     * @brief setName : set name of repo
     * @param name : name of repo
     */
    void setName( const QString& name );

    /**
     * @brief setDescription : set repo description
     * @param description : repo description
     */
    void setDescription( const QString& description );

    /**
     * @brief setSummary : set repo summary
     * @param summary : repo summary
     */
    void setSummary( const QString& summary );

    /**
     * @brief setUrl : set repo url
     * @param url : repo url
     */
    void setUrl( const QString& url );
private:
    QString m_recommended;
    QString m_name;
    QString m_description;
    QString m_summary;
    QString m_url;
};
}
#endif
