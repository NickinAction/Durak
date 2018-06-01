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
#include <stdio.h>
#include <QHBoxLayout>

Toast::Toast(QWidget *parent, QString init_message, int init_duration, int x, int y, int width, int height) : QWidget(parent)
{
    message = new QLabel();
    message->setText(init_message);
    duration = init_duration;
    message->setWordWrap(true);
    message->setTextFormat(Qt::TextFormat(Qt::RichText));
    message->setStyleSheet("QLabel { background-color : white ; color: green ; font : 16pt; border : 2px solid green; }");
    message->setAlignment(Qt::AlignCenter);
    this->setGeometry(x, y, width, height);
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
