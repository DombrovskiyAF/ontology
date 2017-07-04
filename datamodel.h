#ifndef DATAMODEL_H
#define DATAMODEL_H

#include <QObject>
#include <QTextEdit>
#include <QSortFilterProxyModel>
#include <QSqlTableModel>
#include <QSqlRelationalTableModel>
#include <similarity.h>

#define ORGANIZATION_NAME "MILITARYSINCEORGANIZATION"
#define ORGANIZATION_DOMAIN "www.cookmycode.ru"
#define APPLICATION_NAME "VZ PROJECT"

#include "rule.h"
#include "dafrules.h"
#include <QList>
#include "userulesform.h"
#include "vzedge.h"
#include "QSettings"
#include "selectdbwindow.h"

// перечисления с индексами полей таблиц БД

// таблица node_types
enum
{
    nodetype_id,
    nodetype_name
};

// таблица edge_types
enum
{
    edgetype_id,
    edgetype_srctype,
    edgetype_name,
    edgetype_dsttype
};

// таблица node
enum
{
    nodes_id,
    nodes_id_node_type,
    nodes_node_name,
    nodes_brigh,
    nodes_status,
    nodes_author,
    nodes_rem,
    nodes_lvl
};

// таблица edges
enum
{
    edges_id,
    edges_id_node_src,
    edges_edge_type,
    edges_id_node_dst,
    edges_quant_id,
    edges_capacity,
    edges_status,
    edges_isNew
};

// таблица rules
enum
{
    rules_id,
    rules_name,
    rules_author,
    rules_rem,
    rules_template
};

// таблица rules_nodes
enum
{
    rulesnodes_id,
    rulesnodes_id_rules,
    rulesnodes_type_id,
    rulesnodes_is_create,
    rulesnodes_node_name
};

// таблица rules_edges
enum
{
    rulesedges_id,
    rulesedges_id_rules,
    rulesedges_type_id,
    rulesedges_node_id_src,
    rulesedges_node_id_dst,
    rulesedges_is_create,
    rulesedges_quant_id
};

namespace Ui {
    class DataModel;
}

class FormTypes;
class FormNodes;
class FormEdges;
class FormRules;
class DafGraph;
class UseRulesForm;

class DataModel : public QObject
{
    Q_OBJECT

public:

    explicit DataModel();
    ~DataModel();

    QSettings *getSettings();
    void logMessage(QString message);

    /// импорт из ГОСТа, представленного одной строкой
    bool exportFromGost(QString fileName);

    /// вставляет узел и возвращает true, при неуспехе возвращает false
    bool insertNode(QString id_node_type,QString node_name,QString brightness,QString status,QString author,QString rem);
    /// вставляет узел и возвращает true, , в idNodes возвращает nodes.id вставленной записи
    /// при неуспехе возвращает -1
    bool insertNode(QString id_node_type,QString node_name,QString brightness,QString status,QString author,QString rem, int &idNode);

    /// вставляет узел и возвращает true, при неуспехе возвращает false
    bool insertEdge(QString id_node_src,QString id_edge_type,QString id_node_dst,QString quant_id,QString capacity,QString status,QString isNew);
    /// вставляет ребро и возвращает true, в idEdges возвращает edges.id вставленной записи
    /// при неуспехе возвращает false
    bool insertEdge(QString id_node_src,QString id_edge_type,QString id_node_dst,QString quant_id,QString capacity,QString status,QString isNew, int &idEdge);

    bool updateNode(QString p1, QString p2, QString p3, QString p4, QString p5, QString p6, QString nodes);
    bool deleteNode();
    bool updateEdge(QString id_node_src, QString p2, QString p3, QString p4, QString p5, QString p6, QString p7, QString p8);
    bool deleteEdge();

    int getEdgeId(QString id_node_src, QString id_edge_type, QString id_node_dst);
    int getNodeId(QString node_name);

    //---------------------------------------------------------

    // добавляет узел в БД и в DG (если он не NULL)
    // возвращает id добавленного узла, -1 если ошибка
    // int AddNode(QString nodeType, QString name, int brigh, int status, QString auth, QString rem );

    // добавляет ребро в БД и в DG (если он не NULL)
    // возвращает id добавленного ребра
    // int AddEdge(QString edgeType, QString nodeSrc, QString nodeDst);

