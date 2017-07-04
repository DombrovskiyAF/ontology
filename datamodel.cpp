#include "datamodel.h"
#include <QTextCodec>
#include <QProcess>
#include "formtypes.h"
#include "formnodes.h"
#include "formedges.h"
#include "formrules.h"
#include "dafgraph.h"
#include "dafrules.h"
#include "settingsform.h"
#include "ui_settingsform.h"
#include "gostblock.h"

#include "vz_settings.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMessageBox>
#include <QSqlError>
#include <QFileDialog>

#include "perestanovka.h"
#include <QIdentityProxyModel>
#include <QDebug>
#include "qsqlrecord.h"

#include "selectdbwindow.h"

QSqlDatabase db;


Perestanovka *pers[4];
QList<VzRule*> list;
DataModel *dataModel;

DataModel::DataModel()
{
    settings = new QSettings(ORGANIZATION_NAME, APPLICATION_NAME);

    QCoreApplication::setOrganizationName(ORGANIZATION_NAME);
    QCoreApplication::setOrganizationDomain(ORGANIZATION_DOMAIN);
    QCoreApplication::setApplicationName(APPLICATION_NAME);

    int j = 0;
    for(int i = 2; i < 6; i++)
    {
        Perestanovka *p = new Perestanovka(i);
        pers[j] = p;
        j++;
    }

    /*
     * пример использования перестановок
    for(int i = 2; i < 6; i++)
    {
        qDebug() << "Perest " << i << "-------------------------";
        Perestanovka *p = pers[i-2];
        int n = p->getN();
        int perCnt = p->getPerCount();
        for(int j = 0; j < perCnt; j++)
        {
            int* per = p->getPer(j);
            QString perStr = "";
            for(int k = 0; k < n; k++)
            {
                int index = per[k];
                perStr += QString::number(index) + " ";
            }
            qDebug() << perStr << endl;
        }
    }
    */

    m_log = NULL;

    m_nodeTypes = NULL;
    m_edgeTypes = NULL;
    m_quantTypes = NULL;
    m_nodes = NULL;
    m_edges = NULL;
    m_formTypes = new FormTypes();
    m_formNodes = new FormNodes();
    m_formEdges = new FormEdges();
    m_formRules = new FormRules();
    m_graph = new DafGraph();
    m_userulesform = new UseRulesForm();
    sdw = new selectdbwindow();
}

DataModel::~DataModel()
{
    for(int i = 0; i < 4; i++)
    {
        delete pers[i];
    }
    deleteModel();

    delete m_formTypes;
    delete m_formNodes;
    delete m_formEdges;
    delete m_quantTypes;
    delete m_formRules;
    delete m_graph;

    delete settings;
}

QSettings *DataModel::getSettings()
{
    return settings;
}

bool DataModel::insertNode(QString id_node_type,QString node_name,QString brightness,QString status,QString author,QString rem, int &idNode)
{
    qDebug () << "IN=2";
    QSqlQuery query;
    QString str;
    QString strF ="INSERT INTO nodes (id_node_type, node_name, brightness, status, author, rem)"
                              "VALUES(%1, '%2', %3, %4, '%5', '%6');";
    str = strF.arg(id_node_type)
            .arg(node_name)
            .arg(brightness)
            .arg(status)
            .arg(author)
            .arg(rem);

    if (!query.exec(str))
    {
        qDebug() << str;
        qDebug() << "Unable to make insert operation";
        //QMessageBox::information(0, "SQL INSERT:", query.lastError().text());
        dataModel->logMessage(query.lastError().text());
        idNode = -1;
        return false;
    }
    else
    {
        //TO DO: затычка, выдернуть идентификатор добавленной записи оптимальным способом
        idNode = getNodeId(node_name);
        return true;
    }
}

bool DataModel::insertEdge(QString id_node_src,QString id_edge_type,QString id_node_dst,QString quant_id,QString capacity,QString status,QString isNew, int &idEdge)
{
    qDebug () << "IE=2";
    QSqlQuery query;
    QString str;
    QString strF ="INSERT INTO edges "
            "(id_node_src,id_edge_type,id_node_dst,quant_id,capacity,status,isNew) VALUES "
            "(%1, %2, %3, '%4', %5, %6, %7);";
    str = strF.arg(id_node_src)
              .arg(id_edge_type)
              .arg(id_node_dst)
              .arg(quant_id)
              .arg(capacity)
              .arg(status)
              .arg(isNew);
    if (!query.exec(str))
    {
        //QMessageBox::information(0, "SQL INSERT:", query.lastError().text());
        dataModel->logMessage(query.lastError().text());
        idEdge = -1;
        return false;
    }
    else
    {
        dataModel->m_edges->select();
        //затычка:  TO DO: выдернуть идентификатор добавленной записи опт способом
        // INSERT.....RETURNING ...
        // NEXT VALUE FOR ...
        idEdge = getEdgeId(id_node_src, id_edge_type, id_node_dst);
        return true;
    }
}

int DataModel::getEdgeId(QString id_node_src, QString id_edge_type, QString id_node_dst)
{
    QSqlQuery q;
    QString queryStr ="SELECT id FROM edges WHERE id_node_src="+id_node_src;
            queryStr+=" AND id_edge_type="+id_edge_type;
            queryStr+=" AND id_node_dst="+id_node_dst;
    q.exec(queryStr);
    if ( !q.isActive() )
    {
        qDebug() << "SQL Error: " << q.lastError().text();
        return -1;
    }

    q.first();
    return q.value(0).toInt();
}

int DataModel::getNodeId(QString node_name)
{
    QSqlQuery q;
    QString queryStr = QString("SELECT id FROM nodes WHERE node_name = '%1';").arg(node_name);
    q.exec(queryStr);
    if ( !q.isActive() )
    {
        qDebug() << "SQL Error: " << q.lastError().text();
        return -1;
    }

    q.first();
    return q.value(0).toInt();
}

void DataModel::setLog(QTextEdit *log)
{
    m_log = log;
}

void DataModel::logMessage(QString message)
{
    if(m_log != NULL)
    {
        m_log->append(message);
    }
}


int DataModel::getNodeTypeId(QString nodeTypeName)
{
    if (m_graph == NULL)
        return -1;

    return m_graph->getNodeTypeId(nodeTypeName);
}

int DataModel::getNodeInf(QString nodeName, int &nodeTypeId)
{
    //todo как выдернуть из m_nodes?
    QSqlQuery query;
    query.exec("SELECT id, id_node_type FROM nodes WHERE node_name='" + nodeName + "'");

    if ( !query.isActive() )
    {
        qDebug() << "Database Error: " << query.lastError().text();
        return -1;
    }

    if(query.size() == 0)
    {
        qDebug() << "query.size() == 0";
        return 0;
    }

    if(query.size() > 1 )
    {
        qDebug() << "query.size() > 1";
        return -1;
    }

    int id;
    while ( query.next() )
    {
        id = query.value(0).toInt();
        nodeTypeId = query.value(1).toInt();
    }

    qDebug() << nodeName << ": id=" << QString::number(id) << " type_id=" << QString::number(nodeTypeId);
    return id;
}

int DataModel::getEdgeTypeId(int nodeSrcTypeId, QString typeName, int nodeDstTypeId)
{
    qDebug() << "---DataModel::getEdgeTypeId "
             << "nodeSrcTypeId= " << QString::number(nodeSrcTypeId) << " || "
             << "typeName= " << typeName << " || "
             << "nodeDstTypeId= " << QString::number(nodeDstTypeId);

    QSqlQuery query;
    query.exec("SELECT id FROM edge_types WHERE id_node_type_src="  +
               QString::number(nodeSrcTypeId) +
               " AND id_node_type_dst=" + QString::number(nodeDstTypeId) +
               " AND edge_name='" + typeName + "'"
    );

    if ( !query.isActive() )
    {
        qDebug() << "Database Error: " << query.lastError().text();
        return -1;
    }

    if(query.size() == 0)
    {
        qDebug() << "query.size() == 0";
        return 0;
    }

    if(query.size() > 1 )
    {
        qDebug() << "query.size() > 1";
        return -1;
    }

    int id = 0;
    while ( query.next() )
    {
        id = query.value(0).toInt();
    }

    qDebug() << "id=" << QString::number(id);
    return id;
}

int DataModel::addEdgeToModel(QString nodeSrcName, QString typeName, QString nodeDstName, int id)
{
    qDebug() << "start addEdgeToModel: " << nodeSrcName << "->" << typeName << "->" << nodeDstName;
    int nodeSrcTypeId;
    int nodeSrcId = getNodeInf(nodeSrcName, nodeSrcTypeId);
    if(nodeSrcId <= 0)
    {
        qDebug() << "ERROR in line " << __LINE__;
        return -1;
    }
    int nodeDstTypeId;
    int nodeDstId = getNodeInf(nodeDstName, nodeDstTypeId);
    if(nodeDstId <= 0)
    {
        qDebug() << "ERROR in line " << __LINE__;
        return -1;
    }
    int typeId = getEdgeTypeId(nodeSrcTypeId, typeName, nodeDstTypeId);
    if(typeId <= 0)
    {
        qDebug() << "ERROR in line " << __LINE__;
        return -1;
    }
    qDebug() << "work  addEdgeToModel: " << QString::number(nodeSrcId) << "->" << QString::number(typeId) << "->" << QString::number(nodeDstId);

    QSqlTableModel *m;
    m = m_edges->relationModel(edges_id_node_src);
    if(m)
    {
        m->select();
    }
    m = m_edges->relationModel(edges_id_node_dst);
    if(m)
    {
        m->select();
    }

    int row = 0;
    m_edges->insertRows(row,1);
    if(!m_edges->setData(m_edges->index(row, edges_id_node_src), nodeSrcId))
    {
        qDebug() << "ERROR!! False setData in line: " << __LINE__;
        m_edges->revertAll();
        return -1;
    }

    if(!m_edges->setData(m_edges->index(row, edges_edge_type), typeId))
    {
        qDebug() << "ERROR!! False setData in line: " << __LINE__;
        m_edges->revertAll();
        return -1;
    }

    if(!m_edges->setData(m_edges->index(row, edges_id_node_dst), nodeDstId))
    {
        qDebug() << "ERROR!! False setData in line: " << __LINE__;
        m_edges->revertAll();
        return -1;
    }
    qDebug() << "normal stop addEdgeToModel";
    return 0 ;
}

