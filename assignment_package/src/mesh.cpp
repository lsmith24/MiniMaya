#include "mesh.h"
#include <glm/glm.hpp>
#include "smartpointerhelp.h"
#include "iostream"

Mesh::Mesh(OpenGLContext* context)
    : Drawable(context), edges(std::vector<uPtr<HalfEdge>>()), faces(std::vector<uPtr<Face>>()),
      vertices(std::vector<uPtr<Vertex>>())
{}

GLenum Mesh::drawMode() {
    return GL_TRIANGLES;
}

void Mesh::create() {
    unsigned int minIndx = 0;
    std::vector<GLuint> indices;
    std::vector<glm::vec4> pos;
    std::vector<glm::vec4> nor;
    std::vector<glm::vec4> col;
    std::vector<int> jointID;
    std::vector<float> weights;

    for(uPtr<Face> &f : faces) {
        int numVtx = 0;
        HalfEdge* he = f->edge;
        do  {
            numVtx++;
            pos.push_back(glm::vec4(he->vtx->pos, 1));
            col.push_back(glm::vec4(f->rgb, 1));
            Vertex* cur = he->vtx;
            Vertex* prev = he->sym->vtx;
            Vertex* next = he->next->vtx;
            glm::vec3 normal =glm::vec3(glm::cross((cur->pos - prev->pos),
                                                  (next->pos - cur->pos)));
            nor.push_back(glm::vec4(normal, 1));
            for (const auto &pair : cur->influence) {
                jointID.push_back(pair.first);
                weights.push_back(pair.second);
            }
            he = he->next;

        } while (he != f->edge);

        for (int i = 0; i < numVtx - 2; i++) {
            indices.push_back(minIndx);
            indices.push_back(minIndx + i + 1);
            indices.push_back(minIndx + i + 2);
        }
        minIndx += numVtx;
    }

        count = indices.size();
        generateIdx();
        mp_context->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufIdx);
        mp_context->glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

        generatePos();
        mp_context->glBindBuffer(GL_ARRAY_BUFFER, bufPos);
        mp_context->glBufferData(GL_ARRAY_BUFFER, pos.size() * sizeof(glm::vec4), pos.data(), GL_STATIC_DRAW);

        generateNor();
        mp_context->glBindBuffer(GL_ARRAY_BUFFER, bufNor);
        mp_context->glBufferData(GL_ARRAY_BUFFER, nor.size() * sizeof(glm::vec4), nor.data(), GL_STATIC_DRAW);

        generateCol();
        mp_context->glBindBuffer(GL_ARRAY_BUFFER, bufCol);
        mp_context->glBufferData(GL_ARRAY_BUFFER, col.size() * sizeof(glm::vec4), col.data(), GL_STATIC_DRAW);

        generateID();
        mp_context->glBindBuffer(GL_ARRAY_BUFFER, bufID);
        mp_context->glBufferData(GL_ARRAY_BUFFER, jointID.size() * sizeof(glm::ivec2), jointID.data(), GL_STATIC_DRAW);

        generateWeight();
        mp_context->glBindBuffer(GL_ARRAY_BUFFER, bufWeight);
        mp_context->glBufferData(GL_ARRAY_BUFFER, weights.size() * sizeof(glm::ivec2), weights.data(), GL_STATIC_DRAW);
}

