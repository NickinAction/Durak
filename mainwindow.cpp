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

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    deckGenerator();
    giveoutCards();
    decideFirstPlayer();

    connect(this->ui->readInput, SIGNAL(clicked()), this, SLOT(readCardFilter()));
    connect(this->ui->finishTurn, SIGNAL(clicked()), this, SLOT(playerFinishTurn()));
    connect(this->ui->passButton, SIGNAL(clicked()), this, SLOT(playerPass()));
    connect(this->ui->takeButton, SIGNAL(clicked()), this, SLOT(takeCards()));


    impossibleAttackAttempt.setText("You cannot currently place this card.");
    impossibleDefenseAttempt.setText("That card can't beat the opponent's card.");
    incorrectInput.setText("This card does not exist in your deck.");
    incorrectTimeError.setText("Please wait for your turn.");
    impossibleTakeAttempt.setText("You cannot take anything now.");

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
    trumpCard = getCardSuit(shuffledDeck.front());
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
    int smallestTrumpCard = 15;
    for (int i = 0; i < 6; i++) {
        if (getCardSuit(playerDeck[i]) == trumpCard && getCardRank(playerDeck[i]) < smallestTrumpCard) {
            smallestTrumpCard = getCardRank(playerDeck[i]);
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
        state = "CA";
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

}

void MainWindow::playerFinishTurn() { //PA -> CA

}

void MainWindow::placeDefendingCard(int cardNum) { //PD -> CA
    QString defendingCard = playerDeck[cardNum];
    QChar attackingCardSuit = getCardSuit(tableCards[0]);
    int attackingCardRank = getCardRank(tableCards[0]);

    if ((getCardSuit(defendingCard) == trumpCard && attackingCardSuit != trumpCard)
            ||(getCardSuit(defendingCard) == attackingCardSuit &&
               getCardRank(defendingCard) > attackingCardRank)) {
            tempBeatenCards.push_back(tableCards[0]);
            tempBeatenCards.push_back(playerDeck[cardNum]);
            tableCards.erase(tableCards.begin(), tableCards.begin() + 1);
            playerDeck.erase(playerDeck.begin(), playerDeck.begin() + 1);
            update();
        }
        else {
           impossibleDefenseAttempt.exec();
           update();
        }

}

void MainWindow::takeCards() { //PD -> CA

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
            state = "PA";
        }
    }
    else {
        tableCards.push_back(attackingCard);
        opponentDeck.erase(opponentDeck.begin() + attackingCardNum, opponentDeck.begin() + attackingCardNum + 1);
        state = "PD";
    }
    update();
}

void MainWindow::computerDefends() { //CD -> PA
}

void MainWindow::fillHands() { // MID-TURN

}

void MainWindow::readCardFilter() {
    QString card = this->ui->cardInput->text();
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


void MainWindow::update() {
    this->ui->DECK->setText("LEFT IN DECK: " + QString::fromStdString(std::to_string(shuffledDeck.size())) +
                            "\n\n\n\nTRUMP SUIT: " +  trumpCard);
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
