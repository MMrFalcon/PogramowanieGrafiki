#pragma once
#ifndef WieleFigur_H
#define WieleFigur_H


#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/vec3.hpp>

GLuint VAO;
GLuint VBO;
GLuint EBO;
GLuint TROJKAT;
GLuint KOLO;
GLuint rendering_program;
class WieleFigur
{
private:
	int wysokoscOkna;
	int szerokoscOkna;
	int polozenieOknaX;
	int polozenieOknaY;

public:
	WieleFigur(void);
	WieleFigur(int wysokoscOkna, int szerokoscOkna, int polozenieOknaX, int polozenieOknaY);
	~WieleFigur(void);
	void stworzenieOkna(int argc, char** argv);
	void inicjalizacjaGlew();
	static void wyswietl();
	static void usun();
	void stworzenieVAO();
	void stworzenieVBO();
	void sprawdzenieWersji();
	GLuint compile_shaders(void);
};

#endif /* WieleFigur_H */