void Mesh::makeCube() {
    //vertices, no edge pointers yet
    uPtr<Vertex> v0 = mkU<Vertex>(glm::vec3(0.5, -0.5, 0.5));
    uPtr<Vertex> v1 = mkU<Vertex>(glm::vec3(0.5, 0.5, 0.5));
    uPtr<Vertex> v2 = mkU<Vertex>(glm::vec3(-0.5, 0.5, 0.5));
    uPtr<Vertex> v3 = mkU<Vertex>(glm::vec3(-0.5, -0.5, 0.5));
    uPtr<Vertex> v4 = mkU<Vertex>(glm::vec3(0.5, -0.5, -0.5));
    uPtr<Vertex> v5 = mkU<Vertex>(glm::vec3(0.5, 0.5, -0.5));
    uPtr<Vertex> v6 = mkU<Vertex>(glm::vec3(-0.5, 0.5, -0.5));
    uPtr<Vertex> v7 = mkU<Vertex>(glm::vec3(-0.5, -0.5, -0.5));

    //faces, no edge pointers yet
    uPtr<Face> front = mkU<Face>(glm::vec3(1,0,0));
    uPtr<Face> back = mkU<Face>(glm::vec3(0,1,0));
    uPtr<Face> left = mkU<Face>(glm::vec3(0,0,1));
    uPtr<Face> right = mkU<Face>(glm::vec3(1,0,1));
    uPtr<Face> top = mkU<Face>(glm::vec3(1,1,0));
    uPtr<Face> bottom = mkU<Face>(glm::vec3(0,1,1));

    //half edges
    //front
    uPtr<HalfEdge> Fr01 = mkU<HalfEdge>(front.get(), v1.get());
    uPtr<HalfEdge> Fr12 = mkU<HalfEdge>(front.get(), v2.get());
    uPtr<HalfEdge> Fr23 = mkU<HalfEdge>(front.get(), v3.get());
    uPtr<HalfEdge> Fr30 = mkU<HalfEdge>(front.get(), v0.get());
    Fr01->setNext(Fr12.get());
    Fr12->setNext(Fr23.get());
    Fr23->setNext(Fr30.get());
    Fr30->setNext(Fr01.get());

    //back
    uPtr<HalfEdge> Bk76 = mkU<HalfEdge>(back.get(), v6.get());
    uPtr<HalfEdge> Bk65 = mkU<HalfEdge>(back.get(), v5.get());
    uPtr<HalfEdge> Bk54 = mkU<HalfEdge>(back.get(), v4.get());
    uPtr<HalfEdge> Bk47 = mkU<HalfEdge>(back.get(), v7.get());
    Bk76->setNext(Bk65.get());
    Bk65->setNext(Bk54.get());
    Bk54->setNext(Bk47.get());
    Bk47->setNext(Bk76.get());

    //left
    uPtr<HalfEdge> Lt32 = mkU<HalfEdge>(left.get(), v2.get());
    uPtr<HalfEdge> Lt26 = mkU<HalfEdge>(left.get(), v6.get());
    uPtr<HalfEdge> Lt67 = mkU<HalfEdge>(left.get(), v7.get());
    uPtr<HalfEdge> Lt73 = mkU<HalfEdge>(left.get(), v3.get());
    Lt32->setNext(Lt26.get());
    Lt26->setNext(Lt67.get());
    Lt67->setNext(Lt73.get());
    Lt73->setNext(Lt32.get());

    //right
    uPtr<HalfEdge> Rt45 = mkU<HalfEdge>(right.get(), v5.get());
    uPtr<HalfEdge> Rt51 = mkU<HalfEdge>(right.get(), v1.get());
    uPtr<HalfEdge> Rt10 = mkU<HalfEdge>(right.get(), v0.get());
    uPtr<HalfEdge> Rt04 = mkU<HalfEdge>(right.get(), v4.get());
    Rt45->setNext(Rt51.get());
    Rt51->setNext(Rt10.get());
    Rt10->setNext(Rt04.get());
    Rt04->setNext(Rt45.get());

    //top
    uPtr<HalfEdge> Tp15 = mkU<HalfEdge>(top.get(), v5.get());
    uPtr<HalfEdge> Tp56 = mkU<HalfEdge>(top.get(), v6.get());
    uPtr<HalfEdge> Tp62 = mkU<HalfEdge>(top.get(), v2.get());
    uPtr<HalfEdge> Tp21 = mkU<HalfEdge>(top.get(), v1.get());
    Tp15->setNext(Tp56.get());
    Tp56->setNext(Tp62.get());
    Tp62->setNext(Tp21.get());
    Tp21->setNext(Tp15.get());

    //bottom
    uPtr<HalfEdge> Bt40 = mkU<HalfEdge>(bottom.get(), v0.get());
    uPtr<HalfEdge> Bt03 = mkU<HalfEdge>(bottom.get(), v3.get());
    uPtr<HalfEdge> Bt37 = mkU<HalfEdge>(bottom.get(), v7.get());
    uPtr<HalfEdge> Bt74 = mkU<HalfEdge>(bottom.get(), v4.get());
    Bt40->setNext(Bt03.get());
    Bt03->setNext(Bt37.get());
    Bt37->setNext(Bt74.get());
    Bt74->setNext(Bt40.get());

    //assign sym edges
    Fr01->setSym(Rt10.get());
    Fr12->setSym(Tp21.get());
    Fr23->setSym(Lt32.get());
    Fr30->setSym(Bt03.get());
    Bk76->setSym(Lt67.get());
    Bk65->setSym(Tp56.get());
    Bk54->setSym(Rt45.get());
    Bk47->setSym(Bt74.get());
    Tp15->setSym(Rt51.get());
    Tp62->setSym(Lt26.get());
    Bt40->setSym(Rt04.get());
    Bt37->setSym(Lt73.get());

    //assign vertex edge pointers
    v0->setEdge(Fr30.get());
    v1->setEdge(Fr01.get());
    v2->setEdge(Fr12.get());
    v3->setEdge(Fr23.get());
    v4->setEdge(Bk54.get());
    v5->setEdge(Bk65.get());
    v6->setEdge(Bk76.get());
    v7->setEdge(Bk47.get());

    //assign face edge pointers
    front->setEdge(Fr30.get());
    back->setEdge(Bk47.get());
    left->setEdge(Lt73.get());
    right->setEdge(Rt04.get());
    top->setEdge(Tp21.get());
    bottom->setEdge(Bt74.get());

    //fill VBOs
    vertices.push_back(std::move(v0));
    vertices.push_back(std::move(v1));
    vertices.push_back(std::move(v2));
    vertices.push_back(std::move(v3));
    vertices.push_back(std::move(v4));
    vertices.push_back(std::move(v5));
    vertices.push_back(std::move(v6));
    vertices.push_back(std::move(v7));

    faces.push_back(std::move(front));
    faces.push_back(std::move(back));
    faces.push_back(std::move(left));
    faces.push_back(std::move(right));
    faces.push_back(std::move(top));
    faces.push_back(std::move(bottom));

    edges.push_back(std::move(Fr01));
    edges.push_back(std::move(Fr12));
    edges.push_back(std::move(Fr23));
    edges.push_back(std::move(Fr30));

    edges.push_back(std::move(Bk76));
    edges.push_back(std::move(Bk65));
    edges.push_back(std::move(Bk54));
    edges.push_back(std::move(Bk47));

    edges.push_back(std::move(Lt32));
    edges.push_back(std::move(Lt26));
    edges.push_back(std::move(Lt67));
    edges.push_back(std::move(Lt73));

    edges.push_back(std::move(Rt45));
    edges.push_back(std::move(Rt51));
    edges.push_back(std::move(Rt10));
    edges.push_back(std::move(Rt04));

    edges.push_back(std::move(Tp15));
    edges.push_back(std::move(Tp56));
    edges.push_back(std::move(Tp62));
    edges.push_back(std::move(Tp21));

    edges.push_back(std::move(Bt40));
    edges.push_back(std::move(Bt03));
    edges.push_back(std::move(Bt37));
    edges.push_back(std::move(Bt74));
}
