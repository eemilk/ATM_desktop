#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QWindow>


// ****Setting up UI****
//| | | | | | | | | | |
//V V V V V V V V V V V
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowState(Qt::WindowFullScreen);
    //SQL
    objectSQL = new DLLMySQL;

    //Error dialog setup
    dia = new Dialog(this);
    dia->setModal(true);


    //Ajastimen setup
    timeLeft = 20;
    timer = new QTimer(this);
    this->connect(timer, SIGNAL(timeout()), this, SLOT(aikaJ()));

    //rfidDLL setup
    objectRFID = new DLLSerialPort;
    //objectRFID->availablePorts();
    objectRFID->returnCardNumber();
    connect(objectRFID, SIGNAL(layerInterfaceSignal()), this, SLOT(slotSignal()));


    //transActionHistorySetup();

    //accountsViewSetup();

    //Taustakuva setup
    QPixmap background("C:/Users/jkort/Desktop/KouluHommat/banksimu/final/BankSimulFinal/MainExe/bankbg4.JPG");
    background = background.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Background, background);
    this->setPalette(palette);

    //ui->listWidgetFundTransfer->addItem(account1);
    //ui->listWidgetFundTransfer->addItem(account2);


    //ui->listWidgetWithdrawal->addItem(account1);
    //ui->listWidgetWithdrawal->addItem(account2);

    // LanguageDLL setup
    objectLanguage = new LanguageDLL;
    //connect(objectLanguage, SIGNAL(languageSignal()), this, SLOT(mainLanguageSlot()));
    objectLanguage->wordLibrary();
}
/*
void MainWindow::mainLanguageSlot()
{

}
*/
void MainWindow::accountsViewSetup(int accountCount)
{
    objectSQL->findAccounts();
    ui->tableWidgetAccounts->verticalHeader()->setVisible(false);
    ui->tableWidgetAccounts->horizontalHeader()->setVisible(false);

    ui->tableWidgetAccounts->setColumnWidth(0, this->width()/6);
    ui->tableWidgetAccounts->setColumnWidth(1, this->width()/5);
    ui->tableWidgetAccounts->setColumnWidth(2, this->width()/9);

    while (ui->listWidgetWithdrawal->count()>0 && ui->listWidgetFundTransfer->count()>0) {
        ui->listWidgetWithdrawal->takeItem(0);
        ui->listWidgetFundTransfer->takeItem(0);
    }

    for (int row = 0; row < ui->tableWidgetAccounts->rowCount(); row++) {

            ui->tableWidgetAccounts->setItem(row, 0, new QTableWidgetItem(""));
            ui->tableWidgetAccounts->setItem(row, 1, new QTableWidgetItem(""));
            ui->tableWidgetAccounts->setItem(row, 2, new QTableWidgetItem(""));

            qDebug() << "EXE " << ui->tableWidgetAccounts->item(row, 0);
            qDebug() << "EXE " << ui->tableWidgetAccounts->item(row, 1);
            qDebug() << "EXE " << ui->tableWidgetAccounts->item(row, 2);
    }

    qDebug() << "EXE " << accountCount;
    for (int row = 0; row < accountCount; row++) {

            ui->listWidgetWithdrawal->addItem(objectSQL->multiplemultipleAccount.value(row).value(2));
            ui->listWidgetFundTransfer->addItem(objectSQL->multiplemultipleAccount.value(row).value(2));
            ui->tableWidgetAccounts->setItem(row, 0, new QTableWidgetItem(objectSQL->multiplemultipleAccount.value(row).value(0)));
            ui->tableWidgetAccounts->setItem(row, 1, new QTableWidgetItem(objectSQL->multiplemultipleAccount.value(row).value(2)));
            ui->tableWidgetAccounts->setItem(row, 2, new QTableWidgetItem(objectSQL->multiplemultipleAccount.value(row).value(1)));

            qDebug() << "EXE " << ui->tableWidgetAccounts->item(row, 0);
            qDebug() << "EXE " << ui->tableWidgetAccounts->item(row, 1);
            qDebug() << "EXE " << ui->tableWidgetAccounts->item(row, 2);
    }
}

void MainWindow::transActionHistorySetup()
{
    //Tilitapahtumat tablen setup
    ui->tableWidget->verticalHeader()->setVisible(false);
    ui->tableWidget->horizontalHeader()->setVisible(false);

 /*   ui->tableWidget->horizontalHeaderItem(0)->setText("Date");
    ui->tableWidget->horizontalHeaderItem(1)->setText("Action");
    ui->tableWidget->horizontalHeaderItem(2)->setText("Amount");
*/
    ui->tableWidget->setColumnWidth(0, this->width()/6);
    ui->tableWidget->setColumnWidth(1, this->width()/2);
    ui->tableWidget->setColumnWidth(2, this->width()/6);

    int nTempTransactionRows = objectSQL->getTransactions();

    for (int i = 0; i <nTempTransactionRows; i++) {
        QString parseTime = objectSQL->logArray.value(i).value(11);
        QString action;
        parseTime = parseTime.left(parseTime.length() - 13);
        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(parseTime));
        switch (objectSQL->logArray.value(i).value(1).toInt()) {
        case 3:
            action = "Withdrawed ";
            ui->tableWidget->setItem(i, 1, new QTableWidgetItem(action + objectSQL->logArray.value(i).value(3)));
        break;
        case 4:
            action = "Transfered ";
            ui->tableWidget->setItem(i, 1, new QTableWidgetItem(action +objectSQL->logArray.value(i).value(3) +
                " From " + objectSQL->logArray.value(i).value(7) + " to " + objectSQL->logArray.value(i).value(8)));
        break;
        }

        ui->tableWidget->setItem(i, 2, new QTableWidgetItem(objectSQL->logArray.value(i).value(3)));
            /*qDebug() << "EXE " << objectSQL->logArray.value(i).value(8);
            qDebug() << "EXE " << objectSQL->logArray.value(i).value(3);
            qDebug() << "EXE " << objectSQL->logArray.value(i).value(2);*/
    }
}

//rfidDLL setup
void MainWindow::slotSignal()
{
    idCard = objectRFID->returnValue;
    idCard = idCard.left(idCard.length() -3);
    //str = str.left(10);
    qDebug() << idCard;
    qDebug() << "Tapahtuuko mitaan?";
    //qDebug() << objectRFID->printCardInfo();

    // Tähän yhteys tietokantaan jossa tarkistaa rfid kortin tietokannasta
    if (rfidHasBeenRead == false)
    {
        ui->stackedWidget->setCurrentIndex(1);
        rfidHasBeenRead = true;
        timer->start(1000);
        timeLeftReset();
    }
    //ui->passwordLabel->setText(idCard);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete objectSQL;
    delete objectRFID;
    delete objectLanguage;

    objectSQL = nullptr;
}

