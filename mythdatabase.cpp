
#include <QMessageBox>
#include <QTextStream>
#include <QHeaderView>
#include <QStandardItem>
#include <QFileIconProvider>
#include <QIcon>
#include <QFileInfo>
#include <QDateTime>
#include <QtAlgorithms>
#include <QtSql>
#include <QtDebug>

#include "stdlib.h"

#include "mythdatabase.h"
#include "refmem/refmem.h"
#include <string.h>
#include <unistd.h>
#include <cmyth/cmyth.h>




Mythdatabase::Mythdatabase(QWidget *parent) :
    QWidget(parent)
{

    mainLayout = new QVBoxLayout;

    setLayout(mainLayout);

}

Mythdatabase::~Mythdatabase()
{
}

void Mythdatabase::connectdatabase()
{
    QString serverName="localhost";
    QString dbName="mythconverg";
    Mythdatabase::MythtvSrc *src_temp;

    QSqlDatabase db= QSqlDatabase::addDatabase("QSQLITE");

    db.setConnectOptions();

    QString dsn=QString("DRIVER=(SQL Native Client);SERVER=%1;DATABASE=%2;UID=mythtv;PWD=mythtv").arg(serverName).arg(dbName);

    db.setDatabaseName(dsn);

    if(db.open()){
        qDebug() <<"Opened!";
        mythtv_src_start(src_temp);
        db.close();
    }

}

gboolean Mythdatabase::mythtv_src_start(MythtvSrc *src)
{

cmyth_proglist_t episodes;
int i, count;
char user[100];
char pass[100];
char host[100];
char cat[100];
char filename[100];
char *pathname;
char **err=NULL;
gboolean ret = TRUE;
char *c;

src->size = 0;

if(sscanf(src->uri, "myth://%99[^:]:%99[^@]@%99[^/]/%99[^/]%99[^\n]", user, pass, host, cat, filename) != 5) {
return FALSE;
}


if((src->control=cmyth_conn_connect_ctrl(host, 6543, 16*1024, 4096)) == NULL) {
return FALSE;
}

if(strcmp(cat, "recordings") == 0) {
episodes = cmyth_proglist_get_all_recorded(src->control);
if(episodes == NULL) {
ref_release(src->control);
return FALSE;
}
count = cmyth_proglist_get_count(episodes);

for(i=0; i<count; i++) {
src->prog = cmyth_proglist_get_item(episodes, i);

pathname = cmyth_proginfo_pathname(src->prog);
if(strcmp(filename, pathname) == 0) {
break;
}

}
ref_release(episodes);
} else {
// fix channel string
c = strstr(filename, ".ts");
*c = '\0';
// search recorder
for(i=0; i<16; i++) {
src->rec = cmyth_conn_get_recorder_from_num(src->control, i);
if(src->rec) {
if(cmyth_recorder_is_recording(src->rec) == 0 && cmyth_recorder_check_channel(src->rec, filename+1) == 0) {
break;
}
ref_release(src->rec);
src->rec = NULL;
}
}
if(src->rec == NULL) {
return FALSE;
}

//src->rec = cmyth_spawn_live_tv(src->rec, 16*1024, 4096, src->prog, err);

cmyth_recorder_pause(src->rec);
cmyth_recorder_set_channel(src->rec, filename+1);


src->prog = cmyth_recorder_get_cur_proginfo(src->rec);
cmyth_proginfo_rec_start(src->prog);
pathname = cmyth_proginfo_pathname(src->prog);

}

if((src->file=cmyth_conn_connect_file(src->prog, src->control, 16*1024, 32*1024)) == NULL) {
ret = FALSE;
}

ref_release(pathname);

return ret;
}



