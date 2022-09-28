layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;

#ifdef ma_kosti
layout(location = 3) in ivec4 bones_id; 
layout(location = 4) in vec4 weights;
const int MAX_BONES = 100;
const int MAX_BONE_INFLUENCE = 4;
uniform mat4 finalBonesMatrices[MAX_BONES];
#endif

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec3 normal_out;
out vec3 frag_pos;
out vec2 UV;

void main()
{
    #if defined (ma_kosti)
        vec4 totalPosition = vec4(0.0f);
        for(int i = 0 ; i < MAX_BONE_INFLUENCE ; i++)
        {
            if(bones_id[i] == -1)
                continue;
            if(bones_id[i] >=MAX_BONES) 
            {
                totalPosition = vec4(position,1.0f);
                break;
            }
            vec4 localPosition = finalBonesMatrices[bones_id[i]] * vec4(position,1.0f);
            totalPosition += localPosition * weights[i];
        }
        frag_pos=vec3(model*vec4(totalPosition));
    #else
        frag_pos=vec3(model*vec4(position,1.0));
    #endif

    normal_out=mat3(transpose(inverse(model))) * normal;
    UV=uv;

    gl_Position=projection * view *vec4(frag_pos,1.0);

}
