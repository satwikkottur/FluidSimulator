#version 410 core

// Interpolated values from the vertex shaders
in vec3 fragmentColor;

// Ouput data
out vec3 color;

void main(){
    // Discard the fragment is its outside the radius
    vec2 fragPt = gl_PointCoord;
    if(length(fragPt - vec2(0.5)) > 0.5)
        discard;

    // Output color = color specified in the vertex shader, 
    // interpolated between all 3 surrounding vertices
    color = fragmentColor;
}
