// minimal vertex shader
// www.lighthouse3d.com

#version 410

void main()
{	
    vec4 eyePos = gl_ModelViewMatrix * gl_Vertex;
    float depth = eyePos.z / eyePos.w;
    if(depth > 200){
        gl_PointSize = 0.0;
    }
    else{
        vec4 projVoxel = gl_ProjectionMatrix * vec4(1.0, 1.0, eyePos.z, eyePos.w);
        vec2 projSize = 500.0 * projVoxel.xy / projVoxel.w;
        gl_PointSize =  0.05 * (projSize.x+projSize.y);
        gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
        gl_FrontColor = gl_Color;
    }
}
