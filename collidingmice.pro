HEADERS += \
	mouse.h \
    fuzzy.h \
    fuzzyitem.h \
    fuzzyling.h \
    fuzzyfile.h \
    fuzzyrule.h \
    fuzzyset.h
SOURCES += \
	main.cpp \
        mouse.cpp \
    fuzzy.cpp \
    fuzzyfile.cpp \
    fuzzyrule.cpp \
    fuzzyset.cpp

RESOURCES += \
	mice.qrc

# install
target.path = $$[QT_INSTALL_EXAMPLES]/graphicsview/collidingmice
sources.files = $$SOURCES $$HEADERS $$RESOURCES $$FORMS collidingmice.pro images
sources.path = $$[QT_INSTALL_EXAMPLES]/graphicsview/collidingmice
INSTALLS += target sources

symbian {
    TARGET.UID3 = 0xA000A643
    include($$QT_SOURCE_TREE/examples/symbianpkgrules.pri)
}

OTHER_FILES += \
    definition.txt
