#include "toast.h"
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
#include <a.out.h>
#include <stdio.h>
#include <QHBoxLayout>

Toast::Toast(QWidget *parent, QString init_message, int init_duration, int x1, int y1, int x2, int y2) : QWidget(parent)
{
    message = new QLabel();
    message->setText(init_message);
    duration = init_duration;
    message->setWordWrap(true);
    message->setTextFormat(Qt::TextFormat(Qt::RichText));
    message->setStyleSheet("QLabel { background-color : white ; color: green ; font : 24pt; border : 2px solid green; }");
    this->setGeometry(x1, y1, x2, y2);
    QHBoxLayout* layout = new QHBoxLayout();
    layout->addWidget(message);
    this->setLayout(layout);
    this->setWindowFlags(Qt::FramelessWindowHint);
    toastThread = new ToastThread(duration);
    connect(this->toastThread, SIGNAL(finished()), this, SLOT(closeAll()));
    toastThread->start();
}

void Toast::closeAll() {
    toastThread->terminate();
    this->close();
}

ToastThread::ToastThread(int sec)
{
    seconds = sec;
}

void ToastThread::run() {
    this->sleep(seconds);
}
