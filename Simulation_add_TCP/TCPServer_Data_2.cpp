#include <vgl.h>
#include <InitShader.h>
#include <mat.h>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <Eigen/SparseCore>
#include <Eigen/IterativeLinearSolvers>
#include <Eigen/Dense>
#include <chrono>
#include <iostream>
#include <Windows.h>
#include <thread>


#include "simulation/2D/header_background/grid.h"
#include "simulation/2D/header_background/drawgrid.h"
#include "simulation/2D/Simulation/fluid_grid_2D.h"
#include "simulation/2D/header_background/box.h"
#include "Network/Simulation/TCPServer_Data_multithread_to_h.h"
#include "Network/Simulation/Serialization.h"


using namespace std;


//window size
int Width = 1024;
int Height = 1024;

GLuint model_view; // model-view matrix uniform shader variable location

					//Projection transformation parameters
GLuint projection; //projection matrix uniform shader variable location

GLuint vao;
GLuint vbo;

//time
int time_idle;

//simulation ����
Fluid_Simulator_Grid* simulation;

//n����
int n = 12;

//number�� �����ϸ� particle ���� �ٲ��.
int number = 8000;

vector<Vector2D>* points;
vector<vec3>* color;
Box bbox;
vector<Vector2D> grid_line;
vector<Vector2D> box_line;

//grid_N�� �����ϸ� grid ���� �ٲ��.
int grid_N = 40;

//�ùķ��̼� ���¸� ������ option
int Option = 3;

bool isStart = false;
bool isPixelMode = false;
bool isParticleMode = true;
bool isExtrapolationCell = false;

//�����峢���� ���


//========================================================particle mode ========================================================//

//simulation�� particle���� ��ġ�� points�� ����
void pushback_SimulationPoints_to_Points() {
	points->clear();
	for (int i = 0; i < number; i++) {
		points->push_back(simulation->particles[i].Location);
	}

}

void pushback_color() {
	color->clear();

	for (int i = 0; i < number; i++) {
		color->push_back(vec3(0.0f, 0.0f, 0.0f));
	}

	for (int i = 0; i < 4 + 4 * (grid_N - 1); i++) {
		color->push_back(vec3(0.0f, 0.0f, 0.0f));
	}

	//fluid mode�� blue
	for (int i = 0; i < simulation->fluid_cell_center_point->size(); i++) {
		color->push_back(vec3(0.0f, 0.0f, 0.0f));
	}

}

void Update_Points() {
	for (int i = 0; i < simulation->particles.size(); i++) {
			(*points)[i] = simulation->particles[i].Location;
	}
}

//========================================================particle mode ========================================================//



//========================================================circle mode========================================================//
//�� n������ ��ǥ ����

void pushback_Circle_points() {
	Vector2D location;
	points->clear();
	//j��° particle
	for (int j = 0; j < number; j++) {
		location = simulation->particles[j].Location;
		for (int i = 0; i < n; i++) {
			int theta = 360 / n;
			double x = location.X + 0 * cos(theta * i);
			double y = location.Y + 0 * sin(theta * i);

			Vector2D new_location = Vector2D(x, y);

			points->push_back(new_location);
		}
	}
}

void Update_Circle_points() {
	Vector2D location;
	//j��° particle
	for (int i = 0; i < number; i++) {
		location = simulation->particles[i].Location;
		for (int j = 0; j < n; j++) {
			int theta = 360 / n;
			double x = location.X + 0 * cos(theta * j);
			double y = location.Y + 0 * sin(theta * j);

			(*points)[ i * n + j ] = Vector2D(x, y);
		}
	}
}

void pushback_Circle_color() {
	color->clear();

	for (int i = 0; i < n * number; i++) {
		color->push_back(vec3(0.0f, 0.0f, 0.0f));
	}

	for (int i = 0; i < 4 + 4 * (grid_N - 1); i++) {
		color->push_back(vec3(0.0f, 0.0f, 0.0f));
	}

	//fluid mode�� blue
	for (int i = 0; i < simulation->fluid_cell_center_point->size(); i++) {
		color->push_back(vec3(0.0f, 0.0f, 0.0f));
	}
}

//========================================================circle mode========================================================//

