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
     QPixmap pix("fe0005-seder-1.jpg");
     ui->fond->setPixmap(pix);

     //ouvre le fichier selectionné
     QString fichier = QFileDialog :: getOpenFileName( this , tr( "Ouvrir un fichier" ) , "images/" , tr( "Images (* .png * .xpm * .jpg)" ));
     setCheminImage(fichier);

     //positionne l'image sur le label et la mais au forma de celle-ci
     int x = this->ui->image->width();
     int y = this->ui->image->height();
     QPixmap *img = new QPixmap(getCheminImage());
     this->ui->image->setPixmap(img->scaled(x,y));

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

void MainWindow::on_bouton_insertion_clicked()
{
    //ouvre le fichier selectionné
    QString fichier = QFileDialog :: getOpenFileName( this , tr( "Ouvrir un fichier" ) , "/ home" , tr( "Images (* .png * .xpm * .jpg)" ));
    setCheminImage(fichier);

    //positionne l'image sur le label et la mais au forma de celle-ci
    int x = this->ui->image->width();
    int y = this->ui->image->height();
    QPixmap *img = new QPixmap(getCheminImage());
    this->ui->image->setPixmap(img->scaled(x,y));
}

void MainWindow::on_bouton_traitement_clicked()
{

    /*Ouverture de l'image et creation des images cv*/
    cv::String fichier = getCheminImage().toStdString();
    Mat image = imread(fichier, IMREAD_COLOR );
    Mat blur_image;
    Mat gray_image;
    Mat binary_image;
    Mat circle_image;

    /*Filtre median pour le blur*/
    cv::medianBlur(image, blur_image, 15);

    /*Filtre noir et blanc*/
    cvtColor( image, gray_image, COLOR_BGR2GRAY );

    /*Filtre binaire noir ou blanc*/
    cv :: threshold(gray_image, binary_image, 200, 255, cv::THRESH_BINARY);

    /*Detection des cercles*/
    vector<Vec3f> circles;
    cv::HoughCircles( gray_image, circles, HOUGH_GRADIENT, 1, gray_image.rows/4, 100, 30, gray_image.rows/4.5, gray_image.rows/2 );
    for( size_t i = 0; i < circles.size(); i++ )
    {
        Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
        int radius = cvRound(circles[i][2]);
        // circle center
        cv::circle( gray_image, center, 3, Scalar(255,0,0), -1, 8, 0 );
        // circle outline
        cv::circle( gray_image, center, radius, Scalar(0,0,255), 3, 8, 0 );
     }

    /*Detection des contours*/
    std::vector<std::vector<cv::Point> > contours;
    cv::findContours( binary_image, contours, cv::RETR_LIST, cv::CHAIN_APPROX_NONE );
    cv::Mat contour_image(binary_image.size(), CV_8UC3, cv::Scalar(0,0,0));
    cv::Scalar colors[3];
    colors[0] = cv::Scalar(255, 0, 0);
    colors[1] = cv::Scalar(0, 255, 0);
    colors[2] = cv::Scalar(0, 0, 255);
    for (size_t idx = 0; idx < contours.size(); idx++)
    {
        cv::drawContours(contour_image, contours, idx, colors[idx % 3]);
    }

    /*Enregistrement des images filtrées*/
    imwrite( "results/blur_image.jpg", blur_image );
    imwrite( "results/gray_image.jpg", gray_image );
    imwrite( "results/binary_image.jpg", binary_image );
    imwrite( "results/contour_image.jpg", contour_image );

    /*Affichage des images*/
    int x = this->ui->filtre1->width();
    int y = this->ui->filtre1->height();
    QPixmap *filtre1 = new QPixmap("results/blur_image.jpg");
    this->ui->filtre1->setPixmap(filtre1->scaled(x,y));
    QPixmap *filtre2 = new QPixmap("results/gray_image.jpg");
    this->ui->filtre2->setPixmap(filtre2->scaled(x,y));
    QPixmap *filtre3 = new QPixmap("results/binary_image.jpg");
    this->ui->filtre3->setPixmap(filtre3->scaled(x,y));
    QPixmap *filtre4 = new QPixmap("results/contour_image.jpg");
    this->ui->filtre4->setPixmap(filtre4->scaled(x,y));

    /*Affichage du nombre de pommes détectées*/
    this->ui->nb_detect->setText("<html><head/><body><p><span style=\" color:#ffffff;\">Nombre de pomme détectée : " + QString::number(circles.size()) + "</span></p></body></html>");
}
