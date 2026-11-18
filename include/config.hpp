#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>


#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <vector>
#include <cmath>
#include <stdexcept>

#define WIDTH 800
#define HEIGHT 600

#include "vec2.hpp"
#include "vec3.hpp"
#include "mat4.hpp"
#include "shader.hpp" // uses Vec3
#include "texture.hpp"
#include "mesh.hpp" // uses Vec3