bool DataModel::clearModels()
{
    while(m_edges->rowCount() > 0)
    {
        m_edges->removeRow(0);
        submitEdges();
    }

    while(m_nodes->rowCount() > 0)
    {
        m_nodes->removeRow(0);
        submitNodes();
    }
    return true;
}

void DataModel::selectNodesAndEdges()
{
    if(!m_nodes->select())
    {
        qDebug() << "Error: !m_nodes->select()";
        return;
    }
    if(!m_edges->select())
    {
        qDebug() << "Error: !m_edges->select()";
        return;
    }
}
void DataModel::selectNodes()
{
    if(!m_nodes->select())
    {
        qDebug() << "Error: !m_nodes->select()";
        return;
    }
}

int DataModel::addNodeToModel(QString name, QString typeName, int bright, int status, QString author, QString rem, int id)
{
    qDebug() << "start addNodeToModel: " << name << "(" << typeName << ")";
    int row = 0;
    m_nodes->insertRows(row, 1);

    if( !m_nodes->setData(m_nodes->index(row, nodes_id_node_type), dataModel->getNodeTypeId(typeName) ))
    {
        qDebug() << "ERROR in line " << __LINE__ << ": " << "False setDatam_nodes->index(row, nodes_id_node_type)";
        m_nodes->revertAll();
        return -1;
    }

    if( !m_nodes->setData(m_nodes->index(row, nodes_node_name), name))
    {
        qDebug() << "ERROR in line " << __LINE__ << ": " <<  "False setDatam_nodes->index(row, nodes_node_name)";
        m_nodes->revertAll();
        return -1;
    }

    if( !m_nodes->setData(m_nodes->index(row, nodes_brigh), bright))
    {
        qDebug() << "ERROR in line " << __LINE__ << ": " <<  "False setDatam_nodes->index(row, nodes_brigh)";
        m_nodes->revertAll();
        return -1;
    }

    if( !m_nodes->setData(m_nodes->index(row, nodes_status), status))
    {
        qDebug() << "ERROR in line " << __LINE__ << ": " <<  "False setDatam_nodes->index(row, nodes_status)";
        m_nodes->revertAll();
        return -1;
    }

    if( !m_nodes->setData(m_nodes->index(row, nodes_author), author ))
    {
        qDebug() << "ERROR in line " << __LINE__ << ": " <<  "False setDatam_nodes->index(row, nodes_author)";
        m_nodes->revertAll();
        return -1;
    }

    if( !m_nodes->setData(m_nodes->index(row, nodes_rem), rem ))
    {
        qDebug() << "ERROR in line " << __LINE__ << ": " <<  "False setDatam_nodes->index(row, nodes_rem)";
        m_nodes->revertAll();
        return -1;
    }

    qDebug() << "normal stop addNodeToModel";
    return 0;
}

bool DataModel::submitAll()
{
    submitEdges();
    submitNodes();
    return true;
}

bool DataModel::submitNodes()
{
    if ( !m_nodes->submitAll() )
    {
        qDebug() << QString::fromLocal8Bit("Ошибка записи в базу данных: ") << db.lastError().text();
        return false;
    }
    else
        qDebug() << "Record added...";

    return true;
}

int DataModel::addNodeToDb(int nodeTypeId, QString nodeName)
{
    qDebug() << "addNodeToDb: " << nodeName << " type= " << nodeTypeId;
    QSqlQuery queryAddNode;
    queryAddNode.prepare("INSERT INTO nodes (id_node_type, node_name, brightness, status) "
                         "VALUES (:id_node_type, :node_name, :brightness, :status)");

    queryAddNode.bindValue(":id_node_type", nodeTypeId);
    queryAddNode.bindValue(":node_name", nodeName);
    queryAddNode.bindValue(":brightness", 1);
    queryAddNode.bindValue(":status", 1);
    queryAddNode.exec();
    if ( !queryAddNode.isActive() )
    {
        qDebug() << "SQL Error: " << queryAddNode.lastError().text();
        return -1;
    }

    return 0;
}

int DataModel::getNodeParams(QString nodeName, int &id, int &typeId)
{
    QSqlQuery q;
    QString queryStr = QString("SELECT id, id_node_type FROM nodes WHERE node_name = '%1';").arg(nodeName);
    q.exec(queryStr);
    if ( !q.isActive() )
    {
        qDebug() << "SQL Error: " << q.lastError().text();
        return -1;
    }

    q.first();
    id = q.value(0).toInt();
    typeId = q.value(1).toInt();

    return 0;
}

int DataModel::addEdgeToDb(QString nodeSrcName, QString edgeTypeName, QString nodeDstName)
{
    qDebug() << "addEdgeToDb " << nodeSrcName << "-" << edgeTypeName << "-" << nodeDstName;

    int nodeSrcId, nodeSrcTypeId, nodeDstId, nodeDstTypeId, edgeTypeId;
    getNodeParams(nodeSrcName, nodeSrcId, nodeSrcTypeId);
    getNodeParams(nodeDstName, nodeDstId, nodeDstTypeId);

    qDebug() << nodeSrcId << " " << nodeSrcTypeId << " "<< nodeDstId << " "<< nodeDstTypeId;
    QSqlQuery q;
    q.prepare("SELECT id FROM edge_types WHERE edge_name = :edge_name AND id_node_type_src = :id_node_type_src AND id_node_type_dst = :id_node_type_dst");
    q.bindValue(":edge_name", edgeTypeName);
    q.bindValue(":id_node_type_src", nodeSrcTypeId);
    q.bindValue(":id_node_type_dst", nodeDstTypeId);
    q.exec();
    if ( !q.isActive() )
    {
        qDebug() << "SQL Error: " << q.lastError().text();
        return -1;
    }
    q.first();
    edgeTypeId = q.value(0).toInt();

    qDebug() << edgeTypeId;


    //todo: затычка! надо реализовывать проверку средствами СУБД
    QSqlQuery qTest;
    QString qTestStr = QString("SELECT id FROM edges WHERE id_node_src = "
                               "'%1' AND id_node_dst = '%2' AND id_edge_type = '%3';").arg(nodeSrcId).arg(nodeDstId).arg(edgeTypeId);
    qTest.exec(qTestStr);
    if ( !q.isActive() )
    {
        qDebug() << "SQL Error: " << qTest.lastError().text();
        return -1;
    }
    if (qTest.size() > 0)
    {
        qDebug() << "qTest.size() > 0 = " << qTest.size();
        return -1;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO edges (id_node_src, id_edge_type, id_node_dst) "
                  "VALUES (:id_node_src, :id_edge_type, :id_node_dst)");


    query.bindValue(":id_node_src", nodeSrcId);
    query.bindValue(":id_edge_type", edgeTypeId);
    query.bindValue(":id_node_dst", nodeDstId);
    query.exec();
    if ( !query.isActive() )
    {
        qDebug() << "SQL Error: " << query.lastError().text();
        return -1;
    }

    return 0;
}

