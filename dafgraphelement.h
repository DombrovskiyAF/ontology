#ifndef DAFGRAPHELEMENT_H
#define DAFGRAPHELEMENT_H

#include <QObject>

class DafGraphElement : public QObject
{
    Q_OBJECT
public:
    DafGraphElement(QObject *parent = 0);

    int getId();
    void setId(int id);

    QString getName();
    virtual QString getShortName()
    {
        return m_name;
    };
    void setName(QString name);

    int getOpenCnt(){ return m_openCnt; };
    void setOpenCnt( int openCnt ){ m_openCnt = openCnt; };
    void incOpenCnt(){ m_openCnt++; };
    void decOpenCnt(){ m_openCnt--; };
    bool isNew();

    /*! ���������� true ���� ������� (���) �� ����� � �� (������������� - ������� ��� ������������)
     *  false ���� ������� ������� � ��
     */
    bool isProduce();

    /*! ���������� �������� ����� m_isProduce
     */
    void setIsProduce(bool _isProduce){ m_isProduce = _isProduce; };

protected:

    int m_openCnt;

    /*! ������������� �� �� ��� ��������������� - ���� ������� (���) �� ����� � �� (������������� - ������� ��� ������������)
    */
    int m_id;

    /*! true  - ������� �������������, � �� �� �������
     *  false - ������� �������� � ��
     */
    bool m_isProduce;

    QString m_name;

signals:

public slots:

private:


};

#endif // DAFGRAPHELEMENT_H
