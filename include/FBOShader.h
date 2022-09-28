#ifndef FBOShader_H
#define FBOShader_H

#include "shader.h"

class FBOShader : public BaseShader
{
    public:
        FBOShader(string vertex_path,string fragment_path);
        void register_uniforms();

    protected:
        unsigned int texture_id;
};

#endif
