#ifndef DLLMYSQL_H
#define DLLMYSQL_H

#include <QtSql/QSqlTableModel>
#include <QtSql/QSqlRelationalTableModel>
#include <QString>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlTableModel>
#include <QtMath>
#include <QVector>

#include "dllmysql_global.h"
#include "connection.h"

class DLLMySQL
{

public:
    DLLMySQL();

    void selectAccount(QString);
    void withdrawMoney(double);
    void fundsTransfer(QString, double);
    void nullValues();
    bool checkCardDetails(QString, QString);
    int getTransactions();
    int findAccounts();
    void lockCard();

    //Person Table Values
    QString fName;
    QString lName;
    QString idPerson;

    //Card Table Values
    QString idCard;
    QString cardNum;
    int pinCode;

    //Account Table Values
    //QString multipleAccount[4];
    QVector<QVector<QString> > multiplemultipleAccount;
    QString idAccount;
    QString aName;
    double balance;
    int idLog;

    //Log Table Values
    int actionIndex;
    QVector<QVector<QString> > logArray;

private:
    void insertToLog(int);
    void getPersonAndCardInfo();
    void nullLogValues();
    void getLatestLogID();
    int removeMoneyFromAccount(double);

    QString action;
    QString info;
    QString money;
    QString transferAccountName;
    QString transferAccountId;

    double removedMoney;
    int latestLogID;
    int logRowCount;

};

#endif // DLLMYSQL_H
