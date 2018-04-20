#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QMessageBox>
#include <vector>

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
    void placeAttackingCard();
    void playerPass();
    void playerFinishTurn();

    //Player defends
    void placeDefendingCard();
    void takeCards();

    //Computer turns
    void computerAttacks();
    void computerDefends();

    //System commands
    void readCardFilter();
    void update();
    QChar getCardSuit(QString card);
    int getCardRank(QString card);
    QString getCurrentState();

private:
    Ui::MainWindow *ui;

    //For card generation purposes
    QString order[9] = {"6", "7", "8", "9", "10", "J", "Q", "K", "A"};
    QString starterDeck[36] = {"6S", "7S", "8S", "9S", "10S", "JS", "QS", "KS", "AS",
                                   "6C", "7C", "8C", "9C", "10C", "JC", "QC", "KC", "AC",
                                   "6H", "7H", "8H", "9H", "10H", "JH", "QH", "KH", "AH",
                                   "6D", "7D", "8D", "9D", "10D", "JD", "QD", "KD", "AD"};
    std::vector <QString> shuffledDeck;
    QString trumpCard;

    std::vector <QString> playerDeck;
    std::vector <QString> opponentDeck;
    std::vector <QString> tableCards;
    std::vector <QString> tempBeatenCards;

    QString state; //values: "PA", "PD", "CA", "CD"

    //Error messages
    QMessageBox impossibleDefenseAttempt;
    QMessageBox incorrectInput;
    QMessageBox impossibleTakeAttempt;
};

#endif // MAINWINDOW_H
