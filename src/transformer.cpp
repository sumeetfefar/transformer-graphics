#include "gl_framework.hpp"
#include <GL/glu.h>
#include <GL/gl.h>
#include <math.h>
#include <unistd.h>

#define PI 3.1416

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
#define hand_blade 16
#define heli_chest 17
#define blade_base 18
#define blade 19
#define right_foot_wing 20
#define left_foot_wing 21

#define Y_ANGLE_DEFAULT -45
#define X_ANGLE_DEFAULT 35.264

//~ States of transition
#define sHUMANOID -1
#define sVEHICLE 0
#define sTFone 1
#define sTFtwo 2
#define sTFthree 3
#define sTFfour 4

#define TF_BASE_ROT_V 90
#define TF_BASE_ROT_H 0

#define TF_SHOULDER_ROT_V 100
#define TF_SHOULDER_ROT_H 0

#define TF_ELBOW_ANGLE_V 90
#define TF_ELBOW_ANGLE_H 0

#define TF_KNEE_ANGLE_V 60
#define TF_KNEE_ANGLE_H 0

#define TF_NECK_ANGLE_V 180
#define TF_NECK_ANGLE_H 0

#define TF_FOOT_ANGLE_V 30
#define TF_FOOT_ANGLE_H 0

#define TF_HELI_CHEST_V 180
#define TF_HELI_CHEST_H 0

#define TF_BLADE_DIRECTION_V 0
#define TF_BLADE_DIRECTION_H 180

#define TF_BLADE_GAP_V 180
#define TF_BLADE_GAP_H 15

#define TF_HAND_ANGLE_V 90
#define TF_HAND_ANGLE_H 0

#define TF_DELAY_uS 100000

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
	
	void reset(){
		x = 0;
		y = 0;
		z = 0;		
	}
	
	void set(float xx, float yy, float zz){
		x = xx;
		y = yy;
		z = zz;
	}
};

float y_angle = -45;
float x_angle = 35.264;

int state = sHUMANOID, prevState = sHUMANOID;

//~ float y_angle = 0;
//~ float x_angle = 0;

float torso_width, torso_length;

vector rotor_blade_cylinder(0.08, 0.08, 180), rotor_blade_body(0.05, 0.4, 0), rotor_blade_tip(0, 0.1, 0), rotor_base_cylinder(0.06,0.08,180);
vector upper_arm_size(0.15, 0.7, 0.15), lower_arm_size(0.2, 0.5, 0.2), lower_arm_cylinder(0.1, 0.16, 180), upper_arm_sphere(0.2, 90,0);
vector knee_cylinder(0.15, 0.25, 90);
vector thigh_size(0.25, 0.5, 0.25), leg_size(0.22, 0.6, 0.22);
vector hand_size(0.1, 0.03, 0.15), foot_size(0.22, 0.1, 0.4), foot_fin(0.5,0.2,0);
float head_length = 0.5, neck_length = 0.15;

vector upper_torso_size( 0.8, 0.5, 0.3), lower_torso_size(0.6, 0.4, 0.3), upper_torso_front(0.8, 0.6, 0.3);
vector head_size(0.2, 0.3, 0.2);
//~ vector upper_torso_size( 0.8, 0.5, 0.3), lower_torso_size(0.6, 0.4, 0.3);


float right_elbow_angle = 0, left_elbow_angle = 0;
float right_knee_angle = 0, left_knee_angle = 0;
int blade_direction = 180,heli_chest_rot=0, blade_gap=15;

vector neck_rot(0, 0, 0);
vector right_shoulder_rot(0, 0, 0), left_shoulder_rot(0, 0, 0);
vector right_hip_rot(0, 0, 0), left_hip_rot(0, 0, 0);
vector right_hand_rot(0, 0, 0), left_hand_rot(0, 0, 0);
vector right_foot_rot(0, 0, 0), left_foot_rot(0, 0, 0);

float tf_base_rot = 0;
vector tf_neck_translate(0, 0, 0); 
vector tf_left_leg_translt(0, 0, 0); vector tf_right_leg_translt(0, 0, 0);



//~ Go back to base position in sHUMANOID
void reset_angles_H(){
	neck_rot.reset();
	right_shoulder_rot.reset(); left_shoulder_rot.reset();
	right_hip_rot.reset(); left_hip_rot.reset();
	right_hand_rot.reset(); left_hand_rot.reset();
	right_foot_rot.reset(); left_foot_rot.reset();
	
	right_elbow_angle = 0; left_elbow_angle = 0;
	right_knee_angle = 0; left_knee_angle = 0;
	
	blade_gap = 0;
	
	x_angle = X_ANGLE_DEFAULT;
	y_angle = Y_ANGLE_DEFAULT;
}

void reset_angles_V(){
	neck_rot.reset();
	right_shoulder_rot.reset(); left_shoulder_rot.reset();
	right_hip_rot.reset(); left_hip_rot.reset();
	right_hand_rot.reset(); left_hand_rot.reset();
	right_foot_rot.reset(); left_foot_rot.reset();
	
	right_elbow_angle = 0; left_elbow_angle = 0;
	right_knee_angle = 0; left_knee_angle = 0;
	
	blade_gap = 0;
	
	x_angle = X_ANGLE_DEFAULT;
	y_angle = Y_ANGLE_DEFAULT;
	
	tf_base_rot = TF_BASE_ROT_V;
	heli_chest_rot = TF_HELI_CHEST_V;
	neck_rot.x = -TF_NECK_ANGLE_V;
	right_shoulder_rot.z = -TF_SHOULDER_ROT_V; left_shoulder_rot.z = TF_SHOULDER_ROT_V;
	right_hand_rot.x = TF_HAND_ANGLE_V; left_hand_rot.x = TF_HAND_ANGLE_V;
	blade_gap = TF_BLADE_GAP_V;
	right_elbow_angle = TF_ELBOW_ANGLE_V; left_elbow_angle = TF_ELBOW_ANGLE_V;
	right_knee_angle = TF_KNEE_ANGLE_V; left_knee_angle = TF_KNEE_ANGLE_V;
	right_foot_rot.x = TF_FOOT_ANGLE_V; left_foot_rot.x = TF_FOOT_ANGLE_V;
}

