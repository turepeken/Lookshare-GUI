
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
#include "filesystem.h"
#include "storage.h"
#include <mythdatabase.h>

FileSystem::FileSystem(QWidget *parent) :
    QWidget(parent)
{
    initTop1();
    initTop2();
    initSystem();

    mainLayout = new QVBoxLayout;
    mainLayout->addLayout(top1Layout);
    mainLayout->addLayout(top2Layout);
    mainLayout->addLayout(systemLayout);
    setLayout(mainLayout);

    update();
}

FileSystem::~FileSystem()
{
    delete currentDir;
    currentDir = NULL;

    if (!searchFiles)
    {
        return;
    }

    delete searchFiles;
    searchFiles = NULL;
}

void FileSystem::initTop1()
{
    dirLabel = new QLabel;
    usageLabel = new QLabel;
    cloudButton = new QPushButton;
    cloudButton->setText("Pull from Cloud");

    top1Layout = new QHBoxLayout;
    top1Layout->addWidget(dirLabel);
    top1Layout->addWidget(usageLabel);
    top1Layout->addWidget(cloudButton);

    connect(cloudButton, SIGNAL(clicked()),
            this, SLOT(cloudClicked()));
}

void FileSystem::initTop2()
{
    upButton = new QPushButton;
    upButton->setText("Up");
    searchButton = new QPushButton;
    searchButton->setText("Search");
    searchLine = new QLineEdit;
    clearButton = new QPushButton;
    clearButton->setText("Clear");

    top2Layout = new QHBoxLayout;
    top2Layout->addWidget(upButton);
    top2Layout->addWidget(searchButton);
    top2Layout->addWidget(searchLine);
    top2Layout->addWidget(clearButton);

    connect(upButton, SIGNAL(clicked()),
            this, SLOT(upClicked()));
    connect(searchButton, SIGNAL(clicked()),
            this, SLOT(searchClicked()));
    connect(searchLine, SIGNAL(textChanged(const QString &)),
            this, SLOT(lineEdited(const QString &)));
    connect(clearButton, SIGNAL(clicked()),
            this, SLOT(clearClicked()));
}

void FileSystem::initSystem()
{
    searchFiles = new QList<QString>;

    systemModel = new QStandardItemModel;
    systemModel->setColumnCount(3);
    systemModel->setHorizontalHeaderItem(0, new QStandardItem(QString("Type")));
    systemModel->setHorizontalHeaderItem(1, new QStandardItem(QString("Name")));
    systemModel->setHorizontalHeaderItem(2, new QStandardItem(QString("Modified")));

    systemView = new QTableView;
    systemView->setModel(systemModel);
    systemView->verticalHeader()->setVisible(false);

    systemLayout = new QHBoxLayout;
    systemLayout->addWidget(systemView);

    connect(systemView, SIGNAL(clicked(QModelIndex)),
            this, SLOT(itemClicked(QModelIndex)));
    connect(systemView->horizontalHeader(),
            SIGNAL(sortIndicatorChanged(int, Qt::SortOrder)),
            this, SLOT(indicatorClicked(int, Qt::SortOrder)));
}

void FileSystem::updateTop1()
{
    QDir dir(".");
    dirLabel->setText(dir.relativeFilePath(currentDir->path()).append("/"));

    FILE *handler = popen("df -h | egrep '/$' | awk '{print $5}'", "r");
    QTextStream stream(handler, QIODevice::ReadOnly);
    QString text = stream.readLine();
    usageLabel->setText(text.append(" Used"));
    pclose(handler);
}

void FileSystem::updateTop2()
{
    if (dirLabel->text().compare("/home/ture/Videos/") != 0)
    {
        upButton->setEnabled(true);
    }
    else
    {
        upButton->setEnabled(false);
    }

    searchButton->setEnabled(false);
    searchLine->clear();
    clearButton->setEnabled(false);
}

void FileSystem::updateCurrentDir()
{
    QString currentPath = "/home/ture/Videos";
    currentDir = new QDir(".");
    if (!currentDir->exists(currentPath))
    {
        currentDir->mkdir(currentPath);
    }

    currentDir->cd(currentPath);
}

