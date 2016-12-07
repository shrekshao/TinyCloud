cmake_minimum_required(VERSION 3.6)
project(TinyCloud)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
    _site/assets/img/Di-new-avatar.png
    _site/assets/img/drive.png
    _site/assets/img/gmail.png
    _site/css/main.css
    _site/drive.html
    _site/index-login.html
    _site/index.html
    _site/profile.html
        grpc_backend/protos/backend.proto
    http_server
    http_server.cpp
    http_server.h
    Makefile
    README
    README.md grpc_backend/backend_server.cpp grpc_backend/storage_server.cpp grpc_backend/Indexer.cpp grpc_backend/Indexer.h)

add_executable(TinyCloud ${SOURCE_FILES})