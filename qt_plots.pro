TARGET = qt_plots

QMAKE_CXXFLAGS += -std=c++0x

HEADERS += \
    filereader.h \
    curve.h \
    renderarea.h \
    axis.h \
    mainwindow.h \
    curvespool.h \
    arefilling.h \
    aredrawing.h

SOURCES += \
    filereader.cpp \
    curve.cpp \
    main.cpp \
    renderarea.cpp \
    axis.cpp \
    mainwindow.cpp \
    curvespool.cpp

OTHER_FILES += \
    README.md \
    .gitignore \
    input_file.txt \
    input_file_generator.rb

