#ifndef CONNECTION_H
#define CONNECTION_H

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QDebug>


static bool mysliConnect()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("mysli.oamk.fi");
    db.setDatabaseName("opisk_t7koja01");
    db.setUserName("t7koja01");
    db.setPassword("OhjelmistokehityksenSovellusprojekti1000");
    if (!db.open()){
        qDebug() << "Connection error to mysli.oamk.fi" << endl;
        qDebug() << "Unable to establish a database connection" <<endl;
        return false;
    }
    qDebug() << "Connected!!!" << endl;
    return true;
}

#endif // CONNECTION_H
