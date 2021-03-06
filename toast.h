#ifndef TOAST_H
#define TOAST_H

#include <QWidget>
#include <QLabel>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <QMessageBox>
#include <QLabel>
#include <QDebug>
#include <QString>
#include <QShortcut>
#include <QThread>
#include <thread>
#include <QKeySequence>
#include <QToolTip>
#include <unistd.h>
#include <stdio.h>

class ToastThread : public QThread
{
    Q_OBJECT
public:
    explicit ToastThread(int sec = 3);
    void run();
private:
    int seconds;
};

class Toast : public QWidget
{
    Q_OBJECT
public:
    explicit Toast(QWidget *parent = nullptr, QString init_message = "Hello, world!", int init_duration = 3,
                   int x = 0, int y = 0, int width = 200, int height = 150);

signals:

public slots:
    void closeAll();

private:
    QLabel* message;
    int duration; //in seconds
    ToastThread* toastThread;
};

#endif // TOAST_H
