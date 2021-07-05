include(ExternalProject)

ExternalProject_Add(
  libglog
  URL https://github.com/google/glog/archive/refs/tags/v0.5.0.tar.gz
  URL_HASH
    SHA256=eede71f28371bf39aa69b45de23b329d37214016e2055269b3b5e7cfd40b59f5
  PREFIX glog
  BUILD_IN_SOURCE 1
  CMAKE_ARGS -S
             .
             -B
             build
             -G
             "Unix Makefiles"
             -DCMAKE_INSTALL_PREFIX=${DEPENDS_INSTALL_DIR}
             -DWITH_GFLAGS=OFF
             -DWITH_GTEST=OFF
             -DWITH_UNWIND=OFF
  BUILD_COMMAND cmake --build build --target glog
  INSTALL_COMMAND cmake --build build --target install)
