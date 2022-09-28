out vec4 FragColor;

in vec3 normal_out;
in vec2 UV;
in vec3 frag_pos;

#ifdef ma_texture_diffuse
uniform sampler2D texture_diffuse;
#endif
uniform vec3 diffuse_color;

#ifdef ma_shading

struct Light
{
    vec3 pos;
    vec3 color;
    float sila;
};

#define max_svetiel 4
uniform Light svetla[max_svetiel];
uniform int active_svetla;

uniform vec3 global_light_dir;
uniform vec3 global_light_color;
uniform bool global_light_status;

vec3 cal_light(vec3 color,Light svetlo,vec3 normal)
{
    vec3 ambient=0.6f * svetlo.color * color;

    vec3 light_smer=normalize(svetlo.pos- frag_pos);
    float diff = max(dot(normal,light_smer), 0.0);
    float distance=length(svetlo.pos-frag_pos);
    float power=svetlo.sila / distance;
    
    color=color*diff*svetlo.color;
    ambient*=power;
    color*=power;
    color+=ambient;

    return color;
}

vec3 cal_global_light(vec3 color,vec3 normal)
{
    vec3 ambient=0.6f * global_light_color * color;

    float diff = max(dot(normal, global_light_dir), 0.0);
    color=color*diff*global_light_color;
    color+=ambient;
    return color;
}

#endif

uniform vec3 cam_pos;

void main()
{
    vec3 color;
    #if defined(ma_texture_diffuse)
    {
        color=diffuse_color*vec3(texture(texture_diffuse,UV));
    }
    #else
        color=diffuse_color;
    #endif
    #if defined(ma_shading)
    {
        vec3 normal=normalize(normal_out);
        vec3 final_farba=vec3(0,0,0);
    
        if(global_light_status)
            final_farba+=cal_global_light(color,normal);
        for(int i=0;i<active_svetla;i++)
            final_farba+=cal_light(color,svetla[i],normal);
        FragColor=vec4(final_farba,1.0);
    }
    #else
        FragColor=vec4(color,1.0);

    #endif
}

