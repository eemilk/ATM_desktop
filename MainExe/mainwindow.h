#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDebug>
#include <QMainWindow>
#include "dialog.h"
#include <QPixmap>
#include <QWidget>
#include <QModelIndex>
#include <QTimer>
#include <QTableView>
#include <QListWidgetItem>
#include <QWindow>

#include "C:\Users\jkort\Desktop\KouluHommat\banksimu\final\BankSimulFinal\DLLMySQL\DLLMySQL\connection.h"
#include "C:\Users\jkort\Desktop\KouluHommat\banksimu\final\BankSimulFinal\DLLMySQL\DLLMySQL\dllmysql.h"

#include "C:\Users\jkort\Desktop\KouluHommat\banksimu\final\BankSimulFinal\DLLSerialPort\dllserialport.h"

#include "C:\Users\jkort\Desktop\KouluHommat\banksimu\final\BankSimulFinal\LanguageDLL\languagedll.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void receive_albumcover(QPixmap *p);

    void timeLeftReset();

    void transActionHistorySetup();

    void accountsViewSetup(int accountCount);
public slots:
    void aikaJ();

    void slotSignal(); // RFID

    //void mainLanguageSlot();

signals:
    //PINcorrect();

private slots:
    //void on_pushButtonRFIDok_clicked();

    void on_pushButton1_clicked();
    void on_pushButton2_clicked();
    void on_pushButton3_clicked();
    void on_pushButton4_clicked();
    void on_pushButton5_clicked();
    void on_pushButton6_clicked();
    void on_pushButton7_clicked();
    void on_pushButton8_clicked();
    void on_pushButton9_clicked();

    void on_pushButtonCancel_clicked();
    void on_pushButtonLogin_clicked();

    void on_pushButtonLogout_clicked();

    void on_pushButtonWithdrawalMENU_clicked();
    void on_pushButtonFundTransferMENU_clicked();
    void on_pushButtonTransactionHistoryMENU_clicked();
    void on_pushButtonAccountsMENU_clicked();

    void on_pushButtonWithdrawalBack_clicked();
    void on_pushButtonWithdrawal20_clicked();
    void on_pushButtonWithdrawal50_clicked();
    void on_pushButtonWithdrawal100_clicked();
    void on_pushButtonWithdrawal150_clicked();
    void on_pushButtonWithdrawal200_clicked();
    void on_pushButtonWithdrawal500_clicked();
    void on_pushButtonWithdrawalCancel_clicked();
    void on_pushButtonWithdrawalConfirm_clicked();

    void on_pushButtonWithdrawalConfirmYes_clicked();
    void on_pushButtonWithdrawalConfirmNo_clicked();
    void on_listWidgetWithdrawal_itemClicked(QListWidgetItem *item);

    void on_pushButtonTransactionHistoryBack_clicked();

    void on_pushButtonAccountsBack_clicked();

    void on_pushButtonFundTransferBack_clicked();
    void on_pushButtonFundTransfer_1_clicked();
    void on_pushButtonFundTransfer_2_clicked();
    void on_pushButtonFundTransfer_3_clicked();
    void on_pushButtonFundTransfer_4_clicked();
    void on_pushButtonFundTransfer_5_clicked();
    void on_pushButtonFundTransfer_6_clicked();
    void on_pushButtonFundTransfer_7_clicked();
    void on_pushButtonFundTransfer_8_clicked();
    void on_pushButtonFundTransfer_9_clicked();
    void on_pushButtonFundTransfer_return_clicked();
    void on_pushButtonFundTransfer_cancel_clicked();
    void on_pushButtonFundTransfer_confirm_clicked();
    void on_pushButtonFundTransfer_0_clicked();
    void on_listWidgetFundTransfer_itemClicked(QListWidgetItem *item);
    void on_pushButtonTransferFundsSwap_clicked();

    void on_pushButtonFundTransferConfirmYes_clicked();
    void on_pushButtonFundTransferConfirmNo_clicked();

    void on_pushButtonBR_clicked();

    void on_pushButtonBR_2_clicked();

    void on_pushButtonBR_3_clicked();

    void on_pushButtonFIN_clicked();

    void on_pushButtonFIN_2_clicked();

    void on_pushButtonFIN_3_clicked();

    void on_pushButtonSWER_clicked();

    void on_pushButtonSWER_2_clicked();

    void on_pushButtonSWER_3_clicked();

private:
    Ui::MainWindow *ui;

    QTimer *timer;
    int timeLeft;

    Dialog *dia;

    //Serialport
    DLLSerialPort *objectRFID;
    bool rfidHasBeenRead = false;

    //SQL
    DLLMySQL *objectSQL;

    void selactAccountID(QString item);
    void selactAccountIDtoTransfer(QString item);
    int accountRowCount;

    QString passCount;
    QString passPIN;
    QString idCard;
    int incorrectCount = 4;

    int accountFunds = 1000000;
    int atmFunds = 1000000000;

    QString withdrawalAmount = "0";
    QString selectedAccountWithdrawal;
    bool withdrawalAccountSelected = false;

    QString transactionHistory1 = "Höyhtyän grilli";
    QString transactionHistory2 = "Lid";
    QString transactionHistory3 = "Prisma";
    QString transactionHistory4 = "Baari";
    QString transactionHistoryDate1 = "08.04.2019";
    QString transactionHistoryDate2 = "07.04.2019";
    QString transactionHistoryDate3 = "22.03.2019";
    QString transactionHistoryDate4 = "21.03.2019";
    QString transactionHistoryCash1 = "20€";
    QString transactionHistoryCash2 = "50€";
    QString transactionHistoryCash3 = "80€";
    QString transactionHistoryCash4 = "62€";

    QString accountNumber1 = "FI 123 1234 1234 12";
    QString accountNumber2 = "FI 143 1234 1234 12";



    QString accountBalance1 = "5000";
    QString accountBalance2 = "2345";

    QString selectedAccount;
    QString tranferAccount;

    QString transferFundAmount;
    QString lastAddedDigit = 0;
    int transferFundCount = 0;
    bool lineEditText1 = false;
    bool lineEditText2 = false;
    QString accountTransfer1;
    QString accountTransfer2;


    QString attemtp_Remain = "attempts remaining";
    QString attempt_Remain_ENG = "attempts remaining";
    QString attempt_Remain_FIN = "Yritystä jäljellä";
    QString attempt_Remain_SWE = "Försok kvar";

    QString contanct_Bank = " Contact your bank!";
    QString contact_Bank_EN = " Contact your bank!";
    QString contact_Bank_FIN = " Ota yhteys pankkiin!";
    QString contact_Bank_SWE = " Kontakta din bank!";

    LanguageDLL *objectLanguage;



};

#endif // MAINWINDOW_H
