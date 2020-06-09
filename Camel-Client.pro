QT += quick

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Refer to the documentation for the
# deprecated API to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

HEADERS += \
    headers/baseclass.h \
    headers/camel_client.h \
    headers/constants.h \
    headers/filemanager.h \
    headers/task.h \
    headers/taskqueue.h \
    headers/transporter.h


SOURCES += \
        headers/baseclass.cpp \
        sources/camel_client.cpp \
        sources/filemanager.cpp \
        sources/main.cpp \
        sources/task.cpp \
        sources/taskqueue.cpp \
        sources/transporter.cpp

RESOURCES += qtquick/qml.qrc \
    qtquick/icons.qrc \
    qtquick/modules.qrc

INCLUDEPATH += \
    headers/

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

INCLUDEPATH += $$quote(C:/Program Files (x86)/OpenSSL-Win32/include)

LIBS +=$$quote(C:/Program Files (x86)/OpenSSL-Win32/lib/libcrypto.lib)
LIBS +=$$quote(C:/Program Files (x86)/OpenSSL-Win32/lib/libssl.lib)

LIBS += -lWs2_32
