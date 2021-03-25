#include "vertex.h"
#include "halfedge.h"
#include <glm/glm.hpp>
#include "joint.h"

Vertex::Vertex ()
    : pos(glm::vec3(0.f, 0.f, 0.f)), edge(nullptr), id(vtxLastID), influence(std::map<int, float>())
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
    influence = std::map<int, float>();
}

Vertex::Vertex(HalfEdge *e, glm::vec3 pos) {
    this->edge = e;
    e->vtx = this;
    this->pos = pos;
    id = vtxLastID;
    vtxLastID++;
    setText(QString::number(id));
    influence = std::map<int, float>();  //map of joint IDs and their influence on the vertex <ID, influence>
}

void Vertex::setEdge(HalfEdge *e) {
    this->edge = e;
    e->vtx = this;
}

void Vertex::updateInfluence(Joint *j1, float dist1, Joint *j2, float dist2) {
    float sum = dist1 + dist2;
    float j1inf = dist1 / sum;
    float j2inf = dist2 / sum;
    influence.insert({j1->ID, j1inf});
    influence.insert({j2->ID, j2inf});
}

int Vertex::vtxLastID = 0;