bool DataModel::exportFromGost(QString fileName)
{
    //TODO параметры обработки не должны браться из формы
    QList<GostBlock> depListOne;

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "GOST file not open";
        return false;
    }

    QString stringAll;
    QByteArray line;

    while (!file.atEnd())
    {
        line = file.readLine();
        stringAll.append( QString::fromLocal8Bit(line) );
    }

    file.close();

    // нумерация подразделов
    QString reStr_Number = vzsetNumber;
    int posBeginSection = -1;
    int posEndSection = -1;

    // Строковые идентификаторы концептов
    QString reStr_StringId = vzsetStringId;

    // Начало блока перечисления отношений иерархичности
    QString reStr_HierarhRole = vzsetHierarhRole;//ui->leHierarhRole->text();
    int posHierRole = -1;// позиция строки "Иерархический для:"

    // Начало блока перечисления отношений зависимости
    QString reStr_DependRole = vzsetDependRole;//ui->leDependRole->text();
    int posDependRole = -1;

    QRegExp re_number(reStr_Number);
    if ( !re_number.isValid() )
    {
        qDebug() << "re_number not valid";
        return false;
    }
    qDebug() << "re_number.captureCount()" << re_number.captureCount();

    QRegExp re_conceptId(reStr_StringId);
    if ( !re_conceptId.isValid() )
    {
        qDebug() << "re_conceptId not valid";
        return false;
    }

    //QString nodeTypeName = QString::fromLocal8Bit("действие");
    //QString nodeTypeNameAction = QString::fromLocal8Bit("действие");

    //QString edgeTypeName = QString::fromLocal8Bit("аппроксимирует");
    //QString edgeTypeNameApp = QString::fromLocal8Bit("аппроксимирует");
    QString edgeTypeNameApp = "аппроксимирует";
    QString edgeTypeNameSostIs = "состоит из";
    QString edgeTypeNameSledZa = "следует за";

    // находим все стр.идентификаторы концептов
    // заносим в список
    // заносим в БД: концепты + ребра иерархии (компонент->семейство->класс->фтреб)

    qDebug() << "Concept list:";
    QStringList conceptStrings;
    int pos = 0;


    int idNodeType_Action = 1;// TODO: брать из БД по имени

    int nodeNameActionIdInt = 13;
    QString nodeNameActionIdStr = "13"; //id узла "действие"
                                        //TODO: взять имя из настроек импорта, взять по этому имени id из БД

    int edgeNameActAppActIdInt = 23;
    QString edgeNameActAppActIdStr = "23"; //id ребра "действие -> аппроксимирует -> действие"
                                           //TODO: взять имя из настроек импорта, взять по этому имени id из БД

    int quantIdInt = 1;
    QString quantIdStr = "1"; //id кваетора "все-все"
                              //TODO: взять имя из настроек импорта, взять по этому имени id из БД

    while ((pos = re_conceptId.indexIn(stringAll, pos)) != -1)
    {

        QString value = re_conceptId.cap(0);
        QString ftrebStr = value.mid(0, 1);
        int ftrebId;
        if(!conceptStrings.contains(ftrebStr))
        {
            qDebug() << __LINE__ << ": " <<ftrebStr;
            conceptStrings.append(ftrebStr);

            //addNodeToDb(idNodeType_Action, ftrebStr);
            if ( !insertNode(nodeNameActionIdStr, ftrebStr, "1", "1", "autoimport", "", ftrebId) )
                return false;
        }

        QString fclassStr = value.mid(0, 3);
        int fclassId;
        if(!conceptStrings.contains(fclassStr))
        {
            qDebug() << __LINE__ << ": " <<fclassStr;
            conceptStrings.append(fclassStr);
            //addNodeToDb(idNodeType_Action, fclassStr);
            if ( !insertNode(nodeNameActionIdStr, fclassStr, "1", "1", "autoimport", "", fclassId) )
                return false;
            //addEdgeToDb(fclassStr, edgeTypeNameApp, ftrebStr);
            //bool insertEdge(QString id_node_src, QString id_edge_type,QString id_node_dst,QString quant_id,QString capacity,QString status,QString isNew);
            if(! insertEdge( QString::number(fclassId),
                             edgeNameActAppActIdStr,
                             QString::number(ftrebId),
                             quantIdStr, "0", "0", "0"))
                return false;

        }

        QString ffamilStr = value.mid(0, 7);
        int ffamilId;
        if(!conceptStrings.contains(ffamilStr))
        {
            qDebug() << __LINE__ << ": " <<ffamilStr;
            conceptStrings.append(ffamilStr);
            //addNodeToDb(idNodeType_Action, ffamilStr);
            if ( !insertNode(nodeNameActionIdStr, ffamilStr, "1", "1", "autoimport", "", ffamilId) )
                return false;
            //addEdgeToDb(ffamilStr, edgeTypeNameApp, fclassStr);
            if(! insertEdge( QString::number(ffamilId),
                             edgeNameActAppActIdStr,
                             QString::number(fclassId),
                             quantIdStr, "0", "0", "0"))
                return false;
        }


        QString fcompStr = value.mid(0, 9);
        int fcompId;
        if(!conceptStrings.contains(fcompStr))
        {
            qDebug() << __LINE__ << ": " <<fcompStr;
            conceptStrings.append(fcompStr);
            //addNodeToDb(idNodeType_Action, fcompStr);
            if ( !insertNode(nodeNameActionIdStr, fcompStr, "1", "1", "autoimport", "", fcompId) )
                return false;
            ////addEdgeToDb(fcompStr, edgeTypeNameApp, ffamilStr);
            if(! insertEdge( QString::number(fcompId),
                             edgeNameActAppActIdStr,
                             QString::number(ffamilId),
                             quantIdStr, "0", "0", "0"))
                return false;
        }

        pos += re_conceptId.matchedLength();
    }

    qDebug() << "Concept list:";
    for (int i = 0; i < conceptStrings.size(); ++i)
    {
        qDebug() << conceptStrings.at(i);
    }

    pos = 0;
    qDebug() << "Section:";
    do
    {
        qDebug() << "Section HIERARH:---------------------------";
        posHierRole = stringAll.indexOf(reStr_HierarhRole, pos);//"Иерархический для:"
        if (posHierRole == -1)
        {
            break;
        }

        posBeginSection = re_number.lastIndexIn(stringAll, posHierRole);
        if (posBeginSection == -1)
        {
            break;
        }

        posEndSection = re_number.indexIn(stringAll, posHierRole);
        if (posEndSection == -1)
        {
            break;
        }
        pos = posEndSection;


        int sectionSize = posEndSection - posBeginSection;
        QString section = stringAll.mid(posBeginSection, sectionSize);
        posHierRole = posHierRole - posBeginSection;
        posDependRole = section.indexOf(reStr_DependRole, posHierRole);


        qDebug() << "-----------------------------------";
        qDebug() << section;
        qDebug() << "";
        qDebug() << "Парсинг секции:";
        qDebug() << "";

        QString sectionConcept = section.mid(0, posHierRole);
        qDebug() << "sectionConcept= " + sectionConcept;

        int posC = re_conceptId.indexIn(sectionConcept, 0);
        if (posC == -1)
        {
            qDebug() << "------------parsing error: Ошибка документа, концепт не найден";
            qDebug() << "Ошибка документа, концепт не найден";
        }
        QString concept = re_conceptId.cap(0);
        qDebug() << "concept= " + concept;
        QString conceptLongName = sectionConcept.mid(posC + concept.length());
        qDebug() << "conceptLongName= " + conceptLongName;
        qDebug() << "";

        QString sectionHier = section.mid(posHierRole, posDependRole - posHierRole);
        qDebug() << "sectionHier= " + sectionHier;
        posC = 0;
        int cnt = 0;
        while ((posC = re_conceptId.indexIn(sectionHier, posC)) != -1)
        {
            cnt++;
            QString hier = re_conceptId.cap(0);
            qDebug() << "hier_concept= " + hier;
            posC += re_conceptId.matchedLength();
            addEdgeToDb(concept, edgeTypeNameApp, hier);
        }

        if (cnt == 0)
        {
           QString parentNodeName = concept.mid(0, 7);
           addEdgeToDb(concept, edgeTypeNameApp, parentNodeName);
           qDebug() << "add hierarh edge: " << concept <<"->" << edgeTypeNameApp<< "->" << parentNodeName;
        }

        qDebug() << "";

        qDebug() << "Section HIERARH --------------------------- END";
        qDebug() << "Section DEPEND: parsing------------------------";


        QString sectionDepend = section.mid(posDependRole + reStr_DependRole.size(), sectionSize - posDependRole);
        qDebug() << "sectionDepend= " + sectionDepend;

        posC = 0;
        while ((posC = re_conceptId.indexIn(sectionDepend, posC)) != -1)
        {
            QString depend = re_conceptId.cap(0);
            qDebug() << "depend_concept= " + depend;
            posC += re_conceptId.matchedLength();
        }

        // построение дерева

        int posBlock = 0;
        int blocksOrLen = 0;
        QRegExp re_blockOR("\\[([^\\]])+\\]");
        do
        {
            posBlock = re_blockOR.indexIn(sectionDepend, posBlock);
            if(posBlock == -1)
            {
                break;
            }
            QString blockStr = re_blockOR.cap(0);

            qDebug() << "";
            qDebug() << "block= " + blockStr;
            GostBlock gostBlock;
            gostBlock.setIsOr();
            QString gostBlockName = "[";

            int posSubBlock = 0;
            while ((posSubBlock = re_conceptId.indexIn(blockStr, posSubBlock)) != -1)
            {
                QString subBlock = re_conceptId.cap(0);
                qDebug() << "    subBlock= " + subBlock;
                gostBlockName += subBlock + " OR ";
                gostBlock.addSubBlock(subBlock);
                posSubBlock += re_conceptId.matchedLength();
            }

            gostBlockName = gostBlockName.left(gostBlockName.size() - 4) + "]";
            qDebug() << "gostBlockName= " + gostBlockName;
            gostBlock.setName(gostBlockName);
            depListOne.append(gostBlock);

            posBlock += re_blockOR.matchedLength();
            blocksOrLen += re_blockOR.matchedLength();
        }
        while(posBlock != -1);

        int posSubBlock = blocksOrLen;
        while ((posSubBlock = re_conceptId.indexIn(sectionDepend, posSubBlock)) != -1)
        {
            QString block = re_conceptId.cap(0);
            qDebug() << "block 2= " + block;
            GostBlock gostBlock;
            gostBlock.setIsAnd();
            gostBlock.setName(block);
            depListOne.append(gostBlock);
            posSubBlock += re_conceptId.matchedLength();
        }

        qDebug() << "________________________________________________";
        qDebug() << "================================================";


        if(depListOne.size() == 0)
        {
            qDebug() << "Not DEPENDENCES";
            continue;
        }

        QString nodeA_, nodeA_dep;
        // добавить узлы
        nodeA_ = concept + "_a";
        qDebug() << __LINE__ <<": " <<  "Add node: " << nodeA_;
        addNodeToDb(idNodeType_Action, nodeA_);


        nodeA_dep = concept + "_depends";
        qDebug() << __LINE__ <<": " <<  "Add node: " << nodeA_dep;
        addNodeToDb(idNodeType_Action, nodeA_dep);

        // добавить ребра
        // concept->nodeA_
        qDebug() << __LINE__ <<": " <<  "Add edge: " << concept << " -состоит из-> " << nodeA_;
        addEdgeToDb(concept, edgeTypeNameSostIs, nodeA_);

        // concept->nodeA_dep
        qDebug() << __LINE__ <<": " <<  "Add edge: " << concept << " -состоит из-> " << nodeA_dep;
        addEdgeToDb(concept, edgeTypeNameSostIs, nodeA_dep);

        // nodeA_->nodeA_dep
        qDebug() << __LINE__ <<": " <<  "Add edge: " << nodeA_ << " -следует за-> " << nodeA_dep;
        addEdgeToDb(nodeA_, edgeTypeNameSledZa, nodeA_dep);


        for ( int i = 0; i < depListOne.size(); i++)
        {
            GostBlock gb = depListOne.at(i);
            //qDebug() << gb.name();
            if(gb.isOr())
            {
                qDebug() << __LINE__ <<": " << "Add node: " << gb.name();
                addNodeToDb(idNodeType_Action, gb.name());

                // добавить узел gb.name();
                QStringList *sl = gb.getSubBlocks();
                for (int j = 0; j < sl->size(); j++)
                {
                    // добавить ребро от gb.name()-> апп -> sl->at(j)
                    qDebug() << __LINE__ <<": " <<  "Add edge: " << gb.name() << " ->approx-> " << sl->at(j);
                    addEdgeToDb(gb.name(), edgeTypeNameApp, sl->at(j));
                }

            }

        }


        if( depListOne.size() == 1 )
        {
            qDebug() << "bez perest " << __LINE__ <<": " <<   "depListOne.size() == 1";
            // добавить ребро "апп" от nodeA_dep к depListOne.at(0)
            GostBlock gb = depListOne.at(0);
            qDebug() << __LINE__ <<": " <<   "Add edge: " << nodeA_dep << " ->approx-> " << gb.name();
            addEdgeToDb(nodeA_dep, edgeTypeNameApp, gb.name());
        }
        else
        // формируем перестановки и тд
        {
            qDebug() << "s perest " <<__LINE__;
            Perestanovka *p = pers[depListOne.size() - 2];
            int n = p->getN();
            int perCnt = p->getPerCount();
            qDebug() << "depListOne.size()= " << QString::number(depListOne.size());
            qDebug() << "n = p->getN()= " << QString::number(n = p->getN());
            qDebug() << "perCnt = p->getPerCount()= " << QString::number(perCnt);
            for(int j = 0; j < perCnt; j++)
            {
                //qDebug() << __LINE__;
                int* per = p->getPer(j);
                QString perStr = "(";
                for(int k = 0; k < n; k++)
                {
                    int index = per[k];
                    GostBlock gb = depListOne.at(index-1);
                    perStr += gb.name() + "->";
                }
                perStr = perStr.left(perStr.size() - 2) + ")";
                qDebug() << __LINE__ <<": " <<   "Add node: " << perStr;
                addNodeToDb(idNodeType_Action, perStr);

                //qDebug() << __LINE__;
                QStringList layerPP;
                for(int k = 0; k < n; k++)
                {
                    int index = per[k];
                    GostBlock gb = depListOne.at(index-1);
                    QString nodePP = gb.name();
                    nodePP += perStr;
                    qDebug() << __LINE__ <<": " <<  "Add node: " << nodePP;
                    addNodeToDb(idNodeType_Action, nodePP);
                    layerPP.append(nodePP);
                }

                QStringList::iterator it = layerPP.begin();
                do
                {
                    QString node1 = *it;
                    it++;
                    if(it == layerPP.end())
                        break;
                    QString node2 = *it;

                    qDebug() << __LINE__ <<": Add edge " <<   node1 << "->sled_za->" << node2;
                    addEdgeToDb(node1, edgeTypeNameSledZa, node2);
                }
                while(1);
            }

        }

        depListOne.clear();

        qDebug() << "________________________________________________";
        qDebug() << "================================================";
        qDebug() << "";
    }
    while(true);

    dataModel->selectNodesAndEdges();

    return true;
}

