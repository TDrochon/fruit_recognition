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

#include "iostream"

#include "QMessageBox"

using namespace std;
using namespace cv;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

     //image de fond
     QPixmap pix("C:/Users/valbl/Documents/GitHub/fruit_recognition/fe0005-seder-1.jpg");
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

void MainWindow::on_boutuon_traitement_clicked()
{

    //transformer QString en String (std::string imageCV = fichier.toLocal8Bit().constData();)
    cv::String fichier = getCheminImage().toStdString();

    Mat image = imread(fichier, IMREAD_COLOR );
    Mat gray_image;
    Mat src_image;
    cvtColor( image, gray_image, COLOR_BGR2GRAY );
    cv :: threshold(gray_image, src_image, 220, 255, cv::THRESH_BINARY);

    //Find the contours. Use the contourOutput Mat so the original image doesn't get overwritten
       std::vector<std::vector<cv::Point> > contours;
       cv::Mat contourOutput = src_image.clone();
       cv::findContours( contourOutput, contours, cv::RETR_LIST, cv::CHAIN_APPROX_NONE );

       //Draw the contours
       cv::Mat contourImage(src_image.size(), CV_8UC3, cv::Scalar(0,0,0));
       cv::Scalar colors[3];
       colors[0] = cv::Scalar(255, 0, 0);
       colors[1] = cv::Scalar(0, 255, 0);
       colors[2] = cv::Scalar(0, 0, 255);
       for (size_t idx = 0; idx < contours.size(); idx++)
       {
        cv::drawContours(contourImage, contours, idx, colors[idx % 3]);
        cout<<"idx = "<<idx<<endl;
       }

    imwrite( "Gray_Image.jpg", gray_image );
    imwrite( "src_Image.jpg", src_image );
    imwrite( "Coutours.jpg", gray_image );


    int x = this->ui->image_fin->width();
    int y = this->ui->image_fin->height();
    QPixmap *pixmap_img = new QPixmap("Coutours.jpg");
    this->ui->image_fin->setPixmap(pixmap_img->scaled(x,y));



/*
    cv::Mat src = cvmat_from_qimage(image1) ;

    Mat src_gray;


    QImage sortie = qimage_from_cvmat(src);

    int x = this->ui->image_fin->width();
    int y = this->ui->image_fin->height();
    QPixmap *img_aff = new  QPixmap();
    img_aff->convertFromImage(sortie);
    this->ui->image_fin->setPixmap(img_aff->scaled(x,y));*/
}
