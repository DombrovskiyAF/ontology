#ifndef PERESTANOVKA_H
#define PERESTANOVKA_H

#include <QObject>



class Perestanovka : public QObject
{
    Q_OBJECT
public:
    explicit Perestanovka(QObject *parent = 0);
    explicit Perestanovka(int N, QObject *parent = 0);

    int getN();
    int getPerCount();

    int *getPer(int i);

signals:

public slots:

private:
    int m_N;
    int m_perCnt;
    int m_pers[140][6];// до 5!

};

#endif // PERESTANOVKA_H
