#ifndef CONFIG_H
#define CONFIG_H


#define GL_GLEXT_PROTOTYPES
#include <iostream>
#include <string.h>
#include <vector>
#include <iterator>

#include "SDL_timer.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <glm/gtx/string_cast.hpp>
#include "glm/gtx/euler_angles.hpp"
#include"glm/common.hpp"
#include "glm/gtx/matrix_decompose.hpp"
#include"glm/gtc/quaternion.hpp"
#include <glm/gtx/quaternion.hpp>

#include "SDL.h"
#include "SDL_opengl.h"
#include "SDL_events.h"
#include "SDL_image.h"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#ifdef WINDOWS
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif


using namespace std;

void debug_print(string msg);
void print_wd();

template<typename Base, typename T>
inline bool instanceof(const T*) {
   return is_base_of<Base, T>::value;
}


#define MAX_BONES_INFLUENCE 4
#define MAX_BONES 100

glm::mat4 AssimpToGLM(const aiMatrix4x4& from);
glm::vec3 AssimpVec3ToGLM(const aiVector3D &vec);
glm::quat AssimpQuatToGLM(const aiQuaternion &quat);
#endif
