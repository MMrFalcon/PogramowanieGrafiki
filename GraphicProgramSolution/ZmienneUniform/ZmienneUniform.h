#pragma once
#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/vec3.hpp>
#include <vector>


GLuint VAO;
GLuint VBO;
GLuint vertexShaderId;
GLuint fragmentShaderId;
GLuint programZShaderami;
GLint zmianaShader;

class ZmienneUniform
{
private:
	int wysokoscOkna;
	int szerokoscOkna;
	int polozenieOknaX;
	int polozenieOknaY;

public:
	ZmienneUniform(void);
	ZmienneUniform(int wysokoscOkna, int szerokoscOkna, int polozenieOknaX, int polozenieOknaY);
	~ZmienneUniform(void);
	void stworzenieOkna(int argc, char** argv);
	void inicjalizacjaGlew();
	static void wyswietl();
	static void usun();
	void stworzenieVAO();
	void stworzenieVBO();
	void stworzenieProgramu();
	GLuint dodanieDoProgramu(GLuint programZShaderami, const char * tekstShadera, GLenum typShadera);
	void sprawdzenieWersji();
};