include(ExternalProject)

ExternalProject_Add(
  picojson
  URL https://github.com/kazuho/picojson/archive/v1.3.0.tar.gz
  URL_HASH
    SHA256=056805ca2691798f5545935a14bb477f2e1d827c9fb862e6e449dbea22801c7d
  PREFIX picojson
  BUILD_IN_SOURCE 1
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ""
  INSTALL_COMMAND make install prefix=${DEPENDS_INSTALL_DIR})