/*
 * РїСЂРёРјРµСЂ РёСЃРїРѕР»СЊР·РѕРІР°РЅРёСЏ РїРµСЂРµСЃС‚Р°РЅРѕРІРѕРє
for(int i = 2; i < 6; i++)
{
    qDebug() << "Perest " << i << "-------------------------";
    Perestanovka *p = pers[i-2];
    int n = p->getN();
    int perCnt = p->getPerCount();
    for(int j = 0; j < perCnt; j++)
    {
        int* per = p->getPer(j);
        QString perStr = "";
        for(int k = 0; k < n; k++)
        {
            int index = per[k];
            perStr += QString::number(index) + " ";
        }
        qDebug() << perStr << endl;
    }
}
*/
QString getNodeName(QString num)
{
    QSqlQuery q;
    QString str;
    str="SELECT node_name FROM nodes WHERE id="+num;
    q.exec(str);
    q.next();
    return q.value(0).toString();
}


QString getEdgeName(QString num)
{
    QSqlQuery q;
    QString str;
    str="SELECT edge_name FROM edge_types WHERE id="+num;
    q.exec(str);
    q.next();
    return q.value(0).toString();
}
QString getShape(QString num)
{
    QSqlQuery q;
    QString str;
    str="SELECT figure FROM node_types WHERE id="+num;
    q.exec(str);
    q.next();
    return q.value(0).toString().split(";").at(0);
}
QString getColor(QString num)
{
    QSqlQuery q;
    QString str;
    str="SELECT figure FROM node_types WHERE id="+num;
    q.exec(str);
    q.next();
    return q.value(0).toString().split(";").at(1);
}

