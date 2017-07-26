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
    QString a(GVDIR+"/dot.exe -Tpng \""+GVDIR+"/temp.dot\" "+"-o \""+GVDIR+"/temp.png\" ");
    qDebug().noquote() << a;
    dot->start(a);
    dot->waitForFinished();
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
