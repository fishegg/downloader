#include "download.h"

Download::Download(QObject *parent) : QObject(parent)
{
    //connect(network_reply,&QIODevice::readyRead,this,&Download::onReadyRead);
    //connect(network_reply,SIGNAL(readyRead()),this,SLOT(onReadyRead()));
    //connect(network_reply,&QNetworkReply::finished,this,&Download::onFinished);
    //connect(this,&Download::testsignal,this,&Download::test);
    //qDebug()<<"constructed";
    waiting=0;
    emit waiting_changed(waiting);
    downloaded=0;
    emit downloaded_changed(downloaded);
    downloading=false;
    connect(&network_manager,&QNetworkAccessManager::finished,this,&Download::onFinished);
    connect(&network_manager,&QNetworkAccessManager::finished,this,&Download::new_download_after_finished);
    connect(this,&Download::start_successfully,this,&Download::onstart_successfully);
    connect(this,&Download::stop_successfully,this,&Download::onstop_successfully);
    connect(this,&Download::finished_all,this,&Download::onfinished_all);
}

Download::~Download()
{
}

void Download::click_start(const QString &pu, const QString &t, const QString &p)
{
    if(pu.isEmpty()||p.isEmpty()){
        return;
    }
    if(!downloading){
        emit_signal();
        set_pre_url(pu);
        set_replaced_text(t);
        set_path(p);
        qDebug()<<"pu"<<pu<<"text"<<t<<"path"<<p;
        start_download();
    }
}

void Download::click_stop()
{
    if(downloading){
        stop_download();
    }
}

void Download::set_pre_url(const QString &pu)
{
    pre_url=pu;
    //qDebug()<<*(pre_url.end()-1);
    if(*(pre_url.end()-1)=='/')
        pre_url.chop(1);
}

void Download::set_url(QString pu)
{
    QString t;
    if(!replaced_text_queue.isEmpty())
        t=QString::number(replaced_text_queue.dequeue());
    qDebug()<<t;
    url.setUrl(pu.replace(QString("[replace]"),t));
    //qDebug()<<url.toString();
}

void Download::set_replaced_text(const QString &t)
{
    replaced_text_queue.clear();
    QString tmpstr;
    int start = -1,end = -1;
    QString::const_iterator it;
    for(it=t.begin();it!=t.end();++it){
        if(*it>='0' && *it<='9'){
            tmpstr.append(*it);
        }
        else if(*it==',' && start==-1 && end==-1){
            start=tmpstr.toInt();
            end=start;
            tmpstr.clear();
        }
        else if(*it=='-' && start==-1){
            start=tmpstr.toInt();
            tmpstr.clear();
        }
        else if(*it==',' && start!=-1){
            end=tmpstr.toInt();
            tmpstr.clear();
        }
        if(start!=-1 && end!=-1){
            for(int i=start;i<=end;++i){
                replaced_text_queue.enqueue(i);
            }
            start=end=-1;
        }
    }
    if(start != -1){
        end=tmpstr.toInt();
        tmpstr.clear();
    }
    else if(start == -1){
        start=tmpstr.toInt();
        end=start;
        tmpstr.clear();
    }
    for(int i=start;i<=end;++i){
        replaced_text_queue.enqueue(i);
    }
    start=end=-1;

    waiting=replaced_text_queue.size() && pre_url.indexOf("[replace]")>=0?
                replaced_text_queue.size():
                1;
    emit waiting_changed(waiting);

    /*while(replaced_text_queue.isEmpty()==false){
        qDebug()<<replaced_text_queue.dequeue();
    }*/
}

void Download::set_path(const QString &p)
{
    path=p;
    //path="D:\\a";
    if(*(path.end()-1)!='\\')
        path.append('\\');
}

void Download::set_file_name()
{
    file_name=url.fileName();
    set_existed_file_name();
    //qDebug()<<"set file name";
}

void Download::set_existed_file_name()
{
    QFile file(path+file_name);
    QFile file_downloading(path+file_name+".downloading");
    int i=1;
    QString str("("+QString::number(i)+")");
    QString tmp_file_name(file_name);
    int pos=tmp_file_name.indexOf('.');
    tmp_file_name.insert(pos,str);
    //qDebug()<<file_name<<file.exists();
    while(file.exists()||file_downloading.exists()){
        int pos=file_name.indexOf('.');
        QString insert("("+QString::number(i)+")");
        if(pos!=-1){
            file_name.insert(pos,insert);
            file.setFileName(path+file_name);
            file_downloading.setFileName(path+file_name+".downloading");
        }
        i++;
    }
    file.close();
    file_downloading.close();
}