void FileSystem::updateSystem()
{
    systemView->horizontalHeader()->setSortIndicatorShown(true);
    systemView->horizontalHeader()->setSortIndicator(1, Qt::AscendingOrder);
    systemView->horizontalHeader()->setSortIndicator(2, Qt::DescendingOrder);
    systemView->setColumnWidth(0, 55);
    systemView->setColumnWidth(1, 180);
    systemView->setColumnWidth(2, 110);

    clearSearchFiles();
    clearSystem();

    currentDir->setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);
    currentDir->setSorting(QDir::Time);
    showCurrentDir();
    isSystemView = true;
}

void FileSystem::update()
{
    updateCurrentDir();
    back();
}

void FileSystem::back()
{
    updateTop1();
    updateTop2();
    updateSystem();
}

void FileSystem::showDataDir()
{
    currentDir->cd("data");
    back();
}

void FileSystem::cloudClicked()
{
    mythDatabase->connectdatabase();
    //QMessageBox::information(this, "Cloud", "Cloud not enabled");
}

void FileSystem::upClicked()
{
    currentDir->cdUp();
    back();
}

void FileSystem::searchClicked()
{
    clearSearchFiles();

    QString text = searchLine->text();
    dirLabel->setText("Search for '" + text + "'");

    QString cmd;
    if (!text.startsWith("find"))
    {
        cmd = "find ./Local\\ Memory/ ";
        QList<QString> files = text.split(QRegExp("\\s+"));
        for (int i = 0; i < files.size(); i++ )
        {
            if (i == files.size() - 1)
            {
                cmd.append("-name '" + files.at(i) + "'");
            }
            else
            {
                cmd.append("-name '" + files.at(i) + "' -or ");
            }
        }
    }
    else
    {
        cmd = text;
    }

    FILE *handler = popen(cmd.toStdString().c_str(), "r");
    QTextStream stream(handler, QIODevice::ReadOnly);

    while (!stream.atEnd())
    {
        QString line = stream.readLine();

        if (!line.isEmpty() && line.compare("./home/ture/Videos/") != 0)
        {
            searchFiles->append(line);
        }
    }

    pclose(handler);

    qSort(searchFiles->begin(), searchFiles->end(), byTime);
    reverseFiles();
    clearSystem();
    showSearchFiles();

    upButton->setEnabled(false);
    isSystemView = false;
}

void FileSystem::lineEdited(const QString &s)
{
    if (s.isEmpty())
    {
        searchButton->setEnabled(false);
        clearButton->setEnabled(false);

        if (!isSystemView)
        {
            back();
        }
    }
    else
    {
        searchButton->setEnabled(true);
        clearButton->setEnabled(true);
    }
}

void FileSystem::clearClicked()
{
    searchLine->clear();
    searchButton->setEnabled(false);
    clearButton->setEnabled(false);

    if (!isSystemView)
    {
        back();
    }
}

void FileSystem::itemClicked(QModelIndex i)
{
    QFileInfo fileInfo;
    if (isSystemView)
    {
        fileInfo = currentDir->entryInfoList().at(i.row());
    }
    else
    {
        fileInfo.setFile(searchFiles->at(i.row()));
    }

    if (fileInfo.isDir())
    {
        currentDir->cd(fileInfo.absoluteFilePath());
        back();
    }
    else
    {
        openFile(fileInfo);
    }
}

void FileSystem::indicatorClicked(int i, Qt::SortOrder order)
{
    if (isSystemView)
    {
        if (i == 0)
        {
            if (order == Qt::AscendingOrder)
            {
                currentDir->setSorting(QDir::Type);
            }
            else
            {
                currentDir->setSorting(QDir::Type | QDir::Reversed);
            }
        }
        else if (i == 1)
        {
            if (order == Qt::AscendingOrder)
            {
                currentDir->setSorting(QDir::Name);
            }
            else
            {
                currentDir->setSorting(QDir::Name | QDir::Reversed);
            }
        }
        else
        {
            if (order == Qt::AscendingOrder)
            {
                currentDir->setSorting(QDir::Time | QDir::Reversed);
            }
            else
            {
                currentDir->setSorting(QDir::Time);
            }
        }

        systemView->sortByColumn(i, order);

        clearSystem();
        showCurrentDir();

    }
    else if (!searchFiles->isEmpty())
    {
        if (i == 0)
        {
            qSort(searchFiles->begin(), searchFiles->end(), byType);
        }
        else if (i == 1)
        {
            qSort(searchFiles->begin(), searchFiles->end(), byName);
        }
        else
        {
            qSort(searchFiles->begin(), searchFiles->end(), byTime);
        }

        if (order == Qt::DescendingOrder)
        {
              reverseFiles();
        }

        clearSystem();
        showSearchFiles();
    }
}

