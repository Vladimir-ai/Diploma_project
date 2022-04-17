CONFIG += c++17

HEADERS += \
    $$PWD/feature_tracker.h \
    $$PWD/opencvfeaturetracker.h

SOURCES += \
  $$PWD/opencvfeaturetracker.cpp

unix:!macx: LIBS += -lopencv_video
