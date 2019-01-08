#pragma once
#ifndef KWADRAT_H
#define KWADRAT_H


#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/vec3.hpp>

GLuint VAO;
GLuint VBO;
GLuint EBO; 
GLuint wielkoscKwadratu = 10;
GLuint rendering_program;
class Kwadrat
{
private:
	int wysokoscOkna;
	int szerokoscOkna;
	int polozenieOknaX;
	int polozenieOknaY;

public:
	Kwadrat(void);
	Kwadrat(int wysokoscOkna, int szerokoscOkna, int polozenieOknaX, int polozenieOknaY);
	~Kwadrat(void);
	void stworzenieOkna(int argc, char** argv);
	void inicjalizacjaGlew();
	static void wyswietl();
	static void usun();
	void stworzenieVAO();
	void stworzenieVBO();
	void sprawdzenieWersji();
	GLuint compile_shaders(void);
};

#endif /* KWADRAT_H */