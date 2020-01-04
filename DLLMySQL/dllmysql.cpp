#include "dllmysql.h"

DLLMySQL::DLLMySQL()
{
    mysliConnect();
}

//1.    Checks for right Pincode and cards id from database
bool DLLMySQL::checkCardDetails(QString getIDCard, QString getPinCode){
    QSqlQuery qryC;
    idCard = getIDCard;
    pinCode = getPinCode.toInt();
    //Get card id and pincode from card if idcard is QString getIDCard and pincode is QString getPinCode. SQL exec
    qryC.prepare("SELECT idCard, pinCode, cardLock FROM card WHERE idCard=\""+idCard+"\" AND pinCode=\""+QString::number(pinCode)+"\" "
          "AND cardLock=0");
    qryC.exec();
    //Check if query C value 0 is idCard and value 1 is pincode
    if(qryC.next() && qryC.value(0).toString() == idCard && qryC.value(1).toInt() == pinCode && qryC.value(2).toInt() == 0)
    {
        qDebug() << "SQLDebug 1 " << "Card and pin code Correct!!!" << endl; //Debug 1
        qDebug() << "SQLDebug 2 " << "checkCardDetails() Done!!" << endl;    //Debug 2
        getPersonAndCardInfo();
        return true;
    }
    else {
        qDebug() << "SQLDebug 3 " << "Wrong card or pin code!!!" << endl;    //Debug 3
        qDebug() << "SQLDebug 4 " << "checkCardDetails() Done!!" << endl;    //Debug 4
        return false;
    }
}

//1.1    Puts info from persons and Card tables to Variables
void DLLMySQL::getPersonAndCardInfo(){
    qDebug() << "SQLDebug 5 " << "getModelValues"<< endl;    //Debug 5
    qDebug() << "SQLDebug 6 " << "Values: "<< idCard << " " << pinCode << endl;  //Debug 6
    QSqlQuery qryP;     //Query for persons Table
    QSqlQuery qryC;     //Query for card Table
    //Get all info from persons table if idCard and pinCode are correct
    qryP.prepare("SELECT * FROM persons INNER JOIN card ON persons.idPerson=card.idPerson "
                 "WHERE card.idCard=\""+idCard+"\" AND card.pinCode=\""+QString::number(pinCode)+"\"");
    //Get all info from card table if idCard and pinCode are correct
    qryC.prepare("SELECT * FROM card WHERE idCard=\""+idCard+"\" AND pinCode=\""+QString::number(pinCode)+"\"");
    qryP.exec();        //Query P exec
    qryC.exec();        //Query C exec
    while (qryP.next() && qryC.next()){
        //Persons Table
        idPerson = qryP.value(0).toString();    //assign persons table values
        fName = qryP.value(1).toString();       // to variables
        lName = qryP.value(2).toString();
        //Debug############## 7.*
        for (int i = 0; i < 3; i++) {
            qDebug()  << "SQLDebug 7." << i << " " << qryP.value(i).toString() << endl;
        }//##########Debug*
        //Card Table
        idCard = qryC.value(0).toString();      //assign card table values
        cardNum = qryC.value(2).toString();     // to variables
        pinCode = qryC.value(3).toInt();
        //Debug############## 8.*
        for (int i = 0; i < 4; i++) {
            qDebug() << "SQLDebug 8." << i << " " << qryC.value(i).toString() << endl;
        }//############Debug*
    }
    insertToLog(1); //Send login statement (1) to log
    qDebug() << "SQLDebug 9 " << "getPersonAndCardInfo() Done!!" << endl; //Debug 9
}

