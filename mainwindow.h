#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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
#include <QKeySequence>
#include <QToolTip>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    //Starter functions
    void giveoutCards();
    void deckGenerator();
    void decideFirstPlayer();

    //Player attacks
    void placeAttackingCard(int cardNum);
    void playerPass();
    void playerFinishTurn();

    //Player defends
    void placeDefendingCard(int cardNum);
    void takeCards();

    //Computer turns
    void computerAttacks();
    void computerDefends();
    bool doesItTake(QString attackingCard, QString defendingCard);

    //Mid-turn
    void fillHands();

    //System commands
    void readCardFilter();
    void update();
    QChar getCardSuit(QString card);
    int getCardRank(QString card);
    QString getCurrentState();
    int getCardValue(QString card);
    bool firstCardBeatsSecond(QString card1, QString card2);\
    void showCredits();
    void toggleDevMode();

    // Interface changes
    //QString setStatusText(QString func);

private:
    Ui::MainWindow *ui;

    //For card generation purposes
    QString order[9] = {"6", "7", "8", "9", "10", "J", "Q", "K", "A"};
    QString starterDeck[36] = {"6S", "7S", "8S", "9S", "10S", "JS", "QS", "KS", "AS",
                                   "6C", "7C", "8C", "9C", "10C", "JC", "QC", "KC", "AC",
                                   "6H", "7H", "8H", "9H", "10H", "JH", "QH", "KH", "AH",
                                   "6D", "7D", "8D", "9D", "10D", "JD", "QD", "KD", "AD"};
    std::vector <QString> shuffledDeck;
    QString trumpSuit;

    std::vector <QString> playerDeck;
    std::vector <QString> opponentDeck;
    std::vector <QString> tableCards;
    std::vector <QString> tempBeatenCards;

    QString state; //values: "PA", "PD", "CA", "CD"
    bool devMode = false;
    bool gameover = false;

    //Error messages
    QMessageBox incorrectTimeError; //Not the player's turn
    QMessageBox impossibleAttackAttempt;
    QMessageBox impossibleDefenseAttempt;
    QMessageBox incorrectInput;
    QMessageBox impossibleTakeAttempt;
    QMessageBox impossiblePassAttempt;
    QMessageBox impossibleFinishTurnAttempt;
    QMessageBox playerWins;
    QMessageBox computerWins;
    QMessageBox credits;
    QMessageBox cannotAddMoreCards;

    QShortcut* devModeShortcut;
    //QToolTip toast;
};

#endif // MAINWINDOW_H
