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
    README.md grpc_backend/backend_server.cpp grpc_backend/filelist_wu.cpp grpc_backend/Indexer.cpp grpc_backend/Indexer.h grpc_backend/file_wu.cpp grpc_backend/BigTabler.cpp grpc_backend/BigTabler.h)

add_executable(TinyCloud ${SOURCE_FILES})


find_package(Boost 1.58.0 COMPONENTS system filesystem REQUIRED)

if(Boost_FOUND)

    message(STATUS "Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
    message(STATUS "Boost_LIBRARIES: ${Boost_LIBRARIES}")
    message(STATUS "Boost_VERSION: ${Boost_VERSION}")

    include_directories(${Boost_INCLUDE_DIRS})

endif()

add_executable(BoostTest main.cpp)

if(Boost_FOUND)

    target_link_libraries(BoostTest ${Boost_LIBRARIES})

endif()