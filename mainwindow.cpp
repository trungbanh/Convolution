#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QFileDialog"
#include "iostream"

using namespace std;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
                                          ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::DisplayWindown(QImage &image, QString title)
{
    QLabel *label = new QLabel();
    label->setPixmap(QPixmap::fromImage(image));
    label->setWindowTitle(title);
    label->show();
}

void MainWindow::on_btn_browse_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, "Open File",
                                                    "D:/trung/hinh",
                                                    "*.* All File;; *.bmp;; *.jpeg;; *.jpg;; *.png;;");
    if (filename.isEmpty())
    {
        return;
    }
    ui->le_path->setText(filename);
    //        QImage img(filename);
    //        DisplayWindown(img,"meo meo meo");
}

float MainWindow::myRand(int min,int max) {
    assert(min<max) ;
    float ran = (max-min)*(((float) rand())/ (float) RAND_MAX) + min;
    return ran ;
}

QImage MainWindow::noise_maker(QImage &input, float level)
{
    QImage out_image(input.width(), input.height(), QImage::Format_ARGB32);
    float randsalt[input.width()][input.height()];
    for (int x = 0; x < input.width(); x++)
    {
        for (int y = 0; y < input.height(); y++)
        {
            randsalt[x][y] = myRand(0,1);
            if (randsalt[x][y] <= level/2.0)
            {
                cout << "white " ;
                out_image.setPixel(x, y, qRgb(0, 0, 0));
            }
            else if (randsalt[x][y] >= 1 -level/2.0)
            {
                cout << "black " ;
                out_image.setPixel(x, y, qRgb(255, 255, 255));
            }
            else
            {
                cout << " normal  " ;
                out_image.setPixel(x, y, input.pixel(x, y));
            }
        }
    }
    return out_image;
}

void MainWindow::on_btn_noise_clicked()
{
    QString file = ui->le_path->text();
    QImage in_img(file);
    QImage out_img = noise_maker(in_img, 0.01);
    DisplayWindown(out_img, QFileInfo(file).fileName());
    QString selectedFilter;
    QString newFile = QFileDialog::getSaveFileName(this, "Save",
                                                   "D:/trung/hinh",
                                                   "*.* All File;; *.bmp;; *.jpeg;; *.jpg;; *.png;;", &selectedFilter);
//    if (selectedFilter == "BMP (*.bmp)")
//    {
//        out_img.save(newFile, "BMP");
//    }
    if (selectedFilter == "PNG (*.png)")
    {
        out_img.save(newFile, "PNG");
    }
//    if (selectedFilter == "PGM (*.pgm)")
//    {
//        out_img.save(newFile, "PGM");
//    }
    else if (selectedFilter == "JPEG (*.jpg)")
    {
        out_img.save(newFile, "JPEG");
    }
    else
    {
        out_img.save(newFile);
    }
}

void MainWindow::on_btn_mean_color_clicked()
{
    QString file = ui->le_path->text();
    QImage in_img(file);
    QImage out_img(in_img.width(), in_img.height(), QImage::Format_ARGB32);
    out_img.fill(Qt::white);
    int size = 3;
    int cells = size * size;
    int margin = size / 2;
    int sumR, sumG, sumB;
    QColor color;
    for (int x = 0; x < in_img.width() - margin; x++)
    {
        for (int y = 0; y < in_img.height() - margin; y++)
        {
            sumR = sumG = sumB = 0;
            for (int i = -margin; i <= margin; i++)
            {
                for (int j = -margin; j <= margin; j++)
                {
                    color = in_img.pixel(x + i, y + j);
                    sumR += color.red();
                    sumG += color.green();
                    sumB += color.blue();
                }
            }
            out_img.setPixel(x, y, qRgb(sumR / cells, sumG / cells, sumB / cells));
        }
    }

    DisplayWindown(out_img, "_Mean");
}

