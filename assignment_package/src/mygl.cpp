#include "mygl.h"
#include <la.h>

#include <iostream>
#include <QApplication>
#include <QKeyEvent>


MyGL::MyGL(QWidget *parent)
    : OpenGLContext(parent),
      m_geomSquare(this),
      m_progLambert(this), m_progFlat(this), m_progSkeleton(this),
      m_glCamera(), mesh(this), m_edgeDisplay(this),
      m_vtxDisplay(this), m_faceDisplay(this), skeleton(std::vector<uPtr<Joint>>()),
      curJoint(nullptr), m_jointDisplay(this), alreadySkinned(false)
{
    setFocusPolicy(Qt::StrongFocus);
}

MyGL::~MyGL()
{
    makeCurrent();
    glDeleteVertexArrays(1, &vao);
    m_geomSquare.destroy();
    mesh.destroy();
    m_vtxDisplay.destroy();
    m_edgeDisplay.destroy();
    m_faceDisplay.destroy();
    m_jointDisplay.destroy();
    for (uPtr<Joint> &j : skeleton) {
        j->destroy();
    }
}

void MyGL::initializeGL()
{
    // Create an OpenGL context using Qt's QOpenGLFunctions_3_2_Core class
    // If you were programming in a non-Qt context you might use GLEW (GL Extension Wrangler)instead
    initializeOpenGLFunctions();
    // Print out some information about the current OpenGL context
    debugContextVersion();

    // Set a few settings/modes in OpenGL rendering
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    // Set the size with which points should be rendered
    glPointSize(5);
    // Set the color with which the screen is filled at the start of each render call.
    glClearColor(0.5, 0.5, 0.5, 1);

    printGLErrorLog();

    // Create a Vertex Attribute Object
    glGenVertexArrays(1, &vao);

    //Create the instances of Cylinder and Sphere.
 //   m_geomSquare.create();

    // Create and set up the diffuse shader
    m_progLambert.create(":/glsl/lambert.vert.glsl", ":/glsl/lambert.frag.glsl");
    // Create and set up the flat lighting shader
    m_progFlat.create(":/glsl/flat.vert.glsl", ":/glsl/flat.frag.glsl");

    m_progSkeleton.create(":/glsl/skeleton.vert.glsl", ":/glsl/skeleton.frag.glsl");

    mesh.makeCube();
    mesh.create();

    for(uPtr<Face> &f : mesh.faces) {
        emit sendFace(f.get());
    }

    for(uPtr<HalfEdge> &he : mesh.edges) {
        emit sendEdge(he.get());
    }

    for(uPtr<Vertex> &v : mesh.vertices) {
        emit sendVtx(v.get());
    }

    // We have to have a VAO bound in OpenGL 3.2 Core. But if we're not
    // using multiple VAOs, we can just bind one once.
    glBindVertexArray(vao);
}

void MyGL::resizeGL(int w, int h)
{
    //This code sets the concatenated view and perspective projection matrices used for
    //our scene's camera view.
    m_glCamera = Camera(w, h);
    glm::mat4 viewproj = m_glCamera.getViewProj();

    // Upload the view-projection matrix to our shaders (i.e. onto the graphics card)

    m_progLambert.setViewProjMatrix(viewproj);
    m_progFlat.setViewProjMatrix(viewproj);
    m_progSkeleton.setViewProjMatrix(viewproj);

    printGLErrorLog();
}

//This function is called by Qt any time your GL window is supposed to update
//For example, when the function update() is called, paintGL is called implicitly.
void MyGL::paintGL()
{
    // Clear the screen so that we only see newly drawn images
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_progFlat.setViewProjMatrix(m_glCamera.getViewProj());
    m_progLambert.setViewProjMatrix(m_glCamera.getViewProj());
    m_progSkeleton.setViewProjMatrix(m_glCamera.getViewProj());
    m_progLambert.setCamPos(m_glCamera.eye);
    m_progSkeleton.setCamPos(m_glCamera.eye);
    m_progFlat.setModelMatrix(glm::mat4(1.f));
    m_progLambert.setModelMatrix(glm::mat4(1.f));
    m_progSkeleton.setModelMatrix(glm::mat4(1.f));

    if (alreadySkinned) {
        m_progSkeleton.draw(mesh);
    }
    else {
        m_progFlat.draw(mesh);
    }

    if(m_vtxDisplay.repVtx != nullptr) {
        glDisable(GL_DEPTH_TEST);
        m_progFlat.draw(m_vtxDisplay);
        glEnable(GL_DEPTH_TEST);
    }

    if(m_edgeDisplay.repEdge != nullptr) {
        glDisable(GL_DEPTH_TEST);
        m_progFlat.draw(m_edgeDisplay);
        glEnable(GL_DEPTH_TEST);
    }

    if(m_faceDisplay.repFace != nullptr) {
        glDisable(GL_DEPTH_TEST);
        m_progFlat.draw(m_faceDisplay);
        glEnable(GL_DEPTH_TEST);
    }

    if (skeleton.size() > 0) {
        glDisable(GL_DEPTH_TEST);
        for (uPtr<Joint> &j : skeleton) {
            Joint *cur = j.get();
            m_progFlat.draw(*cur);
        }
        glEnable(GL_DEPTH_TEST);
    }

    if (m_jointDisplay.repJoint != nullptr) {
        glDisable(GL_DEPTH_TEST);
        m_progFlat.draw(m_jointDisplay);
        glEnable(GL_DEPTH_TEST);
    }
}


