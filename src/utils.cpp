#include "utils.hpp"

std::vector<glm::vec3> utils::genRandomPoints(int number)
{
    std::random_device rd;
    std::mt19937 gen(rd()); // Mersenne Twister PRNG
    std::uniform_real_distribution<double> dis(-1.0, 1.0);

    std::vector<glm::vec3> points;
    for (int i=0; i<number; i++) {
        glm::vec3 p;
        p[0] = (float)dis(gen);
        p[1] = (float)dis(gen);
        p[2] = (float)dis(gen);
        points.emplace_back(p);
    }

    return points;
}

glm::vec3 utils::genRandomDirection(){
    std::random_device rd;
    std::mt19937 gen(rd()); // Mersenne Twister PRNG
    std::uniform_real_distribution<double> dis(-1.0, 1.0);

    float costheta = (float)dis(gen);
    float phi = 2*M_PI*(float)dis(gen);
    
    float theta = std::acos(costheta);
    
    float x = std::sin(theta)*std::cos(phi);
    float y = std::sin(theta)*std::sin(phi);
    float z = costheta;

    return glm::vec3(x, y, z);

}

