#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public:
    void DisplayWindown(QImage &image,QString title);
    QImage noise_maker (QImage &input, float level);
    int Mean (int cells[9]);
    float myRand (int min ,int max) ;

private slots:
    void on_btn_browse_clicked();

    void on_btn_noise_clicked();

    void on_btn_mean_color_clicked();

    void on_btn_media_gray_clicked();

    void on_btn_mean_gray_clicked();

    void on_btn_median_color_clicked();

    void on_btn_Conv_3x3_clicked();

    void on_btn_Conv_5x5_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
