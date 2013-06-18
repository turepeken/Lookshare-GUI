#ifndef MYTHDATABASE_H
#define MYTHDATABASE_H

#include <QWidget>
#include <QStackedLayout>

#include <gst/gst.h>
#include <gst/base/gstbasesrc.h>
#include <gst/base/gstpushsrc.h>
#include "cmyth/cmyth.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

class Mythdatabase : public QWidget
{
    Q_OBJECT




public:


    explicit Mythdatabase(QWidget *parent = 0);
    ~Mythdatabase();

    void connectdatabase();


    struct MythtvSrc {

    GstPushSrc      element;
    cmyth_conn_t control;
    cmyth_file_t file;
    cmyth_recorder_t rec;
    cmyth_proginfo_t prog;
    char *uri;
    long long pos;
    u_int64_t size;
    };

    struct MythtvSrcClas {
    GstPushSrcClass parent_class;
    };

    gboolean mythtv_src_start(MythtvSrc *src);




public:

    QVBoxLayout *mainLayout;
};

#endif // MYTHDATABASE_H
