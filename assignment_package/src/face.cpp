#pragma once
#include "face.h"
#include "halfedge.h"
#include <glm/glm.hpp>

Face::Face()
    : edge(nullptr), rgb(glm::vec3(0.f, 0.f, 0.f)), id(faceLastID)
{
    faceLastID++;
    setText(QString::number(id));
}

Face::Face(glm::vec3 color) {
    this->rgb = color;
    this->edge = nullptr;
    id = faceLastID;
    faceLastID++;

    setText(QString::number(id));
}

Face::Face(HalfEdge *e, glm::vec3 color) {
    this->edge = e;
    e->face = this;
    this->rgb = color;

    id = faceLastID;
    faceLastID++;

    setText(QString::number(id));
}

void Face::setEdge(HalfEdge *e) {
    this->edge = e;
    e->face = this;
}

int Face::faceLastID = 0;