//2.    Finds how many account one card has,
//      and saves all of the accounts ids to Array
int DLLMySQL::findAccounts(){
    multiplemultipleAccount.clear();    //Clearing 2D vector
    QSqlTableModel accountRowModel;     //Using QSqlTableModel to get rowCount of the accounts
    accountRowModel.setTable("account");    //Selecting account table
    accountRowModel.setFilter("idCard=\""+idCard+"\""); //Checking for Right idCard
    accountRowModel.select();   //Same as query.exec
    QString array[3];
    qDebug() << "SQLDebug 10 "  << accountRowModel.rowCount(); //Debug 10
    qDebug() << "SQLDebug 10.1 "  << idCard; //Debug 10.1
    if(accountRowModel.rowCount() >= 1){    //Checking if the is more than 0 accounts
        for (int i = 0; i < accountRowModel.rowCount(); i++) {
            QVector<QString> tempVec;   //temporay QString Vector
            for (int j = 0; j < 6; j++) {
                switch (j) {    //In case of j=(0,3,4) Tablemodel value is put to array and then put to temporary vector
                case 0:
                    array[0] = accountRowModel.data(accountRowModel.index(i,j)).toString();
                    qDebug() << "SQLDebug 11.1 "  << array[0] << endl; //Debug 11.1
                    tempVec.push_back(array[0]);
                    break;
                case 3:
                    array[1] = accountRowModel.data(accountRowModel.index(i,j)).toString();
                    qDebug() << "SQLDebug 11.2 "   << array[1] << endl; //Debug 11.2
                    tempVec.push_back(array[1]);
                    break;
                case 4:
                    array[2] = accountRowModel.data(accountRowModel.index(i,j)).toString();
                    qDebug() << "SQLDebug 11.3 "   << array[2] << endl; //Debug 11.3
                    tempVec.push_back(array[2]);
                    break;
                default:
                    qDebug() << "SQLDebug 11.4 "   << "false " << i << " " << j << endl; //Debug 11.4
                    break;
                }
            }
            // temporary vector put to 2D vector
            multiplemultipleAccount.push_back(tempVec);     //2D vector for all accounts of the person [rows][QString]
        }
    }else {
        qDebug() << "SQLDebug 12 " << "No Account" << endl; //Debug 12
    }
    //Debug###########
    qDebug() << "SQLDebug 13 "  << "multiaccount debug:" << endl; //Debug 13
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 3; j++) {
            qDebug() << "SQLDebug 14." << i << "." << j << " " << multiplemultipleAccount.value(i).value(j)<< endl; //Debug 14.*.*
        }
    }
    qDebug() << "SQLDebug 15 " << "findAccounts() Done!!" << endl; // Debug 15
    //#########Debug*
    return accountRowModel.rowCount();
}

//4.    Puts account info to variables
void DLLMySQL::selectAccount(QString idAcc){
    QSqlQuery qryA;
    //Get all info from account Table and join card table.
    qryA.prepare("SELECT * FROM account INNER JOIN card ON account.idCard=card.idCard "
                 "WHERE idAccount=\""+idAcc+"\" AND card.idCard=\""+idCard+"\" AND card.pinCode=\""+QString::number(pinCode)+"\"");
    qryA.exec();
    while (qryA.next()){
        //Account Table
        //Debug############### 16.*
        for (int i = 0; i < 6; i++) {
            qDebug() << "SQLDebug 16." << i << " " << qryA.value(i).toString() << endl;
        }//###########Debug*
        idAccount = qryA.value(0).toString();   //assign card table values
        balance = qryA.value(3).toDouble();     // to variables
        aName = qryA.value(4).toString();
        idLog = qryA.value(5).toInt();
    }
    qDebug() << "SQLDebug 17 " << "selectAccount() Done!!" << endl; //Debug 17
}

//4. Withdraw
void DLLMySQL::withdrawMoney(double withdrawableMoney){
    if(removeMoneyFromAccount(fabs(withdrawableMoney)) == true){
        insertToLog(3); //Send withdraw statement (3) to log
    }else {
        qDebug() << "SQLDebug 18" << "Not enough funds to withdraw " << withdrawableMoney << endl; //Debug 18
    }
}

