#include "glutils4.h"

OpenGL::OpenGL(){
    scale = 2.0; // Scale that changes by moving
    scaleChange = 0.005;
    
    angle = 0; // Angle that changes with change in viewpoint
    angleChange = 0.01; // Angle change 

    // Light angle defaults
    lightAngleX = 0.0;
    lightAngleY = 0.0;
}

void OpenGL::initializeOpenGL(const char* vertexShaderPath, const char* fragmentShaderPath){
    // Initializing using OpenGL 4 with GLFW and GLEW, instead of GLUT
	// Initialise GLFW
	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		return;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow( winWidth, winHeight, "Fluid-Simulation", NULL, NULL);
	if( window == NULL ){
		fprintf( stderr, "Failed to open GLFW window.\nIf you have an Intel GPU, they are not 4.1 compatible.\n");
		glfwTerminate();
		return;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return;
	}
    
    // Setting methods for checking the inputs
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    // Setting the background color
    glClearColor(0.9f, 0.9f, 0.9f, 0.0f); // Light gray

	// Enable depth test
	glEnable(GL_DEPTH_TEST);

    // Point size enabling
    glEnable(GL_PROGRAM_POINT_SIZE);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS); 

    // Creating vertex objects
	glGenVertexArrays(1, &vertexArrayId);
	glBindVertexArray(vertexArrayId);
    
    loadShaders(vertexShaderPath, fragmentShaderPath);
    
	// Get a handle for our "MVP" uniform
	mvpMatrixId = glGetUniformLocation(programId, "MVP");

    // Get a handler for Projection matrix, modelview matrix
    projMatrixId = glGetUniformLocation(programId, "projection");
    modelMatrixId = glGetUniformLocation(programId, "modelView");
    frameSizeId = glGetUniformLocation(programId, "frameSize");

    // Handler for light direction
    lightPosId = glGetUniformLocation(programId, "lightPos");

    std::string debugConfig = "PARTICLE_CUBE";
    debugOpenGL(debugConfig);
}

void OpenGL::terminateOpenGL(){
    // Terminating the OpenGL session by clearing all the variables, windows and buffers
	// Cleanup VBO
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteVertexArrays(1, &vertexArrayId);
	glDeleteProgram(programId);

    glfwTerminate(); 
}

void OpenGL::loadShaders(const char * vertex_file_path,const char * fragment_file_path){

	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
	if(VertexShaderStream.is_open()){
		std::string Line = "";
		while(getline(VertexShaderStream, Line))
			VertexShaderCode += "\n" + Line;
		VertexShaderStream.close();
	}else{
		printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_file_path);
		getchar();
		return;
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
	if(FragmentShaderStream.is_open()){
		std::string Line = "";
		while(getline(FragmentShaderStream, Line))
			FragmentShaderCode += "\n" + Line;
		FragmentShaderStream.close();
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;

	// Compile Vertex Shader
	printf("Compiling shader : %s\n", vertex_file_path);
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> VertexShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}

	// Compile Fragment Shader
	printf("Compiling shader : %s\n", fragment_file_path);
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}


	// Link the program
	printf("Linking program\n");
    programId = glCreateProgram();
	glAttachShader(programId, VertexShaderID);
	glAttachShader(programId, FragmentShaderID);
	glLinkProgram(programId);

	// Check the program
	glGetProgramiv(programId, GL_LINK_STATUS, &Result);
	glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> ProgramErrorMessage(InfoLogLength+1);
		glGetProgramInfoLog(programId, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);
}

