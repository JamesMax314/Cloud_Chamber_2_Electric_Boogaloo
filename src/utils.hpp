#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <memory>
#include <cmath>
#include <random>

namespace utils {
    std::vector<glm::vec3> genRandomPoints(int number);

    glm::vec3 genRandomDirection();
}
