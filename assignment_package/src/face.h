#pragma once
#include <glm/glm.hpp>
#include <QListWidgetItem>

class HalfEdge;
class Face : public QListWidgetItem {
public:
    HalfEdge* edge;
    glm::vec3 rgb;
    int id;

    static int faceLastID;

    Face();
    Face(glm::vec3 color);
    Face(HalfEdge *e, glm::vec3 color);
    void setEdge(HalfEdge* e);
};


