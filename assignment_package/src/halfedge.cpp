#pragma once
#include "halfedge.h"
#include "face.h"
#include "vertex.h"
#include <glm/glm.hpp>
#include "smartpointerhelp.h"

HalfEdge::HalfEdge()
    : next(nullptr), sym(nullptr), face(nullptr), id(edgeLastID)
{
    edgeLastID++;
    setText(QString::number(id));
}

HalfEdge::HalfEdge(Face* f, Vertex* v) {
    this->next = nullptr;
    this->sym = nullptr;

    this->face = f;
    f->edge = this;

    this->vtx = v;
    v->edge = this;

    id = edgeLastID;
    edgeLastID++;

    setText(QString::number(id));
}

HalfEdge::HalfEdge(HalfEdge *n, HalfEdge *e, Face *f, Vertex *v) {
    this->next = n;

    this->sym = e;
    e->sym = this;

    this->face = f;
    f->edge = this;

    this->vtx = v;
    v->edge = this;

    id = edgeLastID;
    edgeLastID++;

    setText(QString::number(id));
}


void HalfEdge::setSym(HalfEdge *e){
    this->sym = e;
    e->sym = this;
}

void HalfEdge::setFace(Face* f) {
    this->face = f;
    f->edge = this;
}

void HalfEdge::setVtx(Vertex* v) {
    this->vtx = v;
    v->edge = this;
}

void HalfEdge::setNext(HalfEdge *n) {
    this->next = n;
}

glm::vec3 HalfEdge::getNor() {
    Vertex* cur = this->vtx;
    Vertex* prev = this->sym->vtx;
    Vertex* next = this->next->vtx;
    glm::vec3 normal =glm::vec3(glm::cross((cur->pos - prev->pos),
                                          (next->pos - cur->pos)));
    return normal;
}

int HalfEdge::edgeLastID = 0;
