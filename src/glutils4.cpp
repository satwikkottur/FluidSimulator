#define printOpenGLError() printOglError(__FILE__, __LINE__)
#include "glutils4.h"

OpenGL::OpenGL(){
}

void OpenGL::initOpenGL(){
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
	window = glfwCreateWindow( 1024, 768, "Fluid-Simulation", NULL, NULL);
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
}

int OpenGL::printOglError(char *file, int line){
    //
    // Returns 1 if an OpenGL error occurred, 0 otherwise.
    //
    GLenum glErr;
    int    retCode = 0;

    glErr = glGetError();
    while (glErr != GL_NO_ERROR)
    {
        printf("glError in file %s @ line %d: %s\n", file, line, gluErrorString(glErr));
        retCode = 1;
        glErr = glGetError();
    }
    return retCode;
}

void OpenGL::printShaderInfoLog(GLuint obj){
    int infologLength = 0;
    int charsWritten  = 0;
    char *infoLog;

	glGetShaderiv(obj, GL_INFO_LOG_LENGTH,&infologLength);

    if (infologLength > 0)
    {
        infoLog = (char *)malloc(infologLength);
        glGetShaderInfoLog(obj, infologLength, &charsWritten, infoLog);
		printf("%s\n",infoLog);
        free(infoLog);
    }
}

void OpenGL::printProgramInfoLog(GLuint obj){
    int infologLength = 0;
    int charsWritten  = 0;
    char *infoLog;

	glGetProgramiv(obj, GL_INFO_LOG_LENGTH,&infologLength);

    if (infologLength > 0)
    {
        infoLog = (char *)malloc(infologLength);
        glGetProgramInfoLog(obj, infologLength, &charsWritten, infoLog);
		printf("%s\n",infoLog);
        free(infoLog);
    }
}

void OpenGL::setShaders() {
	char *vs = NULL,*fs = NULL,*fs2 = NULL;

	vertShaderId = glCreateShader(GL_VERTEX_SHADER);
	fragShaderId = glCreateShader(GL_FRAGMENT_SHADER);

	vs = textFileRead("sphereVert.c");
    fs = textFileRead("sphereFrag.c");

	const char * vv = vs;
	const char * ff = fs;

	glShaderSource(vertShaderId, 1, &vv,NULL);
	glShaderSource(fragShaderId, 1, &ff,NULL);

	free(vs);free(fs);

	glCompileShader(vertShaderId);
	glCompileShader(fragShaderId);

	printShaderInfoLog(vertShaderId);
	printShaderInfoLog(fragShaderId);

	programId = glCreateProgram();
	glAttachShader(programId, vertShaderId);
	glAttachShader(programId, fragShaderId);

	glLinkProgram(programId);
	printProgramInfoLog(programId);

	glUseProgram(programId);
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

char* OpenGL::textFileRead(char *fn) {
	FILE *fp;
	char *content = NULL;

	int count=0;

	if (fn != NULL) {
		fp = fopen(fn,"rt");

		if (fp != NULL) {
      
      fseek(fp, 0, SEEK_END);
      count = ftell(fp);
      rewind(fp);

			if (count > 0) {
				content = (char *)malloc(sizeof(char) * (count+1));
				count = fread(content,sizeof(char),count,fp);
				content[count] = '\0';
			}
			fclose(fp);
		}
	}
	return content;
}

int OpenGL::textFileWrite(char *fn, char *s) {
	FILE *fp;
	int status = 0;

	if (fn != NULL) {
		fp = fopen(fn,"w");

		if (fp != NULL) {
			
			if (fwrite(s,sizeof(char),strlen(s),fp) == strlen(s))
				status = 1;
			fclose(fp);
		}
	}
	return(status);
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
    initOpenGL();

    //Initializing a vertex array and binding it to use it
    glGenBuffers(1, &vertexBuffId);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffId);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*3*debugNo*debugNo*debugNo, vertexPos, GL_DYNAMIC_DRAW);

    //Setting up the shaders
	setShaders();
    //GLUT main loop
	glutMainLoop();
}

//*******************************************************************************************************************//


