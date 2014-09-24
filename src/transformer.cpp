#include "gl_framework.hpp"
#include <GL/glu.h>

void drawCubeWireframe(){
	glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);
	glBegin(GL_QUADS); 
    // glNormal3d(0, 0, -1);//Back
    glVertex3f(  0.5, -0.5, 0.5 );
    glVertex3f(  0.5,  0.5, 0.5 );
    glVertex3f( -0.5,  0.5, 0.5 );
    glVertex3f( -0.5, -0.5, 0.5 );
     
    // Purple side - RIGHT
    glVertex3f( 0.5, -0.5, -0.5 );
    glVertex3f( 0.5,  0.5, -0.5 );
    glVertex3f( 0.5,  0.5,  0.5 );
    glVertex3f( 0.5, -0.5,  0.5 );
    
    // Green side - LEFT
    glVertex3f( -0.5, -0.5,  0.5 );
    glVertex3f( -0.5,  0.5,  0.5 );
    glVertex3f( -0.5,  0.5, -0.5 );
    glVertex3f( -0.5, -0.5, -0.5 );
    
    // Blue side - TOP
    glVertex3f(  0.5,  0.5,  0.5 );
    glVertex3f(  0.5,  0.5, -0.5 );
    glVertex3f( -0.5,  0.5, -0.5 );
    glVertex3f( -0.5,  0.5,  0.5 );
    
    // Red side - BOTTOM
    glVertex3f(  0.5, -0.5, -0.5 );
    glVertex3f(  0.5, -0.5,  0.5 );
    glVertex3f( -0.5, -0.5,  0.5 );
    glVertex3f( -0.5, -0.5, -0.5 );
  glEnd();
}

void drawCubeSolid(){
	glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_QUADS); 
    glNormal3d(0, 0, 1);// Front
    glVertex3f(  0.5, -0.5, 0.5 );
    glVertex3f(  0.5,  0.5, 0.5 );
    glVertex3f( -0.5,  0.5, 0.5 );
    glVertex3f( -0.5, -0.5, 0.5 );
  
    glNormal3d(1, 0, 0);// RIGHT
    glVertex3f( 0.5, -0.5, -0.5 );
    glVertex3f( 0.5,  0.5, -0.5 );
    glVertex3f( 0.5,  0.5,  0.5 );
    glVertex3f( 0.5, -0.5,  0.5 );
  
    glNormal3d(-1, 0, 0);// LEFT
    glVertex3f( -0.5, -0.5,  0.5 );
    glVertex3f( -0.5,  0.5,  0.5 );
    glVertex3f( -0.5,  0.5, -0.5 );
    glVertex3f( -0.5, -0.5, -0.5 );
  
    glNormal3d(0, 1, 0);// TOP
    glVertex3f(  0.5,  0.5,  0.5 );
    glVertex3f(  0.5,  0.5, -0.5 );
    glVertex3f( -0.5,  0.5, -0.5 );
    glVertex3f( -0.5,  0.5,  0.5 );
  
    glNormal3d(0, -1, 0);// BOTTOM
    glVertex3f(  0.5, -0.5, -0.5 );
    glVertex3f(  0.5, -0.5,  0.5 );
    glVertex3f( -0.5, -0.5,  0.5 );
    glVertex3f( -0.5, -0.5, -0.5 );
  glEnd();
}


void renderGL(void){
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();
  glEnable(GL_CULL_FACE); 

  
  glEnable(GL_LIGHTING);
  glEnable(GL_COLOR_MATERIAL);
  glEnable(GL_AUTO_NORMAL);
  glShadeModel(GL_SMOOTH);

  glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
  glEnable (GL_LIGHT0);

  GLfloat global_ambient[] = { 0.4, 0.4, 0.4, 1 };
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);
  GLfloat diffuse[] = { 0.5f, 0.5f, 1, 1 };
  glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
  GLfloat light_position[] = { 1.0f, 0.5f, 0, 0 };
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);



  glColor4f(0.5, 0.0, 0.5, 1.0);
  //~ glBegin(GL_TRIANGLES);
  //~ glVertex3f(-0.5f,  0.5f,  0.0f);
  //~ glVertex3f(0.5f, -0.5f,  0.0f);
  //~ glVertex3f(-0.5f, -0.5f,  0.0f);
  //~ glEnd();
  //~ glColor4f(0.0, 1.0, 1.0, 1.0);
  //~ glBegin(GL_TRIANGLES);
  //~ glVertex3f(-0.5f,  0.5f,  0.0f);
  //~ glVertex3f(0.5f, -0.5f,  0.0f);
  //~ glVertex3f(0.5f, 0.5f, 0.0f);
  //~ glEnd();
  //~ glScalef(2, 2, 2);
  //~ drawCube();
  glScalef(0.2,0.2,0.2);
  glRotatef( 35.264, 1.0, 0.0, 0.0);
  glRotatef( -35.0, 0.0, 1.0, 0.0);
  //~ glBegin(GL_QUADS);
  //~ glVertex3f(-0.5f, -0.5f, 0.5f);
  //~ glVertex3f(0.5f, -0.5f, 0.5f);
  //~ glVertex3f(0.5f, 0.5f, 0.5f);
  //~ glVertex3f(-0.5f, 0.5f, 0.5f);
  //~ glEnd();
  drawCubeSolid();
  //~ glColor4f(0.0, 0.0, 0.0, 1.0);
  //~ drawCubeWireframe();
  
  
}

int main(int argc, char** argv)
{
  //! The pointer to the GLFW window
  GLFWwindow* window;

  //! Setting up the GLFW Error callback
  glfwSetErrorCallback(csX75::error_callback);

  //! Initialize GLFW
  if (!glfwInit())
    return -1;

  //! Create a windowed mode window and its OpenGL context
  window = glfwCreateWindow(800, 800, "CS475/CS675 OpenGL Framework", NULL, NULL);
  if (!window)
    {
      glfwTerminate();
      return -1;
    }
  
  //! Make the window's context current 
  glfwMakeContextCurrent(window);

  //Initialize GLEW
  GLenum err = glewInit();
  if (GLEW_OK != err)
    {
      //Problem: glewInit failed, something is seriously wrong.
      std::cerr<<"GLEW Init Failed : %s"<<std::endl;
    }

  //Print and see what context got enabled
  std::cout<<"Vendor: "<<glGetString (GL_VENDOR)<<std::endl;
  std::cout<<"Renderer: "<<glGetString (GL_RENDERER)<<std::endl;
  std::cout<<"Version: "<<glGetString (GL_VERSION)<<std::endl;
  std::cout<<"GLSL Version: "<<glGetString (GL_SHADING_LANGUAGE_VERSION)<<std::endl;

  //Keyboard Callback
  glfwSetKeyCallback(window, csX75::key_callback);
  //Framebuffer resize callback
  glfwSetFramebufferSizeCallback(window, csX75::framebuffer_size_callback);

  // Ensure we can capture the escape key being pressed below
  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

  //Initialize GL state
  csX75::initGL();
  
  // Loop until the user closes the window
  while (glfwWindowShouldClose(window) == 0)
    {
       
      // Render here
      renderGL();

      // Swap front and back buffers
      glfwSwapBuffers(window);
      
      // Poll for and process events
      glfwPollEvents();
    }
  
  glfwTerminate();
  return 0;
}