//Ajastin funktiot
void MainWindow::aikaJ()
{
    qDebug() << timeLeft;
    timeLeft -=1;
    if (timeLeft == 0)
    {
        ui->stackedWidget->setCurrentIndex(0);
        qDebug() << "aika loppui";
        //aikaL();
        timer->stop();
        dia->show();
        dia->errorTimeout();
        objectSQL->nullValues();
        selectedAccount ="";
        transferFundAmount = "";
        rfidHasBeenRead = false;
    }
}

void MainWindow::timeLeftReset()
{
    timeLeft = 20;
}

//****UI FUNCTIONS****
//| | | | | | | | | |
//V V V V V V V V V V
//---------------------------------------------------------------------------
//***************************************************************************
// RFID Login screen
void MainWindow::on_pushButtonLogout_clicked()
{
    timer->stop();
    qDebug() << "loggin out...";
    while (ui->listWidgetWithdrawal->count()>0 && ui->listWidgetFundTransfer->count()>0) {
        ui->listWidgetWithdrawal->takeItem(0);
        ui->listWidgetFundTransfer->takeItem(0);
    }
    objectSQL->nullValues();
    selectedAccount ="";
    transferFundAmount = "";
    ui->stackedWidget->setCurrentIndex(0);
    idCard = "";
    rfidHasBeenRead = false;
}
/*
void MainWindow::on_pushButtonRFIDok_clicked()
{
    timer->start(1000);
    timeLeftReset();
    ui->stackedWidget->setCurrentIndex(1);
}
*/
//----------------------------------------------------------------------------
//***************************************************************************
// PINcode login
void MainWindow::on_pushButton1_clicked()
{
    timeLeftReset();
    if (passCount != "****") {
    passCount = passCount + "*";
    passPIN = passPIN + "1";
    }
    ui->passwordLabel->setText(passCount);

}

void MainWindow::on_pushButton2_clicked()
{
    timeLeftReset();
    if (passCount != "****") {
    passCount = passCount + "*";
    passPIN = passPIN + "2";
    }
    ui->passwordLabel->setText(passCount);

}

void MainWindow::on_pushButton3_clicked()
{
    timeLeftReset();
    if (passCount != "****") {
    passCount = passCount + "*";
    passPIN = passPIN + "3";
    }
    ui->passwordLabel->setText(passCount);

}

void MainWindow::on_pushButton4_clicked()
{
    timeLeftReset();
    if (passCount != "****") {
    passCount = passCount + "*";
    passPIN = passPIN + "4";
    }
    ui->passwordLabel->setText(passCount);

}

void MainWindow::on_pushButton5_clicked()
{
    timeLeftReset();
    if (passCount != "****") {
    passCount = passCount + "*";
    passPIN = passPIN + "5";
    }
    ui->passwordLabel->setText(passCount);

}

void MainWindow::on_pushButton6_clicked()
{
    timeLeftReset();
    if (passCount != "****") {
    passCount = passCount + "*";
    passPIN = passPIN + "6";
    }
    ui->passwordLabel->setText(passCount);

}

void MainWindow::on_pushButton7_clicked()
{
    timeLeftReset();
    if (passCount != "****") {
    passCount = passCount + "*";
    passPIN = passPIN + "7";
    }
    ui->passwordLabel->setText(passCount);

}

void MainWindow::on_pushButton8_clicked()
{
    timeLeftReset();
    if (passCount != "****") {
    passCount = passCount + "*";
    passPIN = passPIN + "8";
    }
    ui->passwordLabel->setText(passCount);

}

void MainWindow::on_pushButton9_clicked()
{
    timeLeftReset();
    if (passCount != "****") {
    passCount = passCount + "*";
    passPIN = passPIN + "9" ;
    }
    ui->passwordLabel->setText(passCount);

}

void MainWindow::on_pushButtonCancel_clicked()
{
    timeLeftReset();
    passCount = "";
    passPIN = "";
    ui->passwordLabel->setText(passCount);
}

void MainWindow::on_pushButtonLogin_clicked()
{
    timeLeftReset();

    //idCard = "0A0079C7BF";
    //idCard = "0A0079C7BA";

    if (objectSQL->checkCardDetails(idCard, passPIN))
    {
        //emit PINcorrect;
        passCount = "";
        passPIN = "";
        ui->passwordLabel->setText(passCount);
        incorrectCount = 4;
        qDebug() << passPIN << " logging in...";

        ui->attemptsLabelNumber->setText("");
        ui->attemptsLabelText->setText("");

        accountRowCount = objectSQL->findAccounts();

        accountsViewSetup(accountRowCount);

        ui->stackedWidget->setCurrentIndex(3);

    }
    else //(passCount != "****" && incorrectCount > 0)
    {
        qDebug() << "incorrect PIN!";
        passCount = "";
        passPIN = "";
        ui->passwordLabel->setText(passCount);
        incorrectCount = incorrectCount - 1;
        ui->attemptsLabelNumber->setText(QString::number(incorrectCount));
        ui->attemptsLabelText->setText(attemtp_Remain);

        if (incorrectCount == 0)
        {
            ui->attemptsLabelText->setText(attemtp_Remain + contanct_Bank);
            objectSQL->lockCard();
            dia->errorAccountLocked();
            dia->show();
        }
    }
    //else if (passCount == "****" && incorrectCount != 0)


}

//---------------------------------------------------------------------------
//***************************************************************************
// Mainmenu
void MainWindow::on_pushButtonWithdrawalMENU_clicked()
{
    timeLeftReset();
    ui->stackedWidget->setCurrentIndex(5);
}

void MainWindow::on_pushButtonTransactionHistoryMENU_clicked()
{
    timeLeftReset();
    transActionHistorySetup();
    ui->stackedWidget->setCurrentIndex(4);
}

void MainWindow::on_pushButtonFundTransferMENU_clicked()
{
    timeLeftReset();
    ui->stackedWidget->setCurrentIndex(6);
}

