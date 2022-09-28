#include "FDraw.h"

FDraw::FDraw()
{
    shader=new BaseShader("shaders/base_vertex.glsl","shaders/base_fragment.glsl");    
    vertices.clear();

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 1000000, nullptr,GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

FDraw::~FDraw()
{
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);

    vertices.clear();
    delete shader;
}

BaseShader* FDraw::get_shader()
{
    return shader;
}

void FDraw::push_draw()
{
    if(vertices.size() > 0)
    {
        glBindBuffer(GL_ARRAY_BUFFER,VBO);
        glBufferSubData(GL_ARRAY_BUFFER,0,sizeof(float) * vertices.size(),&vertices[0]);
        glBindVertexArray(VAO);
        glDrawArrays(GL_LINES,0,vertices.size()/3);
        vertices.clear();
    }
}

void FDraw::drawLine(const btVector3& from, const btVector3& to, const btVector3& color) 
{
    vertices.push_back(from.x());
    vertices.push_back(from.y());
    vertices.push_back(from.z());
    vertices.push_back(to.x());
    vertices.push_back(to.y());
    vertices.push_back(to.z());
}

void FDraw::drawContactPoint(const btVector3&, const btVector3&, btScalar, int, const btVector3&) 
{}

void FDraw::reportErrorWarning(const char *)
{}

void FDraw::draw3dText(const btVector3 &, const char *)
{}

void FDraw::setDebugMode(int p)
{}

int FDraw::getDebugMode(void) const
{
    return 1;
}



