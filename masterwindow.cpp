#include "masterwindow.h"


MasterWindow::MasterWindow(QWidget *parent)
    : QWidget(parent)
{
    resize(400, 300);
    setWindowTitle("Lookshare");

    start = new QWidget;
    home = new HomeScreen;
    storage = new Storage;
    internet= new Internet;
    home->masterWindow = this;

    stackLayout = new QStackedLayout;
    stackLayout->addWidget(start); // 0
    stackLayout->addWidget(home); // 1

    stackLayout->addWidget(storage); //2
    stackLayout->addWidget(internet);  //3

    frame = new QFrame;
    frame->setFrameShape(QFrame::HLine);
    frame->setFrameShadow(QFrame::Raised);
    homeButton = new QPushButton;
    homeButton->setText("Home");

    bottomLayout = new QVBoxLayout;
    bottomLayout->addWidget(frame);
    bottomLayout->addWidget(homeButton, 0, Qt::AlignHCenter);

    mainLayout = new QVBoxLayout;
    mainLayout->addLayout(stackLayout);
    mainLayout->addLayout(bottomLayout);
    setLayout(mainLayout);

    stackLayout->setCurrentIndex(0);

    connect(homeButton, SIGNAL(clicked()),
            this, SLOT(homeClicked()));
}

MasterWindow::~MasterWindow()
{
}

void MasterWindow::homeClicked()
{

    if (stackLayout->currentIndex() == 2)
      {
          if (storage->stackLayout->currentIndex() == 1)
          {
              storage->mplayer->deleteVideo();
          }

      }


    stackLayout->setCurrentIndex(1);
}
