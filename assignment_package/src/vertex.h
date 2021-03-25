#pragma once
#include <glm/glm.hpp>
#include <QListWidgetItem>
#include "joint.h"

class HalfEdge;
class Vertex : public QListWidgetItem {
public:
    glm::vec3 pos;
    HalfEdge* edge;
    int id;
    std::map<int, float> influence;

    static int vtxLastID;

    Vertex();
    Vertex(glm::vec3 pos);
    Vertex(HalfEdge *e, glm::vec3 pos);
    void setEdge(HalfEdge* e);
    void updateInfluence(Joint *j1, float dist1, Joint *j2, float dist2);
};


