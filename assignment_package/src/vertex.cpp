#include "vertex.h"
#include "halfedge.h"
#include <glm/glm.hpp>

Vertex::Vertex ()
    : pos(glm::vec3(0.f, 0.f, 0.f)), edge(nullptr), id(vtxLastID)
{
    vtxLastID++;
    setText(QString::number(id));
}

Vertex::Vertex(glm::vec3 pos) {
    this->pos = pos;
    this->edge = nullptr;
    id = vtxLastID;
    vtxLastID++;
    setText(QString::number(id));
}

Vertex::Vertex(HalfEdge *e, glm::vec3 pos) {
    this->edge = e;
    e->vtx = this;
    this->pos = pos;

    id = vtxLastID;
    vtxLastID++;
    setText(QString::number(id));
}

void Vertex::setEdge(HalfEdge *e) {
    this->edge = e;
    e->vtx = this;
}

int Vertex::vtxLastID = 0;