    //---------------------------------------------------------
    // загрузка правил в DataModel::m_rulesList
    void LoadRulesList();

    // очистка списка правил DataModel::m_rulesList
    void ClearRulesList();

    // получить указатель на список правил
    QList<VzRule*> *getRulesList(){ return &m_rulesList;}

    // применение правил
    void UseRules(QList<VzEdge> *inEdges, QList<VzEdge> *outEdges);

    void UseRule(VzRule* rule, QList<VzEdge> *inEdges, QList<VzEdge> *outEdges);

    void UseRule(int index, QList<VzEdge> *inEdges, QList<VzEdge> *outEdges);

    //---------------------------------------------------------
    // загрузка онтологии из моделей в ДГ
    void LoadDafGraph();

    //---------------------------------------------------------
    // Тип узла
    void insertNodeType(QString name);
    bool updateNodeType(QString name, QString id);
    bool deleteNodeType(QString name);

    // Тип ребра
    void insertEdgeType(QString src, QString name, QString dst);
    bool updateEdgeType(QString src, QString name, QString dst, QString id);
    void deleteEdgeType(QString src, QString name, QString dst);


    // Тип квантора
    void insertQuantType(QString name);
    bool updateQuantType(QString name, QString value);
    void deleteQuantType(QString name);

    //
    void createDafGraphTypes();
    void createDafGraph();
    DafGraph* getGraph(){ return m_graph;}


private:
    // список правил
    QList<VzRule*> m_rulesList;

    QSettings *settings;

//===========================================================
public:
    QSqlRelationalTableModel *m_nodes;
    QSqlRelationalTableModel *m_edgeTypes;
    QSqlRelationalTableModel *m_edges;
    QSqlTableModel *m_nodeTypes;
    QSqlTableModel *m_quantTypes;

    QList<RuleEdge> loadRuleEdges(int ruleID);

    QSqlDatabase db;

    FormEdges *m_formEdges;
    FormNodes *m_formNodes;
    bool connectDB(const QString &dBName);

    void createModel();

private:

    //QList<Rule> loadRules();
    QList<RuleNode> loadRuleNodes(int ruleID);

    // модели данных
    QSqlRelationalTableModel *m_rules;
    QSqlRelationalTableModel *m_rulesNodes;
    QSqlRelationalTableModel *m_rulesEdges;
    QSqlRelationalTableModel *m_rulesEdgesThen;

    QVector <int> edgeType;




    // формы с отображениями моделей
    // типы узлов\ребер и правила
    FormTypes *m_formTypes;
    FormRules *m_formRules;
    UseRulesForm *m_userulesform;

    void disconnectDB();

    void deleteModel();

public:
    void setupView();

    // зачищает списки рёбер, затем узлов
    bool clearModels();
    int addNodeToModel(QString name, QString typeName, int bright, int status, QString author, QString rem, int id = -1);

    int addEdgeToModel(QString nodeSrcName, QString typeName, QString nodeDstName, int id = -1);


    int getNodeTypeId(QString nodeTypeName);

    //int getNodeId(QString nodeName);

    int getNodeInf(QString nodeName, int &nodeTypeId);
    int getEdgeTypeId(int nodeSrcTypeId, QString typeName, int nodeDstTypeId);

    bool submitAll();
    bool submitNodes();
    bool submitEdges();

    void selectNodes();
    void selectNodesAndEdges();
    bool simpleExportDot(QString fileName);
    bool importToDot(QString fileName);

    int getNodeParams(QString nodeName, int &id, int &typeId);
    int addNodeToDb(int nodeTypeId, QString nodeName);
    int addEdgeToDb(QString nodeSrcName, QString edgeTypeName, QString nodeDstName);

    void setLog(QTextEdit *log);

    QString getDBName();

    Similarity form_similarity;

    selectdbwindow *sdw;

    QString m_dbName;

private:




    DafGraph  *m_graph;




private:

    QTextEdit *m_log;


public slots:
    void showFormTypes();
    void showFormNodes();
    void showFormEdges();
    void showFormRules();

    void openDB(QWidget *q);
    void openDB_fromsettings(QWidget *q, QString nam);
};

extern DataModel *dataModel;

#endif // DATAMODEL_H