void init(void) {

	points = new vector<Vector2D>();
	color = new vector<vec3>();
	
	//simulation ���� �� ���ڵ� ����
	simulation = new Fluid_Simulator_Grid(number, grid_N);

	pushback_SimulationPoints_to_Points();
	//pushback_Circle_points();

	pushback_color();
	//pushback_Circle_color();

	//bbox ����
	bbox = Box(0.0, 1.0, 0.0, 1.0);
	box_line.push_back( Vector2D(bbox.xmin, bbox.ymin) );
	box_line.push_back(Vector2D(bbox.xmax, bbox.ymin));
	box_line.push_back(Vector2D(bbox.xmax, bbox.ymax));
	box_line.push_back(Vector2D(bbox.xmin, bbox.ymax));

	//vector�� size�� ���ϴ� ũ�⸸ŭ �ø���.
	for (int i = 0; i < 4 * (grid_N - 1); i++) {
		grid_line.push_back(Vector2D(0, 0));
	}

	//grid�� ���� �׸��� �Լ�
	make_Points_of_grids(grid_line, grid_N);

	glGenVertexArrays(1, &(vao));
	glBindVertexArray(vao);

	glGenBuffers(1, &(vbo));
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(Vector2D) * number + sizeof(box_line) + sizeof(Vector2D) * grid_line.size() + sizeof(Vector2D) * color->size(), NULL, GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vector2D) * points->size() + sizeof(Vector2D) * box_line.size() + sizeof(Vector2D) * grid_line.size() + sizeof(Vector2D) * simulation->fluid_cell_center_point->size() + sizeof(Vector2D) * color->size(), NULL, GL_STATIC_DRAW);

	//particle�� ������
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vector2D) * points->size() , &( (*points)[0] ));
	//box ������
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(Vector2D) * points->size(), sizeof(Vector2D) * box_line.size() , &( box_line[0] ));
	//grid ������
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(Vector2D) * points->size() + sizeof(Vector2D) * box_line.size(), sizeof(Vector2D) * grid_line.size(), &(grid_line[0]) );

	//fluid cell center point ������
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(Vector2D) * points->size() + sizeof(Vector2D) * box_line.size() + sizeof(Vector2D)* grid_line.size(), sizeof(Vector2D) * simulation->fluid_cell_center_point->size(), &((*simulation->fluid_cell_center_point)[0]));

	////air cell center point ������
	//glBufferSubData(GL_ARRAY_BUFFER, sizeof(Vector2D) * points->size() + sizeof(box_line) + sizeof(Vector2D) * grid_line.size() + sizeof(Vector2D) * simulation->fluid_cell_center_point->size(), sizeof(Vector2D) * 
	//	simulation->air_cell_center_point->size(), &((*simulation->air_cell_center_point)[0]));

	//color �Ҵ�
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(Vector2D) * points->size() + sizeof(Vector2D) * box_line.size() + sizeof(Vector2D)* grid_line.size() + sizeof(Vector2D) * simulation->fluid_cell_center_point->size() , sizeof(Vector2D) * color->size(), &( (*color)[0]) );

	//load shaders
	GLuint program = InitShader("simulation/2D/src/vshader_2dBezier_test.glsl", "simulation/2D/src/fshader_2dBezier_test.glsl");
	glUseProgram(program);

	//points memory position ( points and box )
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_DOUBLE, GL_FALSE, 2 * sizeof(double), BUFFER_OFFSET(0));

	//color position
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_DOUBLE, GL_FALSE, 2 * sizeof(double), BUFFER_OFFSET(sizeof(Vector2D) * points->size() + sizeof(Vector2D) * box_line.size() + sizeof(Vector2D) * grid_line.size() + sizeof(Vector2D) * simulation->fluid_cell_center_point->size() ));

	glEnableVertexAttribArray(0);
	//initialize uniform variable from vertex shander
	model_view = glGetUniformLocation(program, "model_view");
	projection = glGetUniformLocation(program, "projection");

	glEnable(GL_DEPTH_TEST);
	glClearColor(1.0, 1.0, 1.0, 0.0);
}

void reshape(int width, int height)
{
	glViewport(0, 0, width, height);
}


void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 'q':
		simulation->delete_vectors();
		delete simulation;
		exit(EXIT_SUCCESS);
		break;

	}
	glutPostRedisplay();
}

