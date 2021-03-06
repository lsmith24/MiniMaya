#pragma once
#include "halfedge.h"
#include "face.h"
#include "vertex.h"
#include <glm/glm.hpp>

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


int HalfEdge::edgeLastID = 0;
