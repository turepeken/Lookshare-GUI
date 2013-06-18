#ifndef FileSystem_H
#define FileSystem_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QDir>
#include <QTableView>
#include <QStandardItemModel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <mythdatabase.h>

class Storage;
class Mythdatabase;

class FileSystem : public QWidget
{
    Q_OBJECT
public:
    explicit FileSystem(QWidget *parent = 0);
    ~FileSystem();

    void update();
    void back();
    void showDataDir();

public slots:
    void cloudClicked();
    void upClicked();
    void searchClicked();
    void lineEdited(const QString &s);
    void clearClicked();
    void itemClicked(QModelIndex i);
    void indicatorClicked(int i, Qt::SortOrder order);

private:

    QLabel *dirLabel;
    QLabel *usageLabel;
    QPushButton *cloudButton;
    QPushButton *upButton;
    QPushButton *searchButton;
    QLineEdit *searchLine;
    QPushButton *clearButton;

    QDir *currentDir;
    QTableView *systemView;
    QStandardItemModel *systemModel;

    QHBoxLayout *top1Layout;
    QHBoxLayout *top2Layout;
    QHBoxLayout *systemLayout;
    QVBoxLayout *mainLayout;

    bool isSystemView;
    QList<QString> *searchFiles;

    void initTop1();
    void initTop2();
    void initSystem();

    void updateCurrentDir();
    void updateTop1();
    void updateTop2();
    void updateSystem();

    void clearSystem();
    void clearSearchFiles();
    void showCurrentDir();
    void showSearchFiles();
    void openFile(const QFileInfo &fileInfo);

    QString addBackRef(QString s);
    void reverseFiles();
    static bool byType(QString s1, QString s2);
    static bool byName(QString s1, QString s2);
    static bool byTime(QString s1, QString s2);

public:
    Storage *storage;
    Mythdatabase *mythDatabase;
};

#endif // FileSystem_H
