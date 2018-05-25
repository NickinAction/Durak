#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMainWindow>
#include <deque>
#include <random>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <QLineEdit>
#include <QMessageBox>
#include <QDebug>
#include <QString>
#include <QShortcut>
#include <thread>
#include <QKeySequence>
#include <QToolTip>
#include <unistd.h>
#include <a.out.h>
#include <stdio.h>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    deckGenerator();
    giveoutCards();
    decideFirstPlayer();

    devModeShortcut = new QShortcut(QKeySequence(tr("Ctrl+Shift+Alt+D")), this);

    connect(this->devModeShortcut, SIGNAL(activated()), this, SLOT(toggleDevMode()));
    connect(this->ui->cardInput, SIGNAL(returnPressed()), this, SLOT(readCardFilter()));
    connect(this->ui->readInput, SIGNAL(clicked()), this, SLOT(readCardFilter()));
    connect(this->ui->finishTurn, SIGNAL(clicked()), this, SLOT(playerFinishTurn()));
    connect(this->ui->passButton, SIGNAL(clicked()), this, SLOT(playerPass()));
    connect(this->ui->takeButton, SIGNAL(clicked()), this, SLOT(takeCards()));
    connect(this->ui->SECRETDEVBUTTON, SIGNAL(clicked()), this, SLOT(showCredits()));

    if (fork() == 0) {
        execl("../Durak_Nikity_2/Toast.py", "../Durak_Nikity_2/Toast.py", "Test title", "Test message", "1", NULL);
        perror("exec Toast failed");
        _exit(127);
    }

    impossibleAttackAttempt.setText("You cannot currently place this card.");
    impossibleDefenseAttempt.setText("That card can't beat the opponent's card.");
    incorrectInput.setText("This card does not exist in your deck.");
    incorrectTimeError.setText("Please wait for your turn.");
    impossibleTakeAttempt.setText("You cannot take anything now.");
    impossiblePassAttempt.setText("You cannot currently pass your turn.");
    impossibleFinishTurnAttempt.setText("You cannot currently finish your turn.");
    playerWins.setText("The Player Wins!");
    computerWins.setText("The Computer Wins!");
    credits.setText("Why did you even click this?"
                    "\nAnyway, this program has been made by NickinAction"
                    "\nwith the help of QuantumCat.");


    update();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::deckGenerator() {
    shuffledDeck.resize(36);
    srand(time(NULL));
    for (int i = 0; i < 36; i++) {
        shuffledDeck[i] = starterDeck[i];
    }
    for (int i = 0; i < 10000; i++) {
        std::swap(shuffledDeck[rand()%36], shuffledDeck[rand()%36]);
    }
}

void MainWindow::giveoutCards() {
    trumpSuit = getCardSuit(shuffledDeck.front());
    //std::string x;
    for (int i = 0; i < 6; i++){
        opponentDeck.push_back(shuffledDeck.back());
        //x = x + " " + shuffledDeck.back();
        shuffledDeck.pop_back();
    }
    for (int i = 0; i < 6; i++){
        playerDeck.push_back(shuffledDeck.back());
        //x = x + " " + shuffledDeck.back();
        shuffledDeck.pop_back();
    }
    update();
}

void MainWindow::decideFirstPlayer() {
    int smallesttrumpSuit = 15;
    for (int i = 0; i < 6; i++) {
        if (getCardSuit(playerDeck[i]) == trumpSuit && getCardRank(playerDeck[i]) < smallesttrumpSuit) {
            smallesttrumpSuit = getCardRank(playerDeck[i]);
        }
    }
    int y = smallesttrumpSuit;
    smallesttrumpSuit = 15;

    for (int i = 0; i < 6; i++) {
        if (getCardSuit(opponentDeck[i]) == trumpSuit && getCardRank(opponentDeck[i]) < smallesttrumpSuit) {
            smallesttrumpSuit = getCardRank(opponentDeck[i]);
        }
    }
    int z = smallesttrumpSuit;
    if (z < y) { //can't be equal since it's a single deck
        state = "PA"; //CHANGE DIS IN FUTUR PLZZZZ !!!11
    }
    else {
        //If there was no trump card, both y & z == 15, so human goes first
        state = "PA";
    }
}

void MainWindow::placeAttackingCard(int cardNum) { //PA -> PA, does not change the state
    QString card = playerDeck[cardNum];
    int cardRank = getCardRank(card);
    if (tableCards.empty() && tempBeatenCards.empty()) {
        tableCards.push_back(card);
        playerDeck.erase(playerDeck.begin() + cardNum, playerDeck.begin() + cardNum + 1);
        update();

    }
    else {
        bool sameRankExists = false;
        for (unsigned i = 0; i < tableCards.size(); i++) {
            if (cardRank == getCardRank(tableCards[i])) {
                sameRankExists = true;
                break;
            }
        }
        if (!sameRankExists) {
            for (unsigned i = 0; i < tempBeatenCards.size(); i++) {
                if (cardRank == getCardRank(tempBeatenCards[i])) {
                    sameRankExists = true;
                    break;
                }
            }
        }
        if (sameRankExists) {
            tableCards.push_back(card);
            playerDeck.erase(playerDeck.begin() + cardNum, playerDeck.begin() + cardNum + 1);
            update();
        }
    }
}