bool DataModel::simpleExportDot(QString fileName)
{
    QFile file(fileName);
    QTextStream stream(&file);
    if (!file.open(QIODevice::WriteOnly|QIODevice::Text))
    {
        qDebug() << "error open file";
        return false;
    }
    stream<<"digraph G{"<<endl;
    QSqlQuery q;
    QString str;

    str="SELECT id_node_type,node_name FROM nodes";
    if(q.exec(str))
        while(q.next())
        {
            stream<<"\""+q.value(1).toString()+"\"";
            stream<<"[shape=\""<<getShape(q.value(0).toString())<<"\",";
            stream<<"style=\"filled\",fillcolor=\""+getColor(q.value(0).toString())+"\"];"<<endl;
        }

    str="SELECT id_node_src,id_edge_type,id_node_dst FROM edges WHERE status=0";
    if(q.exec(str))
        while(q.next())
        {
            stream<<"\""+getNodeName(q.value(0).toString())+"\""<<"->"<<"\""+getNodeName(q.value(2).toString())+"\";"<<endl;

        }

    else
    {
        qDebug()<<q.lastError().text();
    }
    str="SELECT id_node_src,id_edge_type,id_node_dst FROM edges WHERE status=1";
    if(q.exec(str))
        while(q.next())
        {
            stream<<"\""+getNodeName(q.value(0).toString())+"\""<<"->"<<"\""+getNodeName(q.value(2).toString())+"\"";
            stream<<"[style=\"dotted\"];"<<endl;
        }

    else
    {
        qDebug()<<q.lastError().text();
    }
    stream<<"}";
    file.close();
    str="bat.bat "+fileName;
    QProcess dot;
    dot.start(str);
    dot.waitForFinished(-1);
    qDebug()<<"finish iconv"<<dot.exitCode();
    fileName.remove(fileName.length()-3,3);
    dot.start("graphviz\\dot.exe -Tpng -o"+fileName+"png temp.dot");
    dot.waitForFinished(-1);
    qDebug()<<"finish dot"<<dot.exitCode();
    return true;
}
bool DataModel::importToDot(QString fileName)
{

    SettingsForm settingsform;
    qDebug() << "Save to File" << endl;
    QString buffer = "digraph G\{";

    QFile file(fileName);
    QTextStream stream(&file);
    if (!file.open(QIODevice::WriteOnly|QIODevice::Text))
    {
        qDebug() << "error open file";
        return false;
    }

    stream << "digraph G" << endl;
    stream << "{" << endl;
    stream << "rankdir=BT;" << endl;
    stream << "node[color=black,fontsize=12];" << endl;
     stream << "edge[color=black,fontsize=12];" << endl;

    //int numRows;
    QStringList node_ids,node_names;
    QSqlQuery q;
    QString queryStr = "SELECT id_node_type FROM nodes"; //РџРѕР»СѓС‡Р°РµРј Type ID СѓР·Р»РѕРІ
    q.exec(queryStr);
    if ( !q.isActive() )
    {
        qDebug() << "SQL Errorq: " << q.lastError().text();
        return -1;
    }
    q.result();
    while (q.next())

        {
        node_ids << q.value(0).toString();
        }
    QSqlQuery q1;
    QString queryStr1 = QString("SELECT node_name FROM nodes"); //РџРѕР»СѓС‡Р°РµРј РРјРµРЅР° СѓР·Р»РѕРІ
    q1.exec(queryStr1);
        if (!q1.isActive())
        {
            qDebug() << "SQL Errorq1: " << q1.lastError().text();
            return -1;
        }
        q1.first();

        while(q1.next())
        {
            node_names.push_back(q1.value(0).toString());
        }

    qDebug() << "nodes ------------------";
    for (int row = 0; row < node_names.size();++row)
    {

        QString name = node_names.at(row);
        qDebug()<<name;
        QSqlQuery q2;
        QString queryStr2 = QString("SELECT node_type_name FROM node_types WHERE id="+node_ids.at(row)); //РџРѕР»СѓС‡Р°РµРј С‚РёРї СѓР·Р»Р°
            if (!q2.exec(queryStr2))
            {
                qDebug() << "SQL Errorq2: " << q2.lastError().text();
                return -1;
            }
            q2.first();

        QString name2 = q2.value(0).toString();
        qDebug()<<name2;
        QSqlQuery q3;

        QString queryStr3 = QString("SELECT status FROM nodes WHERE node_name = '%1';").arg(name); //РџРѕР»СѓС‡Р°РµРј СЃС‚Р°С‚СѓСЃ СѓР·Р»Р°
            if (!q3.exec(queryStr3))
            {
                qDebug() << "SQL Errorq3: " << q3.lastError().text();
                return -1;
            }
            q3.first();
        QString status = q3.value(0).toString();
        qDebug() << status;

        QString color,colorfont,colorbackground,font_bold,font_italic,figure,fontsize,style;
        settingsform.settings->beginGroup("uzel");
        if(settingsform.settings->contains(name2))
            {
            settingsform.settings->endGroup();
            color = settingsform.settings->value("uzel/"+name2).toString();
               colorbackground = settingsform.settings->value("colorbg/"+name2).toString();
               figure = settingsform.settings->value("uzelfig/"+name2).toString();
               colorfont = settingsform.settings->value("colorfont/"+name2).toString();
               fontsize = settingsform.settings->value("uzelfontcount/"+name2).toString();
               if(status == "0")
               {
                   style = "filled";
               }
               else
               {
                   style = "dotted";
               }
               if(settingsform.settings->value("uzelfontitalic/"+name2).toString()=="1")
               {
                   font_italic = "italic";

               }
               else
               {
                   font_italic = "";
               }
               if(settingsform.settings->value("uzelfontbold/"+name2).toString()=="1")
               {
                   font_bold = "bold";


               }
               else
               {
                   font_bold = "";
               }

            }
            else
            {
            color = "black";
            style ="filled";
            font_bold = "";
            font_italic = "";
            colorbackground="white";
            figure="ellipse";
            colorfont="black";
            fontsize="14";
        }
        stream << "\"" << name << "\"" << "[shape="+figure+",style="+style+",fillcolor=\""+colorbackground+"\",color=\""+color<<"\",fontsize="+fontsize<<",fontname=\"times "+font_bold+" "+font_italic+"\""+",fontcolor=\""+colorfont+"\"];"<<endl;

    }




    qDebug() << "edges ------------------";
    QStringList id_node_src,id_node_dst,id_edge_type;
    QSqlQuery qedges;
    QSqlRecord  record;
    QString query_edges = "SELECT id_node_src, id_node_dst, id_edge_type FROM edges";
    qedges.exec(query_edges);
    if ( !qedges.isActive() )
    {
        qDebug() << "SQL Errorq: " << qedges.lastError().text();
        return -1;
    }
    record = qedges.record();
    while (qedges.next())
       {
        id_node_src << qedges.value(record.indexOf("id_node_src")).toString();
        id_node_dst << qedges.value(record.indexOf("id_node_dst")).toString();
        id_edge_type << qedges.value(record.indexOf("id_edge_type")).toString();
        }
    for(int i =0; i< id_node_src.size();i++)
    {
    qDebug() << id_node_src.at(i);
    qDebug() << id_node_dst.at(i);

    }
    QString rebr,rebr_font_colors,rebr_names,rebr_font_bold,rebr_font_italic,rebr_font_size,rebr_type,rebr_type_str;

    for (int row = 0; row < id_node_src.size(); ++row)
    {


        QSqlQuery qnodesrc;
        QString query_qnodesrc = "SELECT node_name FROM nodes WHERE id="+id_node_src.at(row);
        qnodesrc.exec(query_qnodesrc);
        if ( !qnodesrc.isActive() )
        {
            qDebug() << "SQL Errorq: " << qnodesrc.lastError().text();
            return -1;
        }
        qnodesrc.first();
        QString nodeSrcName = qnodesrc.value(0).toString();
        QSqlQuery edgestype;
        QString query_edgestype = "SELECT edge_name FROM edge_types WHERE id="+id_edge_type.at(row);
        edgestype.exec(query_edgestype);
        if ( !edgestype.isActive() )
        {
            qDebug() << "SQL Errorq: " << edgestype.lastError().text();
            return -1;
        }
        edgestype.first();
        QString typeName = edgestype.value(0).toString();
        QSqlQuery qnodedst;
        QString query_qnodedst = "SELECT node_name FROM nodes WHERE id="+id_node_dst.at(row);
        qnodedst.exec(query_qnodedst);
        if ( !qnodedst.isActive() )
        {
            qDebug() << "SQL Errorq: " << qnodedst.lastError().text();
            return -1;
        }
        qnodedst.first();
        QString nodeDstName = qnodedst.value(0).toString();
        int id1,id2,id3;
             QSqlQuery queryid_node_type;
                  if (!queryid_node_type.exec("SELECT nodes.id FROM nodes WHERE (((node_name) =\'"+nodeSrcName+"\'));"))


                  {
                      //qDebug().noquote()<< "SELECT nodes.id FROM nodes WHERE (((node_name) =\'"+nodeSrcName+"\'));";

                      //QMessageBox::information(0, "SQL INSERT:", queryid_node_type.lastError().text());
                      dataModel->logMessage(queryid_node_type.lastError().text());
                  }
                  while (queryid_node_type.next()) {
                           id2 = queryid_node_type.value(0).toInt();
                       }
                  QSqlQuery queryid_node_type_to;
                       if (!queryid_node_type_to.exec("SELECT nodes.id FROM nodes WHERE (((node_name) ='"+nodeDstName+"'));"))


                       {
                           //qDebug().noquote()<< "SELECT nodes.id FROM nodes WHERE (((node_name) ='"+nodeDstName+"'));";

                           //QMessageBox::information(0, "SQL INSERT:", queryid_node_type_to.lastError().text());
                           dataModel->logMessage(queryid_node_type_to.lastError().text());

                       }
                       while (queryid_node_type_to.next()) {
                                id3 = queryid_node_type_to.value(0).toInt();
                            }
                       QString from;from.setNum(id2);
                       QString to;to.setNum(id3);
        QString status_edge;
        QSqlQuery querystatus;
             if (!querystatus.exec("SELECT edges.status FROM edges WHERE (((edges.id_node_src)="+from+") AND ((edges.id_node_dst)="+to+"));"))
                 {
                    //QMessageBox::information(0, "SQL INSERT:", querystatus.lastError().text());
                    dataModel->logMessage(querystatus.lastError().text());

                 }
             while (querystatus.next()) {
                      status_edge = querystatus.value(0).toString();
                  }


        settingsform.settings_edges->beginGroup("rebr");
        bool s = false;
            if (settingsform.settings_edges->contains(typeName))
            {
                settingsform.settings_edges->endGroup();
                s = true;

            rebr_names=settingsform.settings_edges->value("rebr_names/"+typeName).toString();
            rebr = settingsform.settings_edges->value("rebr/"+typeName).toString();
            rebr_font_colors = settingsform.settings_edges->value("rebr_font_colors/"+typeName).toString();
            if(status_edge == "0")
            {
                rebr_type = settingsform.settings_edges->value("rebr_type/"+typeName).toString();
            }
            else
            {
                rebr_type = "dotted";
            }
            if(settingsform.settings_edges->value("rebr_font_italic/"+typeName).toString()=="1")
            {
                rebr_font_bold="bold";
            }
            else
            {
                rebr_font_bold="";
            }
            if(settingsform.settings_edges->value("rebr_font_italic/"+typeName).toString()=="1")
            {
                rebr_font_italic="italic";
            }
            else
            {
                rebr_font_italic="";
            }
            rebr_font_size = settingsform.settings_edges->value("rebr_font_size/"+typeName).toString();

            }
            else
            {
            settingsform.settings_edges->endGroup();

            }

                       QSqlQuery queryid_quant_type;
                            if (!queryid_quant_type.exec("SELECT edges.quant_id FROM edges WHERE (((edges.id_node_src)="+from+") AND ((edges.id_node_dst)="+to+"));"))


                            {
                                //qDebug().noquote()<< "SELECT edges.quant_id FROM edges WHERE (((edges.id_node_src)="+from+") AND ((edges.id_node_dst)="+to+"));";

                                //QMessageBox::information(0, "SQL INSERT:", queryid_quant_type.lastError().text());
                                dataModel->logMessage(queryid_quant_type.lastError().text());
                            }
                            while (queryid_quant_type.next()) {
                                     id1 = queryid_quant_type.value(0).toInt();
                                 }
                            QSqlQuery queryid_quant_name;
                            QString name,Id1;
                            Id1.setNum(id1);
                                 if (!queryid_quant_name.exec("SELECT quantifiers.quant_name FROM quantifiers WHERE (((quantifiers.id)="+Id1+"));"))


                                 {
                                     //qDebug().noquote()<< "SELECT edges.quant_id FROM edges WHERE (((edges.id_node_src)="+from+") AND ((edges.id_node_dst)="+to+"));";

                                     //QMessageBox::information(0, "SQL INSERT:", queryid_quant_name.lastError().text());
                                     dataModel->logMessage(queryid_quant_name.lastError().text());

                                 }
                                 while (queryid_quant_name.next()) {
                                          name = queryid_quant_name.value(0).toString();
                                      }

                                 rebr_type_str = settingsform.settings_edges->value("rebr_type_str/"+name ).toString();
                                 QString head_str,tail_str;
                                 head_str = rebr_type_str.left(rebr_type_str.indexOf("_"));
                                 tail_str = rebr_type_str.right(rebr_type_str.size()-rebr_type_str.indexOf("_")-1);
        if(s==true)
        {
        stream << "\"" << nodeSrcName << "\""
               << "->"
               << "\"" << nodeDstName << "\""
               << "[dir=\"both\",color=\""+rebr+"\",label=\""+rebr_names+"\",style="+rebr_type+",arrowhead="+head_str+",arrowtail="+tail_str+",fontname=times "+rebr_font_bold+" "+rebr_font_italic+",fontsize="+rebr_font_size+",fontcolor=\""+rebr_font_colors+"\"];"<< endl;
        }
        else
        {
            stream << "\"" << nodeSrcName << "\""
                   << "->"
                   << "\"" << nodeDstName << "\""
                   << "[];"<< endl;
        }
    }

    stream << "}" << endl;
    file.close();
    QProcess dot;
    fileName.remove(fileName.length()-3,3);
    dot.start("graphviz\\bin\\dot.exe -Tpng -o"+fileName+"png "+fileName+"dot");
    dot.waitForFinished(-1);
    qDebug()<<fileName;
    qDebug()<<"finish"<<dot.exitCode();

    return true;
}



bool DataModel::submitEdges()
{
    if ( !m_edges->submitAll() )
    {
        qDebug() << QString::fromLocal8Bit("РћС€РёР±РєР° Р·Р°РїРёСЃРё РІ Р±Р°Р·Сѓ РґР°РЅРЅС‹С…: ") << db.lastError().text();
        return false;
    }
    else
        qDebug() << "Record added...";

    return true;
}