void idle(void)
{
	////����
	//printf("\n__________________________________________________________________________________\n");
	//printf("__________________________________________________________________________________\n");
	//printf("__________________________________________________________________________________\n");
	//printf("__________________________________________________________________________________\n");
	//printf("__________________________________________________________________________________\n");

	//������ �и��� ���
	time_idle = glutGet(GLUT_ELAPSED_TIME);

	//0.06�� ���� particle ��ġ update
	if (time_idle % 120 == 0) {
		if (isStart) {
			//std::chrono::system_clock::time_point start = std::chrono::system_clock::now();

			mtx.lock();

			//particle
			simulation->particle_simulation();

			/*std::chrono::duration<double>sec = std::chrono::system_clock::now() - start;
			std::cout << "simulation �ɸ��� �ð�(��) : " << sec.count() << "seconds" << std::endl;*/

			Update_Points();
			pushback_color();

			//data send
			//1. point to char*
			size_t bufferSize = sizeof(Vector2D) * points->size();
			char* buffer = (char*)malloc(sizeof(char*) * bufferSize );
			SerializeVector2D(*points, buffer, bufferSize);

			mtx.unlock();
			
			////circle
			//simulation->particle_simulation();
			//Update_Circle_points();
		}
	}

	glutPostRedisplay();
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT);


	//ȭ�� mapping 
	mat4 p = Ortho2D(-1.0, 1.0, -1.0, 1.0); // ��� �׷��� ���
	glUniformMatrix4fv(projection, 1, GL_TRUE, p);

	//�ٲ� ��ǥ �ٽ� �޸𸮿� �ֱ�
	glBindVertexArray(vao);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vector2D) * points->size(), &( (*points)[0] ));
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(Vector2D) * points->size() + sizeof(Vector2D) * box_line.size() + sizeof(Vector2D) * grid_line.size(), sizeof(Vector2D) * simulation->fluid_cell_center_point->size(), &((*simulation->fluid_cell_center_point)[0]));
	/*glBufferSubData(GL_ARRAY_BUFFER, sizeof(Vector2D) * points->size() + sizeof(Vector2D) * box_line.size() + sizeof(Vector2D) * grid_line.size() + sizeof(Vector2D) * simulation->fluid_cell_center_point->size(), sizeof(Vector2D) *
		simulation->air_cell_center_point->size(), &((*simulation->air_cell_center_point)[0]));*/

	glPointSize(2.0);
	glLineWidth(1.0);
	
	if (isStart) {
		if (isParticleMode) { 
			//particle
			glDrawArrays(GL_POINTS, 0, points->size()); 

			////circle
			//for (int i = 0; i < number; i++) {
			//	glDrawArrays(GL_LINES, i * n, n );
			//}
		}
	}

	//box �׸���
	glLineWidth(0.1);
	glDrawArrays(GL_LINE_LOOP, points->size(), 4);

	//grid �׸���
	//glDrawArrays(GL_LINES, points->size() + 4, 4 * (grid_N-1));

	glPointSize(10.0);
	if (isStart) {
		if (isPixelMode) { 
			glDrawArrays(GL_POINTS, points->size() + 4 + 4 * (grid_N - 1), simulation->fluid_cell_center_point->size()); 
		}
		if (isExtrapolationCell) {
			//glLineWidth(0.05);
			//glDrawArrays(GL_LINES, points->size() + 4 + 4 * (grid_N - 1) + simulation->fluid_cell_center_point->size(), simulation->air_cell_center_point->size());

			glDrawArrays(GL_POINTS, points->size() + 4 + 4 * (grid_N - 1) + simulation->fluid_cell_center_point->size(), simulation->air_cell_center_point->size());
		}
	}

	glBindVertexArray(0);
	glutSwapBuffers();
}

void Menu(int Option) {

	switch (Option) {
		//particlemode
		case 0 :
			isParticleMode = true;
			isPixelMode = false;
			break;

		//fluidmode
		case 1 :
			isParticleMode = false;
			isPixelMode = true;
			break;

		//start
		case 2 : 
			isStart = true;
			break;

		//finish
		case 3 :
			isStart = false;
			simulation->clear_and_ReInit();
			break;

		//extra_visible
		case 4:
			isExtrapolationCell = !isExtrapolationCell;
			break;
		
	}
	
}

int main(int argc, char** argv) {

	thread t1(&TCP_mine);

	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(Width, Height);

	glutCreateWindow("2D Particle Simulation");

	glewInit();
	init();

	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);

	glutCreateMenu(Menu);
	glutAddMenuEntry("ParticleMode", 0);
	glutAddMenuEntry("PixelMode", 1);
	glutAddMenuEntry("Start", 2);
	glutAddMenuEntry("Finish", 3);
	glutAddMenuEntry("extra_visible", 4);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	glutMainLoop();

	t1.join();
	return 0;
}