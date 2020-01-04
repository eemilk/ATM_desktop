#include "languagedll.h"


LanguageDLL::LanguageDLL()
{
}

void LanguageDLL::wordLibrary()
{
    enter_RFID_FIN = "Ole hyvä ja syötä RFID kortti kortinlukijaan.";
    enter_RFID_ENG = "Please, enter your RFID card to the reader.";
    enter_RFID_SWE = "Vänligen ange ditt RFID kort till kortläsaren";

    welcome_FIN = "Tervetuloa!";
    welcome_EN = "Welcome!";
    welcome_SWE = "Välkommen!";

    log_out_ENG = "Log out";
    log_out_FIN = "Kirjaudu ulos";
    log_out_SWE = "Logga ut";

    log_in_ENG = "Log in";
    log_in_FIN = "Kirjaudu sisään";
    log_in_SWE = "Logga in";

    confirm_EN = "Confirm";
    confirm_FIN = "Vahvista";
    confirm_SWE = "Bekräfta";

    sum_ENG = "Sum";
    sum_FIN = "Summa";
    sum_SWE = "Summa";

    balancEvents_ENG = "Transactions";
    balancEvents_FIN = "Tilitapahtumat";
    balancEvents_SWE = "Transaktioner";

    withd_ENG = "Withdraw";
    withd_FIN = "Otto";
    withd_SWE = "Kontantuttag";

    transfer_ENG = "Fund transfer";
    transfer_FIN = "Tilisiirto";
    transfer_SWE = "Banköverföring";

    cancel_ENG = "Cancel";
    cancel_FIN = "Peruuta";
    cancel_SWE = "Avboka";

    insertPin_EN = "Please, enter your PIN code";
    insertPin_FIN = "Syötä PIN-koodisi ole hyvä.";
    insertPin_SWE = "Vänligen ange din PIN-kod";

    wrongPin_EN = "Wrong PIN code!";
    wrongPin_FIN = "Väärä PIN-koodi!";
    wrongPin_SWE = "Fel PIN kod!";

    rightPin_EN = "Correct PIN code!";
    rightPin_FIN = "PIN-koodi oikein!";
    rightPin_SWE = "Korrekt PIN cod!";

    attemptsRemainENG = "You have attempt(s) remaning.";
    attemptsRemainFIN = "Sinulla on yritystä/yritys jäljellä.";
    attemptsRemainSWE = "Du har företag kvar.";

    error_timeout_EN = "Timeout error, connection terminated!";
    error_timeout_FIN = "Yhteys aikakatkaistiin!";
    error_timeout_SWE = "Timeout fel, uppkoppling avslutad!";

    handling_request_EN = "Processing your reguest...";
    handling_request_FIN = "Tapahtumaa käsitellään...";
    handling_request_SWE = "Behandlar din förfrågan...";

    account_closed_EN = "Your account has been locked.";
    account_closed_FIN = "Tilisi on lukittu.";
    account_closed_SWE = "Ditt konto har stängts.";

    verification_EN = "Are you sure you want to do this procedure?";
    verification_FIN = "Haluatko varmasti suorittaa tämän toiminnon?";
    verification_SWE = "Är du säker på att du vill göra den här proceduren?";

    fundErrorAcc_EN = "There's not enough of money in the account!";
    fundErrorAcc_FIN = "Tilillä ei ole tarpeeksi rahaa!";
    fundErrorAcc_SWE = "Det finns inte tillräckligt med pengar på kontot";

    fundErrorATM_EN = "There's not enough of cash in the ATM!";
    fundErrorATM_FIN = "Pankkiautomaatissa ei ole tarpeeksi käteisvaroja!";
    fundErrorATM_SWE = "Det finns inte tillräckligt med pengar på en bankomat!";

    fromAcc_EN = "From account:";
    fromAcc_FIN = "Tililtä:";
    fromAcc_SWE = "Från kontot:";

    toAcc_EN = "To account:";
    toAcc_FIN = "Tilillle:";
    toAcc_SWE = "Till konto:";

    accounts_EN = "Accounts";
    accounts_FIN = "Tilit";
    accounts_SWE = "Konton";

    selectAcc_EN = "Select account.";
    selectAcc_FIN = "Valitse tili.";
    selectAcc_SWE = "Välj ett konto.";

    transferConf_EN = "Are you sure you want to trasfer?";
    transferConf_FIN = "Oletko varma, että haluat siirtää varoja?";
    transferConf_SWE = "Är du säker på att du vill överföra?";

    withDrawConf_EN = "Are you sure you want to withdraw?";
    withDrawConf_FIN = "Haluatko varmasti nostaa varoja?";
    withDrawConf_SWE = "Är du säker på att du vill dra tillbaka?";

    selectAmount_EN = "Select amount.";
    selectAmount_FIN = "Valitse rahamäärä.";
    selectAmount_SWE = "Välj antal:";

    currAccount_EN = "Current account";
    currAccount_FIN = "Käyttötili";
    currAccount_SWE = "Girokonto";

    savingsAccount_EN = "Savings account";
    savingsAccount_SWE = "Sparkonto";
    savingsAccount_FIN = "Säästötili";

    back_EN = "Return";
    back_FIN = "Takaisin";
    back_SWE = "Tillbaka";

    yes_ENG = "Yes";
    yes_FIN = "Kyllä";
    yes_SWE = "Ja";

    no_ENG = "No";
    no_FIN = "Ei";
    no_SWE = "Nej";

    emitSignalFunction();
}

void LanguageDLL::emitSignalFunction()
{
    emit languageSignal();
    qDebug() << "Emitting the signal.";
}




