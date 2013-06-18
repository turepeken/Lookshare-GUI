#include <QFileInfo>
#include <QDir>
#include <QTimeLine>
#include <QDateTime>

#include "mplayer.h"
#include "storage.h"


Mplayer::Mplayer(QWidget *parent) :
    QWidget(parent)
{
    initTop1();
    initTop2();
    initVideo();
    initButtom1();
    initButtom2();

    mainLayout = new QVBoxLayout;
    mainLayout->addLayout(top1Layout);
    mainLayout->addLayout(top2Layout);
    mainLayout->addLayout(videoLayout);
    mainLayout->addLayout(bottom1Layout);
    mainLayout->addLayout(bottom2Layout);
    setLayout(mainLayout);

    hideTop();
    hideButtom();

    setMouseTracking(true);
}

Mplayer::~Mplayer()
{
    deleteVideo();
}

void Mplayer::initTop1()
{
    closeButton = new QPushButton;
    closeButton->setText("X");
    closeButton->setMaximumWidth(30);

    spacerItem1 = new QSpacerItem(30, 30,
            QSizePolicy::Expanding, QSizePolicy::Minimum);

    top1Layout = new QHBoxLayout;
    top1Layout->addWidget(closeButton);
    top1Layout->addItem(spacerItem1);

    connect(closeButton, SIGNAL(clicked()),
            this, SLOT(closeClicked()));
}

void Mplayer::initTop2()
{
    previousButton = new QPushButton;
    previousButton->setText("Previous");
    spacerItem2 = new QSpacerItem(40, 10,
            QSizePolicy::Expanding, QSizePolicy::Minimum);
    nextButton = new QPushButton;
    nextButton->setText("Next");

    top2Layout = new QHBoxLayout;
    top2Layout->addWidget(previousButton);
    top2Layout->addItem(spacerItem2);
    top2Layout->addWidget(nextButton);

    connect(previousButton, SIGNAL(clicked()),
            this, SLOT(previousClicked()));
    connect(nextButton, SIGNAL(clicked()),
            this, SLOT(nextClicked()));
}

void Mplayer::initVideo()
{
    videoWidget = new QWidget;
    videoLayout = new QHBoxLayout;
    videoLayout->addWidget(videoWidget);

    process = NULL;
}

void Mplayer::initButtom1()
{
    slowRwdButton = new QPushButton;
    slowRwdButton->setText("slowRwd");
    progressBar = new QProgressBar;

    slowFwdButton = new QPushButton;
    slowFwdButton->setText("slowFwd");

    bottom1Layout = new QHBoxLayout;
    bottom1Layout->addWidget(slowRwdButton);
    bottom1Layout->addWidget(progressBar);
    bottom1Layout->addWidget(slowFwdButton);

    connect(slowRwdButton, SIGNAL(clicked()),
            this, SLOT(slowRwdClicked()));
    connect(this, SIGNAL(videoReady()),
            this, SLOT(startTimeLine()));
    connect(slowFwdButton, SIGNAL(clicked()),
            this, SLOT(slowFwdClicked()));

    timeLine = NULL;
}

void Mplayer::initButtom2()
{
    fastRwdButton = new QPushButton;
    fastRwdButton->setText("fastRwd");
    playButton = new QPushButton;
    playButton->setText("pause");
    volumeSlider = new QSlider;
    volumeSlider->setOrientation(Qt::Horizontal);
    volumeSlider->setRange(0, 100);
    volumeSlider->setValue(20);
    fastFwdButton = new QPushButton;
    fastFwdButton->setText("fastFwd");

    bottom2Layout = new QHBoxLayout;
    bottom2Layout->addWidget(fastRwdButton);
    bottom2Layout->addWidget(playButton);
    bottom2Layout->addWidget(volumeSlider);
    bottom2Layout->addWidget(fastFwdButton);

    connect(fastRwdButton, SIGNAL(clicked()),
            this, SLOT(fastRwdClicked()));
    connect(playButton, SIGNAL(clicked()),
            this, SLOT(playClicked()));
    connect(volumeSlider, SIGNAL(valueChanged(int)),
            this, SLOT(volumeChanged(int)));
    connect(volumeSlider, SIGNAL(sliderMoved(int)),
            this, SLOT(volumeChanged(int)));
    connect(fastFwdButton, SIGNAL(clicked()),
            this, SLOT(fastFwdClicked()));
}

void Mplayer::loadVideo(QString path)
{
    progressBar->setFormat("00:00/00:00");

    process = new QProcess;
    connect(process, SIGNAL(readyReadStandardOutput()),
            this, SLOT(getVideoLength()));

    QDir dir(".");
    currentPath = removeBackRef(path);
    QStringList args;
    args << "-slave";
    args << "-quiet";
    args << "-wid";
    args << QString::number(videoWidget->winId());
    args << dir.relativeFilePath(currentPath);

    process->start("mplayer", args);
    process->waitForStarted();

    writeCmd(QString("pausing_keep_force get_property length\n"));
}

void Mplayer::deleteVideo()
{
    if (!process)
    {
        return;
    }

    if (process->state() != QProcess::NotRunning)
    {
        writeCmd("quit\n");
        process->terminate();
        process->kill();
        process->waitForFinished();
    }

    delete process;
    process = NULL;

    if (!timeLine)
    {
        return;
    }

    delete timeLine;
    timeLine = NULL;
}

