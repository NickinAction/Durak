#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <deque>
#include <random>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <QLineEdit>
#include <QMessageBox>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->playerTurnIndicator->setStyleSheet("QLabel {background-color: #5fcc35;}");

    deckGenerator();
    giveoutCards();
    connect(this->ui->readInput, SIGNAL(clicked()), this, SLOT(redirectInput()));
    connect(this->ui->finishTurn, SIGNAL(clicked()), this, SLOT(opponentAttackingTurn()));
    connect(this->ui->passButton, SIGNAL(clicked()), this, SLOT(opponentDefendingTurn()));
    connect(this->ui->takeButton, SIGNAL(clicked()), this, SLOT(takeCards()));
    impossibleFunctionAttempt.setText("You cannot do that.");
    impossibleDefenseAttempt.setText("That card can't beat the opponent's card");
    incorrectInput.setText("Your input was inappropriate, please try again.");
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
    trumpCard = getCardSuit(shuffledDeck.front());
    //std::string x;
    for (int i = 0; i < 6; i++){
        opponentDeck.push_back(shuffledDeck.back());
        //x = x + " " + shuffledDeck.back();
        shuffledDeck.pop_back();
    }
    for (int i = 0; i < 6; i++){
        myDeck.push_back(shuffledDeck.back());
        //x = x + " " + shuffledDeck.back();
        shuffledDeck.pop_back();
    }
    updateAll();
}

void MainWindow::decideFirstPlayer() {
    int smallestTrumpCard = 15;
    for (int i = 0; i < 6; i++) {
        if (getCardSuit(myDeck[i]) == trumpCard && getCardRank(myDeck[i]) < smallestTrumpCard) {
            smallestTrumpCard = getCardRank(myDeck[i]);
        }
    }
    int y = smallestTrumpCard;
    smallestTrumpCard = 15;

    for (int i = 0; i < 6; i++) {
        if (getCardSuit(opponentDeck[i]) == trumpCard && getCardRank(opponentDeck[i]) < smallestTrumpCard) {
            smallestTrumpCard = getCardRank(opponentDeck[i]);
        }
    }
    int z = smallestTrumpCard;
    if (z < y) { //can't be equal since it's a single deck
        currentTurntoAttack = 2;
        opponentAttackingTurn();
    }
    else {
        //If there was no trump card, both y & z == 15, so human goes first
        currentTurntoAttack = 1;
    }
}

void MainWindow::opponentAttackingTurn() {
    this->ui->readInput->setEnabled(false);
    currentTurntoAttack = 2;
    int randomCard = rand() % opponentDeck.size();
    tableCards.push_back(opponentDeck[randomCard]);
    //qDebug() << randomCard;
    //qDebug() << QString::fromStdString(opponentDeck[randomCard]);
    opponentDeck.erase(opponentDeck.begin() + randomCard, opponentDeck.begin() + randomCard + 1);
    updateAll();
    this->ui->readInput->setEnabled(true);
}

void MainWindow::opponentDefendingTurn() {
    this->ui->readInput->setEnabled(false);
    for (unsigned i = 0; i < tableCards.size(); i ++) {
        int defendingCardNumber = -145;
        char cardSuit = getCardSuit(tableCards[0]);
        int cardRank = getCardRank(tableCards[0]);
        if (cardSuit == trumpCard) {
            for (unsigned j = 0; j < opponentDeck.size(); j++) {
                if (cardRank < getCardRank(opponentDeck[j])) {
                    defendingCardNumber = j;
                    break;
                }
            }
        }
        else {
            for (unsigned j = 0; j < opponentDeck.size(); j++) {
                if (cardSuit == getCardSuit(opponentDeck[j])) {
                    if (cardRank < getCardRank(opponentDeck[j])) {
                        defendingCardNumber = j;
                        break;
                    }
                }
            }
        }
        if (defendingCardNumber != -145){
            usedCards.push_back(tableCards[0]);
            usedCards.push_back(opponentDeck[0]);
            tableCards.erase(tableCards.begin(), tableCards.begin() + 1);
            opponentDeck.erase(opponentDeck.begin(), opponentDeck.begin() + 1);
        }
        //end of official for loop
    }
    updateAll();
    this->ui->readInput->setEnabled(true);
}

