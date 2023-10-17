#include "track.hpp"

track::Track::Track(simulation::Position origin){	
    int N_points = 1000;
    float length = 0.5;
    float step = length/N_points;
    float thickness = 0.01;
    
    for(int i = 0; i < N_points-1; i++){
	float r = 2.0*distribution(generator) - 1.0;
        vertices.push_back(simulation::Position(origin.x+i*step, origin.y+thickness*r, 0.9));	    
    }	
}


std::vector<simulation::Position> track::Track::get_vertices(){
    return vertices;
}


