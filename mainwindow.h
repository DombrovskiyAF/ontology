#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QSettings>
#include <QMainWindow>
#include <QSettings>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:


private slots:

    void formTypesShow();
    void formNodesShow();
    void formEdgesShow();
    void formRulesShow();
    void formSettingsShow();
    void formGraphshow();

    void openDB();
    void openDB_fromsettings(QString qst);
    void showDaf();

    void showTitkinForm();

    void showImportGostForm();
    void showExportDotForm();
    void exportToDot();

    void on_action_DOT_triggered();

    void on_actionRollback_DB_triggered();

    void on_push_similarity_clicked();

    void on_actionTemp_triggered();


    void on_actionOpenDB_triggered();


private:
    Ui::MainWindow *ui;
};



#endif // MAINWINDOW_H