void MainWindow::on_pushButtonAccountsMENU_clicked()
{
    timeLeftReset();
    //accountsViewSetup(accountRowCount);
    ui->stackedWidget->setCurrentIndex(7);
}
//---------------------------------------------------------------------------
//**************************************************************************
// Withdrawal
void MainWindow::on_pushButtonWithdrawalBack_clicked()
{
    timeLeftReset();
    ui->stackedWidget->setCurrentIndex(3);
    ui->stackedWidget->setCurrentIndex(3);
    withdrawalAccountSelected = false;
    ui->labelSelectedAccountWithdrawal->setText("");
    ui->labelWithdrawalSelectedAccount->setText("");
    withdrawalAmount = "0";
    ui->labelWithdrawalConfirm->setText("");
    ui->labelWithdrawalAmount->setText("");

}

void MainWindow::on_pushButtonWithdrawal20_clicked()
{
    timeLeftReset();
    withdrawalAmount = "20";
    ui->labelWithdrawalAmount->setText(withdrawalAmount);
}
void MainWindow::on_pushButtonWithdrawal50_clicked()
{
    timeLeftReset();
    withdrawalAmount = "50";
    ui->labelWithdrawalAmount->setText(withdrawalAmount);
}

void MainWindow::on_pushButtonWithdrawal100_clicked()
{
    timeLeftReset();
    withdrawalAmount = "100";
    ui->labelWithdrawalAmount->setText(withdrawalAmount);
}

void MainWindow::on_pushButtonWithdrawal150_clicked()
{
    timeLeftReset();
    withdrawalAmount = "150";
    ui->labelWithdrawalAmount->setText(withdrawalAmount);
}

void MainWindow::on_pushButtonWithdrawal200_clicked()
{
    timeLeftReset();
    withdrawalAmount = "200";
    ui->labelWithdrawalAmount->setText(withdrawalAmount);
}

void MainWindow::on_pushButtonWithdrawal500_clicked()
{
    timeLeftReset();
    withdrawalAmount = "500";
    ui->labelWithdrawalAmount->setText(withdrawalAmount);
}

void MainWindow::on_pushButtonWithdrawalCancel_clicked()
{
    timeLeftReset();
    withdrawalAmount = "0";
    ui->labelWithdrawalAmount->setText(withdrawalAmount);
}

void MainWindow::on_pushButtonWithdrawalConfirm_clicked()
{
    timeLeftReset();
    if (withdrawalAmount != "0" && withdrawalAccountSelected == true)
    {
        qDebug() << "Preparing withdrawal. Please confirm your withdrawal";
        ui->stackedWidget->setCurrentIndex(2);
        ui->labelWithdrawalConfirm->setText(withdrawalAmount);
    }
    else
    {
        qDebug() << "Please enter amount of how much you want to withdraw";
    }
    if (withdrawalAccountSelected == false)
    {
        qDebug() << "Please select an account you want to withdraw from";
        dia->show();
        dia->errorSelectAccount();
    }
    if (accountFunds < withdrawalAmount.toInt())
    {
        dia->show();
        dia->errorFundAccount();
    }
    if (atmFunds < withdrawalAmount.toInt())
    {
        dia->show();
        dia->errorFundATM();
    }

}

void MainWindow::on_listWidgetWithdrawal_itemClicked(QListWidgetItem *item)
{
    ui->labelSelectedAccountWithdrawal->setText(item->text());
    selectedAccountWithdrawal = item->text();
    ui->labelWithdrawalSelectedAccount->setText(item->text());
    selactAccountID(item->text());
    selectedAccount = selectedAccountWithdrawal;
    qDebug() << "EXE " << selectedAccountWithdrawal;

    withdrawalAccountSelected = true;
}
//-------------------------------------------------------------------------------------
//************************************************************************************
// Withdrawal confirmMenu
void MainWindow::on_pushButtonWithdrawalConfirmYes_clicked()
{
    objectSQL->withdrawMoney(withdrawalAmount.toDouble());
    qDebug() << withdrawalAmount;
    timeLeftReset();


    ui->stackedWidget->setCurrentIndex(3);
    withdrawalAccountSelected = false;
    ui->labelSelectedAccountWithdrawal->setText("");
    ui->labelWithdrawalSelectedAccount->setText("");
    withdrawalAmount = "0";
    ui->labelWithdrawalConfirm->setText("");
    ui->labelWithdrawalAmount->setText("");

}

void MainWindow::on_pushButtonWithdrawalConfirmNo_clicked()
{
    timeLeftReset();
    ui->stackedWidget->setCurrentIndex(5);
}

void MainWindow::on_pushButtonTransactionHistoryBack_clicked()
{
    timeLeftReset();
    ui->stackedWidget->setCurrentIndex(3);
}
//----------------------------------------------------------------------------------------
//*******************************************************************************************
// Fund Transfer

void MainWindow::on_pushButtonFundTransferBack_clicked()
{
    timeLeftReset();
    ui->stackedWidget->setCurrentIndex(3);

    ui->lineEditTransferAmount->setText("");
    transferFundAmount = "";
    transferFundCount = 0;
    ui->lineEditAccount1->setText("");
    ui->lineEditAccount2->setText("");
    lineEditText1 = false;
    lineEditText2 = false;

}

void MainWindow::on_pushButtonAccountsBack_clicked()
{
    timeLeftReset();
    ui->stackedWidget->setCurrentIndex(3);
}


void MainWindow::on_pushButtonFundTransfer_1_clicked()
{
    timeLeftReset();
    transferFundCount++;
    transferFundAmount = transferFundAmount + "1";
    ui->lineEditTransferAmount->setText(transferFundAmount);
}

void MainWindow::on_pushButtonFundTransfer_2_clicked()
{
    timeLeftReset();
    transferFundCount++;
    transferFundAmount = transferFundAmount + "2";
    ui->lineEditTransferAmount->setText(transferFundAmount);
}

void MainWindow::on_pushButtonFundTransfer_3_clicked()
{
    timeLeftReset();
    transferFundCount++;
    transferFundAmount = transferFundAmount + "3";
    ui->lineEditTransferAmount->setText(transferFundAmount);
}

void MainWindow::on_pushButtonFundTransfer_4_clicked()
{
    timeLeftReset();
    transferFundCount++;
    transferFundAmount = transferFundAmount + "4";
    ui->lineEditTransferAmount->setText(transferFundAmount);
}

void MainWindow::on_pushButtonFundTransfer_5_clicked()
{
    timeLeftReset();
    transferFundCount++;
    transferFundAmount = transferFundAmount + "5";
    ui->lineEditTransferAmount->setText(transferFundAmount);
}

void MainWindow::on_pushButtonFundTransfer_6_clicked()
{
    timeLeftReset();
    transferFundCount++;
    transferFundAmount = transferFundAmount + "6";
    ui->lineEditTransferAmount->setText(transferFundAmount);
    ui->lineEditTransferAmount->setText(transferFundAmount);
}

