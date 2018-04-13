#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <deque>
#include <string>
#include <random>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <QMessageBox>

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

    //Turns

    void playerAttackingTurn();
    void opponentAttackingTurn();

    void playerDefendingTurn();
    void opponentDefendingTurn();

    //System functions

    void readInput();
    void updateAll();
    void showCards();
    int getCardRank(std::string card);
    char getCardSuit (std::string a);
    int getCurrentTurn();

private:
    int currentTurntoAttack;
    Ui::MainWindow *ui;
    char trumpCard;
    std::string order[9] = {"6", "7", "8", "9", "10", "J", "Q", "K", "A"};
    std::string starterDeck[36] = {"6S", "7S", "8S", "9S", "10S", "JS", "QS", "KS", "AS",
                                   "6C", "7C", "8C", "9C", "10C", "JC", "QC", "KC", "AC",
                                   "6H", "7H", "8H", "9H", "10H", "JH", "QH", "KH", "AH",
                                   "6D", "7D", "8D", "9D", "10D", "JD", "QD", "KD", "AD"};
    std::vector <std::string> shuffledDeck;
    std::vector <std::string> myDeck;
    std::vector <std::string> opponentDeck;
    std::vector <std::string> tableCards;
    std::vector <std::string> usedCards;

    QMessageBox impossibleFunctionAttempt;
    QMessageBox impossibleDefenseAttempt;
};

#endif
