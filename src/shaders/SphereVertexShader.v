#version 410 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPos;

// Output data ; will be interpolated for each fragment.
out vec3 fragmentColor;
// Values that stay constant for the whole mesh.
uniform mat4 MVP;
uniform mat4 projection;
uniform mat4 modelView;
uniform vec2 frameSize;

void main(){	
	// Output position of the vertex, in clip space : MVP * position
    //gl_Position =  MVP * vec4(vertexPos, 1);
    vec4 eyePos = modelView * vec4(vertexPos, 1);

    // Discarding the pixel which is too close or far off
    float depth = eyePos.z / eyePos.w;
    if(depth > 200){
        gl_PointSize = 0.0;
    }
    else{
        vec4 projVoxel = projection * vec4(1.0, 1.0, eyePos.z, eyePos.w);
        vec2 projSize = frameSize * projVoxel.xy / projVoxel.w;
        gl_PointSize = 0.05 * (projSize.x + projSize.y);
    }

    gl_Position = projection * eyePos;
	fragmentColor = vec3(0.1, 0.1, 0.8);
}
