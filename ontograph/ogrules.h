#ifndef DAFRULES_H
#define DAFRULES_H

#include <QObject>
#include <QString>
#include <QList>

enum EnRulesTypes{
    rtNone,
    rtTranz,
    rtNewConcept
};

class VzRule : public QObject
{
    Q_OBJECT

  public:
    VzRule();
    //VzRule(int id, QString name, QString author, QString rem);
    QString getRuleDescription();
    ~VzRule();

    // тип правила
    EnRulesTypes getRuleType(){ return m_ruleType; }
    void setRuleType(EnRulesTypes ruleType){ m_ruleType = ruleType; }

    void setId(int id);
    void setName(QString name);
    void setRem(QString rem);
    void setAuthor(QString author);
    void setTemplate(QString templatee);
    void setM_A(QString name, int id);
    void setM_B(QString name, int id);
    void setM_C(QString name, int id);
    void setM_D(QString name, int id);
    void setM_edgeAB(QString name,int id,bool direct);
    void setM_edgeBC(QString name,int id,bool direct);
    void setM_edgeAC(QString name,int id,bool direct);
    void setM_edgeAD(QString name,int id,bool direct);
    void setM_edgeDC(QString name,int id,bool direct);
    void setM_edgeBD(QString name,int id,bool direct);
    void setM_edgeABQuant(int quant);
    void setM_edgeBCQuant(int quant);
    void setM_edgeACQuant(int quant);
    void setM_edgeADQuant(int quant);
    void setM_edgeDCQuant(int quant);
    void setM_edgeBDQuant(int quant);

    QList<VzRule*> m_rulesList;

    int getId();
    QString getName();
    QString getRem();
    QString getAuthor();
    QString getTemplate();

    QString getM_A_name();
    int getM_A_typeId(){ return m_A_typeId;}

    QString getM_B_name();
    int getM_B_typeId(){ return m_B_typeId;}

    QString getM_C_name();
    int getM_C_typeId(){ return m_C_typeId;}

    QString getM_D_name();
    int getM_D_typeId(){ return m_D_typeId;}

    QString getM_edgeABName();
    int getM_AB_typeId(){ return m_edgeABId;}
    bool getM_AB_direct() { return m_edgeABdirect; }
    int getM_AB_Quant(){ return m_edgeABQuant;}

    QString getM_edgeBCName();
    int getM_BC_typeId(){ return m_edgeBCId;}
    bool getM_BC_direct() { return m_edgeBCdirect; }
    int getM_BC_Quant(){ return m_edgeBCQuant;}

    QString getM_edgeACName();
    int getM_AC_typeId(){ return m_edgeACId;}
    bool getM_AC_direct() { return m_edgeACdirect; }
    int getM_AC_Quant(){ return m_edgeACQuant;}

    QString getM_edgeADName();
    int getM_AD_typeId(){ return m_edgeADId;}
    bool getM_AD_direct() { return m_edgeADdirect; }
    int getM_AD_Quant(){ return m_edgeADQuant;}

    QString getM_edgeDCName();
    int getM_DC_typeId(){ return m_edgeDCId;}
    bool getM_DC_direct() { return m_edgeDCdirect; }
    int getM_DC_Quant(){ return m_edgeDCQuant;}

    QString getM_edgeBDName();
    int getM_BD_typeId(){ return m_edgeBDId;}
    bool getM_BD_direct() { return m_edgeBDdirect; }
    int getM_BD_Quant(){ return m_edgeBDQuant;}

  private:


    int     m_id;
    QString m_name;
    QString m_rem;
    QString m_author;
    QString m_template;

    // тип правила
    EnRulesTypes m_ruleType;

    // если
    QString m_A_typeName;
    int m_A_typeId;

    QString m_B_typeName;
    int m_B_typeId;

    QString m_C_typeName;
    int m_C_typeId;


    QString m_edgeABName;
    int m_edgeABId;
    bool m_edgeABdirect;
    int m_edgeABQuant;

    QString m_edgeBCName;
    int m_edgeBCId;
    bool m_edgeBCdirect;
    int m_edgeBCQuant;

    // то
    QString m_D_typeName;
    int m_D_typeId;

    QString m_edgeACName;
    int m_edgeACId;
    bool m_edgeACdirect;
    int m_edgeACQuant;

    QString m_edgeADName;
    int m_edgeADId;
    bool m_edgeADdirect;
    int m_edgeADQuant;

    QString m_edgeDCName;
    int m_edgeDCId;
    bool m_edgeDCdirect;
    int m_edgeDCQuant;

    QString m_edgeBDName;
    int m_edgeBDId;
    bool m_edgeBDdirect;
    int m_edgeBDQuant;

};

#endif // DAFRULES_H
