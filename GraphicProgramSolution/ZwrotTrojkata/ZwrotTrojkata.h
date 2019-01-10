#pragma once
#ifndef ZwrotTrojkata_H
#define ZwrotTrojkata_H


#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/vec3.hpp>
#include <math.h>

GLuint VAO;
GLuint VBO;

GLuint rendering_program;
class ZwrotTrojkata
{
private:
	int wysokoscOkna;
	int szerokoscOkna;
	int polozenieOknaX;
	int polozenieOknaY;

public:
	ZwrotTrojkata(void);
	ZwrotTrojkata(int wysokoscOkna, int szerokoscOkna, int polozenieOknaX, int polozenieOknaY);
	~ZwrotTrojkata(void);
	void stworzenieOkna(int argc, char** argv);
	void inicjalizacjaGlew();
	static void wyswietl();
	static void usun();
	void stworzenieVAO();
	void stworzenieVBO();
	void sprawdzenieWersji();
	GLuint compile_shaders(void);
	void jakiZwrot(glm::vec3 wierzcholki[3]);
};

#endif /* ZwrotTrojkata_H */