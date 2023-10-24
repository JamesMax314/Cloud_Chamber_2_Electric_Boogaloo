#include "track.hpp"

track::Track::Track(simulation::Position origin){	
    int N_points = 2000;
    float length = 1.0;
    float thickness = 0.05;

    //Define two vectors orthogonal to the direction of the track 
    glm::vec3 d1 = gen_rand_direction(); 
    glm::vec3 d2 = gen_rand_direction();

    //Normalise d1
    d1 /= std::sqrt(glm::dot(d1, d1));

    //Make d2 orthonormal to d1
    d2 -= d1*glm::dot(d2, d1);
    d2 = d2/std::sqrt(glm::dot(d2, d2));

    glm::vec3 direction = glm::cross(d1, d2);
    
    for(int i = 0; i < N_points-1; i++){
	float r = thickness*uniform_dist(generator);
	float theta = 2*M_PI*uniform_dist(generator);
	float z = length*uniform_dist(generator);
	float a = r*std::cos(theta);
	float b = r*std::sin(theta);

	vertices.push_back(origin + a*d1 + b*d2 + z*direction);

    }	
}


std::vector<simulation::Position> track::Track::get_vertices(){
    return vertices;
}

glm::vec3 track::Track::gen_rand_direction(){
    float costheta = 2.0*uniform_dist(generator) - 1.0;
    float phi = 2*M_PI*uniform_dist(generator);
    
    float theta = std::acos(costheta);
    
    float x = std::sin(theta)*std::cos(phi);
    float y = std::sin(theta)*std::sin(phi);
    float z = costheta;

    return glm::vec3(x, y, z);

}
