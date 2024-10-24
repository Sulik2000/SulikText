QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ContextMenu/dircontextmenu.cpp \
    ContextMenu/filecontextmenu.cpp \
    ContextMenu/filedialog.cpp \
    filetreeview.cpp \
    main.cpp \
    mainwindow.cpp \
    optionwindow.cpp

HEADERS += \
    ContextMenu/dircontextmenu.h \
    ContextMenu/filecontextmenu.h \
    ContextMenu/filedialog.h \
    filetreeview.h \
    mainwindow.h \
    optionwindow.h

FORMS += \
    ContextMenu/filedialog.ui \
    mainwindow.ui \
    optionwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