void Download::start_download()
{
    emit start_successfully();
    downloaded=0;
    emit downloaded_changed(downloaded);
    downloading=true;
    set_url(pre_url);
    set_file_name();
    network_request.setUrl(url);
    network_reply=network_manager.get(network_request);
    connect(network_reply,&QNetworkReply::readyRead,this,&Download::onReadyRead);
    //connect(network_reply,&QNetworkReply::finished,this,&Download::onFinished);
    //connect(network_reply,&QNetworkReply::finished,this,&Download::new_download_after_finished);
    /*if(pre_url.indexOf(QString("[replace]")) == -1){
        set_url(pre_url);
        network_request.setUrl(url);
        //qDebug()<<"set url";
        network_reply=network_manager.get(network_request);
        //qDebug()<<network_reply->error();
        connect(network_reply,SIGNAL(readyRead()),this,SLOT(onReadyRead()));
        connect(network_reply,SIGNAL(finished()),this,SLOT(onFinished()));
    }*/
    /*else{
        while(!replaced_text_queue.isEmpty()){
            QString text(QString::number(replaced_text_queue.dequeue()));
            //qDebug()<<text;
            set_url(pre_url,text);
            network_request.setUrl(url);
            network_reply=network_manager.get(network_request);
            connect(network_reply,&QNetworkReply::readyRead,this,&Download::onReadyRead);
            connect(network_reply,&QNetworkReply::finished,this,&Download::onFinished);
            //qDebug()<<network_reply->error();
        }
    }*/
}

void Download::stop_download()
{
    downloading=false;
    network_reply->abort();
    network_reply->deleteLater();
    network_reply=nullptr;
    QFile file(path+file_name+".downloading");
    file.remove();
    qDebug()<<"stop";
    waiting=0;
    emit waiting_changed(waiting);
    downloaded=0;
    emit downloaded_changed(downloaded);
    emit stop_successfully();
}

void Download::test()
{
    qDebug()<<"connected";
}

void Download::onReadyRead()
{
    qDebug()<<"readyread";
    //set_file_name();
    QFile new_file(path+file_name+".downloading");
    if(new_file.open(QIODevice::Append)){
        qDebug()<<"new file"<<file_name;
        new_file.write(network_reply->readAll());
    }
    new_file.close();
    //network_reply->deleteLater();
}

void Download::onFinished()
{
    qDebug()<<"finished";
    network_reply->deleteLater();
    QFile file(path+file_name+".downloading");
    file.rename(path+file_name);
    //emit_signal();
    waiting--;
    downloaded++;
    emit waiting_changed(waiting);
    emit downloaded_changed(downloaded);
}

void Download::new_download_after_finished()
{
    if(!waiting){
        downloading=false;
        network_reply=nullptr;
        emit finished_all();
        return;
    }
    set_url(pre_url);
    set_file_name();
    network_request.setUrl(url);
    network_reply=network_manager.get(network_request);
    connect(network_reply,&QNetworkReply::readyRead,this,&Download::onReadyRead);
    //connect(network_reply,&QNetworkReply::finished,this,&Download::onFinished);
    //connect(network_reply,&QNetworkReply::finished,this,&Download::new_download_after_finished);
    //if(!replaced_text_queue.isEmpty()){
        //QString text(QString::number(replaced_text_queue.dequeue()));
        //qDebug()<<text;
        //set_url(pre_url);
        //network_request.setUrl(url);
        //network_reply=network_manager.get(network_request);
        //connect(network_reply,&QNetworkReply::readyRead,this,&Download::onReadyRead);
        //connect(network_reply,&QNetworkReply::finished,this,&Download::onFinished);
        //qDebug()<<network_reply->error();
    //}
}

void Download::emit_signal()
{
    emit testsignal();
    qDebug()<<"emitted";
}

QString Download::file_text()
{
    QFile file(path+file_name);
    QByteArray s;
    QTextCodec *big5codec=QTextCodec::codecForName("big5");
    file.open(QIODevice::ReadOnly);
    s.append(file.readAll());
    file.close();
    return big5codec->toUnicode(s);
}

void Download::onstart_successfully()
{
    //downloading=true;
}

void Download::onstop_successfully()
{
    //downloading=false;
}

void Download::onfinished_all()
{
    //downloading=false;
}