//~ Transform into vehicle
void transform(){
	prevState = sHUMANOID;
	state = sTFone;
}

//~ Transform back into humanoid
void untransform(){
	prevState = sVEHICLE;
	state = sTFfour;
}

void drawTetrahedron(float s){
    glBegin(GL_TRIANGLES);

    glNormal3f(0, 0.577, -0.816);
    glVertex3f(-s/2, 0, -s*0.707/2);
    glVertex3f(0, s/2, s*0.707/2);
    glVertex3f(s/2, 0, -s*0.707/2);
    
    glNormal3f(0, -0.577, -0.816);
    glVertex3f(s/2, 0, -s*0.707/2);
    glVertex3f(0, -s/2, s*0.707/2);
    glVertex3f(-s/2, 0, -s*0.707/2);
        
    glNormal3f(0.577, 0, 0.816);
    glVertex3f(s/2, 0, -s*0.707/2);
    glVertex3f(0, s/2, s*0.707/2);
    glVertex3f(0, -s/2, s*0.707/2);
    
    glNormal3f(-0.577, 0, 0.816);
    glVertex3f(-s/2, 0, -s*0.707/2);
    glVertex3f(0, -s/2, s*0.707/2);
    glVertex3f(0, s/2, s*0.707/2);

    glEnd();
}

void drawCuboidTetrahedron(float l, float b, float h){
	glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_TRIANGLES);
    
	    glNormal3f(0, 0, -1); // Back
	    glVertex3f( 0, -b/2, -h/2 );
	    glVertex3f( -l/2,  b/2, -h/2 );
	    glVertex3f(  l/2,  b/2, -h/2 );
	    
	    
	    //~ glNormal3f(1, 0, 0);// RIGHT
	    float face_norm = sqrt(b*h*b*h+ l*h*l*h + l*b*l*b);
	    glNormal3f(b*h/face_norm, l*h/face_norm, l*b/face_norm);
	    glVertex3f( 0,  b/2,  h/2 );
	    glVertex3f( 0, -b/2, -h/2 );
	    glVertex3f(  l/2,  b/2, -h/2 );
	    
	    //~ glNormal3f(-1, 0, 0);// LEFT
	    glNormal3f(-b*h/face_norm, l*h/face_norm, l*b/face_norm);
	    glVertex3f(  -l/2,  b/2, -h/2 );
	    glVertex3f( 0, -b/2, -h/2 );
	    glVertex3f(  0,  b/2,  h/2 );
	    
	    // glColor4f(1,0,1,1);
	    glNormal3f(0, 1, 0);// TOP
	    glVertex3f(  0,  b/2,  h/2 );
	    glVertex3f(  l/2,  b/2, -h/2 );
	    glVertex3f( -l/2,  b/2, -h/2 );
	    
  
  glEnd();
}


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

void drawSphere(float r, float res){
	for(int j=0; j<res; j++){
		for(int i=0; i<res; i++){
			glBegin(GL_QUADS);
				glNormal3f( sin(i*2*PI/res)*sin(j*PI/res),  cos(i*PI*2/res)*sin(j*PI/res), cos(j*PI/res) );
			    glVertex3f( r*sin(i*2*PI/res)*sin(j*PI/res),  r*cos(i*PI*2/res)*sin(j*PI/res), r*cos(j*PI/res) );
			    glNormal3f( sin((i+1)*2*PI/res)*sin(j*PI/res),  cos((i+1)*2*PI/res)*sin(j*PI/res), cos(j*PI/res) );
			    glVertex3f( r*sin((i+1)*2*PI/res)*sin(j*PI/res),  r*cos((i+1)*2*PI/res)*sin(j*PI/res), r*cos(j*PI/res) );
			    glNormal3f( sin((i+1)*2*PI/res)*sin((j+1)*PI/res),  cos((i+1)*2*PI/res)*sin((j+1)*PI/res), cos((j+1)*PI/res) );
			    glVertex3f( r*sin((i+1)*2*PI/res)*sin((j+1)*PI/res),  r*cos((i+1)*2*PI/res)*sin((j+1)*PI/res), r*cos((j+1)*PI/res) );
			    glNormal3f( sin(i*2*PI/res)*sin((j+1)*PI/res),  cos(i*2*PI/res)*sin((j+1)*PI/res), cos((j+1)*PI/res) );
			    glVertex3f( r*sin(i*2*PI/res)*sin((j+1)*PI/res),  r*cos(i*2*PI/res)*sin((j+1)*PI/res), r*cos((j+1)*PI/res) );
			glEnd();
		}
	}
}



void drawCylinder(float r, float h, float res){
	for(int i=0; i<res; i++){
		glBegin(GL_TRIANGLES);
			glNormal3f(0, 0, 1);// Front
		    glVertex3f( 0, 0, h/2 );
		    glVertex3f( r*sin((i+1)*2*PI/res),  r*cos((i+1)*2*PI/res), h/2 );
		    glVertex3f( r*sin(i*2*PI/res),  r*cos(i*PI*2/res), h/2 );
		    
		    glNormal3f(0, 0, -1);// Front
		    glVertex3f( 0, 0, -h/2 );
		    glVertex3f( r*sin(i*2*PI/res),  r*cos(i*2*PI/res), -h/2 );
		    glVertex3f( r*sin((i+1)*2*PI/res),  r*cos((i+1)*2*PI/res), -h/2 );
		glEnd();
		glBegin(GL_QUADS);
			//~ glNormal3f((r*sin(i*2*PI/res)+r*sin((i+1)*2*PI/res))/2, (r*cos(i*2*PI/res)+r*cos((i+1)*2*PI/res))/2, 0);// Front
		    glNormal3f( sin(i*2*PI/res),  cos(i*PI*2/res), 0 );
		    glVertex3f( r*sin(i*2*PI/res),  r*cos(i*PI*2/res), h/2 );
		    glNormal3f( sin((i+1)*2*PI/res),  cos((i+1)*2*PI/res), 0 );
		    glVertex3f( r*sin((i+1)*2*PI/res),  r*cos((i+1)*2*PI/res), h/2 );
		    glNormal3f( sin((i+1)*2*PI/res),  cos((i+1)*2*PI/res), 0 );
		    glVertex3f( r*sin((i+1)*2*PI/res),  r*cos((i+1)*2*PI/res), -h/2 );
		    glNormal3f( sin(i*2*PI/res),  cos(i*2*PI/res), 0 );
		    glVertex3f( r*sin(i*2*PI/res),  r*cos(i*2*PI/res), -h/2 );
		glEnd();
	}
}