/*void MainWindow::playerAttackingTurn(int cardNumber) {
    currentTurntoAttack = 1;
    bool foundRankConnection = false;
    // Saves whether the card of the same rank you try to put
    // exists on the tableif (tableCards.empty())
    for (unsigned i = 0; i < tableCards.size(); i++) {
        if (getCardRank(myDeck[cardNumber]) == getCardRank(tableCards[i])){
            foundRankConnection = true;
        }
    }
    if (foundRankConnection) {
        tableCards.push_back(myDeck[cardNumber]);
        myDeck.erase(myDeck.begin() + cardNumber, myDeck.begin() + cardNumber + 1);
        updateAll();
    }
    else {
        impossibleFunctionAttempt.exec();
         updateAll();
    }
}*/

void MainWindow::playerAttackingTurn(int cardNumber) {
    currentTurntoAttack = 1;
    while
}

void MainWindow::playerDefendingTurn(int cardNumber) {
    int defendingCardNumber = cardNumber;
    QString defendingCard = this->ui->cardInput->text();

    char playerCardSuit = getCardSuit(defendingCard.toStdString());
    int playerCardRank = getCardRank(defendingCard.toStdString());

    char tableCardSuit = getCardSuit(tableCards[0]);
    int tableCardRank = getCardRank(tableCards[0]);

    if ((playerCardSuit == trumpCard && tableCardSuit != trumpCard)
        ||(playerCardSuit == tableCardSuit && playerCardRank > tableCardRank)) {
        usedCards.push_back(tableCards[0]);
        usedCards.push_back(myDeck[defendingCardNumber]);
        tableCards.erase(tableCards.begin(), tableCards.begin() + 1);
        myDeck.erase(myDeck.begin(), myDeck.begin() + 1);
        updateAll();
    }
    else {
       impossibleDefenseAttempt.exec();
       updateAll();
    }
}


void MainWindow::takeCards() {
    int turn = getCurrentTurn();
    if (!tableCards.empty() && turn == 2) {
        while (!usedCards.empty()) {
            myDeck.push_back(usedCards.back());
            usedCards.pop_back();
        }
        while (!tableCards.empty()) {
            myDeck.push_back(tableCards.back());
            usedCards.pop_back();
        }
    } else {
        impossibleFunctionAttempt.exec();
    }
}
void MainWindow::updateAll() {
    this->ui->DECK->setText("LEFT IN DECK: " +
                            QString::fromStdString(std::to_string(shuffledDeck.size())) +
                            "\n\n\n\nTRUMP SUIT: " +
                            QString::fromStdString(std::string(1, trumpCard)));
    showCards();
}

void MainWindow::showCards() {
    std::string playerDeckLabel;
    std::string tableCardsLabel;
    for (unsigned i = 0; i < myDeck.size(); i++) {
        playerDeckLabel = playerDeckLabel + " " + myDeck[i];
    }
    for (unsigned i = 0; i < tableCards.size(); i++) {
        tableCardsLabel = tableCardsLabel + " " + tableCards[i];
    }
    this->ui->YOU->setText(QString::fromStdString(playerDeckLabel));
    this->ui->TABLE->setText("TABLE: " + QString::fromStdString(tableCardsLabel));
    this->ui->OPPONENT->setText(QString::fromStdString(std::string(opponentDeck.size(), 'X')));
}

char MainWindow::getCardSuit(QString card) {
    return card[card.size()-1];
}

int MainWindow::getCardRank(QString card) {
    char c = card[0];
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
            return -145;
    }
}

int MainWindow::getCurrentTurn() {
    if (currentTurntoAttack % 2 == 0) {
        return 1; //player 1 -> human
    }
    else {
        return 2; //player 2 -> computer
    }
}

void MainWindow::passTurn() {
    int turn = getCurrentTurn();
    if (turn == 1) {
        pass = 2;
    }
    else if (turn == 2) {
        pass = 1;
    }
}

void MainWindow::redirectInput() {

    int currentTurn = getCurrentTurn();

    int cardNumber = -145;
    QString card = this->ui->cardInput->text();

    bool existence = false;

    for (unsigned i = 0; i < myDeck.size(); i++) {
        if (myDeck[i] == card.toStdString()) {
            existence = true;
            cardNumber = i;
            break;
        }
    }

    if(existence) {
        if (currentTurn == 1) {
            playerAttackingTurn(cardNumber);
        }
        else {
            playerDefendingTurn(cardNumber);
        }

    }
    else {
        incorrectInput.exec();
        updateAll();
    }
}
