#include "formgraphshow.h"
#include "ui_formgraphshow.h"

formGraphShow::formGraphShow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::formGraphShow)
{
    ui->setupUi(this);
}

void formGraphShow::exec(QString GV_DIR)
{
    QProcess *dot = new QProcess();
    GVDIR = GV_DIR;
    qDebug() << GVDIR;
    dot->start("cmd");
    QString Write("cd " + GVDIR + '\n');
    qDebug() << Write;
    dot->write(Write.toUtf8());
    Write = "dot.exe -Tpng temp.dot -o temp.png\n";
    dot->write(Write.toUtf8());
    Write = "exit";
    dot->write(Write.toUtf8());

    dot->waitForFinished(200);
    QGraphicsScene *graph = new QGraphicsScene();
    qDebug() << GVDIR+"\temp.png";
    graph->addPixmap(QPixmap(GVDIR+"/temp.png"));
    ui->graphicsView->setScene(graph);
    ui->graphicsView->show();


}



formGraphShow::~formGraphShow()
{
    delete ui;
}