void MainWindow::on_btn_media_gray_clicked()
{
    QString file = ui->le_path->text();
    QImage in_img(file);
    QImage out_img(in_img.width(), in_img.height(), QImage::Format_ARGB32);
    out_img.fill(Qt::white);
    int size = 3;
    int cells = size * size;
    int margin = size / 2;
    int h[cells];
    for (int x = 0; x < in_img.width() - margin; x++)
    {
        for (int y = 0; y < in_img.height() - margin; y++)
        {
            int k = 0;
            for (int i = -margin; i <= margin; i++)
            {
                for (int j = -margin; j <= margin; j++)
                {
                    QRgb color = in_img.pixel(x + i, y + j);
                    int gray = qGray(color);
                    h[k] = gray;
                    k++;
                }
            }
            qSort(h, h + cells);
            out_img.setPixel(x, y, qRgb(h[cells / 2], h[cells / 2], h[cells / 2]));
        }
    }
    DisplayWindown(out_img, "_Median");
}

int MainWindow::Mean(int cells[])
{
    int mean = 0;
    for (int i = 0; i < sizeof(cells); i++)
    {
        mean += cells[i];
    }
    return mean / sizeof(cells);
}

void MainWindow::on_btn_mean_gray_clicked()
{
    QString file = ui->le_path->text();
    QImage in_img(file);
    QImage out_img(in_img.width(), in_img.height(), QImage::Format_ARGB32);
    out_img.fill(Qt::white);
    int size = 3;
    int cells = size * size;
    int margin = size / 2;
    int h[cells];
    for (int x = 0; x < in_img.width() - margin; x++)
    {
        for (int y = 0; y < in_img.height() - margin; y++)
        {
            int k = 0;
            for (int i = -margin; i <= margin; i++)
            {
                for (int j = -margin; j <= margin; j++)
                {
                    QRgb color = in_img.pixel(x + i, y + j);
                    int gray = qGray(color);
                    h[k] = gray;
                    k++;
                }
            }
            int mean = Mean(h);

            out_img.setPixel(x, y, qRgb(mean, mean, mean));
        }
    }
    DisplayWindown(out_img, "_Mean");
}

void MainWindow::on_btn_median_color_clicked()
{
    QString file = ui->le_path->text();
    QImage in_img(file);
    QImage out_img(in_img.width(), in_img.height(), QImage::Format_ARGB32);
    out_img.fill(Qt::white);
    int size = 3;
    int cells = size * size;
    int margin = size / 2;
    int sumR[cells], sumG[cells], sumB[cells];
    QColor color;
    for (int x = 0; x < in_img.width() - margin; x++)
    {
        for (int y = 0; y < in_img.height() - margin; y++)
        {
            int k = 0;
            for (int i = -margin; i <= margin; i++)
            {
                for (int j = -margin; j <= margin; j++)
                {
                    color = in_img.pixel(x + i, y + j);
                    sumR[k] = color.red();
                    sumG[k] = color.green();
                    sumB[k] = color.blue();
                    k++;
                }
            }
            qSort(sumR, sumR + cells);
            qSort(sumG, sumG + cells);
            qSort(sumB, sumB + cells);

            out_img.setPixel(x, y, qRgb(sumR[cells / 2], sumG[cells / 2], sumB[cells / 2]));
        }
    }

    DisplayWindown(out_img, "_Median");
}

void MainWindow::on_btn_Conv_3x3_clicked()
{
    float x3_1 = ui->le_3_1->text().toFloat();
    float x3_2 = ui->le_3_2->text().toFloat();
    float x3_3 = ui->le_3_3->text().toFloat();
    float x3_4 = ui->le_3_4->text().toFloat();
    float x3_5 = ui->le_3_5->text().toFloat();
    float x3_6 = ui->le_3_6->text().toFloat();
    float x3_7 = ui->le_3_7->text().toFloat();
    float x3_8 = ui->le_3_8->text().toFloat();
    float x3_9 = ui->le_3_9->text().toFloat();

    float mask[9] = {x3_1, x3_2, x3_3, x3_4, x3_5, x3_6, x3_7, x3_8, x3_9};
    //    for (int i =0 ; i< sizeof(mask)/sizeof(mask[0]) ; i++) {
    //        cout << mask[i] << endl ;
    //    }
    QString file = ui->le_path->text();
    QImage in_img(file);

    QImage out_img(in_img.width(), in_img.height(), QImage::Format_ARGB32);
    out_img.fill(Qt::white);
    int size = 3;
    int cells = size * size;
    int margin = size / 2;
    int h[cells];
    int k = 0, conv = 0;
    for (int x = 0; x < in_img.width() - margin; x++)
    {
        for (int y = 0; y < in_img.height() - margin; y++)
        {
            k = 0;
            conv = 0;
            for (int i = -margin; i <= margin; i++)
            {
                for (int j = -margin; j <= margin; j++)
                {
                    QRgb color = in_img.pixel(x + i, y + j);
                    int gray = qGray(color);
                    h[k] = gray;
                    k++;
                }
            }
            for (int i = 0; i < cells; i++)
            {
                conv += int(h[i] * mask[i]);
                if (conv < 0)
                    conv = 0;
                if (conv > 255)
                    conv = 255;
            }
            //            cout << conv << " " ;
            out_img.setPixel(x, y, qRgb(conv, conv, conv));
        }
    }
    DisplayWindown(out_img, "Conv");
}

