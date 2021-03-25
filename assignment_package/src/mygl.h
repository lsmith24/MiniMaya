#ifndef MYGL_H
#define MYGL_H

#include <openglcontext.h>
#include <utils.h>
#include <shaderprogram.h>
#include <scene/squareplane.h>
#include "camera.h"
#include "mesh.h"
#include "edgedisplay.h"
#include "facedisplay.h"
#include "vtxdisplay.h"
#include "joint.h"
#include "jointdisplay.h"

#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>


class MyGL
    : public OpenGLContext
{
    Q_OBJECT
private:
    SquarePlane m_geomSquare;// The instance of a unit cylinder we can use to render any cylinder
    ShaderProgram m_progLambert;// A shader program that uses lambertian reflection
    ShaderProgram m_progFlat;// A shader program that uses "flat" reflection (no shadowing at all)
    ShaderProgram m_progSkeleton;

    GLuint vao; // A handle for our vertex array object. This will store the VBOs created in our geometry classes.
                // Don't worry too much about this. Just know it is necessary in order to render geometry.

    Camera m_glCamera;


public:
    explicit MyGL(QWidget *parent = nullptr);
    ~MyGL();

    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

    Mesh mesh;
    EdgeDisplay m_edgeDisplay;
    VtxDisplay m_vtxDisplay;
    FaceDisplay m_faceDisplay;
    std::vector<uPtr<Joint>> skeleton;
    Joint* curJoint;
    JointDisplay m_jointDisplay;
    bool alreadySkinned;

    void jointTransform();
    void setBind();


protected:
    void keyPressEvent(QKeyEvent *e);

signals:
    void sendFace(QListWidgetItem* f);
    void sendEdge(QListWidgetItem* he);
    void sendVtx(QListWidgetItem* v);
    void nKey(QListWidgetItem* item);
    void mKey(QListWidgetItem* item);
    void fKey(QListWidgetItem* item);
    void vKey(QListWidgetItem* item);
    void hsKey(QListWidgetItem* item);
    void hKey(QListWidgetItem* item);

};


#endif // MYGL_H
