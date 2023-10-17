#include "track.hpp"

track::Track::Track(simulation::Position origin){	
    int N_points = 1000;
    float step = 0.0005;
    
    for(int i = 0; i < N_points-1; i++){
        vertices.push_back(simulation::Position(origin.x+i*step, origin.y, 0.9));	    
    }	
}


std::vector<simulation::Position> track::Track::get_vertices(){
    return vertices;
}


