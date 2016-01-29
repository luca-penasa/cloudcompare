if(USE_QT5)
        add_subdirectory(qtpropertybrowser_qt5)
        set (PATH "./qtpropertybrowser_qt5")
    else()
        add_subdirectory(qtpropertybrowser_qt4)
        set (PATH "./qtpropertybrowser_qt4")
endif()


if(USE_QT5)
        find_package(Qt5Widgets)
        include_directories(${Qt5Widgets_INCLUDE_DIRS})
        list(APPEND ${PATH} ${Qt5Widgets_INCLUDE_DIRS})
endif()


get_filename_component(absinclude ${PATH} ABSOLUTE)


set (QTPROPERTYBROWSER_LIBRARIES QtPropertyBrowser PARENT_SCOPE)
set (QTPROPERTYBROWSER_INCLUDES ${absinclude} PARENT_SCOPE)