//4. Transfer
void DLLMySQL::fundsTransfer(QString accountIdToTransfer, double transferableMoney){
    QSqlQuery qryA;
    //Get account info to transfer where IDaccount is QString accountIdToTransfer
    qryA.prepare("SELECT idAccount, aName FROM account WHERE idAccount=\""+accountIdToTransfer+"\"");
    qryA.exec();
    while (qryA.next()) {   //Putting account id and name to variables
        transferAccountId = qryA.value(0).toString();
        transferAccountName = qryA.value(1).toString();
    }
    if((balance - (fabs(transferableMoney))) > 0){  //Check if there is enough balance
        removeMoneyFromAccount(fabs(transferableMoney));
        QSqlQuery qryA;
        //Update accounts balance
        qryA.prepare("Update account SET balance=balance +\""+QString::number(transferableMoney)+"\""
             "WHERE idAccount=\""+accountIdToTransfer+"\"");
        qryA.exec();    //Query A exec
        insertToLog(4); //Send transfer statement (4) to log
    }else {
        qDebug() << "SQLDebug 19 " << "Not enough funds to transfer" << endl;  //Debug 19
    }
}

//4.1 Deleting money from account (balance)
int DLLMySQL::removeMoneyFromAccount(double deleteMoney){
    removedMoney = fabs(deleteMoney);
    if((balance - removedMoney) >= 0){  //Check if there is enough balance
        balance -= removedMoney;
        QSqlQuery qryA;
        //Update accounts balance
        qryA.prepare("Update account SET balance=\""+QString::number(balance)+"\" WHERE idAccount=\""+idAccount+"\"");
        qryA.exec();    //Query A exec
        qDebug() << "SQLDebug 20 " << balance; //Debug 20
        return true;    //if Return true
    }else {
        qDebug() << "SQLDebug 20.1 " << removedMoney; //Debug 20.1
        return false;   //else Return false
    }
}

//5. Read transactions from log
int DLLMySQL::getTransactions(){
    logArray.clear();   //Clearing 2D vector
    QSqlTableModel transAcionModel;     //Using QSqlTableModel to get rowCount of the accounts
    transAcionModel.setTable("Log");    //Selecting Log table
    transAcionModel.setFilter("idPerson=\""+idPerson+"\" AND (actionIndex=3 OR actionIndex=4)");  //checkin right person and actionIndex
    transAcionModel.select();   //exec
    qDebug() << "SQLDebug 21 " << "transAcionModel.rowCount()" << transAcionModel.rowCount() << endl; //Debug 21
    QString array[12];
    QSqlQuery qryL;
    //Get all info from log table if idperson and actionIndex is 3 or 4
    qryL.prepare("SELECT * FROM Log where idPerson=\""+idPerson+"\" AND (actionIndex=3 OR actionIndex=4) ORDER BY idLog desc");
    qryL.exec();    //Query L exec
    for (int i = 0; i < transAcionModel.rowCount(); i++) {
        while (qryL.next()) {
            QVector<QString> tempVec;   //Temporary QString vector
            for (int j = 0; j < 12; j++) {
                array[j] = qryL.value(j).toString();    //Putting Log values to array
                tempVec.push_back(array[j]);    //Putting array value to vector
                qDebug() << "SQLDebug 22." << i << "." << j << " " << array[j] << endl; //Debug 22.*
            }
        //Temporary vector put to 2D vector
        logArray.push_back(tempVec);    //2D vector for all transactions of the person [rows][QString]
        }
    }
    //Debug############# 23.*
    for (int i = 0; i < logArray.length(); i++) {
        qDebug() << "SQLDebug 23." << i << " " << logArray.length() << endl; //Debug 23.*
        for (int j = 0; j < 12; j++) {
            qDebug() << "SQLDebug  24." << i << "." << j << " " << logArray.value(i).value(j) << endl; //Debug 24.*.*
        }
    }
    qDebug() << "SQLDebug 25 " << logArray.value(0).value(0) << endl; //Debug 25
    qDebug() << "SQLDebug 26 " << logArray.value(1).value(0) << endl; //Debug 26
    qDebug() << "SQLDebug 27 " << logArray.value(2).value(0) << endl; //Debug 27
    qDebug() << "SQLDebug 28 " << logArray.value(3).value(0) << endl; //Debug 28
    //###########Debug*
    return transAcionModel.rowCount();  //retunrs how many transactions there are
}

