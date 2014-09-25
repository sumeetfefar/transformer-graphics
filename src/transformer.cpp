#include "gl_framework.hpp"
#include <GL/glu.h>
#include <GL/gl.h>

#define head 1
#define neck 2
#define torso 3
#define right_upper_arm 4
#define left_upper_arm 5
#define right_lower_arm 6
#define left_lower_arm 7
#define right_thigh 8
#define left_thigh 9
#define right_leg 10
#define left_leg 11
#define right_hand 12
#define left_hand 13
#define right_foot 14
#define left_foot 15

class vector{
	public:
	float x, y, z;
	
	vector(){
		;
	}
	vector(float xx, float yy, float zz){
		x = xx;
		y = yy;
		z = zz;
	}
};

float y_angle = -45;
float x_angle = 35.264;

//~ float y_angle = 0;
//~ float x_angle = 0;

float torso_width, torso_length;

vector upper_arm_size(0.15, 0.4, 0.15), lower_arm_size(0.11, 0.5, 0.11);
vector thigh_size(0.15, 0.6, 0.15), leg_size(0.12, 0.6, 0.12);
vector hand(0.1, 0.03, 0.15), foot(0.1, 0.05, 0.2);


void drawCubeWireframe(){
	glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);
	glBegin(GL_QUADS); 
    // glNormal3d(0, 0, -1);//Front
    glVertex3f(  0.5, -0.5, 0.5 );
    glVertex3f(  0.5,  0.5, 0.5 );
    glVertex3f( -0.5,  0.5, 0.5 );
    glVertex3f( -0.5, -0.5, 0.5 );
    
    //~ glNormal3d(0, 0, -1); // Back
    glVertex3f( -0.5, -0.5, -0.5 );
    glVertex3f( -0.5,  0.5, -0.5 );
    glVertex3f(  0.5,  0.5, -0.5 );
    glVertex3f(  0.5, -0.5, -0.5 ); 
     
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

void drawCuboidSolid(float l, float b, float h){
	glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_QUADS);
    // glColor4f(1,0,0,1);
    glNormal3f(0, 0, 1);// Front
    glVertex3f(  l/2, -b/2, h/2 );
    glVertex3f(  l/2,  b/2, h/2 );
    glVertex3f( -l/2,  b/2, h/2 );
    glVertex3f( -l/2, -b/2, h/2 );
    
    // glColor4f(0,1,0,1);
    glNormal3f(0, 0, -1); // Back
    glVertex3f( -l/2, -b/2, -h/2 );
    glVertex3f( -l/2,  b/2, -h/2 );
    glVertex3f(  l/2,  b/2, -h/2 );
    glVertex3f(  l/2, -b/2, -h/2 );   
    
    // glColor4f(0,0,1,1);
    glNormal3f(1, 0, 0);// RIGHT
    glVertex3f( l/2, -b/2, -h/2 );
    glVertex3f( l/2,  b/2, -h/2 );
    glVertex3f( l/2,  b/2,  h/2 );
    glVertex3f( l/2, -b/2,  h/2 );
    
    // glColor4f(1,1,0,1);
    glNormal3f(-1, 0, 0);// LEFT
    glVertex3f( -l/2, -b/2,  h/2 );
    glVertex3f( -l/2,  b/2,  h/2 );
    glVertex3f( -l/2,  b/2, -h/2 );
    glVertex3f( -l/2, -b/2, -h/2 );
  
    // glColor4f(1,0,1,1);
    glNormal3f(0, 1, 0);// TOP
    glVertex3f(  l/2,  b/2,  h/2 );
    glVertex3f(  l/2,  b/2, -h/2 );
    glVertex3f( -l/2,  b/2, -h/2 );
    glVertex3f( -l/2,  b/2,  h/2 );
  
    // glColor4f(0,1,1,1);
    glNormal3f(0, -1, 0);// BOTTOM
    glVertex3f(  l/2, -b/2, -h/2 );
    glVertex3f(  l/2, -b/2,  h/2 );
    glVertex3f( -l/2, -b/2,  h/2 );
    glVertex3f( -l/2, -b/2, -h/2 );
  glEnd();
}



void struct_head(void){
	glNewList(head, GL_COMPILE);
		glPushMatrix();
			glColor4f(0.5, 0.0, 0.5, 1.0);
			drawCuboidSolid(0.3,0.5,0.3);
			glPushMatrix();
				glTranslatef(0, 0.35, 0.15);
				glColor4f(1, 1, 1, 1);
				glPushMatrix();
					glTranslatef(0.15, 0, 0);
					drawCuboidSolid(0.1, 0.2, 0.1);
				glPopMatrix();
				glPushMatrix();
					glTranslatef(-0.15, 0, 0);
					drawCuboidSolid(0.1, 0.2, 0.1);
				glPopMatrix();
			glPopMatrix();
		glPopMatrix();
	glEndList();
	
}

void struct_neck(void){
	glNewList(neck, GL_COMPILE);
		glColor4f(0.5, 0.0, 0.5, 1.0);
		drawCuboidSolid(0.1, 0.2, 0.1);
	glEndList();
}

void struct_torso(void){
	glNewList(torso, GL_COMPILE);
		drawCuboidSolid(0.7, 0.4, 0.3);
		
		glTranslatef(0, -0.35, 0);
		drawCuboidSolid(0.5, 0.3, 0.3);
	glEndList();
}

