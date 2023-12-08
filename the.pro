QT += core gui widgets multimedia multimediawidgets

CONFIG += c++11


# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        help_section.cpp \
        help_widget.cpp \
        settings_window.cpp \
        the_button.cpp \
        the_player.cpp \
        tomeo.cpp

HEADERS += \
    help_section.h \
    help_widget.h \
    settings_window.h \
    the_button.h \
    the_player.h

FORMS += \
    help_section.ui \
    help_widget.ui \
    settings_window.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc



