CONFIG += c++17

HEADERS += \
    $$PWD/feature_tracker.h \
    $$PWD/opencvfeaturetracker.h \
    $$PWD/opencvfeaturetrackerwidget.h

SOURCES += \
  $$PWD/opencvfeaturetracker.cpp \
  $$PWD/opencvfeaturetrackerwidget.cpp

unix:!macx: LIBS += -lopencv_video
