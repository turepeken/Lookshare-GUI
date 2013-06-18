#ifndef MPLAYER_H
#define MPLAYER_H

#include <QWidget>
#include <QPushButton>
#include <QSpacerItem>
#include <QProgressBar>
#include <QSlider>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QProcess>
#include <QTimeLine>
#include <QMouseEvent>
class Storage;

class Mplayer : public QWidget
{
    Q_OBJECT

public:
    explicit Mplayer(QWidget *parent = 0);
    ~Mplayer();

    void loadVideo(QString path);
    void deleteVideo();

signals:
    void videoReady();

public slots:
    void closeClicked();
    void previousClicked();
    void nextClicked();
    void slowRwdClicked();
    void slowFwdClicked();
    void fastRwdClicked();
    void playClicked();
    void volumeChanged(int v);
    void fastFwdClicked();
    void getVideoLength();
    void startTimeLine();
    void setProgress(int i);

private:
    QPushButton *closeButton;
    QSpacerItem *spacerItem1;
    QPushButton *previousButton;
    QSpacerItem *spacerItem2;
    QPushButton *nextButton;
    QWidget *videoWidget;
    QPushButton *slowRwdButton;
    QProgressBar *progressBar;
    QPushButton *slowFwdButton;
    QPushButton *fastRwdButton;
    QPushButton *playButton;
    QSlider *volumeSlider;
    QPushButton *fastFwdButton;

    QHBoxLayout *top1Layout;
    QHBoxLayout *top2Layout;
    QHBoxLayout *videoLayout;
    QHBoxLayout *bottom1Layout;
    QHBoxLayout *bottom2Layout;
    QVBoxLayout *mainLayout;

    QString currentPath;
    QProcess *process;
    QTimeLine *timeLine;
    int videoLength;

    void initTop1();
    void initTop2();
    void initVideo();
    void initButtom1();
    void initButtom2();

    void mouseMoveEvent(QMouseEvent *event);
    void showTop();
    void hideTop();
    void showButtom();
    void hideButtom();

    void writeCmd(const QString s);
    void setTimeLine(int i);
    QString removeBackRef(QString s);


public:
    Storage *storage;

};

#endif // MPLAYER_H