void drawCuboidEdgeYd(float l, float b, float h){
	glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_QUADS);
    // glColor4f(1,0,0,1);
    glNormal3f(0, 0, 1);// Front
    glVertex3f(  l/2, -b, h/2 );
    glVertex3f(  l/2,  0, h/2 );
    glVertex3f( -l/2,  0, h/2 );
    glVertex3f( -l/2, -b, h/2 );
    
    // glColor4f(0,1,0,1);
    glNormal3f(0, 0, -1); // Back
    glVertex3f( -l/2, -b, -h/2 );
    glVertex3f( -l/2,  0, -h/2 );
    glVertex3f(  l/2,  0, -h/2 );
    glVertex3f(  l/2, -b, -h/2 );   
    
    // glColor4f(0,0,1,1);
    glNormal3f(1, 0, 0);// RIGHT
    glVertex3f( l/2, -b, -h/2 );
    glVertex3f( l/2,  0, -h/2 );
    glVertex3f( l/2,  0,  h/2 );
    glVertex3f( l/2, -b,  h/2 );
    
    // glColor4f(1,1,0,1);
    glNormal3f(-1, 0, 0);// LEFT
    glVertex3f( -l/2, -b,  h/2 );
    glVertex3f( -l/2,  0,  h/2 );
    glVertex3f( -l/2,  0, -h/2 );
    glVertex3f( -l/2, -b, -h/2 );
  
    // glColor4f(1,0,1,1);
    glNormal3f(0, 1, 0);// TOP
    glVertex3f(  l/2,  0,  h/2 );
    glVertex3f(  l/2,  0, -h/2 );
    glVertex3f( -l/2,  0, -h/2 );
    glVertex3f( -l/2,  0,  h/2 );
  
    // glColor4f(0,1,1,1);
    glNormal3f(0, -1, 0);// BOTTOM
    glVertex3f(  l/2, -b, -h/2 );
    glVertex3f(  l/2, -b,  h/2 );
    glVertex3f( -l/2, -b,  h/2 );
    glVertex3f( -l/2, -b, -h/2 );
  glEnd();
}

void drawPrism(float l, float b, float h ){
	glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);

	glBegin(GL_TRIANGLES);
    // glColor4f(1,0,0,1);
    glNormal3f(0, 0, 1);// Front
    glVertex3f(  l/2, -b, h/2 );
    glVertex3f(  l/2,  0, h/2 );
    glVertex3f( -l/2,  0, h/2 );
    //~ glVertex3f( -l/2, -b, h/2 );
    
    // glColor4f(0,1,0,1);
    glNormal3f(0, 0, -1); // Back
    //~ glVertex3f( -l/2, -b, -h/2 );
    glVertex3f( -l/2,  0, -h/2 );
    glVertex3f(  l/2,  0, -h/2 );
    glVertex3f(  l/2, -b, -h/2 );   

    glEnd();
    glBegin(GL_QUADS);
    //~ // glColor4f(0,0,1,1);
    glNormal3f(1, 0, 0);// RIGHT
    glVertex3f( l/2, -b, -h/2 );
    glVertex3f( l/2,  0, -h/2 );
    glVertex3f( l/2,  0,  h/2 );
    glVertex3f( l/2, -b,  h/2 );
    
    // glColor4f(1,1,0,1);
    glNormal3f(-b/sqrt(l*l+b*b), -l/sqrt(l*l+b*b), 0);// LEFT
    glVertex3f( l/2, -b,  h/2 );
    glVertex3f( -l/2,  0,  h/2 );
    glVertex3f( -l/2,  0, -h/2 );
    glVertex3f( l/2, -b, -h/2 );
  
    //~ // glColor4f(1,0,1,1);
    glNormal3f(0, 1, 0);// TOP
    glVertex3f(  l/2,  0,  h/2 );
    glVertex3f(  l/2,  0, -h/2 );
    glVertex3f( -l/2,  0, -h/2 );
    glVertex3f( -l/2,  0,  h/2 );
  
    // glColor4f(0,1,1,1);
    //~ glNormal3f(0, -1, 0);// BOTTOM
    //~ glVertex3f(  l/2, -b, -h/2 );
    //~ glVertex3f(  l/2, -b,  h/2 );
    //~ glVertex3f( -l/2, -b,  h/2 );
    //~ glVertex3f( -l/2, -b, -h/2 );
	glEnd();
}

void struct_head(void){
	glNewList(head, GL_COMPILE);
		glPushMatrix();
			//~ glColor4f(0.5, 0.0, 0.5, 1.0);
			glPushMatrix();
				glTranslatef(0,-head_size.y/8,-head_size.z/4);
				drawCuboidSolid(head_size.x,head_size.y,head_size.z/2);
			glPopMatrix();
			glPushMatrix();
				glTranslatef(0,-head_size.y/2,head_size.z/4);
				drawCuboidTetrahedron(head_size.x,head_size.y*3/2,head_size.z/2);
			glPopMatrix();
			//~ glPushMatrix();
			//~ 
				//~ glTranslatef(0,-head_size.y/2,-head_size.z/8);
				//~ glRotatef(180,0,1,0);
				//~ drawCuboidTetrahedron(head_size.x,head_size.y/2,head_size.z/4);
			//~ glPopMatrix();
			glPushMatrix();
				drawCuboidSolid(head_size.x,head_size.y/4,head_size.z*3/5);
				glTranslatef(0,head_size.y/4,0);
				drawCuboidSolid(head_size.x,head_size.y/4,head_size.z);
				
			glPopMatrix();
			//~ glPushMatrix();
				//~ glTranslatef(0, head_size.y/2 + 0.1, head_size.z/2);
				//~ glColor4f(1, 1, 1, 1);
				//~ glPushMatrix();
					//~ glTranslatef(0.15, 0, 0);
					//~ drawCuboidSolid(0.1, 0.2, 0.1);
				//~ glPopMatrix();
				//~ glPushMatrix();
					//~ glTranslatef(-0.15, 0, 0);
					//~ drawCuboidSolid(0.1, 0.2, 0.1);
				//~ glPopMatrix();
			//~ glPopMatrix();
		glPopMatrix();
		
	glEndList();
	
}

