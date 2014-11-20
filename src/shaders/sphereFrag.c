// minimal fragment shader
// www.lighthouse3d.com

#version 120

varying vec3 lightDir;

void main()
{
    if(gl_FragCoord.z < 0.1 || gl_FragCoord.z > 10) discard;
    float dist = distance(gl_PointCoord, vec2(0.5, 0.5));
    if(dist > 0.5) discard;
    vec3 normal = vec3(gl_PointCoord.x-0.5, gl_PointCoord.y-0.5, sqrt(1.0 - gl_PointCoord.x * gl_PointCoord.x - gl_PointCoord.y * gl_PointCoord.y));
    float dotProduct = dot(normal, vec3(gl_LightSource[0].position));
    if(dotProduct < 0)
        gl_FragColor = -1 * dot(normal, vec3(gl_LightSource[0].position)) * gl_Color;
    else
        gl_FragColor = 0.0 * gl_Color;
        
    //gl_FragColor = gl_Color * (1-dist);
}
