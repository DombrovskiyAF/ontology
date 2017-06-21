#ifndef GOSTBLOCK_H
#define GOSTBLOCK_H

#include <QObject>
#include <QStringList>

class GostBlock : public QObject
{
    Q_OBJECT
public:
    explicit GostBlock(QObject *parent = 0);
    GostBlock(QString name, bool isOr, QObject *parent = 0);

    GostBlock(const GostBlock &gostBlock, QObject *parent = 0);
    void operator=(const GostBlock &gostBlock);

    QString name();
    void setName(QString name);

    void setIsOr();
    void setIsAnd();

    bool isOr();
    bool isAnd();

    QStringList *getSubBlocks();
    void addSubBlock(QString subBlock);

signals:

public slots:

private:
    QString m_name;
    bool m_isOr;
    QStringList m_subBlocks;

};

#endif // GOSTBLOCK_H
