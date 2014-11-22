#version 410 core

// Interpolated values from the vertex shaders
in vec3 fragmentColor;

// Ouput data
out vec3 color;

//Light Position
uniform vec3 lightPos;

void main(){
    // Discard the fragment is its outside the radius
    vec2 fragPt = gl_PointCoord;
    if(length(fragPt - vec2(0.5)) > 0.5)
        discard;

    // Extracting the normal from the point position
    vec2 pointPos = gl_PointCoord;
    vec3 normal = vec3(pointPos.x-0.5, pointPos.y-0.5, 
                sqrt(1.0 - pointPos.x * pointPos.x - pointPos.y * pointPos.y));
    float dotProduct = dot(normal, lightPos);

    // Output color = color specified in the vertex shader, 
    if(dotProduct > 0)
        color = dotProduct * fragmentColor;
    else 
        color = vec3(0.0, 0.0, 0.0);
}