void OpenGL::renderScene(){
    
    // Storing the frame size
    frameSize = glm::vec2(winWidth, winHeight);

	// Projection matrix :
    // 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
    
    // Reading keyboard and updating the state of camera accordingly 
    registerUserInputs();

    // Getting a user transformation matrix from user-interface
    glm::mat4 cameraScaling = glm::scale(glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0, 1.0, 0.0)), glm::vec3(scale, scale, scale));

    glm::vec4 homoCamPos = cameraScaling * glm::vec4(1.0f);

	// Camera matrix
	glm::mat4 View = glm::lookAt(
                    homoCamPos.xyz()/ homoCamPos.w, // Camera is at (4,3,-3), in World Space
                    glm::vec3(0,0,0), // and looks at the origin
                    glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
                   );
    
    // Adjusting the position of the light
    glm::mat4 lightRotate = glm::rotate(glm::mat4(1.0f), lightAngleY, glm::vec3(0.0, 1.0, 0.0)); 
    lightRotate = glm::rotate(lightRotate, lightAngleX, glm::vec3(1.0, 0.0, 0.0)); 
    glm::vec4 homoLightPos = lightRotate * 
                        glm::vec4(lightPosX, lightPosY, lightPosZ, 1.0f);
    lightPos = glm::normalize(glm::vec3(homoLightPos[0], homoLightPos[1], homoLightPos[2]));

    //Debugging the angles for the best possible position for the light
    //printf("Light Angles : %f %f %f\n", lightPos[0], lightPos[1], lightPos[2]);

	// Model matrix : an identity matrix (model will be at the origin)
	glm::mat4 Model = glm::mat4(1.0f);

	// Our ModelViewProjection : multiplication of our 3 matrices
    modelView = View  * Model;
	MVP = projection * modelView; // Remember, matrix multiplication is the other way around
    
    // Clearing the color and depth bits
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Use our shader
    glUseProgram(programId);

    // Send our transformation to the currently bound shader, 
    // in the "MVP" uniform
    glUniformMatrix4fv(mvpMatrixId, 1, GL_FALSE, &MVP[0][0]);

    // Sending projective matrix and modelView matrix seperately along with screensize
    glUniformMatrix4fv(projMatrixId, 1, GL_FALSE, &projection[0][0]);
    glUniformMatrix4fv(modelMatrixId, 1, GL_FALSE, &modelView[0][0]);
    glUniform2f(frameSizeId, frameSize[0], frameSize[1]);
    glUniform3f(lightPosId, lightPos[0], lightPos[1], lightPos[2]);

    // 1rst attribute buffer : vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer(
        0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
        3,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        (void*)0            // array buffer offset
    );

    // Draw the triangle !
    // Drawing points
    glPointSize(20.0);
    glDrawArrays(GL_POINTS, 0, noDebugPts * noDebugPts * noDebugPts); // 12*3 indices starting at 0 -> 12 triangles
    //glDrawArrays(GL_TRIANGLES, 0, 12*3); // 12*3 indices starting at 0 -> 12 triangles

    glDisableVertexAttribArray(0);
        
    // Swap buffers
    glfwSwapBuffers(window);
    glfwPollEvents();
}

void  OpenGL::registerUserInputs(){
    // We assume the user would move in and out (translation)
    // and around the y axis (rotation)
    
    // Estimating the translation part
    // UP ARROW
    if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS){
        scale += scaleChange;
        return;
    }
    
    // DOWN ARROW
    if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS){
        scale -= scaleChange;
        return;
    }
    
    
    // RIGHT ARROW
    if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS){
        angle += angleChange;
        return;
    }
    
    // LEFT ARROW
    if(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS){
        angle -= angleChange;
        return;
    }

    // Changing the light direction using W,S,A,D

    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
        lightAngleX += angleChange;
        return;
    }

    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
        lightAngleX -= angleChange;
        return;
    }

    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
        lightAngleY -= angleChange;
        return;
    }

    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
        lightAngleY += angleChange;
        return;
    }

}

