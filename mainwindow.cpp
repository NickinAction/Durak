#include <QString>
#include <QMessageBox>
#include <vector>
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::deckGenerator() {

}

void MainWindow::decideFirstPlayer() {

}

void MainWindow::giveoutCards() {

}

void MainWindow::placeAttackingCard() {

}

void MainWindow::placeDefendingCard() {

}

void MainWindow::playerFinishTurn() {

}

void MainWindow::playerPass() {

}

void MainWindow::takeCards() {

}

void MainWindow::computerAttacks() {

}

void MainWindow::computerDefends() {
}

void MainWindow::readCardFilter() {

}

QChar MainWindow::getCardSuit(QString card) {
    return QChar(card[card.size()-1]);
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

QString MainWindow::getCurrentState() {

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