void Mplayer::closeClicked()
{
    deleteVideo();
    storage->back();
}

void Mplayer::previousClicked()
{
    QFileInfo info(currentPath);
    QDir dir = info.dir();
    dir.setFilter(QDir::Files);

    QList<QString> filters;
    filters << "*.3gp" << "*.avi" << "*.dat" << "*.flv"
            << "*.mkv" << "*.mov" << "*.mp4" << "*.mpeg" << "*.wmv";
    dir.setNameFilters(filters);

    QList<QFileInfo> files = dir.entryInfoList();
    int index = files.indexOf(info);
    index = index == 0 ? files.count() - 1 : index - 1;
    currentPath = files.at(index).filePath();

    deleteVideo();
    loadVideo(currentPath);
}

void Mplayer::nextClicked()
{
    QFileInfo info(currentPath);
    QDir dir = info.dir();
    dir.setFilter(QDir::Files);

    QList<QString> filters;
    filters << "*.3gp" << "*.avi" << "*.dat" << "*.flv"
            << "*.mkv" << "*.mov" << "*.mp4" << "*.mpeg" << "*.wmv";
    dir.setNameFilters(filters);

    QList<QFileInfo> files = dir.entryInfoList();
    int index = files.indexOf(info);
    index = index == files.count() - 1 ? 0 : index + 1;
    currentPath = files.at(index).filePath();

    deleteVideo();
    loadVideo(currentPath);
}

void Mplayer::slowRwdClicked()
{
    writeCmd("pausing_keep_force seek -10\n");
    setTimeLine(-10 * 1000);
}

void Mplayer::slowFwdClicked()
{
    writeCmd("pausing_keep_force seek +10\n");
    setTimeLine(10 * 1000);
}

void Mplayer::fastRwdClicked()
{
    writeCmd("pausing_keep_force seek -60\n");
    setTimeLine(-60 * 1000);
}

void Mplayer::playClicked()
{
    if (timeLine->state() == QTimeLine::NotRunning)
    {
        return;
    }

    if (playButton->text().compare("pause") == 0)
    {
        playButton->setText("play");
        timeLine->setPaused(true);
    }
    else
    {
        playButton->setText("pause");
        timeLine->resume();
    }

    writeCmd("pause\n");
}

void Mplayer::volumeChanged(int v)
{
    writeCmd(QString("pausing_keep_force volume %1 1\n").arg(v));
}

void Mplayer::fastFwdClicked()
{
    writeCmd("pausing_keep_force seek +60\n");
    setTimeLine(60 * 1000);
}

void Mplayer::getVideoLength()
{
    QByteArray output = process->readAllStandardOutput();
    foreach (QByteArray line, output.split('\n'))
    {
        if (line.startsWith("ANS_length"))
        {
            videoLength = line.split('=').back().toDouble();
            emit videoReady();
            break;
        }
    }
}

void Mplayer::startTimeLine()
{
    timeLine = new QTimeLine(1000 * videoLength, this);
    timeLine->setCurveShape(QTimeLine::LinearCurve);
    timeLine->setFrameRange(0, videoLength);
    progressBar->setRange(0, videoLength);

    connect(timeLine, SIGNAL(frameChanged(int)),
            this, SLOT(setProgress(int)));

    timeLine->start();
}

void Mplayer::setProgress(int i)
{
    QString t = QDateTime::fromTime_t(videoLength).toString("mm:ss");
    QString c = QDateTime::fromTime_t(i).toString("mm:ss");

    progressBar->setFormat(QString("%1/%2").arg(c).arg(t));
    progressBar->setValue(i);
}

void Mplayer::writeCmd(const QString s)
{
    process->write(s.toLocal8Bit());
    process->waitForBytesWritten();
}

void Mplayer::setTimeLine(int i)
{
    if (timeLine->state() == QTimeLine::NotRunning)
    {
        return;
    }

    int currentTime = timeLine->currentTime();
    timeLine->setCurrentTime(currentTime + i);

    if (timeLine->state() == QTimeLine::Running)
    {
        timeLine->setPaused(true);
        timeLine->resume();
    }
}

QString Mplayer::removeBackRef(QString s)
{
    s.replace("\\ ", " ");
    return s;
}

void Mplayer::mouseMoveEvent(QMouseEvent *event)
{
    if (event->pos().y() <= 40)
    {
        showTop();
    }
    else if (event->pos().y() >= 60)
    {
        hideTop();
    }

    if (event->pos().y() >= 230)
    {
        showButtom();
    }
    else if (event->pos().y() <= 190)
    {
        hideButtom();
    }
}

void Mplayer::showTop()
{
    previousButton->setVisible(true);
    nextButton->setVisible(true);
}

void Mplayer::hideTop()
{
    previousButton->setVisible(false);
    nextButton->setVisible(false);
}

void Mplayer::showButtom()
{
    slowRwdButton->setVisible(true);
    progressBar->setVisible(true);
    slowFwdButton->setVisible(true);
    fastRwdButton->setVisible(true);
    playButton->setVisible(true);
    volumeSlider->setVisible(true);
    fastFwdButton->setVisible(true);
}

void Mplayer::hideButtom()
{
    slowRwdButton->setVisible(false);
    progressBar->setVisible(false);
    slowFwdButton->setVisible(false);
    fastRwdButton->setVisible(false);
    playButton->setVisible(false);
    volumeSlider->setVisible(false);
    fastFwdButton->setVisible(false);
}
