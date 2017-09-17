#-------------------------------------------------
#
# Project created by QtCreator 2015-02-28T13:41:25
#
#-------------------------------------------------

QT       += core gui sql axcontainer

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Qstaff
TEMPLATE = app


SOURCES += main.cpp\
        staffdialog.cpp \
    pages.cpp \
    qperson.cpp \
    tableiomapper.cpp \
    qupdateableview.cpp \
    table.cpp \
    viewcatalogform.cpp \
    dedelegate.cpp \
    report.cpp \
    viewimportform.cpp

HEADERS  += staffdialog.h \
    pages.h \
    qperson.h \
    tableiomapper.h \
    qupdateableview.h \
    viewcatalogform.h \
    table.h \
    dedelegate.h \
    report.h \
    viewimportform.h

FORMS    += staffdialog.ui \
    viewcatalogform.ui \
    ViewImportForm.ui

RESOURCES += \
    Qstaff.qrc
RC_FILE = qstaff.rc

OTHER_FILES +=

LIBS +=  -loleaut32 -lole32 -lgdi32 -luuid
//LIBS += -lqaxserver -lqaxcontainer


