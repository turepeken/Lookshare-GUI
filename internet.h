#ifndef INTERNET_H
#define INTERNET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QDir>
#include <QTableView>
#include <QStandardItemModel>
#include <QHBoxLayout>
#include <QVBoxLayout>


class Internet : public QWidget
{
    Q_OBJECT
public:
    explicit Internet(QWidget *parent = 0);
    ~Internet();


private:

    QHBoxLayout *Layout;
    QVBoxLayout *mainLayout;


    void initLayout();

};


#endif // INTERNET_H
