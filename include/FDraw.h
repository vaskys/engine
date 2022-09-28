#ifndef FDRAW_H
#define FDRAW_H

#include "config.h"
#include "LinearMath/btIDebugDraw.h"
#include "shader.h"

class FDraw : public btIDebugDraw
{
    public:
        FDraw();
        ~FDraw();

        BaseShader*get_shader();

        void push_draw();


        void drawLine(const btVector3& from, const btVector3& to, const btVector3& color) override;
        void drawContactPoint(const btVector3 &, const btVector3 &, btScalar, int, const btVector3 &) override;
        void reportErrorWarning(const char *) override;
        void draw3dText(const btVector3 &, const char *) override;
        void setDebugMode(int p) override;
        int getDebugMode(void) const override;
    private:
        BaseShader *shader;

        unsigned int VBO;
        unsigned int VAO;

        vector<float> vertices;
};


#endif