void MainWindow::on_pushButtonFundTransfer_7_clicked()
{
    timeLeftReset();
    transferFundCount++;
    transferFundAmount = transferFundAmount + "7";
    ui->lineEditTransferAmount->setText(transferFundAmount);
}

void MainWindow::on_pushButtonFundTransfer_8_clicked()
{
    timeLeftReset();
    transferFundCount++;
    transferFundAmount = transferFundAmount + "8";
    ui->lineEditTransferAmount->setText(transferFundAmount);
}

void MainWindow::on_pushButtonFundTransfer_9_clicked()
{
    timeLeftReset();
    transferFundCount++;
    transferFundAmount = transferFundAmount + "9";
    ui->lineEditTransferAmount->setText(transferFundAmount);
}

void MainWindow::on_pushButtonFundTransfer_0_clicked()
{
    timeLeftReset();
    if (transferFundAmount != "")
    {
        transferFundCount++;
        transferFundAmount = transferFundAmount + "0";
        ui->lineEditTransferAmount->setText(transferFundAmount);
    }
}

void MainWindow::on_pushButtonFundTransfer_return_clicked()
{
    timeLeftReset();
    transferFundAmount.chop(1);
    ui->lineEditTransferAmount->setText(transferFundAmount);
}

void MainWindow::on_pushButtonFundTransfer_cancel_clicked()
{
    timeLeftReset();
    transferFundAmount = "";
    transferFundCount = 0;
    ui->lineEditTransferAmount->setText("Select transfer amount");
}

void MainWindow::on_pushButtonFundTransfer_confirm_clicked()
{
    timeLeftReset();
    if (lineEditText1 == true && lineEditText2 == true && transferFundAmount != "0")
    {
        ui->labelTransferAmount->setText(transferFundAmount);
        accountTransfer1 = ui->lineEditAccount1->text();
        accountTransfer2 = ui->lineEditAccount2->text();

        ui->label_Account1->setText(accountTransfer1);
        ui->label_Account2->setText(accountTransfer2);

        ui->stackedWidget->setCurrentIndex(8);
    }
    if (transferFundAmount == "")
    {
        qDebug() << "Please select amount of the transfer";
    }
    if (lineEditText1 == false || lineEditText2 == false)
    {
        qDebug() << "Please select accounts which you want to transfer funds between";
        dia->show();
        dia->errorSelectAccounts();
    }

}

void MainWindow::on_listWidgetFundTransfer_itemClicked(QListWidgetItem *item)
{
    timeLeftReset();

    if (lineEditText2 == false && lineEditText1 == true)
    {
        ui->lineEditAccount2->setText(item->text());
        lineEditText2 = true;
    }

    if (lineEditText1 == false)
    {

        ui->lineEditAccount1->setText(item->text());
        selactAccountID(ui->lineEditAccount1->text());
        lineEditText1 = true;
    }
}

void MainWindow::on_pushButtonTransferFundsSwap_clicked()
{
    timeLeftReset();
    if ( lineEditText1 == true && lineEditText2 == true)
    {
        accountTransfer1 = ui->lineEditAccount1->text();
        accountTransfer2 = ui->lineEditAccount2->text();
        ui->lineEditAccount1->setText(accountTransfer2);
        ui->lineEditAccount2->setText(accountTransfer1);
        selactAccountID(ui->lineEditAccount1->text());
    }
}


//-------------------------------------------------------------------
//*********************************************************************
// Fund Transfer Confirm näkymä

void MainWindow::on_pushButtonFundTransferConfirmYes_clicked()
{
    timeLeftReset();
    selactAccountIDtoTransfer(ui->lineEditAccount2->text());
    ui->stackedWidget->setCurrentIndex(3);
}

void MainWindow::on_pushButtonFundTransferConfirmNo_clicked()
{
    timeLeftReset();
    ui->stackedWidget->setCurrentIndex(6);
}

//SQL Select account
void MainWindow::selactAccountID(QString item){
    qDebug() << "EXE item " << item;
    for (int i = 0; i < accountRowCount; i++) {
        if(item == objectSQL->multiplemultipleAccount.value(i).value(2))
        {
            objectSQL->selectAccount(objectSQL->multiplemultipleAccount.value(i).value(0));
        }
    }
}

//SQL Select account to transfer
void MainWindow::selactAccountIDtoTransfer(QString item){
    qDebug() << "EXE item " << item;
    for (int i = 0; i < accountRowCount; i++) {
        if(item == objectSQL->multiplemultipleAccount.value(i).value(2))
        {
            objectSQL->fundsTransfer(objectSQL->multiplemultipleAccount.value(i).value(0), transferFundAmount.toDouble());
        }
    }
}

void MainWindow::on_pushButtonBR_clicked()
{

      attemtp_Remain = attempt_Remain_ENG;
      contanct_Bank = contact_Bank_EN;


      ui->label->setText(objectLanguage->enter_RFID_ENG);
      ui->attemptsLabelText->setText(objectLanguage->attemptsRemainENG);
      ui->pushButtonCancel->setText(objectLanguage->cancel_ENG);
      ui->pushButtonLogin->setText(objectLanguage->log_in_ENG);
      ui->pushButtonLogout->setText(objectLanguage->log_out_ENG);
      ui->label_2->setText(objectLanguage->withDrawConf_EN);
      ui->labelWithdrawalConfirm_2->setText(objectLanguage->fromAcc_EN);
      ui->pushButtonWithdrawalConfirmYes->setText(objectLanguage->yes_ENG);
      ui->pushButtonWithdrawalConfirmNo->setText(objectLanguage->no_ENG);
      ui->pushButtonWithdrawalMENU->setText(objectLanguage->withd_ENG);
      ui->pushButtonTransactionHistoryMENU->setText(objectLanguage->balancEvents_ENG);
      ui->pushButtonFundTransferMENU->setText(objectLanguage->transfer_ENG);
      ui->pushButtonAccountsMENU->setText(objectLanguage->accounts_EN);
      ui->labelTransactionHistory->setText(objectLanguage->balancEvents_ENG);
      ui->pushButtonTransactionHistoryBack->setText(objectLanguage->back_EN);
      ui->label_3->setText(objectLanguage->selectAmount_EN);
      ui->label_9->setText(objectLanguage->selectAcc_EN);
      ui->pushButtonWithdrawalBack->setText(objectLanguage->back_EN);
      ui->pushButtonWithdrawalCancel->setText(objectLanguage->cancel_ENG);
      ui->pushButtonWithdrawalConfirm->setText(objectLanguage->confirm_EN);
      ui->label_4->setText(objectLanguage->transfer_ENG);
      ui->label_5->setText(objectLanguage->selectAcc_EN);
      ui->lineEditTransferAmount->setText(objectLanguage->selectAmount_EN);
      ui->pushButtonFundTransferBack->setText(objectLanguage->back_EN);
      ui->pushButtonFundTransfer_cancel->setText(objectLanguage->cancel_ENG);
      ui->pushButtonFundTransfer_confirm->setText(objectLanguage->confirm_EN);
      ui->label_6->setText(objectLanguage->accounts_EN);
      ui->pushButtonAccountsBack->setText(objectLanguage->back_EN);
      ui->label_7->setText(objectLanguage->transferConf_EN);
      ui->label_8->setText(objectLanguage->fromAcc_EN);
      ui->label_10->setText(objectLanguage->toAcc_EN);
      ui->pushButtonFundTransferConfirmYes->setText(objectLanguage->yes_ENG);
      ui->pushButtonFundTransferConfirmNo->setText(objectLanguage->no_ENG);
}

