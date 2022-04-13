CONFIG += c++17

HEADERS += \
    $$PWD/boostlogger.h \
    $$PWD/logger.h

SOURCES += \
  $$PWD/boostlogger.cpp

DEFINES += BOOST_LOG_DYN_LINK

unix:!macx: LIBS += -lboost_thread -lboost_log -lboost_log_setup
