CONFIG += c++17

HEADERS += \
  $$PWD/abstract_pose_estimator.h \
  $$PWD/opencv_pose_estimator.h \
  $$PWD/opencv_pose_estimator_widget.h

SOURCES += \
  $$PWD/opencv_pose_estimator.cpp \
  $$PWD/opencv_pose_estimator_widget.cpp

unix:!macx: LIBS += -lopencv_calib3d -lopencv_imgcodecs -lopencv_highgui