//6. Inserts actions to log
//1 Logged in
//2 Logged out
//3 withdrawn
//4 fundstransfer
void DLLMySQL::insertToLog(int function){
    getLatestLogID();
    switch (function) {
    case 1:     //LOGIN
        actionIndex = 1;
        action = "Logged in";
        info = fName + " " + lName + " has logged in.";
        break;
    case 2:     //LOGOUT
        actionIndex = 2;
        action = "Logged out";
        info = fName + " " + lName + " has logged out.";
        break;
    case 3:     //WITHDRAWAL
        actionIndex = 3;
        action = "withdrawal";
        money = QString::number(removedMoney) + "€";
        info = fName + " " + lName + " has withdrawn " + QString::number(removedMoney) + "€ from " + aName + " account";
        break;
    case 4:     //FUND TRANSFER
        actionIndex = 4;
        action = "fund transfer";
        money = QString::number(removedMoney) + "€";
        info = fName + " " + lName + " has transfared "
                + QString::number(removedMoney) + "€ from " + aName + " account to " + transferAccountName +  " account";
        break;
    default:
        break;
    }
    //Creating (insert) log rows
    QSqlQuery qryL;
    qryL.prepare("INSERT INTO Log VALUES (\""+QString::number(latestLogID)+"\","
         "\""+QString::number(actionIndex)+"\", \""+action+"\", \""+money+"\", \""+info+"\", "
         "\""+idPerson+"\", \""+idCard+"\", \""+aName+"\", \""+transferAccountName+"\", \""+idAccount+"\","
         "\""+transferAccountId+"\", now())");
    qryL.exec();
    //Debug########
    qDebug() << "SQLDebug 29 " << "INSERT INTO Log VALUES (\""+QString::number(latestLogID)+"\","
          "\""+QString::number(actionIndex)+"\", \""+action+"\", \""+money+"\", \""+info+"\","
          "\""+idPerson+"\", \""+idCard+"\", \""+aName+"\", \""+transferAccountName+"\", \""+idAccount+"\","
          "\""+transferAccountId+"\", now())" << endl; //Debug 29
    //#########Debug*
    nullLogValues();
}

//6.1 Getting latest log id
void DLLMySQL::getLatestLogID(){
    QSqlQuery qryL;
    //Get Max log id that has been inserted
    qryL.prepare("SELECT MAX(idLog) from Log order by idLog desc");
    qryL.exec();
    while (qryL.next()) {
        latestLogID = qryL.value(0).toInt() + 1;    //Adding one to Max log id
        qDebug() << "SQLDebug 30 " << "latestLogID = " << latestLogID << endl;  //Debug 30
    }
}

//NULL All variables
void DLLMySQL::nullValues(){
    insertToLog(2);
    fName = "";
    lName = "";
    idPerson = "";
    //Card Table Values
    idCard = "";
    cardNum = "";
    pinCode = 0;
    //Account Table Values
    idAccount = "";
    balance = 0;
    aName = "";
    idLog = 0;
    //Log Table values
    nullLogValues();
    qDebug()  << "SQLDebug 31 " << "nullValues() Done!!" << endl;  //Debug 31
}

//NULL all log values
void DLLMySQL::nullLogValues(){
    latestLogID = 0;
    action = "";
    info = "";
    money = "";
    actionIndex = 0;
    transferAccountName = "";
    transferAccountId = "";
    logArray.clear();
}

//Lock card
void DLLMySQL::lockCard(){
    QSqlQuery qryC;
    //locking card by setting cardlock from 0 to 1
    qryC.prepare("UPDATE card SET cardLock=1 WHERE idCard=\""+idCard+"\" AND cardLock=0");
    qryC.exec();
}
