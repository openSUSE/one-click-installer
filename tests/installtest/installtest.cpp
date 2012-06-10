#include "installtest.h"

void
InstallTest::installTest()
{
    zypp::misc::defaultLoadSystem( "/" );
    zypp::ZYpp::Ptr zypp_pointer = zypp::getZYpp();
    zypp_pointer->initializeTarget( "/" );
    zypp::ResPoolProxy selectablePool( zypp::ResPool::instance().proxy() );
    OCI::Package package;
    package.setName( "dragonplayer" );
    package.installPackage( zypp_pointer );
    QVERIFY( package.isInstalled() == true );
}

QTEST_MAIN( InstallTest )