void DataModel::createDafGraphTypes()
{
    int numRows;

    qDebug() << "nodeTypes ------------------";
    numRows= m_nodeTypes->rowCount(QModelIndex());
    for (int row = 0; row < numRows; ++row)
    {
        QModelIndex index = m_nodeTypes->index(row, nodetype_id, QModelIndex());
        int id = m_nodeTypes->data(index, Qt::DisplayRole).toInt();

        index = m_nodeTypes->index(row, nodetype_name, QModelIndex());
        QString name = m_nodeTypes->data(index, Qt::DisplayRole).toString();
        qDebug() << name;
        m_graph->addNodeType(name, id);
    }
    qDebug() << "edgeTypes ------------------";

    numRows = m_edgeTypes->rowCount(QModelIndex());
    for (int row = 0; row < numRows; ++row)
    {
        QModelIndex index;
        index = m_edgeTypes->index(row, edgetype_id, QModelIndex());
        int id = m_edgeTypes->data(index, Qt::DisplayRole).toInt();

        index = m_edgeTypes->index(row, edgetype_srctype, QModelIndex());
        QString srcTypeName = m_edgeTypes->data(index, Qt::DisplayRole).toString();

        index = m_edgeTypes->index(row, edgetype_name, QModelIndex());
        QString name = m_edgeTypes->data(index, Qt::DisplayRole).toString();

        index = m_edgeTypes->index(row, edgetype_dsttype, QModelIndex());
        QString dstTypeName = m_edgeTypes->data(index, Qt::DisplayRole).toString();

        //index = m_edgeTypes->index(row, edgetype_mult, QModelIndex());
        //QString mult = m_edgeTypes->data(index, Qt::DisplayRole).toString();

        qDebug() << name << " || " << srcTypeName << " || " << dstTypeName;
        m_graph->addEdgeType( name, srcTypeName, dstTypeName, "mult", id);
    }
}

void DataModel::createDafGraph()
{
    int numRows;

    qDebug() << "nodes ------------------";
    numRows = m_nodes->rowCount(QModelIndex());
    for (int row = 0; row < numRows; ++row)
    {
        QModelIndex index;

        index = m_nodes->index(row, nodes_id, QModelIndex());
        int id = m_nodes->data(index, Qt::DisplayRole).toInt();

        index = m_nodes->index(row, nodes_id_node_type, QModelIndex());
        QString nodeTypeName = m_nodes->data(index, Qt::DisplayRole).toString();

        index = m_nodes->index(row, nodes_node_name, QModelIndex());
        QString name = m_nodes->data(index, Qt::DisplayRole).toString();

        index = m_nodes->index(row, nodes_brigh, QModelIndex());
        int brigh = m_nodes->data(index, Qt::DisplayRole).toInt();

        index = m_nodes->index(row, nodes_status, QModelIndex());
        int status = m_nodes->data(index, Qt::DisplayRole).toInt();

        qDebug() << name << " || " << nodeTypeName;
        m_graph->addNode( name, nodeTypeName, brigh, status, id);
    }

    qDebug() << "edges ------------------";
    numRows = m_edges->rowCount(QModelIndex());
    for (int row = 0; row < numRows; ++row)
    {
        QModelIndex index;

        index = m_edges->index(row, edges_id, QModelIndex());
        int id = m_edges->data(index, Qt::DisplayRole).toInt();

        index = m_edges->index(row, edges_id_node_src, QModelIndex());
        QString nodeSrcName = m_edges->data(index, Qt::DisplayRole).toString();

        index = m_edges->index(row, edges_edge_type, QModelIndex());
        QString typeName = m_edges->data(index, Qt::DisplayRole).toString();

        index = m_edges->index(row, edges_id_node_dst, QModelIndex());
        QString nodeDstName = m_edges->data(index, Qt::DisplayRole).toString();
        qDebug() << typeName << " || " << nodeSrcName << " || " << nodeDstName;
        m_graph->addEdge( typeName, nodeSrcName, nodeDstName, id);
    }

}

void DataModel::deleteModel()
{
    if (m_nodeTypes != NULL) delete m_nodeTypes;
    if (m_edgeTypes != NULL) delete m_edgeTypes;
    if (m_nodes != NULL) delete m_nodes;
    if (m_edges != NULL) delete m_edges;
}

void DataModel::openDB(QWidget *q)
{
    sdw->show();
}

void DataModel::openDB_fromsettings(QWidget *q,QString nam)
{
    m_dbName = nam;
    qDebug() << __LINE__;
    connectDB(m_dbName);
    qDebug() << __LINE__;
    createModel();
    qDebug() << __LINE__;
    setupView();
    qDebug() << __LINE__;
}

bool DataModel::connectDB(const QString &dBName)
{
    if(db.isOpen())
    {
        disconnectDB();
        qDebug() << "    " <<__LINE__;
    }
    else
    {
        qDebug() << "    " <<__LINE__;
        db = QSqlDatabase::addDatabase("QODBC");
        QString fullDBName = "DRIVER={Microsoft Access Driver (*.mdb)};FIL={MS Access};DBQ=" + dBName;
        db.setDatabaseName(fullDBName);
    }
    if (!db.open()) {
        //QMessageBox::critical(this, QString::fromLocal8Bit("РћС€РёР±РєР°"), db.lastError().text());
        logMessage( QString::fromLocal8Bit("РћС€РёР±РєР° РїРѕРґРєР»СЋС‡РµРЅРёСЏ Рє Р±Р°Р·Рµ РґР°РЅРЅС‹С…: ")
                   + db.lastError().text());
        return false;
    }
//	types->re();

///////////////// Р‘Р°РєРёСЂРѕРІ РґР»СЏ Р°РєС‚РёРІР°С†РёРё С„РѕСЂРјС‹ Nodes//////////
    qDebug() << "    " <<__LINE__;
    emit m_formNodes->activate();
/////////////////////////////////////////////////////////////

///////////////// Р‘Р°РєРёСЂРѕРІ РґР»СЏ Р°РєС‚РёРІР°С†РёРё С„РѕСЂРјС‹ Edges//////////
    qDebug() << "    " <<__LINE__;
    emit m_formEdges->open_add();
/////////////////////////////////////////////////////////////

///////////////// Р‘Р°РєРёСЂРѕРІ РґР»СЏ Р°РєС‚РёРІР°С†РёРё С„РѕСЂРјС‹ Types//////////
    qDebug() << "    " <<__LINE__;
    emit m_formTypes->on_db_open();
/////////////////////////////////////////////////////////////

    qDebug() << "    " <<__LINE__;

    return true;
}
void DataModel::disconnectDB()
{
    qDebug() << "DataModel::disconnectDB() " <<__LINE__;
    db.close();
}

void DataModel::createModel()
{
    deleteModel();
    QSqlRelation relNodeTypes("node_types", "id", "node_type_name");
    m_nodes = new QSqlRelationalTableModel();
    m_nodes->setTable("nodes");
    m_nodes->setEditStrategy(QSqlTableModel::OnManualSubmit);
    m_nodes->setRelation(m_nodes->fieldIndex("id_node_type"), relNodeTypes);

    if(!m_nodes->select())
    {
        qDebug() << "Error: !m_nodes->select()";
        return;
    }

    m_nodeTypes = new QSqlTableModel();
    m_nodeTypes->setTable("node_types");
    m_nodeTypes->setEditStrategy(QSqlTableModel::OnManualSubmit);


    m_edgeTypes = new QSqlRelationalTableModel();
    m_edgeTypes->setTable("edge_types");
    m_edgeTypes->setEditStrategy(QSqlTableModel::OnManualSubmit);

    m_edgeTypes->setRelation(m_edgeTypes->fieldIndex("id_node_type_src") ,relNodeTypes);
    m_edgeTypes->setRelation(m_edgeTypes->fieldIndex("id_node_type_dst") ,relNodeTypes);
    edgeType = m_formRules->setEdgeTypeCB(m_edgeTypes);

    m_quantTypes = new QSqlTableModel();
    m_quantTypes->setTable("quantifiers");
    m_quantTypes->setEditStrategy(QSqlTableModel::OnManualSubmit);




    m_rules = new QSqlRelationalTableModel();
    m_rules->setTable("rules");
    m_rules->setEditStrategy(QSqlTableModel::OnManualSubmit);

    QSqlRelation relEdgeType("edge_types", "id", "edge_name");
    QSqlRelation relNodes("nodes", "id", "node_name");
    QSqlRelation relQuant("quantifiers", "id", "quant_name");

    m_edges = new QSqlRelationalTableModel();
    m_edges->setTable("edges");
    m_edges->setEditStrategy(QSqlTableModel::OnManualSubmit);
    m_edges->setRelation(m_edges->fieldIndex("id_edge_type"), relEdgeType);
    m_edges->setRelation(m_edges->fieldIndex("id_node_src"), relNodes);
    m_edges->setRelation(m_edges->fieldIndex("id_node_dst"), relNodes);
    m_edges->setRelation(m_edges->fieldIndex("quant_id"), relQuant);


    if (!m_nodeTypes->select())
    {
        qDebug() << "Error: !m_nodeTypes->select()";
        return;
    }
    if (!m_edgeTypes->select())
    {
        qDebug() << "Error: !m_edgeTypes->select()";
        return;
    }
    if (!m_quantTypes->select())
    {
        qDebug() << "Error: !m_quantTypes->select()";
        return;
    }

    if(!m_edges->select())
    {
        qDebug() << "Error: !m_edges->select()";
        return;
    }
}

QString DataModel::getDBName()
{
    return m_dbName;
}


void DataModel::setupView()
{
    m_formTypes->setNodeTypesModel(m_nodeTypes);
    m_formTypes->setEdgeTypesModel(m_edgeTypes);
    m_formTypes->setQuantTypesModel(m_quantTypes);
    m_formNodes->setNodesModel(m_nodes);
    m_formEdges->setEdgesModel(m_edges);
    m_formRules->setRulesModel(m_rules);
    m_formTypes->formnewedgetype->on_setup_view();
    m_formNodes->formnewnode->on_setup_view();
}

void DataModel::showFormTypes()
{
    m_formTypes->show();
}

void DataModel::showFormRules()
{
    m_formRules->show();
}

