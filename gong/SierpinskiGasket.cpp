#include <iostream>
#include <string.h>
#include <Math.h>
#include <GL/glew.h>
#include "gl/glut.h"
#include "GL/freeglut.h"
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

using namespace glm;

#define BUFFER_OFSET(offset) ((GLvoid*) (offset))

GLuint InitShader(const char* vShaderFile, const char* fShaderFile);

const int NumTimesToSubdivide = 5;
const int NumTriangles = 729;
const int NumVertices = 3 * NumTriangles;

glm::vec2 points[NumVertices];

void display(void) {
	glClear(GL_COLOR_BUFFER_BIT);
	glDrawArrays(GL_TRIANGLES, 0, NumTriangles);
	glFlush();
}

void triangle(vec2 a, vec2 b, vec2 c) {
	static int i = 0;
	points[i++] = a;
	points[i++] = b;
	points[i++] = c;
}

void divide_triangle(vec2 a, vec2 b, vec2 c, int m) {
	glm::vec2 ab, ac, bc;

	if (m > 0) {
		ab = (a + b) / 2.0f;
		ac = (a + c) / 2.0f;
		bc = (b + c) / 2.0f;

		divide_triangle(a, ab, ac, m - 1);
		divide_triangle(c, ac, bc, m - 1);
		divide_triangle(b, bc, ab, m - 1);
	}
	else
		triangle(a, b, c); /* draw triangle at end of recursion */
}


void init(void) {
	// glm/vec2.hpp include해야 함
	glm::vec2 vertices[3] = {
		glm::vec2(-1.0, -1.0),
		glm::vec2(0.0, 1.0),
		glm::vec2(1.0, -1.0)
	};

	divide_triangle(vertices[0], vertices[1], vertices[2], NumTimesToSubdivide);

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), NULL, GL_STATIC_DRAW);
	//load data separately
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points), points);


	GLuint program = InitShader("vshader.glsl", "fshader.glsl");
	//GLuint myprogObj = glCreateProgram();
	glLinkProgram(program);
	//glUseProgram(myprogObj);

	GLuint loc = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(loc);
	glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glClearColor(1.0, 1.0, 1.0, 1.0);

}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA);
	glutInitWindowSize(512, 512);
	glutCreateWindow("Sierpinski Gasket");

	glewInit();

	init();

	glutDisplayFunc(display);

	glutMainLoop();

	return EXIT_SUCCESS;
}