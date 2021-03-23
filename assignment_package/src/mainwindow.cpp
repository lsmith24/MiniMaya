#include "mainwindow.h"
#include <ui_mainwindow.h>
#include "cameracontrolshelp.h"
#include <iostream>
#include <unordered_set>
#include <unordered_map>
#include <QSet>
#include <QFileDialog>
#include <math.h>

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
    connect(ui->subdivide, SIGNAL(clicked(bool)), this, SLOT(subdivide()));
    connect(ui->load, SIGNAL(pressed()), this, SLOT(loadOBJ()));
    connect(ui->json, SIGNAL(pressed()), this, SLOT(loadJSON()));
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

        uPtr<Face> newFace = mkU<Face>(f->rgb);
        //make diagonal edges
        uPtr<HalfEdge> triEdge1 = mkU<HalfEdge>(f, v2);
        uPtr<HalfEdge> triEdge2 = mkU<HalfEdge>(newFace.get(), v1);
        triEdge1->next = edge4;
        triEdge2->next = edge2;
        triEdge1->sym = triEdge2.get();
        triEdge2->sym = triEdge1.get();

        //update existing edges info
        edge1->next = triEdge1.get();
        edge3->next = triEdge2.get();
        edge2->face = newFace.get();
        edge3->face = newFace.get();
        newFace->edge = triEdge2.get();

        ui->facesListWidget->addItem(newFace.get());
        ui->halfEdgesListWidget->addItem(triEdge1.get());
        ui->halfEdgesListWidget->addItem(triEdge2.get());
        ui->mygl->mesh.edges.push_back(std::move(triEdge1));
        ui->mygl->mesh.edges.push_back(std::move(triEdge2));
        ui->mygl->mesh.faces.push_back(std::move(newFace));
        ui->mygl->mesh.create();
        ui->mygl->m_faceDisplay.create();
        ui->mygl->update();
    }
}


bool MainWindow::borderEdge(HalfEdge *e) {
    //border edge will not have same normal as its sym
    return (e->getNor() != e->sym->getNor());
}

void MainWindow::splitEdge(glm::vec3 newPos, HalfEdge *e) {
    HalfEdge* symEdge = e->sym;

    //new edges
    uPtr<HalfEdge> newHalfEdge = mkU<HalfEdge>(e->next, symEdge, e->face, e->vtx);
    uPtr<HalfEdge> newSym = mkU<HalfEdge>(symEdge->next, e, symEdge->face, symEdge->vtx);

    //midpoint vertex
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
    ui->mygl->mesh.edges.push_back(std::move(newHalfEdge));
    ui->mygl->mesh.edges.push_back(std::move(newSym));
    ui->mygl->mesh.vertices.push_back(std::move(newVtx));

}

void MainWindow::quadrangulate(Face *f, Vertex *centroid) {
    Vertex *startVtx = f->edge->sym->vtx;
    HalfEdge *e = f->edge;
    HalfEdge *firstSym = nullptr;
    HalfEdge *lastSym = nullptr;
    HalfEdge *nextEdge;
    int i = 0;

    do {
        i++;
        uPtr<Face> newFace = mkU<Face>(f->rgb);
        newFace->edge = e;
        uPtr<HalfEdge> toCentr = mkU<HalfEdge>();
        uPtr<HalfEdge> fromCentr = mkU<HalfEdge>();
        Vertex *nextVtx;
        nextEdge = e->next->next;
        nextVtx = e->next->vtx;

        e->face = newFace.get();
        e->next->face = newFace.get();
        e->next->next = toCentr.get();
        toCentr->next = fromCentr.get();
        toCentr->face = newFace.get();
        toCentr->vtx = centroid;
        fromCentr->face = newFace.get();
        fromCentr->next = e;
        fromCentr->vtx = startVtx;

        if (firstSym == nullptr) {
            firstSym = fromCentr.get();
        }
        if (lastSym != nullptr) {
            fromCentr->sym = lastSym;
            lastSym->sym = fromCentr.get();
        }

        lastSym = toCentr.get();
        startVtx = nextVtx;
        e = nextEdge;

        ui->mygl->mesh.faces.push_back(std::move(newFace));
        ui->mygl->mesh.edges.push_back(std::move(toCentr));
        ui->mygl->mesh.edges.push_back(std::move(fromCentr));

    } while (e->next->next != f->edge);

    //fix first/last iterations
    uPtr<HalfEdge> toCentr = mkU<HalfEdge>();
    uPtr<HalfEdge> fromCentr = mkU<HalfEdge>();

    f->edge = e;
    e->face = f;
    e->next->face = f;
    e->next->next = toCentr.get();

    toCentr->face = f;
    fromCentr->face = f;
    toCentr->next = fromCentr.get();
    fromCentr->next = e;
    toCentr->vtx = centroid;
    fromCentr->vtx = startVtx;
    std::cout << glm::to_string(startVtx->pos) << std::endl;

    toCentr->sym = firstSym;
    firstSym->sym = toCentr.get();
    fromCentr->sym = lastSym;
    lastSym->sym = fromCentr.get();

    centroid->edge = toCentr.get();

    ui->mygl->mesh.edges.push_back(std::move(toCentr));
    ui->mygl->mesh.edges.push_back(std::move(fromCentr));

}

