#pragma once
#include <QListWidgetItem>
#include <glm/glm.hpp>

class Face;
class Vertex;
class HalfEdge : public QListWidgetItem{
public:
    HalfEdge* next;
    HalfEdge* sym;
    Face* face;
    Vertex* vtx;
    int id;

    static int edgeLastID;

    HalfEdge();
    HalfEdge(Face* f, Vertex* v);
    HalfEdge(HalfEdge* n, HalfEdge* e, Face* f, Vertex* v);
    void setNext(HalfEdge* n);
    void setSym(HalfEdge *e);
    void setFace(Face* f);
    void setVtx(Vertex* v);
    glm::vec3 getNor();
};


