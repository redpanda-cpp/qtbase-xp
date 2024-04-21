# Qt/Windows only configuration file
# --------------------------------------------------------------------

!winrt {
    LIBS_PRIVATE *= -luxtheme
    QMAKE_USE_PRIVATE += shell32
}
