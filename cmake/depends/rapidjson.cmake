include(ExternalProject)

ExternalProject_Add(
  rapidjson
  URL https://github.com/Tencent/rapidjson/archive/dfbe1db9da455552f7a9ad5d2aea17dd9d832ac1.tar.gz
  URL_HASH
    SHA256=a2faafbc402394df0fa94602df4b5e4befd734aad6bb55dfef46f62fcaf1090b
  PREFIX rapidjson
  BUILD_IN_SOURCE 1
  CMAKE_ARGS -DRAPIDJSON_BUILD_DOC=OFF -DRAPIDJSON_BUILD_EXAMPLES=OFF
             -DRAPIDJSON_BUILD_TESTS=OFF
             -DCMAKE_INSTALL_PREFIX=${DEPENDS_INSTALL_DIR})
