 QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = vz

TEMPLATE = app



SOURCES += main.cpp\
    qupdateableview.cpp \
    mainwindow.cpp \
    datamodel.cpp \
    dafgraphelement.cpp \
    dafnodetype.cpp \
    dafnode.cpp \
    dafsuperdialog.cpp \
    dafedgetype.cpp \
    dafedge.cpp \
    dafgraph.cpp \
    semchform.cpp \
    titkinform.cpp \
    exporform.cpp \
    formtypes.cpp \
    formedges.cpp \
    formnodes.cpp \
    vz_settings.cpp \
    gostblock.cpp \
    perestanovka.cpp \
    formrules.cpp \
    dafrules.cpp \
    rule.cpp \
    settingsform.cpp \
    userulesform.cpp \
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
    selectdbwindow.cpp

HEADERS  += \
    qupdateableview.h \
    mainwindow.h \
    datamodel.h \
    dafgraphelement.h \
    dafnodetype.h \
    dafnode.h \
    dafsuperdialog.h \
    dafedgetype.h \
    dafedge.h \
    dafgraph.h \
    semchform.h \
    titkinform.h \
    exporform.h \
    formtypes.h \
    formedges.h \
    formnodes.h \
    vz_settings.h \
    gostblock.h \
    perestanovka.h \
    formrules.h \
    dafrules.h \
    rule.h \
    settingsform.h \
    userulesform.h \
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
    selectdbwindow.h

FORMS    += \
    mainwindow.ui \
    dafsuperdialog.ui \
    semchform.ui \
    titkinform.ui \
    exporform.ui \
    formtypes.ui \
    formedges.ui \
    formnodes.ui \
    formrules.ui \
    settingsform.ui \
    userulesform.ui \
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
    selectdbwindow.ui

RESOURCES += \
    vz.qrc