void FileSystem::clearSystem()
{
    systemModel->removeRows(0, systemModel->rowCount());
}

void FileSystem::clearSearchFiles()
{
    searchFiles->clear();
}

void FileSystem::showCurrentDir()
{
    QList<QFileInfo> infoList = currentDir->entryInfoList();

    QFileIconProvider iconPro;
    foreach (QFileInfo fileInfo, infoList)
    {
        QIcon icon = iconPro.icon(fileInfo);
        QString name = fileInfo.fileName();
        QString date = fileInfo.lastModified().toString(Qt::SystemLocaleDate);

        QList<QStandardItem *> itemList;
        itemList.append(new QStandardItem(QIcon(icon), QString("")));
        itemList.append(new QStandardItem(QString(name)));
        itemList.append(new QStandardItem(QString(date)));
        systemModel->appendRow(itemList);
    }
}

void FileSystem::showSearchFiles()
{
    QFileIconProvider iconPro;
    foreach (QString filePath, *searchFiles)
    {
        QFileInfo fileInfo(filePath);
        QIcon icon = iconPro.icon(fileInfo);
        QString name = fileInfo.fileName();
        QString date = fileInfo.lastModified().toString(Qt::SystemLocaleDate);

        QList<QStandardItem *> itemList;
        itemList.append(new QStandardItem(QIcon(icon), QString("")));
        itemList.append(new QStandardItem(QString(name)));
        itemList.append(new QStandardItem(QString(date)));

        systemModel->appendRow(itemList);
    }
}

void FileSystem::openFile(const QFileInfo &fileInfo)
{
    QString suffix = fileInfo.suffix();
    QString path = addBackRef(fileInfo.filePath());


    if (suffix.compare("3gp") == 0 ||
        suffix.compare("avi") == 0 ||
        suffix.compare("dat") == 0 ||
        suffix.compare("flv") == 0 ||
        suffix.compare("mkv") == 0 ||
        suffix.compare("mov") == 0 ||
        suffix.compare("mp4") == 0 ||
        suffix.compare("mpeg") == 0 ||
        suffix.compare("wmv") == 0)
    {
        storage->mplayer->loadVideo(path);
        storage->stackLayout->setCurrentIndex(1);
    }
    else
    {
        QMessageBox::information(this, "File", "File suffix not supported");
    }
}

QString FileSystem::addBackRef(QString s)
{
    s.replace(" ", "\\ ");
    return s;
}

void FileSystem::reverseFiles()
{
    for (int i = 0; i < searchFiles->size() / 2; i++)
    {
        searchFiles->swap(i, searchFiles->size() - i - 1);
    }
}

bool FileSystem::byType(QString s1, QString s2)
{
    QFileInfo f1(s1);
    QFileInfo f2(s2);

    if (f1.isDir() && f2.isDir())
    {
        return f1.fileName() < f2.fileName();
    }
    else if (f1.isDir() && !f2.isDir())
    {
        return true;
    }
    else if (!f1.isDir() && f2.isDir())
    {
        return false;
    }
    else
    {
        if (f1.suffix() == f2.suffix())
        {
            return f1.baseName() < f2.baseName();
        }
        else
        {
            return f1.suffix() < f2.suffix();
        }
    }
}

bool FileSystem::byName(QString s1, QString s2)
{
    QFileInfo f1(s1);
    QFileInfo f2(s2);

    return f1.fileName() < f2.fileName();
}

bool FileSystem::byTime(QString s1, QString s2)
{
    QFileInfo f1(s1);
    QFileInfo f2(s2);

    return f1.lastModified() < f2.lastModified();
}