void MainWindow::on_pushButtonBR_2_clicked()
{
    timeLeftReset();

        attemtp_Remain = attempt_Remain_ENG;
        contanct_Bank = contact_Bank_EN;


        //account1 = objectLanguage->currAccount_EN;
        //account2 = objectLanguage->savingsAccount_EN;
        ui->label->setText(objectLanguage->enter_RFID_ENG);
        ui->attemptsLabelText->setText(objectLanguage->attemptsRemainENG);
        ui->pushButtonCancel->setText(objectLanguage->cancel_ENG);
        ui->pushButtonLogin->setText(objectLanguage->log_in_ENG);
        ui->pushButtonLogout->setText(objectLanguage->log_out_ENG);
        ui->label_2->setText(objectLanguage->withDrawConf_EN);
        ui->labelWithdrawalConfirm_2->setText(objectLanguage->fromAcc_EN);
        ui->pushButtonWithdrawalConfirmYes->setText(objectLanguage->yes_ENG);
        ui->pushButtonWithdrawalConfirmNo->setText(objectLanguage->no_ENG);
        ui->pushButtonWithdrawalMENU->setText(objectLanguage->withd_ENG);
        ui->pushButtonTransactionHistoryMENU->setText(objectLanguage->balancEvents_ENG);
        ui->pushButtonFundTransferMENU->setText(objectLanguage->transfer_ENG);
        ui->pushButtonAccountsMENU->setText(objectLanguage->accounts_EN);
        ui->labelTransactionHistory->setText(objectLanguage->balancEvents_ENG);
        ui->pushButtonTransactionHistoryBack->setText(objectLanguage->back_EN);
        ui->label_3->setText(objectLanguage->selectAmount_EN);
        ui->label_9->setText(objectLanguage->selectAcc_EN);
        ui->pushButtonWithdrawalBack->setText(objectLanguage->back_EN);
        ui->pushButtonWithdrawalCancel->setText(objectLanguage->cancel_ENG);
        ui->pushButtonWithdrawalConfirm->setText(objectLanguage->confirm_EN);
        ui->label_4->setText(objectLanguage->transfer_ENG);
        ui->label_5->setText(objectLanguage->selectAcc_EN);
        ui->lineEditTransferAmount->setText(objectLanguage->selectAmount_EN);
        ui->pushButtonFundTransferBack->setText(objectLanguage->back_EN);
        ui->pushButtonFundTransfer_cancel->setText(objectLanguage->cancel_ENG);
        ui->pushButtonFundTransfer_confirm->setText(objectLanguage->confirm_EN);
        ui->label_6->setText(objectLanguage->accounts_EN);
        ui->pushButtonAccountsBack->setText(objectLanguage->back_EN);
        ui->label_7->setText(objectLanguage->transferConf_EN);
        ui->label_8->setText(objectLanguage->fromAcc_EN);
        ui->label_10->setText(objectLanguage->toAcc_EN);
        ui->pushButtonFundTransferConfirmYes->setText(objectLanguage->yes_ENG);
        ui->pushButtonFundTransferConfirmNo->setText(objectLanguage->no_ENG);
}

void MainWindow::on_pushButtonBR_3_clicked()
{
    timeLeftReset();

        attemtp_Remain = attempt_Remain_ENG;
        contanct_Bank = contact_Bank_EN;


        //account1 = objectLanguage->currAccount_EN;
        //account2 = objectLanguage->savingsAccount_EN;
        ui->label->setText(objectLanguage->enter_RFID_ENG);
        ui->attemptsLabelText->setText(objectLanguage->attemptsRemainENG);
        ui->pushButtonCancel->setText(objectLanguage->cancel_ENG);
        ui->pushButtonLogin->setText(objectLanguage->log_in_ENG);
        ui->pushButtonLogout->setText(objectLanguage->log_out_ENG);
        ui->label_2->setText(objectLanguage->withDrawConf_EN);
        ui->labelWithdrawalConfirm_2->setText(objectLanguage->fromAcc_EN);
        ui->pushButtonWithdrawalConfirmYes->setText(objectLanguage->yes_ENG);
        ui->pushButtonWithdrawalConfirmNo->setText(objectLanguage->no_ENG);
        ui->pushButtonWithdrawalMENU->setText(objectLanguage->withd_ENG);
        ui->pushButtonTransactionHistoryMENU->setText(objectLanguage->balancEvents_ENG);
        ui->pushButtonFundTransferMENU->setText(objectLanguage->transfer_ENG);
        ui->pushButtonAccountsMENU->setText(objectLanguage->accounts_EN);
        ui->labelTransactionHistory->setText(objectLanguage->balancEvents_ENG);
        ui->pushButtonTransactionHistoryBack->setText(objectLanguage->back_EN);
        ui->label_3->setText(objectLanguage->selectAmount_EN);
        ui->label_9->setText(objectLanguage->selectAcc_EN);
        ui->pushButtonWithdrawalBack->setText(objectLanguage->back_EN);
        ui->pushButtonWithdrawalCancel->setText(objectLanguage->cancel_ENG);
        ui->pushButtonWithdrawalConfirm->setText(objectLanguage->confirm_EN);
        ui->label_4->setText(objectLanguage->transfer_ENG);
        ui->label_5->setText(objectLanguage->selectAcc_EN);
        ui->lineEditTransferAmount->setText(objectLanguage->selectAmount_EN);
        ui->pushButtonFundTransferBack->setText(objectLanguage->back_EN);
        ui->pushButtonFundTransfer_cancel->setText(objectLanguage->cancel_ENG);
        ui->pushButtonFundTransfer_confirm->setText(objectLanguage->confirm_EN);
        ui->label_6->setText(objectLanguage->accounts_EN);
        ui->pushButtonAccountsBack->setText(objectLanguage->back_EN);
        ui->label_7->setText(objectLanguage->transferConf_EN);
        ui->label_8->setText(objectLanguage->fromAcc_EN);
        ui->label_10->setText(objectLanguage->toAcc_EN);
        ui->pushButtonFundTransferConfirmYes->setText(objectLanguage->yes_ENG);
        ui->pushButtonFundTransferConfirmNo->setText(objectLanguage->no_ENG);
}

