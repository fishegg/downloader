#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include "download.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_start_button_clicked();
    void onfinished();
    void onstart_successfully();
    void onstop_successfully();
    void onfinished_all();

    void on_stop_button_clicked();

    void on_browse_button_clicked();
    void onwaiting_changed(int w);
    void ondownloaded_changed(int d);

    void on_download_url_edit_textChanged();

    void on_path_edit_textChanged(const QString &arg1);

private:
    Ui::MainWindow *ui;
    Download download_manager;
    QDir dir;
    int waiting;
    int downloaded;

};

#endif // MAINWINDOW_H
