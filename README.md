# QS_Act
QS actor


```bash
sudo apt install libboost-dev libcurl4-openssl-dev libprotobuf-dev \
    libgrpc++-dev protobuf-compiler libssl-dev build-essential curl

1, compile proto in src
(base) ➜  src git:(main) ✗ protoc --cpp_out=./ rpc.proto
2, mkdir build && cd build && cmake ..
3, make -j 8
4, make run
5, cp src/config.json build/
6, ./run
```
