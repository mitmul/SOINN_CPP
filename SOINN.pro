#-------------------------------------------------
#
# Project created by QtCreator 2012-07-17T00:29:51
#
#-------------------------------------------------

QT       += core gui

TARGET = SOINN
TEMPLATE = app


SOURCES += main.cpp\
        MainWindow.cpp \
    SOINN.cpp \
    Graph.cpp

HEADERS  += MainWindow.h \
    SOINN.h \
    Graph.h

FORMS    += MainWindow.ui

mac {

# Eigen
INCLUDEPATH += /usr/local/Cellar/eigen/3.1.0/include/eigen3

# OpenCV
INCLUDEPATH += /usr/local/include/opencv2
LIBS += -L/usr/local/lib \
  -lopencv_gpu \
  -lopencv_video \
  -lopencv_objdetect \
  -lopencv_ml \
  -lopencv_legacy \
  -lopencv_imgproc \
  -lopencv_highgui \
  -lopencv_contrib \
  -lopencv_flann \
  -lopencv_features2d \
  -lopencv_core \
  -lopencv_calib3d

}

unix {

# OpenCV
INCLUDEPATH += /usr/local/include/opencv2
LIBS += -L/usr/local/lib \
  -lopencv_calib3d \
  -lopencv_contrib \
  -lopencv_core \
  -lopencv_features2d \
  -lopencv_flann \
  -lopencv_gpu \
  -lopencv_highgui \
  -lopencv_imgproc \
  -lopencv_legacy \
  -lopencv_ml \
  -lopencv_nonfree \
  -lopencv_objdetect \
  -lopencv_photo \
  -lopencv_stitching \
  -lopencv_ts \
  -lopencv_video \
  -lopencv_videostab

# Eigen
INCLUDEPATH += /usr/include/eigen3

}
