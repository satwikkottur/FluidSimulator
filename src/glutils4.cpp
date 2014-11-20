#include "glutils4.h"

OpenGL::OpenGL(){
}

void OpenGL::initializeOpenGL(GLFWwindow* window){
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
    
    // Setting the background color
    glClearColor(1.0f, 0.1f, 0.1f, 0.0f); // Light gray
}

void OpenGL::terminateOpenGL(){
    // Terminating the OpenGL session by clearing all the variables, windows and buffers
    glfwTerminate(); 
}

GLuint OpenGL::loadShaders(const char * vertex_file_path,const char * fragment_file_path){

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
		return 0;
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
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> ProgramErrorMessage(InfoLogLength+1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);
	
    return ProgramID;
}

void OpenGL::renderScene(GLFWwindow* window) {
    // Clearing the color and depth bits
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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

void OpenGL::debugOpenGL(int argc,char** argv, GLuint vertexBuffId){
    //Debugging variabes
    spacing = 0.12;
    vertexPos = new float[3 * debugNo * debugNo * debugNo];

    int count = 0;
    for(int i = 0; i < debugNo; i++){
        for(int j = 0; j < debugNo; j++){
            for(int k = 0; k < debugNo; k++){
                vertexPos[3 * count] = (float)i * spacing;
                vertexPos[3 * count + 1] = (float)j * spacing;
                vertexPos[3 * count + 2] = (float)k * spacing;
                count++;
            }
        }
    }
    
    //Initializing opengl
    //initializeOpenGL();

    //Initializing a vertex array and binding it to use it
    glGenBuffers(1, &vertexBuffId);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffId);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*3*debugNo*debugNo*debugNo, vertexPos, GL_DYNAMIC_DRAW);

    //Setting up the shaders
	//setShaders();
    //GLUT main loop
	glutMainLoop();
}

//*******************************************************************************************************************//
