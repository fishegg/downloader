#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    connect(&download_manager,&Download::start_successfully,this,&MainWindow::onstart_successfully);
    connect(&download_manager,&Download::stop_successfully,this,&MainWindow::onstop_successfully);
    connect(&download_manager,&Download::finished_all,this,&MainWindow::onfinished_all);
    connect(&download_manager,&Download::waiting_changed,this,&MainWindow::onwaiting_changed);
    connect(&download_manager,&Download::downloaded_changed,this,&MainWindow::ondownloaded_changed);
    ui->setupUi(this);
    ui->label_5->setText("0 downloaded");
    ui->label_6->setText("0 left");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_start_button_clicked()
{
    //Download download_manager;
    download_manager.click_start(ui->download_url_edit->toPlainText(),ui->replaced_text_edit->toPlainText(),ui->path_edit->text());
    //connect(&download_manager,&Download::start_successfully,this,&MainWindow::onstart_successfully);
    //connect(&download_manager,&Download::stop_successfully,this,&MainWindow::onstop_successfully);
    //connect(&download_manager,&Download::finished_all,this,&MainWindow::onfinished_all);
    //ui->start_button->setText(QString("clicked"));
    //connect(&download_manager,&Download::testsignal,this,&MainWindow::on_finished);
    //ui->replaced_text_label->setText(download_manager.file_text());
    //emit start_button_clicked();
}

void MainWindow::onfinished()
{
    //ui->replaced_text_label->setText(download_manager.file_text());
    //ui->textBrowser->setText(download_manager.file_text());
}

void MainWindow::onstart_successfully()
{
    ui->start_button->setEnabled(false);
    //ui->start_button->setText(QString("Started"));
    ui->stop_button->setEnabled(true);
}

void MainWindow::onstop_successfully()
{
    ui->start_button->setEnabled(true);
    ui->start_button->setText(QString("Start"));
    ui->stop_button->setEnabled(false);
}

void MainWindow::on_stop_button_clicked()
{
    download_manager.click_stop();
    //emit stop_button_clicked();
}

void MainWindow::onfinished_all()
{
    ui->start_button->setEnabled(true);
    ui->stop_button->setEnabled(false);
}

void MainWindow::on_browse_button_clicked()
{
    //file_dialog.open();
    ui->path_edit->setText(QFileDialog::getExistingDirectory(this,"","/"));
}

void MainWindow::onwaiting_changed(int w)
{
    waiting=w;
    ui->label_6->setText(QString::number(waiting)+" left");
}

void MainWindow::ondownloaded_changed(int d)
{
    downloaded=d;
    ui->label_5->setText(QString::number(downloaded)+" downloaded");
}

void MainWindow::on_download_url_edit_textChanged()
{
    if(!ui->download_url_edit->toPlainText().isEmpty() && dir.exists(ui->path_edit->text()))//!ui->path_edit->text().isEmpty())
        ui->start_button->setEnabled(true);
    else
        ui->start_button->setEnabled(false);
}

void MainWindow::on_path_edit_textChanged(const QString &arg1)
{
    if(!ui->download_url_edit->toPlainText().isEmpty() && dir.exists(ui->path_edit->text()))//!ui->path_edit->text().isEmpty())
        ui->start_button->setEnabled(true);
    else
        ui->start_button->setEnabled(false);
}
