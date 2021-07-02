include(ExternalProject)

ExternalProject_Add(libevent
    URL https://github.com/libevent/libevent/archive/62c152d9a7cd264b993dad730c4163c6ede2e0a3.tar.gz
    URL_HASH SHA256=4c80e5fe044ce5f8055b20a2f141ee32ec2614000f3e95d2aa81611a4c8f5213
    PREFIX libevent
    BUILD_IN_SOURCE 1
    CONFIGURE_COMMAND ./autogen.sh COMMAND ./configure --prefix=${DEPENDS_INSTALL_DIR}
    BUILD_COMMAND  make -j8
    INSTALL_COMMAND make install
    )