void MainWindow::on_pushButtonFIN_clicked()
{
    attemtp_Remain = attempt_Remain_FIN;
    contanct_Bank = contact_Bank_FIN;

    //account1 = objectLanguage->currAccount_FIN;
    //account2 = objectLanguage->savingsAccount_FIN;
    ui->label->setText(objectLanguage->enter_RFID_FIN);
    ui->attemptsLabelText->setText(objectLanguage->attemptsRemainFIN);
    ui->pushButtonCancel->setText(objectLanguage->cancel_FIN);
    ui->pushButtonLogin->setText(objectLanguage->log_in_FIN);
    ui->pushButtonLogout->setText(objectLanguage->log_out_FIN);
    ui->label_2->setText(objectLanguage->withDrawConf_FIN);
    ui->labelWithdrawalConfirm_2->setText(objectLanguage->fromAcc_FIN);
    ui->pushButtonWithdrawalConfirmYes->setText(objectLanguage->yes_FIN);
    ui->pushButtonWithdrawalConfirmNo->setText(objectLanguage->no_FIN);
    ui->pushButtonWithdrawalMENU->setText(objectLanguage->withd_FIN);
    ui->pushButtonTransactionHistoryMENU->setText(objectLanguage->balancEvents_FIN);
    ui->pushButtonFundTransferMENU->setText(objectLanguage->transfer_FIN);
    ui->pushButtonAccountsMENU->setText(objectLanguage->accounts_FIN);
    ui->labelTransactionHistory->setText(objectLanguage->balancEvents_FIN);
    ui->pushButtonTransactionHistoryBack->setText(objectLanguage->back_FIN);
    ui->label_3->setText(objectLanguage->selectAmount_FIN);
    ui->label_9->setText(objectLanguage->selectAcc_FIN);
    ui->pushButtonWithdrawalBack->setText(objectLanguage->back_FIN);
    ui->pushButtonWithdrawalCancel->setText(objectLanguage->cancel_FIN);
    ui->pushButtonWithdrawalConfirm->setText(objectLanguage->confirm_FIN);
    ui->label_4->setText(objectLanguage->transfer_FIN);
    ui->label_5->setText(objectLanguage->selectAcc_FIN);
    ui->lineEditTransferAmount->setText(objectLanguage->selectAmount_FIN);
    ui->pushButtonFundTransferBack->setText(objectLanguage->back_FIN);
    ui->pushButtonFundTransfer_cancel->setText(objectLanguage->cancel_FIN);
    ui->pushButtonFundTransfer_confirm->setText(objectLanguage->confirm_FIN);
    ui->label_6->setText(objectLanguage->accounts_FIN);
    ui->pushButtonAccountsBack->setText(objectLanguage->back_FIN);
    ui->label_7->setText(objectLanguage->transferConf_FIN);
    ui->label_8->setText(objectLanguage->fromAcc_FIN);
    ui->label_10->setText(objectLanguage->toAcc_FIN);
    ui->pushButtonFundTransferConfirmYes->setText(objectLanguage->yes_FIN);
    ui->pushButtonFundTransferConfirmNo->setText(objectLanguage->no_FIN);
}

void MainWindow::on_pushButtonFIN_2_clicked()
{
    timeLeftReset();

    attemtp_Remain = attempt_Remain_FIN;
    contanct_Bank = contact_Bank_FIN;


    //account1 = objectLanguage->currAccount_FIN;
    //account2 = objectLanguage->savingsAccount_FIN;
    ui->label->setText(objectLanguage->enter_RFID_FIN);
    ui->attemptsLabelText->setText(objectLanguage->attemptsRemainFIN);
    ui->pushButtonCancel->setText(objectLanguage->cancel_FIN);
    ui->pushButtonLogin->setText(objectLanguage->log_in_FIN);
    ui->pushButtonLogout->setText(objectLanguage->log_out_FIN);
    ui->label_2->setText(objectLanguage->withDrawConf_FIN);
    ui->labelWithdrawalConfirm_2->setText(objectLanguage->fromAcc_FIN);
    ui->pushButtonWithdrawalConfirmYes->setText(objectLanguage->yes_FIN);
    ui->pushButtonWithdrawalConfirmNo->setText(objectLanguage->no_FIN);
    ui->pushButtonWithdrawalMENU->setText(objectLanguage->withd_FIN);
    ui->pushButtonTransactionHistoryMENU->setText(objectLanguage->balancEvents_FIN);
    ui->pushButtonFundTransferMENU->setText(objectLanguage->transfer_FIN);
    ui->pushButtonAccountsMENU->setText(objectLanguage->accounts_FIN);
    ui->labelTransactionHistory->setText(objectLanguage->balancEvents_FIN);
    ui->pushButtonTransactionHistoryBack->setText(objectLanguage->back_FIN);
    ui->label_3->setText(objectLanguage->selectAmount_FIN);
    ui->label_9->setText(objectLanguage->selectAcc_FIN);
    ui->pushButtonWithdrawalBack->setText(objectLanguage->back_FIN);
    ui->pushButtonWithdrawalCancel->setText(objectLanguage->cancel_FIN);
    ui->pushButtonWithdrawalConfirm->setText(objectLanguage->confirm_FIN);
    ui->label_4->setText(objectLanguage->transfer_FIN);
    ui->label_5->setText(objectLanguage->selectAcc_FIN);
    ui->lineEditTransferAmount->setText(objectLanguage->selectAmount_FIN);
    ui->pushButtonFundTransferBack->setText(objectLanguage->back_FIN);
    ui->pushButtonFundTransfer_cancel->setText(objectLanguage->cancel_FIN);
    ui->pushButtonFundTransfer_confirm->setText(objectLanguage->confirm_FIN);
    ui->label_6->setText(objectLanguage->accounts_FIN);
    ui->pushButtonAccountsBack->setText(objectLanguage->back_FIN);
    ui->label_7->setText(objectLanguage->transferConf_FIN);
    ui->label_8->setText(objectLanguage->fromAcc_FIN);
    ui->label_10->setText(objectLanguage->toAcc_FIN);
    ui->pushButtonFundTransferConfirmYes->setText(objectLanguage->yes_FIN);
    ui->pushButtonFundTransferConfirmNo->setText(objectLanguage->no_FIN);
}

