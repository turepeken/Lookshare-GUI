#ifndef MASTERWINDOW_H
#define MASTERWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QFrame>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QStackedLayout>

#include "homescreen.h"
#include "storage.h"
#include "internet.h"

class MasterWindow : public QWidget
{
    Q_OBJECT

public:
    MasterWindow(QWidget *parent = 0);
    ~MasterWindow();

public slots:
    void homeClicked();

private:
    HomeScreen *home;

    QFrame *frame;
    QPushButton *homeButton;

    QVBoxLayout *bottomLayout;
    QVBoxLayout *mainLayout;

public:
    QWidget *start;
    Storage *storage;
    Internet *internet;
    QStackedLayout *stackLayout;
};

#endif // MASTERWINDOW_H