void struct_neck(void){
	glNewList(neck, GL_COMPILE);
		//~ glColor4f(0.5, 0.0, 0.5, 1.0);
		glPushMatrix();
			glTranslatef(0,neck_length/2,0);
			glRotatef(90, 1, 0, 0);
			//~ drawCuboidEdgeYd(0.1, neck_length, 0.1);
			
			drawCylinder(0.08, neck_length,20);
		glPopMatrix();
		
		glTranslatef(0,neck_length/4,0);
		glPushMatrix();
			glRotatef(90, 1, 0, 0);
			drawCylinder(0.11, neck_length/2,20);
		glPopMatrix();
		//~ 
			//~ glTranslatef(0,neck_length/4,0);
			//~ glRotatef(90, 1, 0, 0);
			//~ drawCylinder(0.10, neck_length/4,20);
		//~ 
	glEndList();
}

void struct_torso(void){
	glNewList(torso, GL_COMPILE);
		drawCuboidSolid(upper_torso_size.x, upper_torso_size.y, upper_torso_size.z);
				
		glTranslatef(0, -lower_torso_size.y/2-upper_torso_size.y/2, 0);
		drawCuboidSolid(lower_torso_size.x, lower_torso_size.y, lower_torso_size.z);
	glEndList();
}

void struct_heli_chest(void){
	glNewList(heli_chest, GL_COMPILE);
		
		drawCuboidTetrahedron(upper_torso_front.x, upper_torso_front.y, upper_torso_front.z);
		glTranslatef(0,-upper_torso_front.y/2,-upper_torso_front.z/4);
		drawCuboidTetrahedron(upper_torso_front.x/2, upper_torso_front.y, upper_torso_front.z/2);
	glEndList();
}

void struct_right_upper_arm(void){
	glNewList(right_upper_arm, GL_COMPILE);
		drawSphere(upper_arm_sphere.x,upper_arm_sphere.y);
		drawCuboidEdgeYd(upper_arm_size.x, upper_arm_size.y, upper_arm_size.z);
	glEndList();
}

void struct_right_lower_arm(void){
	glNewList(right_lower_arm, GL_COMPILE);
		drawCuboidEdgeYd(lower_arm_size.x, lower_arm_size.y, lower_arm_size.z);
		glPushMatrix();
			glRotatef(90,0,1,0);
			drawCylinder(lower_arm_cylinder.x, lower_arm_cylinder.y, lower_arm_cylinder.z);
		glPopMatrix();
	glEndList();
}

void struct_left_upper_arm(void){
	glNewList(left_upper_arm, GL_COMPILE);
	drawSphere(upper_arm_sphere.x,upper_arm_sphere.y);
		drawCuboidEdgeYd(upper_arm_size.x, upper_arm_size.y, upper_arm_size.z);
	glEndList();
}

void struct_left_lower_arm(void){
	glNewList(left_lower_arm, GL_COMPILE);
		drawCuboidEdgeYd(lower_arm_size.x, lower_arm_size.y, lower_arm_size.z);
		glPushMatrix();
			glRotatef(90,0,1,0);
			drawCylinder(lower_arm_cylinder.x, lower_arm_cylinder.y, lower_arm_cylinder.z);
		glPopMatrix();
		
	glEndList();
}

void struct_right_thigh(void){
	glNewList(right_thigh, GL_COMPILE);
		drawCuboidEdgeYd(thigh_size.x, thigh_size.y, thigh_size.z);
	glEndList();
}

void struct_right_leg(void){
	glNewList(right_leg, GL_COMPILE);
		glPushMatrix();
			glRotatef(90,0,1,0);
			drawCylinder(knee_cylinder.x, knee_cylinder.y, knee_cylinder.z);
		glPopMatrix();
		drawCuboidEdgeYd(leg_size.x, leg_size.y, leg_size.z);
	glEndList();
}

void struct_left_thigh(void){
	glNewList(left_thigh, GL_COMPILE);
		drawCuboidEdgeYd(thigh_size.x, thigh_size.y, thigh_size.z);
	glEndList();
}

void struct_left_leg(void){
	glNewList(left_leg, GL_COMPILE);
		glPushMatrix();
			glRotatef(90,0,1,0);
			drawCylinder(knee_cylinder.x, knee_cylinder.y, knee_cylinder.z);
		glPopMatrix();
		drawCuboidEdgeYd(leg_size.x, leg_size.y, leg_size.z);
	glEndList();
}

void struct_left_hand(void){
	glNewList(left_hand, GL_COMPILE);
		//~ glTranslatef(0, 0, hand_size.z/2);
		//~ glRotatef(45, 1, 0, 0);
		//~ drawCuboidSolid(hand_size.x, hand_size.y, hand_size.z);
		glPushMatrix();
			glRotatef(90,0,1,0);
			//~ glTranslatef(0.06,0,0);
			drawCylinder(rotor_base_cylinder.x,rotor_base_cylinder.y,rotor_base_cylinder.z);
		glPopMatrix();
	glEndList();
}

void struct_left_foot(void){
	glNewList(left_foot, GL_COMPILE);
		drawCuboidSolid(foot_size.x, foot_size.y, foot_size.z);
		//~ glTranslatef(foot_size.x/2, -foot_size.y/2, foot_size.z/2);
		//~ glBegin(GL_QUADS);
			//~ glNormal3f(0,-1,0);
			//~ glVertex3f(0,0,0);
			//~ glVertex3f(0,0,-foot_size.z);
			//~ glVertex3f(foot_fin.x,0,-foot_size.z + foot_fin.y);
			//~ glVertex3f(foot_fin.x,0,0);
			//~ 
			//~ glNormal3f(0,1,0);
			//~ glVertex3f(foot_fin.x,0,0);
			//~ glVertex3f(foot_fin.x,0,-foot_size.z + foot_fin.y);
			//~ glVertex3f(0,0,-foot_size.z);
			//~ glVertex3f(0,0,0);
		//~ glEnd();
	glEndList();
}