void MainWindow::on_pushButtonFIN_3_clicked()
{
    timeLeftReset();

    attemtp_Remain = attempt_Remain_FIN;
    contanct_Bank = contact_Bank_FIN;


    //account1 = objectLanguage->currAccount_FIN;
    //account2 = objectLanguage->savingsAccount_FIN;
    ui->label->setText(objectLanguage->enter_RFID_FIN);
    ui->attemptsLabelText->setText(objectLanguage->attemptsRemainFIN);
    ui->pushButtonCancel->setText(objectLanguage->cancel_FIN);
    ui->pushButtonLogin->setText(objectLanguage->log_in_FIN);
    ui->pushButtonLogout->setText(objectLanguage->log_out_FIN);
    ui->label_2->setText(objectLanguage->withDrawConf_FIN);
    ui->labelWithdrawalConfirm_2->setText(objectLanguage->fromAcc_FIN);
    ui->pushButtonWithdrawalConfirmYes->setText(objectLanguage->yes_FIN);
    ui->pushButtonWithdrawalConfirmNo->setText(objectLanguage->no_FIN);
    ui->pushButtonWithdrawalMENU->setText(objectLanguage->withd_FIN);
    ui->pushButtonTransactionHistoryMENU->setText(objectLanguage->balancEvents_FIN);
    ui->pushButtonFundTransferMENU->setText(objectLanguage->transfer_FIN);
    ui->pushButtonAccountsMENU->setText(objectLanguage->accounts_FIN);
    ui->labelTransactionHistory->setText(objectLanguage->balancEvents_FIN);
    ui->pushButtonTransactionHistoryBack->setText(objectLanguage->back_FIN);
    ui->label_3->setText(objectLanguage->selectAmount_FIN);
    ui->label_9->setText(objectLanguage->selectAcc_FIN);
    ui->pushButtonWithdrawalBack->setText(objectLanguage->back_FIN);
    ui->pushButtonWithdrawalCancel->setText(objectLanguage->cancel_FIN);
    ui->pushButtonWithdrawalConfirm->setText(objectLanguage->confirm_FIN);
    ui->label_4->setText(objectLanguage->transfer_FIN);
    ui->label_5->setText(objectLanguage->selectAcc_FIN);
    ui->lineEditTransferAmount->setText(objectLanguage->selectAmount_FIN);
    ui->pushButtonFundTransferBack->setText(objectLanguage->back_FIN);
    ui->pushButtonFundTransfer_cancel->setText(objectLanguage->cancel_FIN);
    ui->pushButtonFundTransfer_confirm->setText(objectLanguage->confirm_FIN);
    ui->label_6->setText(objectLanguage->accounts_FIN);
    ui->pushButtonAccountsBack->setText(objectLanguage->back_FIN);
    ui->label_7->setText(objectLanguage->transferConf_FIN);
    ui->label_8->setText(objectLanguage->fromAcc_FIN);
    ui->label_10->setText(objectLanguage->toAcc_FIN);
    ui->pushButtonFundTransferConfirmYes->setText(objectLanguage->yes_FIN);
    ui->pushButtonFundTransferConfirmNo->setText(objectLanguage->no_FIN);
}

void MainWindow::on_pushButtonSWER_clicked()
{
    attemtp_Remain = attempt_Remain_SWE;
    contanct_Bank = contact_Bank_SWE;


    //account1 = objectLanguage->currAccount_SWE;
    //account2 = objectLanguage->savingsAccount_SWE;
    ui->label->setText(objectLanguage->enter_RFID_SWE);
    ui->attemptsLabelText->setText(objectLanguage->attemptsRemainSWE);
    ui->pushButtonCancel->setText(objectLanguage->cancel_SWE);
    ui->pushButtonLogin->setText(objectLanguage->log_in_SWE);
    ui->pushButtonLogout->setText(objectLanguage->log_out_SWE);
    ui->label_2->setText(objectLanguage->withDrawConf_SWE);
    ui->labelWithdrawalConfirm_2->setText(objectLanguage->fromAcc_SWE);
    ui->pushButtonWithdrawalConfirmYes->setText(objectLanguage->yes_SWE);
    ui->pushButtonWithdrawalConfirmNo->setText(objectLanguage->no_SWE);
    ui->pushButtonWithdrawalMENU->setText(objectLanguage->withd_SWE);
    ui->pushButtonTransactionHistoryMENU->setText(objectLanguage->balancEvents_SWE);
    ui->pushButtonFundTransferMENU->setText(objectLanguage->transfer_SWE);
    ui->pushButtonAccountsMENU->setText(objectLanguage->accounts_SWE);
    ui->labelTransactionHistory->setText(objectLanguage->balancEvents_SWE);
    ui->pushButtonTransactionHistoryBack->setText(objectLanguage->back_SWE);
    ui->label_3->setText(objectLanguage->selectAmount_SWE);
    ui->label_9->setText(objectLanguage->selectAcc_SWE);
    ui->pushButtonWithdrawalBack->setText(objectLanguage->back_SWE);
    ui->pushButtonWithdrawalCancel->setText(objectLanguage->cancel_SWE);
    ui->pushButtonWithdrawalConfirm->setText(objectLanguage->confirm_SWE);
    ui->label_4->setText(objectLanguage->transfer_SWE);
    ui->label_5->setText(objectLanguage->selectAcc_SWE);
    ui->lineEditTransferAmount->setText(objectLanguage->selectAmount_SWE);
    ui->pushButtonFundTransferBack->setText(objectLanguage->back_SWE);
    ui->pushButtonFundTransfer_cancel->setText(objectLanguage->cancel_SWE);
    ui->pushButtonFundTransfer_confirm->setText(objectLanguage->confirm_SWE);
    ui->label_6->setText(objectLanguage->accounts_SWE);
    ui->pushButtonAccountsBack->setText(objectLanguage->back_SWE);
    ui->label_7->setText(objectLanguage->transferConf_SWE);
    ui->label_8->setText(objectLanguage->fromAcc_SWE);
    ui->label_10->setText(objectLanguage->toAcc_SWE);
    ui->pushButtonFundTransferConfirmYes->setText(objectLanguage->yes_SWE);
    ui->pushButtonFundTransferConfirmNo->setText(objectLanguage->no_SWE);
}

