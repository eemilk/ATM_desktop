#ifndef LANGUAGEDLL_H
#define LANGUAGEDLL_H

#include "languagedll_global.h"
#include <QString>
#include <QDebug>
#include <QObject>

class LANGUAGEDLLSHARED_EXPORT LanguageDLL : public QObject
{
    Q_OBJECT

public:
    LanguageDLL();
    void wordLibrary();

    QString enter_RFID_FIN;
    QString enter_RFID_ENG;
    QString enter_RFID_SWE;

    QString welcome_FIN;
    QString welcome_SWE;
    QString welcome_EN;

    QString insertPin_EN;
    QString insertPin_FIN;
    QString insertPin_SWE;

    QString wrongPin_EN;
    QString wrongPin_FIN;
    QString wrongPin_SWE;

    QString rightPin_EN;
    QString rightPin_FIN;
    QString rightPin_SWE;

    QString attemptsRemainFIN;
    QString attemptsRemainENG;
    QString attemptsRemainSWE;

    QString cancel_FIN;
    QString cancel_ENG;
    QString cancel_SWE;

    QString log_in_FIN;
    QString log_out_FIN;

    QString log_in_ENG;
    QString log_out_ENG;

    QString log_in_SWE;
    QString log_out_SWE;

    QString withd_FIN;
    QString withd_ENG;
    QString withd_SWE;

    QString balancEvents_FIN;
    QString balancEvents_ENG;
    QString balancEvents_SWE;

    QString transfer_FIN;
    QString transfer_ENG;
    QString transfer_SWE;

    QString sum_ENG;
    QString sum_FIN;
    QString sum_SWE;

    QString confirm_EN;
    QString confirm_FIN;
    QString confirm_SWE;

    QString verification_FIN;
    QString verification_SWE;
    QString verification_EN;

    QString error_timeout_EN;
    QString error_timeout_FIN;
    QString error_timeout_SWE;

    QString handling_request_EN;
    QString handling_request_FIN;
    QString handling_request_SWE;

    QString account_closed_EN;
    QString account_closed_FIN;
    QString account_closed_SWE;

    short attemptNumber = 4;

    QString fundErrorAcc_EN;
    QString fundErrorAcc_FIN;
    QString fundErrorAcc_SWE;

    QString fundErrorATM_EN;
    QString fundErrorATM_FIN;
    QString fundErrorATM_SWE;

    QString fromAcc_EN;
    QString fromAcc_FIN;
    QString fromAcc_SWE;

    QString toAcc_EN;
    QString toAcc_FIN;
    QString toAcc_SWE;

    QString accounts_EN;
    QString accounts_FIN;
    QString accounts_SWE;

    QString selectAcc_EN;
    QString selectAcc_FIN;
    QString selectAcc_SWE;

    QString transferConf_EN;
    QString transferConf_FIN;
    QString transferConf_SWE;

    QString selectAmount_EN;
    QString selectAmount_FIN;
    QString selectAmount_SWE;

    QString currAccount_EN;
    QString currAccount_FIN;
    QString currAccount_SWE;

    QString savingsAccount_EN;
    QString savingsAccount_FIN;
    QString savingsAccount_SWE;

    QString back_EN;
    QString back_FIN;
    QString back_SWE;

    QString withDrawConf_EN;
    QString withDrawConf_FIN;
    QString withDrawConf_SWE;

    QString yes_ENG;
    QString yes_FIN;
    QString yes_SWE;

    QString no_ENG;
    QString no_FIN;
    QString no_SWE;



    void emitSignalFunction();
    //QString changeAttemptNum();
    //QString returnValue;

private:

signals:
    void languageSignal();

public slots:


};

#endif // LANGUAGEDLL_H
