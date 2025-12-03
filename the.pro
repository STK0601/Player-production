QT += core gui widgets multimedia multimediawidgets

CONFIG += c++11


# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        bottom_navigation_bar.cpp \
        control_bar.cpp \
        cover_page.cpp \
        home_page.cpp \
        library_page.cpp \
        main_window.cpp \
        player_page.cpp \
        profile_page.cpp \
        search_page.cpp \
        the_button.cpp \
        the_player.cpp \
        tomeo.cpp
HEADERS += \
    bottom_navigation_bar.h \
    control_bar.h \
    cover_page.h \
    home_page.h \
    library_page.h \
    main_window.h \
    player_page.h \
    profile_page.h \
    search_page.h \
    settings_dialog.h \
    the_button.h \
    the_player.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    images.qrc