void DataModel::showFormNodes()
{
    m_formNodes->show();
}

void DataModel::showFormEdges()
{
    m_formEdges->show();
}

//Р±СѓРґРµС‚ СЂР°Р±РѕС‚Р°С‚СЊ РїРѕСЃР»Рµ РІС‹РїРѕР»РЅРµРЅРёСЏ createModel()
/*QList<Rule> DataModel::loadRules()
{
    qDebug()<<"load start";
    QList<Rule> list;
    QSqlQuery query;
    QString str="SELECT id,rule_name,author,rem FROM rules";
    if(query.exec(str))
    {
        int q0;
        QString q1,q2,q3;
        while(query.next())
        {
            q0=query.value(0).toInt();
            q1=query.value(1).toString();
            q2=query.value(2).toString();
            q3=query.value(3).toString();
            Rule t={q0,q1,q2,q3};
            list.append(t);
        }
        return list;
    }


}
*/
QList<RuleNode> DataModel::loadRuleNodes(int ruleID)
{
    QList<RuleNode> list;
    QSqlQuery q;
    QString str="SELECT id,node_type_id,node_is_create,node_name FROM rules_nodes WHERE id_rule="+QString::number(ruleID);
    if(q.exec(str))
    {
        int id,type;
        bool is_create;
        QString name;
        while(q.next())
        {
            id=q.value(0).toInt();
            type=q.value(1).toInt();
            is_create=q.value(2).toBool();
            name=q.value(3).toString();
            RuleNode t={id,type,is_create,name};
            list.append(t);
        }
    }
    return list;
}

QList<RuleEdge> DataModel::loadRuleEdges(int ruleID) //Bad quant
{
    QList<RuleEdge> list;
    QSqlQuery q;
    QString str="SELECT id,edge_type_id,node_id_src,node_id_dst,edge_is_create,quant_src,quant_dst FROM rules_edges WHERE id_rules="+
            QString::number(ruleID);
    if(q.exec(str))
    {
        int id,type,node_src,node_dst;
        bool is_create;
        QString quant_src,quant_dst;
        while(q.next())
        {
            id=q.value(0).toInt();
            type=q.value(1).toInt();
            node_src=q.value(2).toInt();
            node_dst=q.value(3).toInt();
            is_create=q.value(4).toBool();
            quant_src=q.value(5).toString();
            quant_dst=q.value(5).toString();
            RuleEdge t={id,type,node_src,node_dst,is_create,quant_src,quant_dst};
            list.append(t);
        }
    }
    return list;
}


void DataModel::LoadDafGraph()
{
    m_graph->clearGraphData();
    m_graph->clearGraphTypes();

    this->createDafGraphTypes();
    this->createDafGraph();
}

void DataModel::LoadRulesList()
{

    ClearRulesList();

    QSqlQuery query,query2,query3;
    QString str="SELECT id,rule_name,author,rem,template FROM rules;";
    if(query.exec(str))
        while(query.next())
        {
            VzRule *rule = new VzRule;
            rule->setId(query.value(0).toInt());
            rule->setName(query.value(1).toString());
            rule->setAuthor(query.value(2).toString());
            rule->setRem(query.value(3).toString());
            rule->setTemplate(query.value(4).toString());


            QString str2="SELECT node_type_id,node_name FROM rules_nodes WHERE id_rule="+query.value(0).toString()+";";
            if(query2.exec(str2))
                while(query2.next())
                {
                    QString str3="SELECT node_type_name FROM node_types WHERE id="+query2.value(0).toString()+";";
                    if(query3.exec(str3))
                    {
                        query3.next();
                        QString ntypename=query3.value(0).toString();
                        if(query2.value(1).toString()=="A") rule->setM_A(ntypename,query2.value(0).toInt());
                        if(query2.value(1).toString()=="B") rule->setM_B(ntypename,query2.value(0).toInt());
                        if(query2.value(1).toString()=="C") rule->setM_C(ntypename,query2.value(0).toInt());
                        if(query2.value(1).toString()=="D") rule->setM_D(ntypename,query2.value(0).toInt());
                    }
                }
            else qDebug()<<"q2"<<query2.lastError().text();
            str2="SELECT edge_type_id,node_id_src,node_id_dst,rule_quant_id FROM rules_edges WHERE id_rules="+query.value(0).toString()+";";
            QString src,dst,etypename;
            if(query2.exec(str2))
                while(query2.next())
                {
                    QString str3="SELECT edge_name FROM edge_types WHERE id="+query2.value(0).toString()+";";
                    if(query3.exec(str3))
                    {
                        query3.next();
                        etypename=query3.value(0).toString();
                    }
                    else qDebug()<<"q31"<<query3.lastError().text();

                    str3="SELECT node_name FROM rules_nodes WHERE id="+query2.value(1).toString()+";";
                    if(query3.exec(str3))
                    {
                        query3.next();
                        src=query3.value(0).toString();
                    }
                    else qDebug()<<"q32"<<query3.lastError().text();

                    str3="SELECT node_name FROM rules_nodes WHERE id="+query2.value(2).toString()+";";
                    if(query3.exec(str3))
                    {
                        query3.next();
                        dst=query3.value(0).toString();
                    }
                    else qDebug()<<"q33"<<query3.lastError().text();
                    if(src=="A" && dst=="B"){
                        rule->setM_edgeAB(etypename,query2.value(0).toInt(),1);
                        rule->setM_edgeABQuant(query2.value(3).toInt());
                    }
                    if(src=="B" && dst=="A"){
                        rule->setM_edgeABQuant(query2.value(3).toInt());
                        rule->setM_edgeAB(etypename,query2.value(0).toInt(),0);
                    }
                    if(src=="B" && dst=="C"){
                        rule->setM_edgeBC(etypename,query2.value(0).toInt(),1);
                        rule->setM_edgeBCQuant(query2.value(3).toInt());
                    }
                    if(src=="C" && dst=="B"){
                        rule->setM_edgeBC(etypename,query2.value(0).toInt(),0);
                        rule->setM_edgeBCQuant(query2.value(3).toInt());
                    }
                    if(src=="A" && dst=="C"){
                        rule->setM_edgeAC(etypename,query2.value(0).toInt(),1);
                        rule->setM_edgeACQuant(query2.value(3).toInt());
                    }
                    if(src=="C" && dst=="A"){
                        rule->setM_edgeAC(etypename,query2.value(0).toInt(),0);
                        rule->setM_edgeACQuant(query2.value(3).toInt());
                    }
                    if(src=="A" && dst=="D"){
                        rule->setM_edgeAD(etypename,query2.value(0).toInt(),1);
                        rule->setM_edgeADQuant(query2.value(3).toInt());
                    }
                    if(src=="D" && dst=="A"){
                        rule->setM_edgeAD(etypename,query2.value(0).toInt(),0);
                        rule->setM_edgeADQuant(query2.value(3).toInt());
                    }
                    if(src=="D" && dst=="C"){
                        rule->setM_edgeDC(etypename,query2.value(0).toInt(),1);
                        rule->setM_edgeDCQuant(query2.value(3).toInt());
                    }
                    if(src=="C" && dst=="D"){
                        rule->setM_edgeDC(etypename,query2.value(0).toInt(),0);
                        rule->setM_edgeDCQuant(query2.value(3).toInt());
                    }
                    if(src=="B" && dst=="D"){
                        rule->setM_edgeBD(etypename,query2.value(0).toInt(),1);
                        rule->setM_edgeBDQuant(query2.value(3).toInt());
                    }
                    if(src=="D" && dst=="B"){
                        rule->setM_edgeBD(etypename,query2.value(0).toInt(),0);
                        rule->setM_edgeBDQuant(query2.value(3).toInt());
                    }
                }

            // Р·Р°С‚С‹С‡РєР° - РѕРїСЂРµРґРµР»РµРЅРёРµ С‚РёРїР° РїСЂР°РІРёР»Р°. С‚СѓРґСѓ- РЅР°РґРѕ РїСЂРѕРІРµСЂСЏС‚СЊ РєРѕСЂСЂРµРєС‚РЅРѕСЃС‚СЊ РїСЂР°РІРёР»Р° Рё РѕРїСЂРµРґРµР»СЏС‚СЊ С‚РёРї
            if (rule->getM_D_typeId() == -1)
                rule->setRuleType(rtTranz);
            else
                rule->setRuleType(rtNewConcept);
            m_rulesList.append(rule);
        }
    else qDebug()<<"q1"<<query.lastError().text();

}

void DataModel::UseRule(VzRule* rule, QList<VzEdge> *inEdges, QList<VzEdge> *outEdges)
{
    qDebug() << "use rule, id== " << rule->getId();
    m_graph->UseRule(rule);
}

void DataModel::UseRule(int index, QList<VzEdge> *inEdges, QList<VzEdge> *outEdges)
{
    qDebug() << "use rule, index== " << index;
    if (index >= m_rulesList.size())
    {
        qDebug() << "index out of range";
        return;
    }
    VzRule *r = m_rulesList.at(index);
    qDebug() << "use rule, id== " << r->getId();
}

void DataModel::UseRules(QList<VzEdge> *inEdges, QList<VzEdge> *outEdges)
{
    QList<VzRule*>::iterator i;
    for (i = m_rulesList.begin(); i != m_rulesList.end(); ++i)
    {
        VzRule* r = *i;
        UseRule(r, inEdges, outEdges);
    }
}

void DataModel::ClearRulesList()
{
    QList<VzRule*>::iterator i;
    for (i = m_rulesList.begin(); i != m_rulesList.end(); ++i)
    {
        VzRule* r = *i;
        qDebug() << "delete rule, id== " << r->getId();
        delete r;
    }
    m_rulesList.clear();
}