void struct_left_foot_wing(void){
	glNewList(left_foot_wing, GL_COMPILE);
		
		//~ glTranslatef(foot_size.x/2, -foot_size.y/2, foot_size.z/2);
		glBegin(GL_QUADS);
			glNormal3f(0,-1,0);
			glVertex3f(0,0,0);
			glVertex3f(0,0,-foot_size.z);
			glVertex3f(foot_fin.x,0,-foot_size.z + foot_fin.y);
			glVertex3f(foot_fin.x,0,0);
			
			glNormal3f(0,1,0);
			glVertex3f(foot_fin.x,0,0);
			glVertex3f(foot_fin.x,0,-foot_size.z + foot_fin.y);
			glVertex3f(0,0,-foot_size.z);
			glVertex3f(0,0,0);
		glEnd();
	glEndList();
}

void struct_right_hand(void){
	glNewList(right_hand, GL_COMPILE);
		//~ glTranslatef(0, 0, hand_size.z/2);
		//~ glRotatef(45, 1, 0, 0);
		//~ drawCuboidSolid(hand_size.x, hand_size.y, hand_size.z);
		glPushMatrix();
			glRotatef(90,0,1,0);
			//~ glTranslatef(0.06,0,0);
			drawCylinder(rotor_base_cylinder.x,rotor_base_cylinder.y,rotor_base_cylinder.z);
		glPopMatrix();
	glEndList();
}

void struct_right_foot(void){
	glNewList(right_foot, GL_COMPILE);
		drawCuboidSolid(foot_size.x, foot_size.y, foot_size.z);
		//~ glTranslatef(-foot_size.x/2, -foot_size.y/2, foot_size.z/2);
		//~ glBegin(GL_QUADS);
			//~ glNormal3f(0,1,0);
			//~ glVertex3f(0,0,0);
			//~ glVertex3f(0,0,-foot_size.z);
			//~ glVertex3f(-foot_fin.x,0,-foot_size.z + foot_fin.y);
			//~ glVertex3f(-foot_fin.x,0,0);
			//~ 
			//~ glNormal3f(0,-1,0);
			//~ glVertex3f(-foot_fin.x,0,0);
			//~ glVertex3f(-foot_fin.x,0,-foot_size.z + foot_fin.y);
			//~ glVertex3f(0,0,-foot_size.z);
			//~ glVertex3f(0,0,0);
		//~ glEnd();
		
	glEndList();
}
void struct_right_foot_wing(void){
	glNewList(right_foot_wing, GL_COMPILE);
		//~ glTranslatef(-foot_size.x/2, -foot_size.y/2, foot_size.z/2);
		glBegin(GL_QUADS);
			glNormal3f(0,1,0);
			glVertex3f(0,0,0);
			glVertex3f(0,0,-foot_size.z);
			glVertex3f(-foot_fin.x,0,-foot_size.z + foot_fin.y);
			glVertex3f(-foot_fin.x,0,0);
			
			glNormal3f(0,-1,0);
			glVertex3f(-foot_fin.x,0,0);
			glVertex3f(-foot_fin.x,0,-foot_size.z + foot_fin.y);
			glVertex3f(0,0,-foot_size.z);
			glVertex3f(0,0,0);
		glEnd();
		
	glEndList();
}

void struct_blade_base(void){
	glNewList(blade_base, GL_COMPILE);
		drawCylinder(rotor_blade_cylinder.x,rotor_blade_cylinder.y,rotor_blade_cylinder.z);
	glEndList();
}

void struct_blade(void){
	glNewList(blade, GL_COMPILE);
		drawCuboidEdgeYd(rotor_blade_body.x, rotor_blade_body.y, rotor_blade_body.z);
		glTranslatef(0,-rotor_blade_body.y,0);
		drawPrism(rotor_blade_body.x, rotor_blade_tip.y, rotor_blade_body.z);
	glEndList();
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
	struct_heli_chest();
	struct_blade_base();
	struct_blade();
	struct_left_foot_wing();
	struct_right_foot_wing();
}

