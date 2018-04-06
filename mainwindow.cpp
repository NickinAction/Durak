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
    deckGenerator();
    giveoutCards();
    connect(this->ui->readInput, SIGNAL(clicked()), this, SLOT(playerTurn()));
    impossibleFunctionAttempt.setText("You cannot do that.");
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
        currentTurn = 2;
        opponentTurn();
    }
    else {
        //If there was no trump card, both y & z == 15, so human goes first
        currentTurn = 1;
    }
}

void MainWindow::opponentTurn() {
    updateAll();
}

void MainWindow::playerTurn() {
    int iRemember;
    //qDebug("Hello, it's your turn now! =)");
    QString card = this->ui->cardInput->text();
    bool existence = false;
    for (unsigned i = 0; i < myDeck.size(); i++) {
        if (myDeck[i] == card.toStdString()) {
            existence = true;
            iRemember = i;
            break;
        }
    }
    bool foundRankConnection = false;
    if(!existence) {
        QMessageBox incorrectInput;
        incorrectInput.setText("Your input was inappropriate, please try again.");
        incorrectInput.exec();
        updateAll();
    }
    else {

        if (tableCards.empty()) {
            tableCards.push_back(myDeck[iRemember]);
            myDeck.erase(myDeck.begin() + iRemember, myDeck.begin() + iRemember + 1);
            updateAll();
        }
        else {
            for (unsigned i = 0; i < tableCards.size(); i++) {
                if (getCardRank(myDeck[iRemember]) == getCardRank(tableCards[i])){
                    foundRankConnection = true;
                }
            }
            if (foundRankConnection) {
                tableCards.push_back(myDeck[iRemember]);
                myDeck.erase(myDeck.begin() + iRemember, myDeck.begin() + iRemember + 1);
                updateAll();
            }
            else {
                impossibleFunctionAttempt.exec();
                updateAll();
            }
        }
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

char MainWindow::getCardSuit(std::string a) {
    return a[a.size()-1];
}

int MainWindow::getCardRank(std::string card) {
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
