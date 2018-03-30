#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <deque>
#include <random>
#include <ctime>
#include <cstdlib>
#include <vector>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    deckGenerator();
    giveoutCards();
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
    trumpCard = cardSuit(shuffledDeck.front());
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
    setCards();
    this->ui->DECK->setText("DECK\n\n\n\nTRUMP CARD: " + QString::fromStdString(std::string(1, trumpCard)));
}

void MainWindow::decideFirstPlayer() {
    int smallestTrumpCard = 15;
    for (int i = 0; i < 6; i++) {
        if (cardSuit(myDeck[i]) == trumpCard && cardRank(myDeck[i]) < smallestTrumpCard) {
            smallestTrumpCard = cardRank(myDeck[i]);
        }
    }
    int y = smallestTrumpCard;
    smallestTrumpCard = 15;

    for (int i = 0; i < 6; i++) {
        if (cardSuit(opponentDeck[i]) == trumpCard && cardRank(opponentDeck[i]) < smallestTrumpCard) {
            smallestTrumpCard = cardRank(opponentDeck[i]);
        }
    }
    int z = smallestTrumpCard;
    if (z < y) { //can't be equal since it's a single deck
        firstPlayer = 2;
    }
    else {
        //If there was no trump card, both y & z == 15, so human goes first
        firstPlayer = 1;
    }
}

void MainWindow::setCards() {
    std::string deckLabel;
    for (unsigned i = 0; i < myDeck.size(); i++) {
        deckLabel = deckLabel + " " + myDeck[i];
    }
    this->ui->YOU->setText(QString::fromStdString(deckLabel));
    this->ui->OPPONENT->setText(QString::fromStdString(std::string(opponentDeck.size(), 'X')));
}

char MainWindow::cardSuit(std::string a) {
    return a[a.size()-1];
}

int MainWindow::cardRank(std::string card) {
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
