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

    /*! Возвращает true если элемент (ещё) не внесён в БД (спродуцирован - выведен или импортирован)
     *  false если элемент записан в БД
     */
    bool isProduce();

    /*! Выставляет значение флага m_isProduce
     */
    void setIsProduce(bool _isProduce){ m_isProduce = _isProduce; };

protected:

    int m_openCnt;

    /*! идентификатор из БД или сгенерированный - если элемент (ещё) не внесён в БД (спродуцирован - выведен или импортирован)
    */
    int m_id;

    /*! true  - элемент спродуцирован, в БД не занесен
     *  false - элемент сохранен в БД
     */
    bool m_isProduce;

    QString m_name;

signals:

public slots:

private:


};

#endif // DAFGRAPHELEMENT_H
