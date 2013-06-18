#include "storage.h"

Storage::Storage(QWidget *parent) :
    QWidget(parent)
{
    system = new FileSystem;
    mplayer = new Mplayer;
    system->storage = this;
    mplayer->storage = this;

    stackLayout = new QStackedLayout;
    stackLayout->addWidget(system);
    stackLayout->addWidget(mplayer);
    setLayout(stackLayout);

    stackLayout->setCurrentIndex(0);
}

Storage::~Storage()
{
}

void Storage::update()
{
    //system->update();
    stackLayout->setCurrentIndex(0);
}

void Storage::back()
{
    system->back();
    stackLayout->setCurrentIndex(0);
}