/*
int DataModel::AddNode(QString nodeType, QString name, int brigh, int status, QString auth, QString rem )
{
  // РґРѕР±Р°РІР»СЏРµРј СѓР·РµР» SQL-Р·Р°РїСЂРѕСЃРѕРј
    // РґРѕР±Р°РІР»СЏРµРј
    QSqlQuery query;
    QString str;
    QString strF ="INSERT INTO nodes (id_node_type, node_name, brightness, status, author, rem)"
                              "VALUES(%1, '%2', %3, %4, '%5', '%6');";
    str = strF.arg(nodeType)
             .arg(name)
             .arg(QString::number(brigh))
             .arg(QString::number(status))
             .arg(auth)
             .arg(rem);

    if (!query.exec(str))
    {
        qDebug() << "Query exec ERROR: " << str;
        qDebug() << query.lastError().text();
        //QMessageBox::information(0, "SQL INSERT:", query.lastError().text());
        dataModel->logMessage(query.lastError().text());
    }
    return -1;

    // РїРµСЂРµР·Р°РїСЂР°С€РёРІР°РµРј id (РіСЂС‘Р±Р°РЅРЅС‹Р№ Р°РєС†РµСЃСЃ, РІ РїРѕСЃС‚РіСЂРµСЃРµ РІСЃС‘ Р»СѓС‡С€Рµ)
    int node_id;
    QSqlQuery query_2;
    str ="SELECT id FROM nodes WHERE node_name=" + name;
    if (!query_2.exec(str))
    {
        qDebug() << "Query_2 exec ERROR: " << str;
        qDebug() << query_2.lastError().text();
        //QMessageBox::information(0, "SQL INSERT:", query_2.lastError().text());
        dataModel->logMessage(query.lastError().text());
    }
    else
    {
        query_2.next();
        node_id = query_2.value(0).toInt();
    }

    // РґРѕР±Р°РІР»СЏРµРј РІ DG
    if(m_graph != NULL)
    {
        // РІСЃСЏРєР°СЏ С…РµСЂРЅСЏ
    }
    return node_id;
}
*/

/*int DataModel::getNodeId(QString nodeName)
{
    //todo РєР°Рє РІС‹РґРµСЂРЅСѓС‚СЊ РёР· m_nodes?

    QSqlTableModel m;
    m.setTable("nodes");
   m.setFilter("node_name = '" + nodeName + "'");
    m.select();
    if(m.rowCount() == 0)
    {
        return 0;
    }

    if(m.rowCount() > 1 )
    {
        return -1;
    }

    QModelIndex index = m.index(0, nodes_id, QModelIndex());
    int id = m.data(index, Qt::DisplayRole).toInt();
    //int id = m_edgeTypes->data(index, Qt::DisplayRole).toInt();
    return id;
}
*/

///////////////////////////////////////////////
///     По заданию
///////////////////////////////////////////////
///////////////////////////////////////////////
///     Тип узла
///////////////////////////////////////////////
void DataModel::insertNodeType(QString name){
    qDebug () << "insertNodeType";
    QSqlQuery query;
    QString str;
    QString strF ="INSERT INTO node_types (node_type_name) VALUES('%1');";
    str = strF.arg(name);
    if (!query.exec(str)){
        QMessageBox::information(0, "SQL INSERT:", query.lastError().text());
    }else{
        dataModel->m_nodeTypes->select();
    }
}
bool DataModel::updateNodeType(QString name, QString id)
{
    qDebug () << "updateNodeType";
    QSqlQuery query;
    QString str;
    QString strF ="UPDATE node_types SET node_types.node_type_name = '%1' WHERE (((node_types.id)=%2));";
    str = strF.arg(name)
              .arg(id);
    if(query.exec(str)){
        return 1;
    }else{
        QMessageBox::information(0, "SQL UPDATE:", query.lastError().text());
        return 0;
    }
}
bool DataModel::deleteNodeType(QString name)
{

}

///////////////////////////////////////////////
///     Тип ребра
///////////////////////////////////////////////
void DataModel::insertEdgeType(QString src, QString name, QString dst){
    qDebug () << "insertEdgeType";
    QSqlQuery query;
    QString str;

    QString strF ="INSERT INTO edge_types "
            "(id_node_type_src,edge_name,id_node_type_dst) VALUES "
            "(%1, '%2', %3);";
    str = strF.arg(src)
              .arg(name)
              .arg(dst);

    if (!query.exec(str)){
        QMessageBox::information(0, "SQL INSERT:", query.lastError().text());
    }
    else{
        dataModel->m_edgeTypes->select();
    }
}

bool DataModel::updateEdgeType(QString src, QString name, QString dst, QString id){
    qDebug () << "updateEdgeType";
    QSqlQuery query;
    QString str;
    QString strF ="UPDATE edge_types SET "
            "edge_types.id_node_type_src=%0, "
            "edge_types.edge_name='%1', "
            "edge_types.id_node_type_dst=%2 "
            "WHERE (((edge_types.id)=%4));";
    str = strF.arg(src)
              .arg(name)
              .arg(dst)
              .arg(id);
    if(query.exec(str)){
        return 1;
    }else{
        QMessageBox::information(0, "SQL UPDATE:", query.lastError().text());
        return 0;
    }
}

void DataModel::deleteEdgeType(QString src, QString name, QString dst)
{
    qDebug () << "deleteEdgeType";
}

///////////////////////////////////////////////
///     Тип квантора
///////////////////////////////////////////////

void DataModel::insertQuantType(QString name){
    qDebug () << "insertQuantType";
    QSqlQuery query;
    QString str;
    QString strF ="INSERT INTO quantifiers (quant_name) VALUES('%1');";
    str = strF.arg(name);
    if (!query.exec(str)){
        QMessageBox::information(0, "SQL INSERT:", query.lastError().text());
    }
    else{
        dataModel->m_quantTypes->select();
    }
}

bool DataModel::updateQuantType(QString name, QString value){
    qDebug () << "updateQuantType";
    QSqlQuery query;
    QString str;
    QString strF ="UPDATE quantifiers SET quantifiers.quant_name = '%1' WHERE (((quantifiers.id)=%2));";
    str = strF.arg(name)
              .arg(value);
    if(query.exec(str)){
        return 1;
    }else{
        QMessageBox::information(0, "SQL UPDATE:", query.lastError().text());
        return 0;
    }
}

void DataModel::deleteQuantType(QString name)
{

}

///////////////////////////////////////////////
///     Узел
///////////////////////////////////////////////

bool DataModel::insertNode(QString id_node_type,QString node_name,QString brightness,QString status,QString author,QString rem)
{
    qDebug () << "insertNode";
    QSqlQuery query;
    QString str;
    QString strF ="INSERT INTO nodes (id_node_type, node_name, brightness, status, author, rem)"
                              "VALUES(%1, '%2', %3, %4, '%5', '%6');";
    str = strF.arg(id_node_type)
            .arg(node_name)
            .arg(brightness)
            .arg(status)
            .arg(author)
            .arg(rem);

    if (!query.exec(str))
    {
        qDebug() << str;
        qDebug() << "Unable to make insert operation";
        //QMessageBox::information(0, "SQL INSERT:", query.lastError().text());
        dataModel->logMessage(query.lastError().text());
        return false;
    }
    else
    {
        return true;
    }
}

bool DataModel::updateNode(QString p1,QString p2,
                           QString p3,QString p4,
                           QString p5,QString p6,
                           QString p7)
{
    qDebug () << "updateNode";
    QSqlQuery query;
    QString str;
    QString strF ="UPDATE nodes SET "
            "nodes.id_node_type = %1, "
            "nodes.node_name = '%2', "
            "nodes.brightness = %3, "
            "nodes.status = %4, "
            "nodes.author = '%5', "
            "nodes.rem = '%6' "
            "WHERE nodes.id=%7;";

    str = strF.arg(p1)
                 .arg(p2)
                 .arg(p3)
                 .arg(p4)
                 .arg(p5)
                 .arg(p6)
                 .arg(p7);
    if(query.exec(str)){
        return 1;
    }else{
        QMessageBox::information(0, "SQL UPDATE:", query.lastError().text());
        return 0;
    }
}

bool DataModel::deleteNode()
{

}

///////////////////////////////////////////////
///     Ребро
///////////////////////////////////////////////
bool DataModel::insertEdge(QString id_node_src,QString id_edge_type,QString id_node_dst,QString quant_id,QString capacity,QString status,QString isNew)
{
    qDebug () << "insertEdge";
    QSqlQuery query;
    QString str;
    QString strF ="INSERT INTO edges "
            "(id_node_src,id_edge_type,id_node_dst,quant_id,capacity,status,isNew) VALUES "
            "(%1, %2, %3, '%4', %5, %6, %7);";
    str = strF.arg(id_node_src)
              .arg(id_edge_type)
              .arg(id_node_dst)
              .arg(quant_id)
              .arg(capacity)
              .arg(status)
              .arg(isNew);
    if (!query.exec(str))
    {
        //QMessageBox::information(0, "SQL INSERT:", query.lastError().text());
        dataModel->logMessage(query.lastError().text());
        return false;
    }
    else
    {
        dataModel->m_edges->select();
        return true;
    }
}
bool DataModel::updateEdge(QString p1,QString p2,
                           QString p3,QString p4,
                           QString p5,QString p6,
                           QString p7,QString p8)
{
    qDebug () << "updateEdge";
    QSqlQuery query;
    QString str;
    QString strF ="UPDATE edges SET "
            "edges.id_node_src=%1, "
            "edges.id_edge_type=%2, "
            "edges.id_node_dst=%3, "
            "edges.quant_id='%4', "
            "edges.capacity=%5, "
            "edges.status=%6, "
            "edges.isNew=%7 "
            "WHERE edges.id=%8;";
    str = strF.arg(p1)
            .arg(p2)
            .arg(p3)
            .arg(p4)
            .arg(p5)
            .arg(p6)
            .arg(p7)
            .arg(p8);
    if(query.exec(str)){
        return 1;
    }else{
        QMessageBox::information(0, "SQL UPDATE:", query.lastError().text());
        return 0;
    }
}

bool DataModel::deleteEdge()
{

}
