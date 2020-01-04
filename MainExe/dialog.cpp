#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    objectDialogLanguage  = new LanguageDLL;
    //connect(objectDialogLanguage, SIGNAL(languageSignal()), this, SLOT(dialogSlotLang()));
    objectDialogLanguage->wordLibrary();
}

Dialog::~Dialog()
{
    delete objectDialogLanguage;
    delete ui;
}

void Dialog::on_pushButton_clicked()
{
    this->close();
}

void Dialog::errorAccountLocked()
{
    ui->labelErrorDialog->setWordWrap(true);
    ui->labelErrorDialog->setText(objectDialogLanguage->account_closed_EN + "\n" + objectDialogLanguage->account_closed_FIN + "\n" + objectDialogLanguage->account_closed_SWE);
}

void Dialog::errorFundAccount()
{
    ui->labelErrorDialog->setWordWrap(true);
    ui->labelErrorDialog->setText(objectDialogLanguage->fundErrorAcc_EN + "\n" + objectDialogLanguage->fundErrorAcc_FIN + "\n" +  objectDialogLanguage->fundErrorAcc_SWE);
}

void Dialog::errorFundATM()
{
    ui->labelErrorDialog->setWordWrap(true);
    ui->labelErrorDialog->setText(objectDialogLanguage->fundErrorATM_EN + "\n" + objectDialogLanguage->fundErrorATM_FIN + "\n" + objectDialogLanguage->fundErrorATM_SWE);
}

void Dialog::errorSelectAccounts()
{
    ui->labelErrorDialog->setWordWrap(true);
    ui->labelErrorDialog->setText(objectDialogLanguage->selectAcc_EN + "\n" + objectDialogLanguage->selectAcc_FIN + "\n" + objectDialogLanguage->selectAcc_SWE);
}

void Dialog::errorSelectAccount()
{
    ui->labelErrorDialog->setWordWrap(true);
    ui->labelErrorDialog->setText(objectDialogLanguage->selectAcc_EN + "\n" + objectDialogLanguage->selectAcc_FIN + "\n" + objectDialogLanguage->selectAcc_SWE);
}

void Dialog::errorTimeout()
{
    ui->labelErrorDialog->setWordWrap(true);
    ui->labelErrorDialog->setText(objectDialogLanguage->error_timeout_EN + "\n" + objectDialogLanguage->error_timeout_FIN + "\n" + objectDialogLanguage->error_timeout_SWE);
}
/*
void Dialog::dialogSlotLang()
{

}
*/
