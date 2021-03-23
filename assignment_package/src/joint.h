#pragma once
#include <glm/glm.hpp>
#include <QTreeWidgetItem>
#include <drawable.h>

class Joint : public QTreeWidgetItem, public Drawable {
public:
    QString name;
    Joint* parent;
    QSet<Joint*> children;
    glm::vec3 pos;
    glm::quat rotation;
    glm::mat4 bindMatrix;

    static int jointLastID;

    Joint(OpenGLContext *context, QString s, Joint* p, glm::vec3 pos, glm::quat q, glm::mat4 b);
    GLenum drawMode() override;
    void create() override;
    glm::mat4 getLocalTransformation();
    glm::mat4 getOverallTransformation();
};
