#include "mainwindow.h"
#include <ui_mainwindow.h>
#include "cameracontrolshelp.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->mygl->setFocus();
    connect(ui->mygl, SIGNAL(sendEdge(QListWidgetItem*)), this, SLOT(addEdge(QListWidgetItem*)));
    connect(ui->mygl, SIGNAL(sendFace(QListWidgetItem*)), this, SLOT(addFace(QListWidgetItem*)));
    connect(ui->mygl, SIGNAL(sendVtx(QListWidgetItem*)), this, SLOT(addVtx(QListWidgetItem*)));
    connect(ui->vertsListWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(itemDisplay(QListWidgetItem*)));
    connect(ui->halfEdgesListWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(itemDisplay(QListWidgetItem*)));
    connect(ui->facesListWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(itemDisplay(QListWidgetItem*)));
    connect(ui->faceRedSpinBox, SIGNAL(valueChanged(double)), this, SLOT(changeRed(double)));
    connect(ui->faceGreenSpinBox, SIGNAL(valueChanged(double)), this, SLOT(changeGreen(double)));
    connect(ui->faceBlueSpinBox, SIGNAL(valueChanged(double)), this, SLOT(changeBlue(double)));
    connect(ui->vertPosXSpinBox, SIGNAL(valueChanged(double)), this, SLOT(changeX(double)));
    connect(ui->vertPosYSpinBox, SIGNAL(valueChanged(double)), this, SLOT(changeY(double)));
    connect(ui->vertPosZSpinBox, SIGNAL(valueChanged(double)), this, SLOT(changeZ(double)));
    connect(ui->mygl, SIGNAL(nKey(QListWidgetItem*)), this, SLOT(keyEdge(QListWidgetItem*)));
    connect(ui->mygl, SIGNAL(mKey(QListWidgetItem*)), this, SLOT(keyEdge(QListWidgetItem*)));
    connect(ui->mygl, SIGNAL(fKey(QListWidgetItem*)), this, SLOT(keyFace(QListWidgetItem*)));
    connect(ui->mygl, SIGNAL(vKey(QListWidgetItem*)), this, SLOT(keyVert(QListWidgetItem*)));
    connect(ui->mygl, SIGNAL(hsKey(QListWidgetItem*)), this, SLOT(keyEdge(QListWidgetItem*)));
    connect(ui->mygl, SIGNAL(hKey(QListWidgetItem*)), this, SLOT(keyEdge(QListWidgetItem*)));
    connect(ui->addMidpoint, SIGNAL(clicked(bool)), this, SLOT(addMid()));
    connect(ui->triangulate, SIGNAL(clicked(bool)), this, SLOT(triangulate()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionQuit_triggered()
{
    QApplication::exit();
}

void MainWindow::on_actionCamera_Controls_triggered()
{
    CameraControlsHelp* c = new CameraControlsHelp();
    c->show();
}

void MainWindow::addFace(QListWidgetItem *f) {
    ui->facesListWidget->addItem(f);
}

void MainWindow::addEdge(QListWidgetItem *he) {
    ui->halfEdgesListWidget->addItem(he);
}

void MainWindow::addVtx(QListWidgetItem *v) {
    ui->vertsListWidget->addItem(v);
}

void MainWindow::itemDisplay(QListWidgetItem *item) {
    if (Vertex *v = dynamic_cast<Vertex*>(item)) {
        ui->vertsListWidget->setCurrentItem(item);
        ui->mygl->m_vtxDisplay.updateVertex(v);
        ui->mygl->m_vtxDisplay.create();
    }
    else if (HalfEdge *edge = dynamic_cast<HalfEdge*>(item)) {
        ui->halfEdgesListWidget->setCurrentItem(item);
        ui->mygl->m_edgeDisplay.updateEdge(edge);
        ui->mygl->m_edgeDisplay.create();
    }
    else if (Face *face = dynamic_cast<Face*>(item)) {
        ui->facesListWidget->setCurrentItem(item);
        ui->mygl->m_faceDisplay.updateFace(face);
        ui->mygl->m_faceDisplay.create();
    }
    ui->mygl->update();
}

void MainWindow::changeRed(double d) {
    if (ui->mygl->m_faceDisplay.repFace) {
        ui->mygl->m_faceDisplay.repFace->rgb.r = d;
        ui->mygl->mesh.create();
        ui->mygl->m_faceDisplay.create();
        ui->mygl->update();
    }
}

void MainWindow::changeGreen(double d) {
    if (ui->mygl->m_faceDisplay.repFace) {
        ui->mygl->m_faceDisplay.repFace->rgb.g = d;
        ui->mygl->mesh.create();
        ui->mygl->m_faceDisplay.create();
        ui->mygl->update();
    }
}

void MainWindow::changeBlue(double d) {
    if (ui->mygl->m_faceDisplay.repFace) {
        ui->mygl->m_faceDisplay.repFace->rgb.b = d;
        ui->mygl->mesh.create();
        ui->mygl->m_faceDisplay.create();
        ui->mygl->update();
    }
}

void MainWindow::changeX(double d) {
    if (ui->mygl->m_vtxDisplay.repVtx) {
        ui->mygl->m_vtxDisplay.repVtx->pos.x = d;
        ui->mygl->mesh.create();
        ui->mygl->m_vtxDisplay.create();
        ui->mygl->update();
    }
}

void MainWindow::changeY(double d) {
    if (ui->mygl->m_vtxDisplay.repVtx) {
        ui->mygl->m_vtxDisplay.repVtx->pos.y = d;
        ui->mygl->mesh.create();
        ui->mygl->m_vtxDisplay.create();
        ui->mygl->update();
    }
}

void MainWindow::changeZ(double d) {
    if (ui->mygl->m_vtxDisplay.repVtx) {
        ui->mygl->m_vtxDisplay.repVtx->pos.z = d;
        ui->mygl->mesh.create();
        ui->mygl->m_vtxDisplay.create();
        ui->mygl->update();
    }
}

void MainWindow::keyEdge(QListWidgetItem *item) {
    ui->halfEdgesListWidget->setCurrentItem(item);
    if (HalfEdge *edge = dynamic_cast<HalfEdge*>(item)) {
        ui->mygl->m_edgeDisplay.updateEdge(edge);
        ui->mygl->m_edgeDisplay.create();
        ui->mygl->update();
    }
}

void MainWindow::keyFace(QListWidgetItem *item) {
    ui->facesListWidget->setCurrentItem(item);
    if (Face *face = dynamic_cast<Face*>(item)) {
        ui->mygl->m_faceDisplay.updateFace(face);
        ui->mygl->m_faceDisplay.create();
        ui->mygl->update();
    }
}

void MainWindow::keyVert(QListWidgetItem *item) {
    ui->vertsListWidget->setCurrentItem(item);
    if (Vertex *v = dynamic_cast<Vertex*>(item)) {
        ui->mygl->m_vtxDisplay.updateVertex(v);
        ui->mygl->m_vtxDisplay.create();
        ui->mygl->update();
    }
}

void MainWindow::addMid() {
    if (HalfEdge *e = ui->mygl->m_edgeDisplay.repEdge) {
        HalfEdge* symEdge = e->sym;

        //new edges
        uPtr<HalfEdge> newHalfEdge = mkU<HalfEdge>(e->next, symEdge, e->face, e->vtx);
        uPtr<HalfEdge> newSym = mkU<HalfEdge>(symEdge->next, e, symEdge->face, symEdge->vtx);

        //midpoint vertex
        glm::vec3 newPos = (e->vtx->pos + e->sym->vtx->pos) / 2.f;
        uPtr<Vertex> newVtx = mkU<Vertex>(e, newPos);

        //assign edge pointers
        e->next = newHalfEdge.get();
        e->vtx = newVtx.get();
        e->sym = newSym.get();
        newSym->sym = e;
        symEdge->next = newSym.get();
        symEdge->vtx = newVtx.get();
        symEdge->sym = newHalfEdge.get();
        newHalfEdge->sym = symEdge;

        //update
        ui->vertsListWidget->addItem(newVtx.get());
        ui->halfEdgesListWidget->addItem(newHalfEdge.get());
        ui->halfEdgesListWidget->addItem(newSym.get());
        ui->mygl->mesh.edges.push_back(std::move(newHalfEdge));
        ui->mygl->mesh.edges.push_back(std::move(newSym));
        ui->mygl->mesh.vertices.push_back(std::move(newVtx));
        ui->mygl->mesh.create();
        ui->mygl->m_edgeDisplay.create();
        ui->mygl->update();
    }
}

void MainWindow::triangulate() {
    if (Face *f = ui->mygl->m_faceDisplay.repFace) {
        HalfEdge *edge1 = f->edge;
        HalfEdge *edge2 = edge1->next;
        HalfEdge *edge3 = edge2->next;
        HalfEdge *edge4 = edge3->next;
        Vertex *v1 = edge1->vtx;
        Vertex *v2 = edge3->vtx;

        //make diagonal edges
        uPtr<HalfEdge> triEdge1 = mkU<HalfEdge>(f, v2);
        uPtr<HalfEdge> triEdge2 = mkU<HalfEdge>(f, v1);
        triEdge1->next = edge4;
        triEdge2->next = edge2;
        triEdge1->sym = triEdge2.get();
        triEdge2->sym = triEdge1.get();

        //update existing edges info
        edge1->next = triEdge1.get();
        edge3->next = triEdge2.get();

        ui->halfEdgesListWidget->addItem(triEdge1.get());
        ui->halfEdgesListWidget->addItem(triEdge2.get());
        ui->mygl->mesh.edges.push_back(std::move(triEdge1));
        ui->mygl->mesh.edges.push_back(std::move(triEdge2));
        ui->mygl->mesh.create();
        ui->mygl->m_faceDisplay.create();
        ui->mygl->update();
    }
}
