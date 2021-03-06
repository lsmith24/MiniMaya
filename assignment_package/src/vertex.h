#pragma once
#include <glm/glm.hpp>
#include <QListWidgetItem>

class HalfEdge;
class Vertex : public QListWidgetItem {
public:
    glm::vec3 pos;
    HalfEdge* edge;
    int id;

    static int vtxLastID;

    Vertex();
    Vertex(glm::vec3 pos);
    Vertex(HalfEdge *e, glm::vec3 pos);
    void setEdge(HalfEdge* e);
};


