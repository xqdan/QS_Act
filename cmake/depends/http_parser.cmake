include(ExternalProject)

ExternalProject_Add(
  libhttp_parser
  URL https://github.com/nodejs/http-parser/archive/4f15b7d510dc7c6361a26a7c6d2f7c3a17f8d878.tar.gz
  URL_MD5 e6cb2c467d3916e6a3304c165f6a6026
  PREFIX http_parser
  CONFIGURE_COMMAND ""
  BUILD_IN_SOURCE 1
  BUILD_COMMAND make library
  INSTALL_COMMAND make install PREFIX=${DEPENDS_INSTALL_DIR})
