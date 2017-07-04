#include "mainwindow.h"
#include "ui_mainwindow.h"

/* Defining */


#include "datamodel.h"
#include "dotimport.h"
#include "importform.h"
#include "dafgraph.h"
#include "dafsuperdialog.h"
#include "titkinform.h"
//#include "semchform.h"
#include "exporform.h"
#include "exportontoform.h"
#include "userulesform.h"
#include <QFileDialog>
#include <QDebug>
#include "settingsform.h"
#include "rollbackform.h"
//DataModel *dataModel;
importform *importForm;
SettingsForm *settingForm;
DafSuperDialog *dafForm;
TitkinForm *titkinForm;
//SemchForm *semchForm;
ImportGostForm *importGostForm;
ExportOntoForm *exportOntoForm;
UseRulesForm *useRulesForm;
RollbackForm *rollbackform;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    dataModel = new DataModel();
    dafForm = new DafSuperDialog(this);
    titkinForm = new TitkinForm(this);
    //semchForm = new SemchForm(this);
    settingForm = new SettingsForm(this);
    importGostForm = new ImportGostForm(this);
    useRulesForm = new UseRulesForm(this);
    rollbackform = new RollbackForm();
    importForm = new importform(dataModel,settingForm->settings,settingForm->settings_edges,this);
    //exportOntoForm = new ExportOntoForm(this);

    dataModel->setLog(ui->textEdit);
    connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(close()));
    connect(ui->actionOpenDB, SIGNAL(triggered()), this, SLOT(openDB()));
    connect(ui->actionExportDOT, SIGNAL(triggered()), this, SLOT(exportToDot()));
    connect(ui->actionImportGost, SIGNAL(triggered()), this, SLOT(showImportGostForm()));
    connect(ui->actionDaf, SIGNAL(triggered()), this, SLOT(showDaf()));
    connect(ui->actionTestFormTR, SIGNAL(triggered()), this, SLOT(showTitkinForm()));
    connect(ui->actionFormTypesShow, SIGNAL(triggered()), this, SLOT(formTypesShow()));
    connect(ui->actionFormNodesShow, SIGNAL(triggered()), this, SLOT(formNodesShow()));
    connect(ui->actionFormEdgesShow, SIGNAL(triggered()), this, SLOT(formEdgesShow()));
    connect(ui->actionFormRulesShow, SIGNAL(triggered()), this, SLOT(formRulesShow()));
    connect(ui->actionUseRulesFormShow, SIGNAL(triggered()), this, SLOT(showUseRulesForm()));
    connect(ui->actionSettings, SIGNAL(triggered()), this, SLOT(formSettingsShow()));

    if (dataModel->getSettings()->value("settings/autoload", false).toBool()) {
        qDebug () << "load Settings";
        openDB_fromsettings(dataModel->getSettings()->value("settings/file", false).toString());
    }


}

MainWindow::~MainWindow()
{
    delete dataModel;
    delete ui;
}

void MainWindow::showUseRulesForm()
{
    useRulesForm->show();
}

void MainWindow::formSettingsShow()
{
    settingForm->show();
}

void MainWindow::formTypesShow()
{
    dataModel->showFormTypes();
}

void MainWindow::formRulesShow()
{
    dataModel->showFormRules();
}


void MainWindow::formNodesShow()
{
    dataModel->showFormNodes();
}

void MainWindow::formEdgesShow()
{
    dataModel->showFormEdges();
}

/*QString string;
QByteArray line;

void MainWindow::loadGost()
{
    QString fName = QFileDialog::getOpenFileName(0, "Open file");
    QFile file(fName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    while (!file.atEnd())
    {
        line = file.readLine();
        string.append( QString::fromLocal8Bit(line) );
    }

    ui->textEdit->append(QString::fromLocal8Bit("Загрузка ГОСТ из файла ") + fName);
    ui->textEdit->append(string);

    file.close();

    return;
}

void MainWindow::findAbbr()
{
    int offset = 0;
    int cnt = 0;
    int posIED = -1;// позиция строки "Иерархический для:"
    //int posConc = -1;// позиция строкового идентификатора
    // "\\b[F|A]([A-Z]){2}_([A-Z]){3}(\\.\\d{1,2}){1,3}\\b"
    // "\\b[F|A]([A-Z]){2}_([A-Z]){3}(.){0,5}(\\.\\d{1,2}){1,3}\\b"
    // "\\b\\d{1,3}\\.(\\d{1,3}\\.){1,4}\\d{1,3}\\b"

    QRegExp re_number("\\b\\d{1,3}\\.(\\d{1,3}\\.){1,4}\\d{1,3}\\b");
    if ( !re_number.isValid() )
    {
        qDebug() << "re_number not valid";
        return;
    }
    qDebug() << "re_number.captureCount()" << re_number.captureCount();

    QRegExp re_conceptId("\\b[F|A]([A-Z]){2}_([A-Z]){3}(.){0,5}(\\.\\d{1,2}){1,3}\\b");
    if ( !re_conceptId.isValid() )
    {
        qDebug() << "re_conceptId not valid";
        return;
    }
    qDebug() << "re_conceptId.captureCount()" << re_conceptId.captureCount();


    do
    {
        posIED = string.indexOf(QString::fromLocal8Bit("Иерархический для:"), offset);
        if (posIED > -1)
        {
            offset = posIED + 1;
            qDebug() << cnt << ". pos= " << posIED;
            cnt++;
        }
    }
    while(posIED > -1);

    cnt = 0;
    QString value;
    int pos = 0;
    while ((pos = re_number.indexIn(string, pos)) != -1)
    {
        value = re_number.cap(0);
        ++cnt;
        pos += re_number.matchedLength();
        qDebug() << value;
    }

    pos = 0;
    cnt = 0;
    while ((pos = re_conceptId.indexIn(string, pos)) != -1)
    {
        value = re_conceptId.cap(0);
        ++cnt;
        pos += re_conceptId.matchedLength();
        qDebug() << value;
    }

    return;
}
*/

void MainWindow::showImportGostForm()
{
    importGostForm->show();
}

void MainWindow::showExportDotForm()
{
    //exportDotForm->show();
}

void MainWindow::showTitkinForm()
{
    titkinForm->show();
}

void MainWindow::openDB()
{
    dataModel->openDB(this);
    setWindowTitle(dataModel->getDBName());
}

void MainWindow::openDB_fromsettings(QString qst)
{
    dataModel->openDB_fromsettings(this,qst);
    setWindowTitle("Load from settings");
}

void MainWindow::showDaf()
{
    dafForm->show();
}

void MainWindow::exportToDot()
{
    QString fName = QFileDialog::getSaveFileName(0, "Save file","","DOT files (*.dot)");
    dataModel->importToDot(fName);

    //dataModel->simpleExportDot(fName);
}

void MainWindow::on_actionTemp_triggered()
{
    titkinForm->show();
}




void MainWindow::on_action_DOT_triggered()
{

    importForm->show();
    importForm->refresh_tables();
    //QString fName = QFileDialog::getOpenFileName(0, "Open file","","DOT files (*.dot)");
    //dot_import->ImportFromDot(fName);

}

void MainWindow::on_actionRollback_DB_triggered()
{
    rollbackform->show();
}

void MainWindow::on_push_similarity_clicked()
{
    dataModel->form_similarity.show();
}

void MainWindow::on_actionOpenDB_triggered()
{

}
