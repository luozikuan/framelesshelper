TARGET = framelessapplication
debug: TARGET = $$join(TARGET,,,d)
TEMPLATE = app
QT += gui-private
qtHaveModule(widgets): QT += widgets
qtHaveModule(quick) {
    QT += quick
    HEADERS += framelessquickhelper.h
    SOURCES += framelessquickhelper.cpp
}
CONFIG += c++17 strict_c++ utf8_source warn_on windeployqt
DEFINES += WIN32_LEAN_AND_MEAN QT_NO_CAST_FROM_ASCII QT_NO_CAST_TO_ASCII
CONFIG -= embed_manifest_exe
RC_FILE = resources.rc
HEADERS += winnativeeventfilter.h
SOURCES += winnativeeventfilter.cpp main_windows.cpp
RESOURCES += resources.qrc
OTHER_FILES += manifest.xml
