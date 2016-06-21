#include <string>
#include "keyringcallbacks.h"

class RepositoryData
{
public:

    /**
     * Default Constructor
     */
    RepositoryData();

    /**
     * Set Base Url of Repository
     */
    void setBaseUrl( const std::string& url );

    /**
     * Set Alias of Repository
     */
    void setAlias( const std::string& alias );

    /**
     * Return the KeyRing
     */
    zypp::KeyRingCallbacks keyring();

private:
    std::string m_baseUrl;
    std::string m_alias;
    
    zypp::KeyRingCallbacks m_keyring;
};
