#version 300 es
precision mediump float;

in vec2 texturePos;
out vec4 densityVal;

uniform float time;
uniform float dT;
uniform sampler2D texture2D; //The density texture at the previous time step 
uniform sampler2D velocity2D; //The precomputed velocity

float random(vec2 st)
{
    return 2.0*fract(sin(dot(st.xy, vec2(12.9898,78.233))) * 43758.5453123)-1.0;
} 

// SDF for track bounding boxes
float sdfCuboid(vec3 p, vec3 minCorner, vec3 maxCorner)
{
    vec3 c = (minCorner + maxCorner) * 0.5;;
    vec3 s = (maxCorner - minCorner);
    float x = max
    (   p.x - c.x - s.x / 2.,
        c.x - p.x - s.x / 2.
    );
    float y = max
    (   p.y - c.y - s.y / 2.,
        c.y - p.y - s.y / 2.
    );
    
    float z = max
    (   p.z - c.z - s.z / 2.,
        c.z - p.z - s.z / 2.
    );
    float d = x;
    d = max(d,y);
    d = max(d,z);
    return d;
}

float N = 100.0; //Grid size in each direction

vec3 BoxCoords(vec2 texturecoords){
//Converts between texture coords and spatial coordinates in the box
    float floorFix = 0.000001;
    vec3 boxcoords = vec3(0.0);
    vec2 zFactors = vec2(0.0, 0.0);

    // Goes zero to one every 1/sqrt(N)
    boxcoords.x = mod(texturecoords.x, 1.0/sqrt(N)) * sqrt(N);
    boxcoords.y = mod(texturecoords.y, 1.0/sqrt(N)) * sqrt(N);

    // Bottom to top then left to right
    zFactors.x = floor(texturecoords.x * sqrt(N) + floorFix) * 1.0/sqrt(N);
    zFactors.y = floor(texturecoords.y * sqrt(N) + floorFix) * 1.0/N;

    boxcoords.z = zFactors.x + zFactors.y;
    return 2.0*boxcoords-1.0;
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

void main()
{
    vec3 boxpos = BoxCoords(texturePos); //Convert to position in box space
    // vec4 u = texture(texture2D, texturePos); //Calculate the velocity field at this point 
    vec4 u = vec4(0.0, 0.5, 0.0, 0.0);

    vec3 prev_box_pos = boxpos - u.xyz*dT*0.002; //Step back position using semi-lagrangian discretisation

    if (sdfCuboid(prev_box_pos, vec3(-1.0), vec3(1.0)) < 0.0) {
      vec2 prev_tex_pos = TexCoords(prev_box_pos); //Map the previous position to the density texture
      densityVal = texture(texture2D, prev_tex_pos)-0.001; //Get the density value at that point
    } else {
      densityVal = vec4(0.0, 0.0, 0.0, 1.0);
    }

}
