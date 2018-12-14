#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/background_segm.hpp>
#include <opencv2/highgui/highgui_c.h>
#include <QPixmap>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/objdetect.hpp >
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "QMessageBox"

using namespace std;
using namespace cv;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

     //image de fond
     QPixmap pix("C:/Users/valbl/Pictures/fe0005-seder-1.jpg");
     ui->fond->setPixmap(pix);

     //ouvre le fichier selectionné
     QString fichier = QFileDialog :: getOpenFileName( this , tr( "Ouvrir un fichier" ) , "/ home" , tr( "Images (* .png * .xpm * .jpg)" ));
     setCheminImage(fichier);

     //positionne l'image sur le label et la mais au forma de celle-ci
     QString image = getCheminImage();
     int x = this->ui->image_debut->width();
     int y = this->ui->image_debut->height();
     QPixmap *pixmap_img = new QPixmap(image);
     this->ui->image_debut->setPixmap(pixmap_img->scaled(x,y));

}

QString MainWindow::getCheminImage()
{
    return cheminImage;
}

void MainWindow::setCheminImage(QString fichier)
{
    cheminImage = fichier;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_bouton_insertion_image_clicked()
{
    //ouvre le fichier selectionné
    QString fichier = QFileDialog :: getOpenFileName( this , tr( "Ouvrir un fichier" ) , "/ home" , tr( "Images (* .png * .xpm * .jpg)" ));
    setCheminImage(fichier);

    //positionne l'image sur le label et la mais au forma de celle-ci
    QString image = getCheminImage();
    int x = this->ui->image_debut->width();
    int y = this->ui->image_debut->height();
    QPixmap *pixmap_img = new QPixmap(image);
    this->ui->image_debut->setPixmap(pixmap_img->scaled(x,y));
}


cv::Mat cvmat_from_qimage(const QImage& qimage)
{
    cv::Mat mat = cv::Mat(qimage.height(), qimage.width(), CV_8UC4, (uchar*)qimage.bits(), qimage.bytesPerLine());
    cv::Mat mat2 = cv::Mat(mat.rows, mat.cols, CV_8UC3 );
    int from_to[] = { 0,0,  1,1,  2,2 };
    cv::mixChannels( &mat, 1, &mat2, 1, from_to, 3 );
    return mat2;
}

QImage qimage_from_cvmat(const cv::Mat& mat)
{
    cv::Mat rgb;
    cvtColor(mat, rgb, CV_BGR2RGB);
    return QImage((const unsigned char*)(rgb.data), rgb.cols, rgb.rows, QImage::Format_RGB888);
}

void MainWindow::on_boutuon_traitement_clicked()
{
    QMessageBox msgBox;


    //transformer QString en String (std::string imageCV = fichier.toLocal8Bit().constData();)
    QString fichier = getCheminImage();
    msgBox.setText("fichier."+ fichier);


    QImage      image1(fichier);

    cv::Mat src = cvmat_from_qimage(image1) ;
    Mat src_gray;

    std::string imageCV = fichier.toLocal8Bit().constData();
    cv::GaussianBlur (src, src_gray, cv::Size (1, 1), 0, 0, cv::BORDER_DEFAULT);
    QImage sortie = qimage_from_cvmat(src_gray);
    QFile file("image.png");
    file.open(QIODevice::WriteOnly);

    sortie.save(&file, "PNG");
    file.close();

    msgBox.exec();
}

void MainWindow::on_fond_linkActivated(const QString &link)
{

}
