#include "track.hpp"

track::Track::Track(simulation::Position origin){	
    std::random_device rd;
    std::mt19937 gen(rd()); // Mersenne Twister PRNG
    std::uniform_real_distribution<double> dis(0.0, 1.0);

    int N_points = 500;
    float length = 0.5;
    float thickness = 0.05;

    //Define two vectors orthogonal to the direction of the track 
    glm::vec3 d1 = utils::genRandomDirection(); 
    glm::vec3 d2 = utils::genRandomDirection();

    //Normalise d1
    d1 /= std::sqrt(glm::dot(d1, d1));

    //Make d2 orthonormal to d1
    d2 -= d1*glm::dot(d2, d1);
    d2 = d2/std::sqrt(glm::dot(d2, d2));

    glm::vec3 direction = glm::cross(d1, d2);
    
    for(int i = 0; i < N_points; i++){
	float r = thickness*dis(gen);
	float theta = 2*M_PI*dis(gen);
	float z = length*dis(gen);
	float a = r*std::cos(theta);
	float b = r*std::sin(theta);

	vertices.push_back(origin + a*d1 + b*d2 + z*direction);

    }	
}


std::vector<simulation::Position> track::Track::get_vertices(){
    return vertices;
}