void draw_robot(){
	glPushMatrix();
		
		if (state == sTFone && prevState == sHUMANOID){
			if (tf_base_rot != TF_BASE_ROT_V)
				tf_base_rot += 10;
			else{
				state = sTFtwo;
				prevState = sTFone;
			}
		}
		
		if (state == sTFfour && prevState == sVEHICLE){
			if (tf_base_rot != TF_BASE_ROT_H)
				tf_base_rot -= 10;
			else{
				state = sTFthree;
				prevState = sTFfour;
			}
		}
		glRotatef(tf_base_rot, 1, 0, 0);
		
		//~ Drawing the head and the neck
		glPushMatrix();
			if (state == sTFfour && prevState == sTFthree){
				//~ std::cout<< "here\n";
				if (tf_neck_translate.z > -(upper_torso_size.z/2+head_size.z/2))
					tf_neck_translate.z -= 0.05;
				
				if (heli_chest_rot != TF_HELI_CHEST_V){
					heli_chest_rot += 30;
				}
				
				if (neck_rot.x != -TF_NECK_ANGLE_V)
					neck_rot.x -= 30;
				else{
					state = sVEHICLE;
					prevState = sTFfour;
				}
			}
			if (state == sTFthree && prevState == sTFfour){
				if (tf_neck_translate.z < 0)
					tf_neck_translate.z += 0.05;
				
				if (heli_chest_rot != TF_HELI_CHEST_H){
					heli_chest_rot -= 30;
				}
				
				if (neck_rot.x != -TF_NECK_ANGLE_H)
					neck_rot.x += 30;
				else{
					state = sTFtwo;
					prevState = sTFthree;
				}
			}
			
			glTranslatef(tf_neck_translate.x, tf_neck_translate.y, tf_neck_translate.z);
			glTranslatef(0, upper_torso_size.y/2, 0);
			glRotatef(neck_rot.x, 1, 0, 0);
			glRotatef(neck_rot.y, 0, 1, 0);
			glRotatef(neck_rot.z, 0, 0, 1);
			glPushMatrix(); // because using drawcuboidedgeYd and rotating 180
				glCallList(neck);
			glPopMatrix();
			glPushMatrix();
				glTranslatef(0, neck_length + head_size.y/2, 0);
				glCallList(head);
			glPopMatrix();
		glPopMatrix();
		
		//~ Torso
		
		
		glCallList(torso);
		glPushMatrix();
			glTranslatef(0, lower_torso_size.y/2+upper_torso_size.y/2, 0);
			glTranslatef(0, (upper_torso_size.y - upper_torso_front.y)/2, (upper_torso_size.z+upper_torso_front.z)/2);
			glTranslatef(0, upper_torso_front.y/2, -upper_torso_front.z/2);
			glRotatef(-heli_chest_rot,1,0,0);
			glTranslatef(0,-upper_torso_front.y/2, upper_torso_front.z/2);
			glCallList(heli_chest);
		glPopMatrix();
		
		
		
		//~ Drawing the Upper Limbs 
		glPushMatrix();
			glTranslatef(0, upper_torso_size.y+lower_torso_size.y/2-upper_arm_size.x/2, 0);
			
			//~ Right Upper Limb - Upper arm, lower arm, hand
			glPushMatrix();
				glTranslatef(-upper_torso_size.x/2-upper_arm_size.x/2, 0, 0);
				
				if (state == sTFtwo && prevState == sTFone){
					if (right_shoulder_rot.z != -TF_SHOULDER_ROT_V)
						right_shoulder_rot.z -= 10;
					
					if (right_hand_rot.x != TF_HAND_ANGLE_V){
						right_hand_rot.x += 30;
					}
					
					if (blade_gap < TF_BLADE_GAP_V){
						blade_gap += 20;
					}
					if (blade_direction > TF_BLADE_DIRECTION_V){
						blade_direction -=45;
					}
				}
				
				if (state == sTFone && prevState == sTFtwo){
					if (right_shoulder_rot.z != -TF_SHOULDER_ROT_H)
						right_shoulder_rot.z += 10;
					
					if (right_hand_rot.x != TF_HAND_ANGLE_H){
						right_hand_rot.x -= 30;
					}
					
					if (blade_gap > TF_BLADE_GAP_H){
						 blade_gap -= 20;
					}
					if (blade_direction < TF_BLADE_DIRECTION_H){
						blade_direction +=45;
					}
				}
				glRotatef(right_shoulder_rot.x, 1, 0, 0);
				glRotatef(right_shoulder_rot.y, 0, 1, 0);
				glRotatef(right_shoulder_rot.z, 0, 0, 1);
				glCallList(right_upper_arm);
				glPushMatrix();
					glTranslatef(0, -upper_arm_size.y, 0);
					
					if (state == sTFthree && prevState == sTFtwo){
						if (right_elbow_angle != TF_ELBOW_ANGLE_V)
							right_elbow_angle += 10;
					}
					if (state == sTFtwo && prevState == sTFthree){
						if (right_elbow_angle != TF_ELBOW_ANGLE_H)
							right_elbow_angle -= 10;
					}
					
					glRotatef(right_elbow_angle, 1, 0, 0); // Elbow rotation
					glCallList(right_lower_arm);
					
					glPushMatrix();
						glTranslatef(0, -lower_arm_size.y-hand_size.y/2, hand_size.z/2);
						//~ glCallList(hand_blade);
						
						//~ glRotatef(45, 1, 0, 0);
						glTranslatef(0,0,-0.06);
						glRotatef(right_hand_rot.y, 0, 1, 0);
						glCallList(right_hand);
						glRotatef(right_hand_rot.x, 1, 0, 0);
						glRotatef(right_hand_rot.z, 0, 0, 1);
						glTranslatef(0,0,0.06);
						glPushMatrix();
							glCallList(blade_base);
							glPushMatrix();
								glTranslatef(0.04,0,0);
								glRotatef(blade_gap/2,0,0,1);
								glRotatef(blade_direction,0,1,0);
								glCallList(blade);
							glPopMatrix();
							glPushMatrix();
								glTranslatef(-0.04,0,0);
								glRotatef(-blade_gap/2,0,0,1);
								glCallList(blade);
							glPopMatrix();
						glPopMatrix();
					glPopMatrix();
				glPopMatrix();
			glPopMatrix();
			
			//~ Left Upper Limb - Upper arm, lower arm, hand
			glPushMatrix();
				glTranslatef(upper_torso_size.x/2+upper_arm_size.x/2, 0, 0);
				
				if (state == sTFtwo && prevState == sTFone){
					if (left_hand_rot.x != TF_HAND_ANGLE_V){
						left_hand_rot.x += 30;
					}
					
					if (left_shoulder_rot.z != TF_SHOULDER_ROT_V)
						left_shoulder_rot.z += 10;
					else{
						state = sTFthree;
						prevState = sTFtwo;
					}
				}
				if (state == sTFone && prevState == sTFtwo){
					if (left_hand_rot.x != TF_HAND_ANGLE_H){
						left_hand_rot.x -= 30;
					}
					
					if (left_shoulder_rot.z != TF_SHOULDER_ROT_H)
						left_shoulder_rot.z -= 10;
					else{
						state = sHUMANOID;
						prevState = sTFone;
					}
				}
				
				glRotatef(left_shoulder_rot.x, 1, 0, 0);
				glRotatef(left_shoulder_rot.y, 0, 1, 0);
				glRotatef(left_shoulder_rot.z, 0, 0, 1);
				glCallList(left_upper_arm);
				glPushMatrix();
					glTranslatef(0, -upper_arm_size.y, 0);
					
					if (state == sTFthree && prevState == sTFtwo){
						if (left_elbow_angle != TF_ELBOW_ANGLE_V)
							left_elbow_angle += 10;
						else{
							state = sTFfour;
							prevState = sTFthree;
						}
					}
					if (state == sTFtwo && prevState == sTFthree){
						if (left_elbow_angle != TF_ELBOW_ANGLE_H)
							left_elbow_angle -= 10;
						else{
							state = sTFone;
							prevState = sTFtwo;
						}
					}
					
					glRotatef(left_elbow_angle, 1, 0, 0); // Elbow rotation
					glCallList(left_lower_arm);
					glPushMatrix();
						glTranslatef(0, -lower_arm_size.y-hand_size.y/2, hand_size.z/2);
						
						glTranslatef(0,0,-0.06);
						//~ glRotatef(45, 1, 0, 0);
						glRotatef(left_hand_rot.y, 0, 1, 0);
						glCallList(left_hand);
						glRotatef(left_hand_rot.x, 1, 0, 0);
						glRotatef(left_hand_rot.z, 0, 0, 1);
						glTranslatef(0,0,0.06);
						glPushMatrix();
							glCallList(blade_base);
							glPushMatrix();
								glTranslatef(0.04,0,0);
								glRotatef(blade_gap/2,0,0,1);
								glRotatef(blade_direction,0,1,0);
								glCallList(blade);
							glPopMatrix();
							glPushMatrix();
								glTranslatef(-0.04,0,0);
								glRotatef(-blade_gap/2,0,0,1);
								glCallList(blade);
							glPopMatrix();
						glPopMatrix();
					glPopMatrix();
				glPopMatrix();
			glPopMatrix();
			
		glPopMatrix();
		
		//~ Draw Lower limbs
		glPushMatrix();
			glTranslatef(0, -lower_torso_size.y/2, 0);
			
			//~ Draw right - thigh, leg, foot
			glPushMatrix();
			
				if (state == sTFfour && prevState == sTFthree){
					if (right_knee_angle != TF_KNEE_ANGLE_V)
						right_knee_angle += 10;
					
					if (tf_right_leg_translt.x <= (lower_torso_size.x/2 - thigh_size.x))
						tf_right_leg_translt.x += 0.05;
					
					if(right_foot_rot.x < TF_FOOT_ANGLE_V){
						right_foot_rot.x += 10;
					}
				}
				if (state == sTFthree && prevState == sTFfour){
					if (right_knee_angle != TF_KNEE_ANGLE_H)
						right_knee_angle -= 10;
					
					if (tf_right_leg_translt.x > 0)
						tf_right_leg_translt.x -= 0.05;
					
					if(right_foot_rot.x >= TF_FOOT_ANGLE_H){
						right_foot_rot.x -= 10;
					}
				}
				
				glTranslatef(tf_right_leg_translt.x, 0, 0);
				glTranslatef(-lower_torso_size.x/2+thigh_size.x/2, 0, 0);
				//~ glTranslatef(-thigh_size.x/2, 0, 0);
				glRotatef(right_hip_rot.x, 1, 0, 0);
				glRotatef(right_hip_rot.y, 0, 1, 0);
				glRotatef(right_hip_rot.z, 0, 0, 1);
				glCallList(right_thigh);
				glPushMatrix();
					glTranslatef(0, -thigh_size.y, 0);
					
					
					glRotatef(right_knee_angle, 1, 0, 0);
					glCallList(right_leg);
					glPushMatrix();
						glTranslatef(0, -leg_size.y-foot_size.y/2, foot_size.z/2-leg_size.z/2);
						glRotatef(right_foot_rot.x, 1, 0, 0);
						glRotatef(right_foot_rot.y, 0, 1, 0);
						glRotatef(right_foot_rot.z, 0, 0, 1);
						glCallList(right_foot);
						
						//~ glTranslatef(0,foot_fin.x/2,0);
						//~ glRotatef(90,0,0,-1);
						//~ glTranslatef(0,0,-foot_size.z/2);
						//~ glRotatef(90,0,-1,0);
						//~ glTranslatef(foot_size.x/2,0,0);
						glTranslatef(-foot_size.x/2, -foot_size.y/2, foot_size.z/2);
						glRotatef(90,0,0,-1);
						glCallList(right_foot_wing);
					glPopMatrix();
				glPopMatrix();
			glPopMatrix();
			
			//~ Draw left - thigh, leg, foot
			glPushMatrix();
			
				if (state == sTFfour && prevState == sTFthree){
					if (left_knee_angle != TF_KNEE_ANGLE_V)
						left_knee_angle += 10;
					
					if (tf_left_leg_translt.x >= -(lower_torso_size.x/2 - thigh_size.x))
						tf_left_leg_translt.x -= 0.05;
					
					if(left_foot_rot.x < TF_FOOT_ANGLE_V){
						left_foot_rot.x += 10;
					}
				}
				if (state == sTFthree && prevState == sTFfour){
					if (left_knee_angle != TF_KNEE_ANGLE_H)
						left_knee_angle -= 10;
					
					if (tf_left_leg_translt.x < 0)
						tf_left_leg_translt.x += 0.05;
					
					if(left_foot_rot.x >= TF_FOOT_ANGLE_H){
						left_foot_rot.x -= 10;
					}
				}				

				glTranslatef(tf_left_leg_translt.x, 0, 0);
				glTranslatef(lower_torso_size.x/2-thigh_size.x/2, 0, 0);
				//~ glTranslatef(+thigh_size.x/2, 0, 0);
				glRotatef(left_hip_rot.x, 1, 0, 0);
				glRotatef(left_hip_rot.y, 0, 1, 0);
				glRotatef(left_hip_rot.z, 0, 0, 1);
				glCallList(left_thigh);
				glPushMatrix();
					glTranslatef(0, -thigh_size.y, 0);
					
					
					glRotatef(left_knee_angle, 1, 0, 0);
					glCallList(left_leg);
					glPushMatrix();
						glTranslatef(0, -leg_size.y-foot_size.y/2, foot_size.z/2-leg_size.z/2);
						glRotatef(left_foot_rot.x, 1, 0, 0);
						glRotatef(left_foot_rot.y, 0, 1, 0);
						glRotatef(left_foot_rot.z, 0, 0, 1);
						glCallList(left_foot);
						
						//~ glTranslatef(0,foot_fin.x/2,0);
						//~ glRotatef(90,0,0,1);
						//~ glTranslatef(0,0,-foot_size.z/2);
						//~ glRotatef(90,0,1,0);
						//~ glTranslatef(-foot_size.x/2,0,0);
						
						glTranslatef(foot_size.x/2, -foot_size.y/2, foot_size.z/2);
						glRotatef(90,0,0,1);
						glCallList(left_foot_wing);
					glPopMatrix();
				glPopMatrix();
			glPopMatrix();
			
		glPopMatrix();
	glPopMatrix();
}

