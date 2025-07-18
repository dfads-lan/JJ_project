QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    admi.cpp \
    camerathread.cpp \
    changepwd.cpp \
    keyboard.cpp \
    main.cpp \
    mainwindow.cpp \
    passwd.cpp \
    record.cpp \
    sqlit.cpp

HEADERS += \
    admi.h \
    camerathread.h \
    changepwd.h \
    keyboard.h \
    mainwindow.h \
    passwd.h \
    record.h \
    sqlit.h

FORMS += \
    admi.ui \
    changepwd.ui \
    keyboard.ui \
    mainwindow.ui \
    passwd.ui \
    record.ui



INCLUDEPATH +=  /home/lx/linux/project/AI_recognition/install/include\
                /home/lx/linux/project/AI_recognition/install/include/opencv\
                /home/lx/linux/project/AI_recognition/install/include/opencv2
LIBS += /home/lx/linux/project/AI_recognition/install/lib/libopencv*


#INCLUDEPATH +=  /home/lx/linux/project/AI_recognition/install_arm/include\
#                /home/lx/linux/project/AI_recognition/install_arm/include/opencv\
#                /home/lx/linux/project/AI_recognition/install_arm/include/opencv2
#LIBS += /home/lx/linux/project/AI_recognition/install_arm/lib/libopencv*


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc
