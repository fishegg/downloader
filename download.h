#ifndef DOWNLOAD_H
#define DOWNLOAD_H

#include <QObject>
#include <QQueue>
#include <QVariant>
#include <QUrl>
//#include <QDebug>
#include <QFile>
#include <QNetworkAccessManager>
#include <QNetworkReply>
//#include <QNetworkRequest>
//#include <QIODevice>
#include <QTextCodec>

class Download : public QObject
{
    Q_OBJECT
public:
    explicit Download(QObject *parent = nullptr);
    ~Download();
    void click_start(const QString &pu,const QString &t,const QString &p);
    void click_stop();
    QString file_text();

public slots:
    //void onReadyRead();
    //void onFinished();
    void test();
    //void new_download_after_finished();

private slots:
    void onReadyRead();
    void onFinished();
    void onstart_successfully();
    void onstop_successfully();
    void onfinished_all();

private:
    QQueue<int> replaced_text_queue;
    QUrl url;
    QString pre_url;
    QString path;
    QString file_name;
    int waiting;
    int downloaded;
    bool downloading;
    QNetworkAccessManager network_manager;
    QNetworkRequest network_request;
    QNetworkReply *network_reply;
    void create_file();
    void set_replaced_text(const QString &t);
    void set_pre_url(const QString &pu);
    void set_url(QString pu);
    void set_path(const QString &p);
    void set_file_name();
    void set_existed_file_name();
    void start_download();
    void new_download_after_finished();
    void stop_download();
    void emit_signal();

signals:
    void testsignal();
    void start_successfully();
    void stop_successfully();
    void finished_all();
    void waiting_changed(int waiting);
    void downloaded_changed(int downloaded);

public slots:

};

#endif // DOWNLOAD_H