void MainWindow::subdivide() {
    //need to keep original vertices
    std::vector<Vertex*> verts;
    for (int i = 0; i < int(ui->mygl->mesh.vertices.size()); i++) {
        verts.push_back(ui->mygl->mesh.vertices[i].get());
    }

    //find centroids
    std::unordered_map<Face*, Vertex*> centroids;

    for (int i = 0; i < int(ui->mygl->mesh.faces.size()); i++) {
        Face *f = ui->mygl->mesh.faces[i].get();
        HalfEdge *cur = f->edge;
        QSet<Vertex*> vertsOnFace;

        int num = 0;
        glm::vec3 sum = glm::vec3(0.f);

        do {
            sum += cur->vtx->pos;
            cur = cur->next;
            vertsOnFace.insert(cur->vtx);
            num++;
        } while (cur != f->edge);

        uPtr<Vertex> centroid = mkU<Vertex>(sum / float(num)); //compute average
        centroids.insert({f, centroid.get()});
        ui->mygl->mesh.vertices.push_back(std::move(centroid));

    }

    QSet<HalfEdge*> split;
    std::vector<Vertex*> midpoints = std::vector<Vertex*>();
    std::unordered_map<Vertex*, QSet<Vertex*>> vtxMids;

    int numEdges = int(ui->mygl->mesh.edges.size());
    for (int i = 0; i < numEdges; i++) {
        //get current edge, e
        HalfEdge *e = ui->mygl->mesh.edges[i].get();
        if (!split.contains(e) && !split.contains(e->sym)) {
            split.insert(e);
            split.insert(e->sym);

            //calculate midpoints
            Vertex *v1 = e->vtx;
            Vertex *v2 = e->sym->vtx;
            glm::vec3 f1 = centroids[e->face]->pos;
            glm::vec3 f2 = centroids[e->sym->face]->pos;
            glm::vec3 midPos = (v1->pos + v2->pos + f1 + f2) / 4.f;

            //split edge at calculated midpoint
            splitEdge(midPos, e);
        }
    }

    //smooth vertices
    for (int i = 0; i < int(verts.size()); i++) {
        Vertex *vtx = verts[i]; //current vtx
        HalfEdge *cur = vtx->edge;
        int n = 0; //num adjacent
        glm::vec3 v = vtx->pos; //original pos
        glm::vec3 e = glm::vec3(0.f);
        glm::vec3 f = glm::vec3(0.f);

        do {
            e += cur->sym->vtx->pos;
            f += centroids[cur->face]->pos;
            cur = cur->next->sym;
            n++;
        } while (cur != vtx->edge);

        v = v * (n - 2.f);
        v = v / float(n);
        e = e / float(std::pow(n, 2));
        f = f / float(std::pow(n, 2));

        vtx->pos = v + e + f;
    }

    int numFaces = int(ui->mygl->mesh.faces.size());
    //quadrangulate each face
    for (int i = 0; i < numFaces; i++) {
        Face *f = ui->mygl->mesh.faces[i].get();
        quadrangulate(f, centroids[f]);
    }

    //update list widget items
    for (uPtr<Face> &fc : ui->mygl->mesh.faces) {
        ui->facesListWidget->addItem(fc.get());
    }

    for (uPtr<Vertex> &vt : ui->mygl->mesh.vertices) {
        ui->vertsListWidget->addItem(vt.get());
    }

    for (uPtr<HalfEdge> &eg : ui->mygl->mesh.edges) {
        ui->halfEdgesListWidget->addItem(eg.get());
    }

    ui->mygl->mesh.destroy();
    ui->mygl->mesh.create();
    ui->mygl->m_vtxDisplay.destroy();
    ui->mygl->m_edgeDisplay.destroy();
    ui->mygl->m_faceDisplay.destroy();
    ui->mygl->update();
}