void initGL(void){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glEnable(GL_CULL_FACE); 
	
	//~ glEnable(GL_NORMALIZE);
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
	gluLookAt(0,0,2, 0, 0, 0, 0, 1,0);
	
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
	
	draw_robot();
	//~ drawSphere(0.5,60);
	//~ drawTetrahedron(1);
	
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
	//!Close the window if the ESC key was pressed
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	//~ Change Viewpoint with Arrow Keys
	else if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
		y_angle += 10;
	else if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
		y_angle -= 10;
	else if (key == GLFW_KEY_UP && action == GLFW_PRESS)
		x_angle -= 10;
	else if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
		x_angle += 10;
	
	//~ Rotate Elbows and Knees
	else if (key == GLFW_KEY_1 && action == GLFW_PRESS)
		right_elbow_angle += 10;
	else if (key == GLFW_KEY_2 && action == GLFW_PRESS)
		right_elbow_angle -= 10;
	else if (key == GLFW_KEY_3 && action == GLFW_PRESS)
		left_elbow_angle += 10;
	else if (key == GLFW_KEY_4 && action == GLFW_PRESS)
		left_elbow_angle -= 10;
	else if (key == GLFW_KEY_5 && action == GLFW_PRESS)
		right_knee_angle += 10;
	else if (key == GLFW_KEY_6 && action == GLFW_PRESS)
		right_knee_angle -= 10;
	else if (key == GLFW_KEY_7 && action == GLFW_PRESS)
		left_knee_angle += 10;
	else if (key == GLFW_KEY_8 && action == GLFW_PRESS)
		left_knee_angle -= 10;
		
	//~ Rotate neck and head
	else if (key == GLFW_KEY_Q && action == GLFW_PRESS)
		neck_rot.x += 10;
	else if (key == GLFW_KEY_W && action == GLFW_PRESS)
		neck_rot.x -= 10;
	else if (key == GLFW_KEY_A && action == GLFW_PRESS)
		neck_rot.y += 10;
	else if (key == GLFW_KEY_S && action == GLFW_PRESS)
		neck_rot.y -= 10;
	else if (key == GLFW_KEY_Z && action == GLFW_PRESS)
		neck_rot.z += 10;
	else if (key == GLFW_KEY_X && action == GLFW_PRESS)
		neck_rot.z -= 10;
	
	//~ Rotate Right shoulder
	else if (key == GLFW_KEY_E && action == GLFW_PRESS)
		right_shoulder_rot.x -= 10;
	else if (key == GLFW_KEY_D && action == GLFW_PRESS)
		right_shoulder_rot.y -= 10;
	else if (key == GLFW_KEY_C && action == GLFW_PRESS)
		right_shoulder_rot.z -= 10;
	
	//~ Rotate left shoulder	
	else if (key == GLFW_KEY_R && action == GLFW_PRESS)
		left_shoulder_rot.x -= 10;
	else if (key == GLFW_KEY_F && action == GLFW_PRESS)
		left_shoulder_rot.y -= 10;
	else if (key == GLFW_KEY_V && action == GLFW_PRESS)
		left_shoulder_rot.z -= 10;
		
	//~ Rotate right hip	
	else if (key == GLFW_KEY_T && action == GLFW_PRESS)
		right_hip_rot.x -= 10;
	else if (key == GLFW_KEY_G && action == GLFW_PRESS)
		right_hip_rot.y -= 10;
	else if (key == GLFW_KEY_B && action == GLFW_PRESS)
		right_hip_rot.z -= 10;
		
	//~ Rotate left hip	
	else if (key == GLFW_KEY_Y && action == GLFW_PRESS)
		left_hip_rot.x -= 10;
	else if (key == GLFW_KEY_H && action == GLFW_PRESS)
		left_hip_rot.y -= 10;
	else if (key == GLFW_KEY_N && action == GLFW_PRESS)
		left_hip_rot.z -= 10;
		
	//~ Rotate right hand	
	else if (key == GLFW_KEY_U && action == GLFW_PRESS)
		right_hand_rot.x -= 10;
	else if (key == GLFW_KEY_J && action == GLFW_PRESS)
		right_hand_rot.y -= 10;
	else if (key == GLFW_KEY_M && action == GLFW_PRESS)
		right_hand_rot.z -= 10;
		
	//~ Rotate left hand	
	else if (key == GLFW_KEY_I && action == GLFW_PRESS)
		left_hand_rot.x -= 10;
	else if (key == GLFW_KEY_K && action == GLFW_PRESS)
		left_hand_rot.y -= 10;
	else if (key == GLFW_KEY_COMMA && action == GLFW_PRESS)
		left_hand_rot.z -= 10;
		
	//~ Rotate right foot	
	else if (key == GLFW_KEY_O && action == GLFW_PRESS)
		right_foot_rot.x -= 10;
	else if (key == GLFW_KEY_L && action == GLFW_PRESS)
		right_foot_rot.y -= 10;
	else if (key == GLFW_KEY_PERIOD && action == GLFW_PRESS)
		right_foot_rot.z -= 10;
		
	//~ Rotate left foot	
	else if (key == GLFW_KEY_P && action == GLFW_PRESS)
		left_foot_rot.x -= 10;
	else if (key == GLFW_KEY_SEMICOLON && action == GLFW_PRESS)
		left_foot_rot.y -= 10;
	else if (key == GLFW_KEY_SLASH && action == GLFW_PRESS)
		left_foot_rot.z -= 10;
	
	//~ Transform	
	else if (key == GLFW_KEY_TAB && action == GLFW_PRESS){
		if (state == sHUMANOID){
			reset_angles_H();
			transform();
		}
		else if (state == sVEHICLE){
			reset_angles_V();
			untransform();
		}
	}
	
}

int main(int argc, char** argv){
	//! The pointer to the GLFW window
	GLFWwindow* window;

	//! Setting up the GLFW Error callback
	glfwSetErrorCallback(csX75::error_callback);

	//! Initialize GLFW
	if (!glfwInit())
	return -1;

	//! Create a windowed mode window and its OpenGL context
	window = glfwCreateWindow(800, 800, "CS475 Assignment 2.1 | 10D070048 | 10D070001", NULL, NULL);
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
		
		if ((state == sHUMANOID) || (state == sVEHICLE)){
			// Poll for and process events
			glfwPollEvents();
		} else {
			usleep(TF_DELAY_uS);
			//~ right_hip_rot.set(20, 50, 82);
		}
	}

	glfwTerminate();
	return 0;
}

