 QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = vz

TEMPLATE = app



SOURCES += main.cpp\
    qupdateableview.cpp \
    mainwindow.cpp \
    datamodel.cpp \
    titkinform.cpp \
    exporform.cpp \
    formtypes.cpp \
    formedges.cpp \
    formnodes.cpp \
    vz_settings.cpp \
    gostblock.cpp \
    perestanovka.cpp \
    formrules.cpp \
    rule.cpp \
    settingsform.cpp \
    exportontoform.cpp \
    settingsadd.cpp \
    vzedge.cpp \
    dotimport.cpp \
    importform.cpp \
    ruleaddwidget.cpp \
    formnewquant.cpp \
    ruleeditwidget.cpp \
    formnewedge.cpp \
    tree.cpp \
    formnewnode.cpp \
    formnewnodetype.cpp \
    formnewedgetype.cpp \
    rollbackform.cpp \
    similarity.cpp \
    selectdbwindow.cpp \
    ontograph/ogedge.cpp \
    ontograph/ogedgetype.cpp \
    ontograph/oggraph.cpp \
    ontograph/oggraphelement.cpp \
    ontograph/ognode.cpp \
    ontograph/ognodetype.cpp \
    ontograph/ogrules.cpp \
    ontograph/ogsuperdialog.cpp \
    formgraphshow.cpp

HEADERS  += \
    qupdateableview.h \
    mainwindow.h \
    datamodel.h \
    titkinform.h \
    exporform.h \
    formtypes.h \
    formedges.h \
    formnodes.h \
    vz_settings.h \
    gostblock.h \
    perestanovka.h \
    formrules.h \
    rule.h \
    settingsform.h \
    exportontoform.h \
    settingsadd.h \
    vzedge.h \
    dotimport.h \
    importform.h \
    ruleaddwidget.h \
    formnewquant.h \
    ruleeditwidget.h \
    formnewedge.h \
    tree.h \
    formnewnode.h \
    formnewnodetype.h \
    formnewedgetype.h \
    rollbackform.h \
    similarity.h \
    selectdbwindow.h \
    ontograph/ogedge.h \
    ontograph/ogedgetype.h \
    ontograph/oggraph.h \
    ontograph/oggraphelement.h \
    ontograph/ognode.h \
    ontograph/ognodetype.h \
    ontograph/ogrules.h \
    ontograph/ogsuperdialog.h \
    formgraphshow.h

FORMS    += \
    mainwindow.ui \
    titkinform.ui \
    exporform.ui \
    formtypes.ui \
    formedges.ui \
    formnodes.ui \
    formrules.ui \
    settingsform.ui \
    exportontoform.ui \
    settingsadd.ui \
    ruleaddwidget.ui \
    ruleeditwidget.ui \
    importform.ui \
    formnewquant.ui \
    formnewedge.ui \
    tree.ui \
    formnewnode.ui \
    formnewnodetype.ui \
    formnewedgetype.ui \
    rollbackform.ui \
    similarity.ui \
    selectdbwindow.ui \
    ontograph/ogsuperdialog.ui \
    formgraphshow.ui

RESOURCES += \
    vz.qrc