void MainWindow::playerPass() { //PA -> CD
    if (state != "PA" || tableCards.empty()) {
        impossiblePassAttempt.exec();
        return;
    }
    state = "CD";
    qDebug("Player passed.");
    computerDefends();
}

void MainWindow::playerFinishTurn() { //PA -> CA
    if (state != "PA" || !(tableCards.empty() && !tempBeatenCards.empty())) {
        impossibleFinishTurnAttempt.exec();
        return;
    }
    tempBeatenCards.clear();
    fillHands();
    state = "CA";
    computerAttacks();
}

void MainWindow::placeDefendingCard(int cardNum) { //PD -> CA
    QString defendingCard = playerDeck[cardNum];
    QChar attackingCardSuit = getCardSuit(tableCards[0]);
    int attackingCardRank = getCardRank(tableCards[0]);

    if ((getCardSuit(defendingCard) == trumpSuit && attackingCardSuit != trumpSuit)
            ||(getCardSuit(defendingCard) == attackingCardSuit &&
               getCardRank(defendingCard) > attackingCardRank)) {
        tempBeatenCards.push_back(tableCards[0]);
        tempBeatenCards.push_back(playerDeck[cardNum]);
        tableCards.erase(tableCards.begin(), tableCards.begin() + 1);
        playerDeck.erase(playerDeck.begin() + cardNum, playerDeck.begin() + cardNum + 1);
        update();
    }
    else {
        impossibleDefenseAttempt.exec();
        update();
    }

    if (tableCards.empty()) {
        state = "CA";
        computerAttacks();
    }

}

void MainWindow::takeCards() { //PD -> CA
    for (unsigned i = 0; i < tableCards.size(); i++) {
        playerDeck.push_back(tableCards[i]);
    }
    tableCards.clear();

    for (unsigned i = 0; i < tempBeatenCards.size(); i++) {
        playerDeck.push_back(tempBeatenCards[i]);
    }
    tempBeatenCards.clear();
    fillHands();
    state = "CA";
    computerAttacks();

}

void MainWindow::computerAttacks() { //CA -> PA/PD
    QString attackingCard;
    bool skip = false;
    int attackingCardNum;

    if (!tableCards.empty() || !tempBeatenCards.empty()) {
        bool cardFound = false;
        for (unsigned i = 0; i < tableCards.size(); i++) {
            for (unsigned j = 0; j < opponentDeck.size(); j++) {
                if (opponentDeck[j] == tableCards[i]) {
                    attackingCard = opponentDeck[j];
                    attackingCardNum = j;
                    skip = true;
                    cardFound = true;
                    break;

                }
            }
            if (skip) break;
        }
        if (cardFound) {
            tableCards.push_back(attackingCard);
            opponentDeck.erase(opponentDeck.begin() + attackingCardNum, opponentDeck.begin() + attackingCardNum + 1);
            state = "PD";
        } else {
            tableCards.clear();
            tempBeatenCards.clear();
            fillHands();
            state = "PA";
        }
    }
    else {
        attackingCardNum = rand() % 6;
        attackingCard = opponentDeck[attackingCardNum];
        tableCards.push_back(attackingCard);
        opponentDeck.erase(opponentDeck.begin() + attackingCardNum, opponentDeck.begin() + attackingCardNum + 1);
        state = "PD";
    }
    update();
}

void MainWindow::computerDefends() { //CD -> PA
    qDebug("Computer starts its defence");
    bool existence;
    int bestCardNum = -1;
    while(!tableCards.empty()) {
        existence = false;
        QString attackingCard = tableCards[0];

        for (unsigned i = 0; i < opponentDeck.size(); i++) {
           if (firstCardBeatsSecond(opponentDeck[i], attackingCard)) {
               existence = true;
               if (bestCardNum == -1 ||
                       firstCardBeatsSecond(opponentDeck[bestCardNum], opponentDeck[i])) {
                   bestCardNum = i;
               }
            }
        }
        if (existence) {
            tempBeatenCards.push_back(tableCards[0]);
            tempBeatenCards.push_back(opponentDeck[bestCardNum]);
            tableCards.erase(tableCards.begin(), tableCards.begin() + 1);
            opponentDeck.erase(opponentDeck.begin() + bestCardNum, opponentDeck.begin() + bestCardNum + 1);
        }
        else break;
    }

    //here we'll take
    if(!existence) {
        qDebug("The card doesn't exist");
        for (unsigned i = 0; i < tableCards.size(); i++) {
            opponentDeck.push_back(tableCards[i]);
        }

        for (unsigned i = 0; i < tempBeatenCards.size(); i++) {
            opponentDeck.push_back(tempBeatenCards[i]);
        }
        tableCards.clear();
        tempBeatenCards.clear();
        fillHands();
    }
    update();
    state = "PA";
}