void MainWindow::on_btn_Conv_5x5_clicked()
{
    float x5_1 = ui->le_5_1->text().toFloat();
    float x5_2 = ui->le_5_2->text().toFloat();
    float x5_3 = ui->le_5_3->text().toFloat();
    float x5_4 = ui->le_5_4->text().toFloat();
    float x5_5 = ui->le_5_5->text().toFloat();
    float x5_6 = ui->le_5_6->text().toFloat();
    float x5_7 = ui->le_5_7->text().toFloat();
    float x5_8 = ui->le_5_8->text().toFloat();
    float x5_9 = ui->le_5_9->text().toFloat();
    float x5_10 = ui->le_5_10->text().toFloat();
    float x5_11 = ui->le_5_11->text().toFloat();
    float x5_12 = ui->le_5_12->text().toFloat();
    float x5_13 = ui->le_5_13->text().toFloat();
    float x5_14 = ui->le_5_14->text().toFloat();
    float x5_15 = ui->le_5_15->text().toFloat();
    float x5_16 = ui->le_5_16->text().toFloat();
    float x5_17 = ui->le_5_17->text().toFloat();
    float x5_18 = ui->le_5_18->text().toFloat();
    float x5_19 = ui->le_5_19->text().toFloat();
    float x5_20 = ui->le_5_20->text().toFloat();
    float x5_21 = ui->le_5_21->text().toFloat();
    float x5_22 = ui->le_5_22->text().toFloat();
    float x5_23 = ui->le_5_23->text().toFloat();
    float x5_24 = ui->le_5_24->text().toFloat();
    float x5_25 = ui->le_5_25->text().toFloat();

    float mask[25] = {x5_1, x5_2, x5_3, x5_4, x5_5, x5_6, x5_7, x5_8, x5_9, x5_10, x5_11, x5_12, x5_13, x5_14, x5_15, x5_16, x5_17, x5_18 ,x5_19, x5_20, x5_21, x5_22, x5_23, x5_24, x5_25};
    //    for (int i =0 ; i< sizeof(mask)/sizeof(mask[0]) ; i++) {
    //        cout << mask[i] << endl ;
    //    }
    QString file = ui->le_path->text();
    QImage in_img(file);

    QImage out_img(in_img.width(), in_img.height(), QImage::Format_ARGB32);
    out_img.fill(Qt::white);
    int size = 5;
    int cells = size * size;
    int margin = size / 2;
    int h[cells];
    int k = 0, conv = 0;
    for (int x = 0; x < in_img.width() - margin; x++)
    {
        for (int y = 0; y < in_img.height() - margin; y++)
        {
            k = 0;
            conv = 0;
            for (int i = -margin; i <= margin; i++)
            {
                for (int j = -margin; j <= margin; j++)
                {
                    QRgb color = in_img.pixel(x + i, y + j);
                    int gray = qGray(color);
                    h[k] = gray;
                    k++;
                }
            }
            for (int i = 0; i < cells; i++)
            {
                conv += int(h[i] * mask[i]);
                if (conv < 0)
                    conv = 0;
                if (conv > 255)
                    conv = 255;
            }
            //            cout << conv << " " ;
            out_img.setPixel(x, y, qRgb(conv, conv, conv));
        }
    }
    DisplayWindown(out_img, "Conv");
}
