#version 300 es
precision mediump float;

in vec4 fragPos;
out vec4 FragColor;

float N = 20.0;
float tol = 0.001;

void main()
{
    float x_aligned = -1.0*floor(mod((1.0+fragPos.x)/2.0, 1.0/N)-tol); 
    float y_aligned = -1.0*floor(mod((1.0+fragPos.y)/2.0, 1.0/N)-tol); 
    float z_aligned = -1.0*floor(mod((1.0+fragPos.z)/2.0, 1.0/N)-tol); 
    //float col = float(x_aligned == 1.0 || y_aligned == 1.0|| z_aligned == 1.0);
    FragColor = vec4(x_aligned, y_aligned, z_aligned, 0.0);
}
