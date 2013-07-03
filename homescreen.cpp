#include "homescreen.h"
#include "masterwindow.h"


HomeScreen::HomeScreen(QWidget *parent)
    : QWidget(parent)
{
    linkButton = new QPushButton;
    linkButton->setText("Firefox");

    storageButton = new QPushButton;
    storageButton->setText("Storage");

    gridLayout = new QGridLayout;
    gridLayout->addWidget(linkButton, 0, 0, Qt::AlignHCenter);

    gridLayout->addWidget(storageButton, 0, 1, Qt::AlignHCenter);
    setLayout(gridLayout);

    connect(storageButton, SIGNAL(clicked()),
                this, SLOT(storageClicked()));
    connect(linkButton, SIGNAL(clicked()),
                this, SLOT(linkClicked()));


}

HomeScreen::~HomeScreen()
{
}

void HomeScreen::storageClicked()
{
    masterWindow->storage->update();
    masterWindow->stackLayout->setCurrentIndex(2);
}

void HomeScreen::linkClicked()
{
    masterWindow->internet->update();
    masterWindow->stackLayout->setCurrentIndex(3);
}
