QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

include(Logger/Logger.pri)

include(VideoReader/VideoReader.pri)
include(FeatureTracker/FeatureTracker.pri)
include(FeatureDetector/FeatureDetector.pri)

include(Common/Common.pri)

include(PathProcessor/PathProcessor.pri)

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000 QT_ASSERTION    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += console

SOURCES += \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    mainwindow.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

unix
{
    INCLUDEPATH += /usr/include/opencv4 \
                    /usr/include/eigen3

    LIBS += -lopencv_videoio -lopencv_features2d -lopencv_core -lopencv_imgproc
}
