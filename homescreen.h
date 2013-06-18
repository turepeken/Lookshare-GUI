#ifndef HOMESCREEN_H
#define HOMESCREEN_H

#include <QWidget>
#include <QPushButton>
#include <QGridLayout>

class MasterWindow;

class HomeScreen : public QWidget
{
    Q_OBJECT

public:
    explicit HomeScreen(QWidget *parent = 0);
    ~HomeScreen();

public slots:
    void storageClicked();

private:
    QPushButton *linkButton;
    QPushButton *storageButton;
    QGridLayout *gridLayout;

public:
    MasterWindow *masterWindow;
};

#endif // HOMESCREEN_H
