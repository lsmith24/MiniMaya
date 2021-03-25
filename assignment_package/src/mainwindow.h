#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "face.h"
#include "vertex.h"
#include "halfedge.h"
#include "joint.h"

namespace Ui {
class MainWindow;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void setChildren(Joint* parent, QJsonArray children);
    void displayJointTransform();
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

    bool borderEdge(HalfEdge *e);
    void splitEdge(glm::vec3 newPos, HalfEdge *e1);
    void quadrangulate(Face *f, Vertex *centroid);
    void subdivide();
    void loadOBJ();
    void loadJSON();
    void jointDisplay(QTreeWidgetItem* item);
    void skin();
    void rotateJointX();
    void rotateJointY();
    void rotateJointZ();
    void changeJointX(double d);
    void changeJointY(double d);
    void changeJointZ(double d);

private:
    Ui::MainWindow *ui;
};


#endif // MAINWINDOW_H
