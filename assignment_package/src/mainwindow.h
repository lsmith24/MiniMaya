#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "face.h"
#include "vertex.h"
#include "halfedge.h"

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
    void on_actionQuit_triggered();

    void on_actionCamera_Controls_triggered();

public slots:
    void addFace(QListWidgetItem *f);
    void addEdge(QListWidgetItem *he);
    void addVtx(QListWidgetItem *v);
    void itemDisplay(QListWidgetItem *item);
    void changeRed(double d);
    void changeGreen(double d);
    void changeBlue(double d);
    void changeX(double d);
    void changeY(double d);
    void changeZ(double d);
    void keyEdge(QListWidgetItem *item);
    void keyFace(QListWidgetItem *item);
    void keyVert(QListWidgetItem *item);
    void addMid();
    void triangulate();

private:
    Ui::MainWindow *ui;
};


#endif // MAINWINDOW_H
