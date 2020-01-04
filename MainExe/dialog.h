#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QLabel>
#include "C:\Users\jkort\Desktop\KouluHommat\banksimu\final\BankSimulFinal\LanguageDLL\languagedll.h"

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

    void errorTimeout();
    void errorAccountLocked();
    void errorFundAccount();
    void errorFundATM();
    void errorSelectAccounts();
    void errorSelectAccount();


private slots:
    void on_pushButton_clicked();

    //void dialogSlotLang();

private:
    Ui::Dialog *ui;

    LanguageDLL *objectDialogLanguage;
};

#endif // DIALOG_H
