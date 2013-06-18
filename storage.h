#ifndef STORAGE_H
#define STORAGE_H

#include <QWidget>
#include <QStackedLayout>

#include "filesystem.h"
#include "mplayer.h"

class Storage : public QWidget
{
    Q_OBJECT

public:
    explicit Storage(QWidget *parent = 0);
    ~Storage();

    void update();
    void back();

public:
    FileSystem *system;
    Mplayer *mplayer;

    QStackedLayout *stackLayout;
};

#endif // STORAGE_H
