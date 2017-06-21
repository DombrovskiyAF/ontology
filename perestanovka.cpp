#include "perestanovka.h"

Perestanovka::Perestanovka(QObject *parent) :
    QObject(parent)
{
}

Perestanovka::Perestanovka(int N, QObject *parent) :
    QObject(parent)
{
    m_N = N;//p->n = n;
    m_perCnt = 0;// p->perCnt = 0;
    int per[10], obr[10];
    int i, j, k, tmp, min, raz;//, n;
    bool flag;

    for (i = 0; i < N; i++)
    {
        per[i] = i+1;
    }

    while (1)
    {
        for (k = 0; k < N; k++)
        {
            m_pers[m_perCnt][k] = per[k];//p->pers[p->perCnt][k] = per[k];

        }
        m_perCnt++;//p->perCnt++;
        flag = false;
        for (i = N - 2; i >= 0; i--)
        {
            if (per[i] < per[i + 1])
            {
                flag = true;
                break;
            }
        }
        if (flag == false)
        {
            break;
        }
        raz = per[i+1];
        for (j = i+1; j < N; j++)
        {
            if (((per[j] - per[i]) < raz) && (per[i] < per[j]))
            {
                min = j;
            }
        }
        tmp	      = per[i];
        per[i] 	   = per[min];
        per[min] = tmp;
        for (j = i + 1; j < N; j++)
        {
            obr[j] = per[j];
        }
        j = i + 1;
        for (k = N-1; k >= i+1; k--)
        {
            per[j] = obr[k];
            j++;
        }
    }
}

int Perestanovka::getN()
{
    return m_N;
}

int Perestanovka::getPerCount()
{
    return m_perCnt;
}

int *Perestanovka::getPer(int i)
{
    if (i > m_perCnt)
        return 0;

    return &m_pers[i][0];
}



