#version 300 es
layout(location = 0) in vec3 inPos;

out vec3 vPosition;

uniform float dT;
uniform sampler2D velocity2D; //The precomputed velocity
uniform int is_track_vert;

float N = 100.0;

float random(vec2 st)
{
    return 2.0*fract(sin(dot(st.xy, vec2(12.9898,78.233))) * 43758.5453123)-1.0;
} 

vec2 TexCoords(vec3 boxcoords){
//Converts between box coordinates and texture coordinates
    float floorFix = 0.000001;
    vec3 newboxcoords = (boxcoords+1.0)/2.0;
    vec2 texcoords = vec2(0.0);

    // Bottom to top then left to right
    texcoords.x = floor(newboxcoords.z * sqrt(N) + floorFix) * 1.0/sqrt(N) + newboxcoords.x * 1.0/sqrt(N);
    texcoords.y = mod(floor(newboxcoords.z * N + floorFix), sqrt(N)) * 1.0/sqrt(N) + newboxcoords.y * 1.0/sqrt(N);
    return texcoords;
}

void main(){

	vec2 texturePos = TexCoords(inPos);
	vec4 u = texture(velocity2D, texturePos); //Calculate the velocity field at this point 
	u += vec4(0.0, 0.0, 0.05, 0.0); //z drift velocity
	
	vPosition = inPos + u.xyz*dT;

    //Background bubbles respawn in the chamber
    if(is_track_vert==0){
        if(abs(vPosition.x)>1.0 || abs(vPosition.y)>1.0 || abs(vPosition.z)>1.0){
    	    float randx = random(vec2(3.1415, vPosition.z)); //Generate random x/y value
    	    float randy = random(vec2(6.6262, vPosition.x));
    	    vPosition = vec3(randx, randy, 0.0);
    	}
    }

    gl_Position = vec4(vPosition.x, vPosition.y, vPosition.z, 1.0);
    gl_PointSize = 10.0;
}
