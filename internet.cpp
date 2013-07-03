
#include <QMessageBox>
#include <QTextStream>
#include <QHeaderView>
#include <QStandardItem>
#include <QFileIconProvider>
#include <QIcon>
#include <QFileInfo>
#include <QDateTime>
#include <QtAlgorithms>

#include "stdlib.h"
#include "internet.h"

Internet::Internet(QWidget *parent) :
    QWidget(parent)
{
    initLayout();

    mainLayout = new QVBoxLayout;
    mainLayout->addLayout(Layout);
    setLayout(mainLayout);

}

Internet::~Internet()
{

}

void Internet::initLayout()
{

    Layout = new QHBoxLayout;
}