void MainWindow::on_pushButtonSWER_2_clicked()
{
    timeLeftReset();

    attemtp_Remain = attempt_Remain_SWE;
    contanct_Bank = contact_Bank_SWE;

    //account1 = objectLanguage->currAccount_SWE;
    //account2 = objectLanguage->savingsAccount_SWE;
    ui->label->setText(objectLanguage->enter_RFID_SWE);
    ui->attemptsLabelText->setText(objectLanguage->attemptsRemainSWE);
    ui->pushButtonCancel->setText(objectLanguage->cancel_SWE);
    ui->pushButtonLogin->setText(objectLanguage->log_in_SWE);
    ui->pushButtonLogout->setText(objectLanguage->log_out_SWE);
    ui->label_2->setText(objectLanguage->withDrawConf_SWE);
    ui->labelWithdrawalConfirm_2->setText(objectLanguage->fromAcc_SWE);
    ui->pushButtonWithdrawalConfirmYes->setText(objectLanguage->yes_SWE);
    ui->pushButtonWithdrawalConfirmNo->setText(objectLanguage->no_SWE);
    ui->pushButtonWithdrawalMENU->setText(objectLanguage->withd_SWE);
    ui->pushButtonTransactionHistoryMENU->setText(objectLanguage->balancEvents_SWE);
    ui->pushButtonFundTransferMENU->setText(objectLanguage->transfer_SWE);
    ui->pushButtonAccountsMENU->setText(objectLanguage->accounts_SWE);
    ui->labelTransactionHistory->setText(objectLanguage->balancEvents_SWE);
    ui->pushButtonTransactionHistoryBack->setText(objectLanguage->back_SWE);
    ui->label_3->setText(objectLanguage->selectAmount_SWE);
    ui->label_9->setText(objectLanguage->selectAcc_SWE);
    ui->pushButtonWithdrawalBack->setText(objectLanguage->back_SWE);
    ui->pushButtonWithdrawalCancel->setText(objectLanguage->cancel_SWE);
    ui->pushButtonWithdrawalConfirm->setText(objectLanguage->confirm_SWE);
    ui->label_4->setText(objectLanguage->transfer_SWE);
    ui->label_5->setText(objectLanguage->selectAcc_SWE);
    ui->lineEditTransferAmount->setText(objectLanguage->selectAmount_SWE);
    ui->pushButtonFundTransferBack->setText(objectLanguage->back_SWE);
    ui->pushButtonFundTransfer_cancel->setText(objectLanguage->cancel_SWE);
    ui->pushButtonFundTransfer_confirm->setText(objectLanguage->confirm_SWE);
    ui->label_6->setText(objectLanguage->accounts_SWE);
    ui->pushButtonAccountsBack->setText(objectLanguage->back_SWE);
    ui->label_7->setText(objectLanguage->transferConf_SWE);
    ui->label_8->setText(objectLanguage->fromAcc_SWE);
    ui->label_10->setText(objectLanguage->toAcc_SWE);
    ui->pushButtonFundTransferConfirmYes->setText(objectLanguage->yes_SWE);
    ui->pushButtonFundTransferConfirmNo->setText(objectLanguage->no_SWE);
}

void MainWindow::on_pushButtonSWER_3_clicked()
{
    timeLeftReset();

    attemtp_Remain = attempt_Remain_SWE;
    contanct_Bank = contact_Bank_SWE;


    //account1 = objectLanguage->currAccount_SWE;
    //account2 = objectLanguage->savingsAccount_SWE;
    ui->label->setText(objectLanguage->enter_RFID_SWE);
    ui->attemptsLabelText->setText(objectLanguage->attemptsRemainSWE);
    ui->pushButtonCancel->setText(objectLanguage->cancel_SWE);
    ui->pushButtonLogin->setText(objectLanguage->log_in_SWE);
    ui->pushButtonLogout->setText(objectLanguage->log_out_SWE);
    ui->label_2->setText(objectLanguage->withDrawConf_SWE);
    ui->labelWithdrawalConfirm_2->setText(objectLanguage->fromAcc_SWE);
    ui->pushButtonWithdrawalConfirmYes->setText(objectLanguage->yes_SWE);
    ui->pushButtonWithdrawalConfirmNo->setText(objectLanguage->no_SWE);
    ui->pushButtonWithdrawalMENU->setText(objectLanguage->withd_SWE);
    ui->pushButtonTransactionHistoryMENU->setText(objectLanguage->balancEvents_SWE);
    ui->pushButtonFundTransferMENU->setText(objectLanguage->transfer_SWE);
    ui->pushButtonAccountsMENU->setText(objectLanguage->accounts_SWE);
    ui->labelTransactionHistory->setText(objectLanguage->balancEvents_SWE);
    ui->pushButtonTransactionHistoryBack->setText(objectLanguage->back_SWE);
    ui->label_3->setText(objectLanguage->selectAmount_SWE);
    ui->label_9->setText(objectLanguage->selectAcc_SWE);
    ui->pushButtonWithdrawalBack->setText(objectLanguage->back_SWE);
    ui->pushButtonWithdrawalCancel->setText(objectLanguage->cancel_SWE);
    ui->pushButtonWithdrawalConfirm->setText(objectLanguage->confirm_SWE);
    ui->label_4->setText(objectLanguage->transfer_SWE);
    ui->label_5->setText(objectLanguage->selectAcc_SWE);
    ui->lineEditTransferAmount->setText(objectLanguage->selectAmount_SWE);
    ui->pushButtonFundTransferBack->setText(objectLanguage->back_SWE);
    ui->pushButtonFundTransfer_cancel->setText(objectLanguage->cancel_SWE);
    ui->pushButtonFundTransfer_confirm->setText(objectLanguage->confirm_SWE);
    ui->label_6->setText(objectLanguage->accounts_SWE);
    ui->pushButtonAccountsBack->setText(objectLanguage->back_SWE);
    ui->label_7->setText(objectLanguage->transferConf_SWE);
    ui->label_8->setText(objectLanguage->fromAcc_SWE);
    ui->label_10->setText(objectLanguage->toAcc_SWE);
    ui->pushButtonFundTransferConfirmYes->setText(objectLanguage->yes_SWE);
    ui->pushButtonFundTransferConfirmNo->setText(objectLanguage->no_SWE);
}
