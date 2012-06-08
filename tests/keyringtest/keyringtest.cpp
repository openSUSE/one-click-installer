#include <QtTest/QTest>
#include "keyringtest.h"

void
KeyRingTest::checkKeyRing()
{
    QString baseUrl( "http://download.opensuse.org/distribution/12.1/repo/oss/" );
    zypp::KeyRingCallbacks keyringcallbacks;
    OCI::Repository repo;
    repo.setUrl( baseUrl );
    repo.addRepository();
    QCOMPARE( keyringcallbacks.exists(),true );
}

QTEST_MAIN( KeyRingTest )
