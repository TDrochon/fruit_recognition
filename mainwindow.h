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
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void setCheminImage(QString fichier);
    QString getCheminImage();


private slots:
    void on_bouton_insertion_image_clicked();

    void on_fond_linkActivated(const QString &link);

    void on_boutuon_traitement_clicked();

private:
    Ui::MainWindow *ui;
    QString cheminImage;
};

#endif // MAINWINDOW_H
