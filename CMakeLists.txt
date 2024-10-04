cmake_minimum_required(VERSION 3.14)

project(ArbitrageScaner LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Network REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Network REQUIRED)

include_directories(
    src

    src/marketManager
    src/marketManager/base
    src/marketManager/markets
    src/telegram
)

add_executable(
  ArbitrageScaner
  main.cpp

  arbitragescaner.h
  arbitragescaner.cpp

  src/marketManager/marketmanager.h
  src/marketManager/marketmanager.cpp

  src/marketManager/base/marketapi.h
  src/marketManager/base/marketapi.cpp

  src/marketManager/markets/bybitapi.h
  src/marketManager/markets/bybitapi.cpp
  src/marketManager/markets/mexcapi.h
  src/marketManager/markets/mexcapi.cpp
  src/marketManager/markets/bingxapi.h
  src/marketManager/markets/bingxapi.cpp
  src/marketManager/markets/gateioapi.h
  src/marketManager/markets/gateioapi.cpp
  src/marketManager/markets/htxapi.h
  src/marketManager/markets/htxapi.cpp

  src/telegram/telegramapi.h
  src/telegram/telegramapi.cpp
)

target_link_libraries(
    ArbitrageScaner
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Network
    )