void MainWindow::fillHands() { // MID-TURN
    if (state == "PA" || state == "CD"){
        for (int i = 0; playerDeck.size() < 6; i++) {
                if (shuffledDeck.empty()) break;
                playerDeck.push_back(shuffledDeck.back());
                shuffledDeck.pop_back();
            }
        for (int i = 0; opponentDeck.size() < 6; i++) {
               if (shuffledDeck.empty()) break;
               opponentDeck.push_back(shuffledDeck.back());
               shuffledDeck.pop_back();
           }
        update();
    }
    else {
        for (int i = 0; opponentDeck.size() < 6; i++) {
               if (shuffledDeck.empty()) break;
               opponentDeck.push_back(shuffledDeck.back());
               shuffledDeck.pop_back();
           }
        for (int i = 0; playerDeck.size() < 6; i++) {
                if (shuffledDeck.empty()) break;
                playerDeck.push_back(shuffledDeck.back());
                shuffledDeck.pop_back();
            }
        update();
    }
    if (playerDeck.empty()) {
        playerWins.exec();
        QApplication::quit();
    }
    if (opponentDeck.empty()) {
        computerWins.exec();
        QApplication::quit();
    }

}

void MainWindow::readCardFilter() {
    QString card = this->ui->cardInput->text().toUpper();
    this->ui->cardInput->setText("");
    int cardNum = -145;
    QString currentState = getCurrentState();

    bool existence_of_card = false;

    for (unsigned i = 0; i < playerDeck.size(); i++) {
        if (playerDeck[i] == card) {
            existence_of_card = true;
            cardNum = i;
            break;
        }
    }

    if (!existence_of_card) {
        incorrectInput.exec();
        return;
    }

    if (currentState == "PA") placeAttackingCard(cardNum);
    else if (currentState == "PD") placeDefendingCard(cardNum);
    else incorrectTimeError.exec();
}

bool MainWindow::firstCardBeatsSecond(QString card1, QString card2) {
    if ((getCardSuit(card1) == trumpSuit && getCardSuit(card2) != trumpSuit)
            ||(getCardSuit(card1) == getCardSuit(card2) &&
               getCardRank(card1) > getCardRank(card2))) {
        return true;
    }
    else return false;
}

QChar MainWindow::getCardSuit(QString card) {
    return QChar(card[card.size()-1]);
}

QString MainWindow::getCurrentState() {
    return state;
}

int MainWindow::getCardRank(QString card) {
    char c = QChar(card[0]).QChar::toLatin1();
    if (c >= '6' && c <= '9') return c-'0';
    switch (c) {
        case '1':
            return(10);
        case 'J':
            return(11);
        case 'Q':
            return(12);
        case 'K':
            return(13);
        case 'A':
            return(14);
        default:
            return -145; //Error code
    }
}

void MainWindow::showCredits(){
    if(devMode) { //Show the opponent's cards
    QString opponentDeckLabel = "";
    for (unsigned i = 0; i < opponentDeck.size(); i++) {
        opponentDeckLabel = opponentDeckLabel + " " + opponentDeck[i];
    }
    this->ui->OPPONENT->setText(opponentDeckLabel);
    } else {
        credits.exec();
    }
}

void MainWindow::update() {
    this->ui->DECK->setText("LEFT IN DECK: " + QString::fromStdString(std::to_string(shuffledDeck.size())) +
                            "\n\n\n\nTRUMP SUIT: " +  trumpSuit);
    QString playerDeckLabel;
    QString tableCardsLabel;
    QString tbcLabel;

    for (unsigned i = 0; i < playerDeck.size(); i++) {
        playerDeckLabel = playerDeckLabel + " " + playerDeck[i];
    }

    for (unsigned i = 0; i < tableCards.size(); i++) {
        tableCardsLabel = tableCardsLabel + " " + tableCards[i];
    }

    for (unsigned i = 0; i < tempBeatenCards.size(); i++) {
        tbcLabel = tbcLabel + " " + tempBeatenCards[i];
    }

    this->ui->YOU->setText(playerDeckLabel);
    this->ui->TABLE->setText("TABLE: " + tableCardsLabel);
    this->ui->OPPONENT->setText(QString::fromStdString(std::string(opponentDeck.size(), 'X')));
    this->ui->TBC->setText("TBC: " + tbcLabel);
}

void MainWindow::toggleDevMode() {
    devMode = !devMode;
}