int OpenGL::windowDump(){
   int i,j;
   FILE *fptr;
   static int counter = 0; /* This supports animation sequences */
   char fname[32];
   unsigned char *image;
   int width = winWidth;
   int height = winHeight;
   bool stereo = false;

   /* Allocate our buffer for the image */
   if ((image = (unsigned char*)malloc(3*width*height*sizeof(char))) == NULL) {
      fprintf(stderr,"Failed to allocate memory for image\n");
      return(false);
   }

   glPixelStorei(GL_PACK_ALIGNMENT,1);

   /* Open the file */
   if (stereo)
      sprintf(fname,"L_%04d.raw",counter);
   else
      sprintf(fname,"images/C_%04d.raw",counter);
   if ((fptr = fopen(fname,"w")) == NULL) {
      fprintf(stderr,"Failed to open file for window dump\n");
      return(false);
   }

   /* Copy the image into our buffer */
   glReadBuffer(GL_BACK_LEFT);
   glReadPixels(0,0,width,height,GL_RGB,GL_UNSIGNED_BYTE,image);

   /* Write the raw file */
   /* fprintf(fptr,"P6\n%d %d\n255\n",width,height); for ppm */
   for (j=height-1;j>=0;j--) {
      for (i=0;i<width;i++) {
         fputc(image[3*j*width+3*i+0],fptr);
         fputc(image[3*j*width+3*i+1],fptr);
         fputc(image[3*j*width+3*i+2],fptr);
      }
   }
   fclose(fptr);

   if (stereo) {
      /* Open the file */
      sprintf(fname,"R_%04d.raw",counter);
      if ((fptr = fopen(fname,"w")) == NULL) {
         fprintf(stderr,"Failed to open file for window dump\n");
         return(false);
      }

      /* Copy the image into our buffer */
      glReadBuffer(GL_BACK_RIGHT);
      glReadPixels(0,0,width,height,GL_RGB,GL_UNSIGNED_BYTE,image);

      /* Write the raw file */
      /* fprintf(fptr,"P6\n%d %d\n255\n",width,height); for ppm */
      for (j=height-1;j>=0;j--) {
         for (i=0;i<width;i++) {
            fputc(image[3*j*width+3*i+0],fptr);
            fputc(image[3*j*width+3*i+1],fptr);
            fputc(image[3*j*width+3*i+2],fptr);
         }
      }
      fclose(fptr);
   }

   /* Clean up */
   counter++;
   free(image);
   return(true);
}

void OpenGL::debugOpenGL(std::string debugConfig){
    if(debugConfig.compare("SURFACE_CUBE") == 0){
        // Our vertices. Tree consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
        // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
        static const GLfloat vertexPos[] = { 
            -1.0f,-1.0f,-1.0f,
            -1.0f,-1.0f, 1.0f,
            -1.0f, 1.0f, 1.0f,
             1.0f, 1.0f,-1.0f,
            -1.0f,-1.0f,-1.0f,
            -1.0f, 1.0f,-1.0f,
             1.0f,-1.0f, 1.0f,
            -1.0f,-1.0f,-1.0f,
             1.0f,-1.0f,-1.0f,
             1.0f, 1.0f,-1.0f,
             1.0f,-1.0f,-1.0f,
            -1.0f,-1.0f,-1.0f,
            -1.0f,-1.0f,-1.0f,
            -1.0f, 1.0f, 1.0f,
            -1.0f, 1.0f,-1.0f,
             1.0f,-1.0f, 1.0f,
            -1.0f,-1.0f, 1.0f,
            -1.0f,-1.0f,-1.0f,
            -1.0f, 1.0f, 1.0f,
            -1.0f,-1.0f, 1.0f,
             1.0f,-1.0f, 1.0f,
             1.0f, 1.0f, 1.0f,
             1.0f,-1.0f,-1.0f,
             1.0f, 1.0f,-1.0f,
             1.0f,-1.0f,-1.0f,
             1.0f, 1.0f, 1.0f,
             1.0f,-1.0f, 1.0f,
             1.0f, 1.0f, 1.0f,
             1.0f, 1.0f,-1.0f,
            -1.0f, 1.0f,-1.0f,
             1.0f, 1.0f, 1.0f,
            -1.0f, 1.0f,-1.0f,
            -1.0f, 1.0f, 1.0f,
             1.0f, 1.0f, 1.0f,
            -1.0f, 1.0f, 1.0f,
             1.0f,-1.0f, 1.0f
        };

        //Initializing a vertex array and binding it to use it
        glGenBuffers(1, &vertexbuffer);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPos), vertexPos, GL_STATIC_DRAW);
        return;
    }

    if(debugConfig.compare("PARTICLE_CUBE") == 0){
        //Debugging variabes
        spacing = 0.10;
        GLfloat vertexPos[3 * noDebugPts * noDebugPts * noDebugPts];

        int count = 0;
        for(int i = 0; i < noDebugPts; i++){
            for(int j = 0; j < noDebugPts; j++){
                for(int k = 0; k < noDebugPts; k++){
                    vertexPos[3 * count] = (float)i * spacing;
                    vertexPos[3 * count + 1] = (float)j * spacing;
                    vertexPos[3 * count + 2] = (float)k * spacing;
                    count++;
                }
            }
        }
        //Initializing a vertex array and binding it to use it
        glGenBuffers(1, &vertexbuffer);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPos), vertexPos, GL_STATIC_DRAW);
    }
}

//*******************************************************************************************************************//