void struct_right_upper_arm(void){
	glNewList(right_upper_arm, GL_COMPILE);
		drawCuboidSolid(upper_arm_size.x, upper_arm_size.y, upper_arm_size.z);
	glEndList();
}

void struct_right_lower_arm(void){
	glNewList(right_lower_arm, GL_COMPILE);
		drawCuboidSolid(lower_arm_size.x, lower_arm_size.y, lower_arm_size.z);
	glEndList();
}

void struct_left_upper_arm(void){
	glNewList(left_upper_arm, GL_COMPILE);
		drawCuboidSolid(upper_arm_size.x, upper_arm_size.y, upper_arm_size.z);
	glEndList();
}

void struct_left_lower_arm(void){
	glNewList(left_lower_arm, GL_COMPILE);
		drawCuboidSolid(lower_arm_size.x, lower_arm_size.y, lower_arm_size.z);
	glEndList();
}

void struct_right_thigh(void){
	glNewList(right_thigh, GL_COMPILE);
		drawCuboidSolid(thigh_size.x, thigh_size.y, thigh_size.z);
	glEndList();
}

void struct_right_leg(void){
	glNewList(right_leg, GL_COMPILE);
		drawCuboidSolid(leg_size.x, leg_size.y, leg_size.z);
	glEndList();
}

void struct_left_thigh(void){
	glNewList(left_thigh, GL_COMPILE);
		drawCuboidSolid(thigh_size.x, thigh_size.y, thigh_size.z);
	glEndList();
}

void struct_left_leg(void){
	glNewList(left_leg, GL_COMPILE);
		drawCuboidSolid(leg_size.x, leg_size.y, leg_size.z);
	glEndList();
}

void struct_left_hand(void){
	glNewList(left_hand, GL_COMPILE);
		glTranslatef(0, 0, hand.z/2);
		glRotatef(45, 1, 0, 0);
		drawCuboidSolid(hand.x, hand.y, hand.z);
	glEndList();
}

void struct_left_foot(void){
	glNewList(left_foot, GL_COMPILE);
		drawCuboidSolid(foot.x, foot.y, foot.z);
	glEndList();
}

void struct_right_hand(void){
	glNewList(right_hand, GL_COMPILE);
		glTranslatef(0, 0, hand.z/2);
		glRotatef(45, 1, 0, 0);
		drawCuboidSolid(hand.x, hand.y, hand.z);
	glEndList();
}

void struct_right_foot(void){
	glNewList(right_foot, GL_COMPILE);
		drawCuboidSolid(foot.x, foot.y, foot.z);
	glEndList();
}

void draw_robot(){
	
}

void init_structures(){
	struct_head();
	struct_neck();
	struct_torso();
	struct_right_upper_arm();
	struct_right_lower_arm();
	struct_left_upper_arm();
	struct_left_lower_arm();
	struct_right_thigh();
	struct_right_leg();
	struct_left_thigh();
	struct_left_leg();
	struct_left_hand();
	struct_left_foot();
	struct_right_hand();
	struct_right_foot();
}

void initGL(void){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glEnable(GL_CULL_FACE); 

	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_AUTO_NORMAL);
	glShadeModel(GL_SMOOTH);

	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glEnable (GL_LIGHT0);

	GLfloat global_ambient[] = { 0.4, 0.4, 0.4, 1 };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);
	GLfloat diffuse[] = { 0.5f, 0.5f, 1, 1 };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	GLfloat light_position[] = { 1.0f, 0.5f, 0, 0 };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

}

void renderGL(void){
  
	initGL();
	//~ glTranslatef(0, 0, -1);
	glMatrixMode(GL_MODELVIEW);
	gluPerspective(120,1,0.1,10);
	gluLookAt(0,0,1, 0, 0, 0, 0, 1,0);
	
	glRotatef( x_angle, 1.0, 0.0, 0.0);
	glRotatef( y_angle, 0.0, 1.0, 0.0);

	//~ drawCuboidSolid(0.5, 0.5, 0.5);
	//~ glPushMatrix();
	//~ glTranslatef(1.0, 1.0, 4.0);
	//~ glRotatef(150.0, 1.0, 0.0, 0.0);
	//~ drawCuboidSolid(0.5, 1.0, 0.5);
	//~ glPopMatrix();
	//~ glRotatef(30, 0, 1, 0);

	//~ glCallList(right_upper_arm);
	//~ glPushMatrix();
		//~ glTranslatef(0, -(upper_arm_size.y+lower_arm_size.y)/2, 0);
		//~ glCallList(right_lower_arm);
	//~ glPopMatrix();
	
	//~ glCallList(right_thigh);
	//~ glPushMatrix();
		//~ glTranslatef(0, -(thigh_size.y+leg_size.y)/2, 0);
		//~ glCallList(right_leg);
	//~ glPopMatrix();
	
	glCallList(left_foot);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
	//!Close the window if the ESC key was pressed
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	else if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
		y_angle += 10;
	else if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
		y_angle -= 10;
	else if (key == GLFW_KEY_UP && action == GLFW_PRESS)
		x_angle -= 10;
	else if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
		x_angle += 10;
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
	glfwSetKeyCallback(window, key_callback);
	//Framebuffer resize callback
	glfwSetFramebufferSizeCallback(window, csX75::framebuffer_size_callback);

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	//Initialize GL state
	csX75::initGL();
	init_structures();
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