void MainWindow::loadOBJ() {

    std::map<std::pair<Vertex*, Vertex*>, HalfEdge*> sym_pairs =  std::map<std::pair<Vertex*, Vertex*>, HalfEdge*>();
    std::vector<std::pair<Vertex*, Vertex*>> pairs;

    //get file
    QString filename = QFileDialog::getOpenFileName(0, QString("load"), QString("../../"), tr("*.obj"));
    QFile file(filename);

    QList<Vertex*> v;
    QList<glm::vec2> vt;
    QList<glm::vec4> vn;

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {

        //clear qlistwidget
        ui->mygl->mesh.destroy();

        Vertex::vtxLastID = 0;
        HalfEdge::edgeLastID = 0;
        Face::faceLastID = 0;
        ui->mygl->mesh.edges = std::vector<uPtr<HalfEdge>>();
        ui->mygl->mesh.faces = std::vector<uPtr<Face>>();
        ui->mygl->mesh.vertices = std::vector<uPtr<Vertex>>();

        QTextStream in(&file);
        QString line = in.readLine();

        while(!line.isNull()){
            char name[3];
            float x, y, z;

            //vertex
            if(line.indexOf(QString("v")) == 0 && line.indexOf(QString(" ")) == 1) {
                QTextStream stream(&line);
                stream >> name[0] >> x >> y >> z;
                uPtr<Vertex> vert = mkU<Vertex>();
                vert->pos = glm::vec3(x,y,z);
                v.push_back(vert.get());
                ui->mygl->mesh.vertices.push_back(std::move(vert));
            }

            //vert texture
            if(line.indexOf(QString("v")) == 0 && line.indexOf(QString("t")) == 1) {
                QTextStream stream(&line);
                stream >> name[1] >> name[2] >> x >> y;
                vt.push_back(glm::vec2(x,y));
            }

            //vert normal
            if(line.indexOf(QString("v")) == 0 && line.indexOf(QString("n")) == 1) {
                QTextStream stream(&line);
                stream >> name[1] >> name[2] >> x >> y >> z;
                vn.push_back(glm::vec4(x, y, z, 1));
            }


            //face
            if(line.indexOf(QString("f")) == 0) {
                QTextStream stream(&line);
                QString prefix;
                stream >> prefix;
                std::vector<QString> corners;
                while (!stream.atEnd()) {
                    QString curr;
                    stream >> curr;
                    corners.push_back(curr);
                }
                //face loop
                HalfEdge *prev = nullptr;
                uPtr<Face> face = mkU<Face>();
                std::vector<HalfEdge*> face_HE;
                bool gotFirst = false;
                Vertex *first = nullptr;
                for(QString corner : corners) {
                    QStringList list = corner.split("/", QString::SkipEmptyParts);
                    Vertex* vrt = ui->mygl->mesh.vertices.at(list.at(0).toInt() - 1).get();

                    if (!gotFirst) {
                        first = vrt;
                        gotFirst = true;
                    }

                    //face color
                    float r1 = ((double) rand() / (RAND_MAX));
                    float r2 = ((double) rand() / (RAND_MAX));
                    float r3 = ((double) rand() / (RAND_MAX));
                    glm::vec3 color = glm::vec3(r1,r2,r3);
                    face->rgb = color;

                    uPtr<HalfEdge> eg = mkU<HalfEdge>(face.get(), vrt);
                    ui->mygl->mesh.edges.push_back(std::move(eg));
                    uPtr<HalfEdge> &new_HE = ui->mygl->mesh.edges.back();
                    vrt->edge = new_HE.get();
                    new_HE->vtx = vrt;
                    face->edge = new_HE.get();
                    face_HE.push_back(new_HE.get());

                    //sym pairs
                    if (prev != nullptr) {
                        Vertex *prevVert = prev->vtx;
                        std::pair<Vertex*, Vertex*> vert_pair1 = std::pair<Vertex*, Vertex*>(prevVert, new_HE->vtx);
                        sym_pairs[vert_pair1] = new_HE.get();
                        pairs.push_back(vert_pair1);
                    }
                    prev = new_HE.get();

                }
                std::pair<Vertex*, Vertex*> vert_pair1 = std::pair<Vertex*, Vertex*>(prev->vtx, first);
                sym_pairs[vert_pair1] = first->edge;
                pairs.push_back(vert_pair1);

                for(int i = 0; i < int(face_HE.size()); i++) {
                    HalfEdge* new1 = face_HE.at(i % face_HE.size());
                    HalfEdge* new2 = face_HE.at((i + 1) % face_HE.size());
                    new1->next = new2;
                }
                ui->mygl->mesh.faces.push_back(std::move(face));
            }
            line = in.readLine();
        }
    }
    //sym pointers
    for (auto p : pairs) {
        HalfEdge *e1 = sym_pairs[p];
        std::pair<Vertex*, Vertex*> flip = std::pair(p.second, p.first);
        HalfEdge *e2 = sym_pairs[flip];
        e1->sym = e2;
        e2->sym = e1;
    }

    //add items to list widget
    for (uPtr<Vertex> &v : ui->mygl->mesh.vertices) {
        ui->vertsListWidget->addItem(v.get());
    }

    for (uPtr<HalfEdge> &e : ui->mygl->mesh.edges) {
        ui->halfEdgesListWidget->addItem(e.get());
    }

    for (uPtr<Face> &f : ui->mygl->mesh.faces) {
        ui->facesListWidget->addItem(f.get());
    }

    ui->mygl->mesh.create();
    ui->mygl->update();
}

void MainWindow::loadJSON() {

}