void MyGL::keyPressEvent(QKeyEvent *e)
{
    float amount = 2.0f;
    if(e->modifiers() & Qt::ShiftModifier){
        amount = 10.0f;
    }
    // http://doc.qt.io/qt-5/qt.html#Key-enum
    // This could all be much more efficient if a switch
    // statement were used
    if (e->key() == Qt::Key_Escape) {
        QApplication::quit();
    } else if (e->key() == Qt::Key_Right) {
        m_glCamera.RotatePhi(-amount);
    } else if (e->key() == Qt::Key_Left) {
        m_glCamera.RotatePhi(amount);
    } else if (e->key() == Qt::Key_Up) {
        m_glCamera.RotateTheta(-amount);
    } else if (e->key() == Qt::Key_Down) {
        m_glCamera.RotateTheta(amount);
    } else if (e->key() == Qt::Key_1) {
        m_glCamera.fovy += amount;
    } else if (e->key() == Qt::Key_2) {
        m_glCamera.fovy -= amount;
    } else if (e->key() == Qt::Key_W) {
        m_glCamera.Zoom(amount);
    } else if (e->key() == Qt::Key_S) {
        m_glCamera.Zoom(-amount);
    } else if (e->key() == Qt::Key_D) {
        m_glCamera.TranslateAlongRight(amount);
    } else if (e->key() == Qt::Key_A) {
        m_glCamera.TranslateAlongRight(-amount);
    } else if (e->key() == Qt::Key_Q) {
        m_glCamera.TranslateAlongUp(-amount);
    } else if (e->key() == Qt::Key_E) {
        m_glCamera.TranslateAlongUp(amount);
    } else if (e->key() == Qt::Key_R) {
        m_glCamera = Camera(this->width(), this->height());
    } else if (e->key() == Qt::Key_N) {
        if (m_edgeDisplay.repEdge != nullptr) {
            emit nKey(m_edgeDisplay.repEdge->next);
        }
    } else if (e->key() == Qt::Key_M) {
        if (m_edgeDisplay.repEdge != nullptr) {
            emit mKey(m_edgeDisplay.repEdge->sym);
        }
    } else if (e->key() == Qt::Key_F) {
        if (m_edgeDisplay.repEdge != nullptr) {
            emit fKey(m_edgeDisplay.repEdge->face);
        }
    } else if (e->key() == Qt::Key_V) {
        if (m_edgeDisplay.repEdge != nullptr) {
            emit vKey(m_edgeDisplay.repEdge->vtx);
        }
    } else if (e->key() == Qt::Key_H && e->modifiers() == Qt::ShiftModifier) {
        if (m_faceDisplay.repFace != nullptr) {
            emit hsKey(m_faceDisplay.repFace->edge);
        }
    } else if (e->key() == Qt::Key_H) {
        if (m_vtxDisplay.repVtx != nullptr) {
            emit hKey(m_vtxDisplay.repVtx->edge);
        }
    }
    m_glCamera.RecomputeAttributes();
    update();  // Calls paintGL, among other things
}

void MyGL::jointTransform() {
    m_progSkeleton.useMe();
    glm::mat4 transform[100];

    for (int i = 0; i < int(skeleton.size()); i++) {
        Joint *j = skeleton[i].get();
        transform[i] = j->getOverallTransformation();
    }

    if (m_progSkeleton.unifJointTransforms != -1) {
        this->glUniformMatrix4fv(m_progSkeleton.unifJointTransforms, skeleton.size(), GL_FALSE,
                                 &(transform[0][0][0]));
    }
}

void MyGL::setBind() {
    m_progSkeleton.useMe();
    glm::mat4 bindMats[100];

    for (int i = 0; i < int(skeleton.size()); i++) {
        Joint *j = skeleton[i].get();
        j->setBind();
        bindMats[i] = j->bindMatrix;
    }
    if (m_progSkeleton.unifBindMatrices != -1) {
        this->glUniformMatrix4fv(m_progSkeleton.unifBindMatrices, skeleton.size(), GL_FALSE,
                                 &(bindMats[0][0][0]));
    }
}
