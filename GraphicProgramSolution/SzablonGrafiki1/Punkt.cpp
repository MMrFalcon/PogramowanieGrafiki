#include "punkt.h"


Punkt::Punkt(void)
{
	wysokoscOkna = 768;
	szerokoscOkna = 1024;
	polozenieOknaX = 100;
	polozenieOknaY = 100;
}

Punkt::Punkt(int wysokoscOkna, int szerokoscOkna, int polozenieOknaX, int polozenieOknaY)
{
	this->wysokoscOkna = wysokoscOkna;
	this->szerokoscOkna = szerokoscOkna;
	this->polozenieOknaX = polozenieOknaX;
	this->polozenieOknaY = polozenieOknaY;
}

Punkt::~Punkt()
{

}

void Punkt::stworzenieOkna(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(szerokoscOkna, wysokoscOkna);
	glutInitWindowPosition(polozenieOknaX, polozenieOknaY);
	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow("punkt");
}

void Punkt::inicjalizacjaGlew()
{
	GLenum wynik = glewInit();
	if (wynik != GLEW_OK)
	{
		std::cerr << "Nie udalo sie zainicjalizowac GLEW. Blad: " << glewGetErrorString(wynik) << std::endl;
		system("pause");
		exit(1);
	}
}



void Punkt::usun()
{
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
	glDeleteProgram(rendering_program);
}

void Punkt::stworzenieVAO()
{
	rendering_program = compile_shaders(); //kompilacja shadera
	glGenVertexArrays(1, &VAO); //dzia³a jak glGenBuffers, ale na wierzcho³ki, zapamietuje wszystkie vbo oraz ich layaut
	std::cout << "VAO: " << VAO << std::endl;
	glBindVertexArray(VAO); //uaktywanie wierzcho³ki
}

void Punkt::stworzenieVBO() //tworzy obiekty buffora oraz definiuje jego zawartosc
{
	//tu powstaja punkty (x,y,z)
	glm::vec3 Wierzcholki[30] = { }; 

	//pozycjonowanie wierzcholkow
	float x = -0.4f;
	float y = 0.5f;
	float z = 0.0f;


		for (int i = 0; i < 30; i++) {

			if (i % 10 == 0) {
				x = -0.5f;
				y += -0.2f;
				Wierzcholki[i] = glm::vec3(x, y, z);
			} else {
				Wierzcholki[i] = glm::vec3(x, y, z);
				x += 0.1f;
			}
		}
	
	glGenBuffers(1, &VBO); //zapisuje ile jest wolnych buforów, podajemy ile chcemy buforów 
	glBindBuffer(GL_ARRAY_BUFFER, VBO); //podajemy cel, czyli co chcemy z t¹ zmienn¹ zrobiæ
	std::cout << "VBO: " << VBO << std::endl;
	glBufferData(GL_ARRAY_BUFFER, sizeof(Wierzcholki), Wierzcholki, GL_STATIC_DRAW); //podajemy rozmiar, tablice, i co chcemy z tymi wierzcho³kami zrobiæ

}

GLuint Punkt::compile_shaders(void)
{
	GLuint fragment_shader;
	GLuint program;


	// Kod Ÿród³owy shadera fragmentów.
	static const GLchar * fragment_shader_source[] =
	{
		"#version 450 core \n"
		" \n"
		"out vec4 color; \n"
		" \n"
		"void main(void) \n"
		"{ \n"
		" color = vec4(1.0f, 1.0f, 1.0f, 1.0f); \n"
		"} \n"
	};

	// Utworzenie i kompilacja shadera fragmentów.
	fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, fragment_shader_source, NULL);
	glCompileShader(fragment_shader);

	// Utworzenie programu, dodanie shadera
	program = glCreateProgram();
	
	glAttachShader(program, fragment_shader);
	glLinkProgram(program);
	// Usuniêcie shadera, bo znajduj¹ siê ju¿ w programie.
	
	glDeleteShader(fragment_shader);

		return program;

}
void  Punkt::wyswietl()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnableVertexAttribArray(0); //uaktywniamy index

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0); //po kolei: indeks, ile wymiarów, typ danych, normalizacja
	glPointSize(wielkoscPunktu);
	
	glUseProgram(rendering_program); //u¿ycie shadera fragmentow do pokolorowania

	glDrawArrays(GL_POINTS, 0, 30); //co chcemy rysowac (punkty), index poczatkowy w wlaczonych tablicach
								   //liczba indexow do renderowania

	glDisableVertexAttribArray(0);
	
	glutSwapBuffers();
}

void Punkt::sprawdzenieWersji()
{
	std::cout << "Wersja GLEW: " << glewGetString(GLEW_VERSION) << std::endl;
	std::cout << "Wersja VENDOR: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "Wersja REDERER: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "Wersja GL: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "Wersja GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
}



int main(int argc, char** argv)
{
	Punkt punkt(786, 786, 100, 100);

	punkt.stworzenieOkna(argc, argv);
	punkt.inicjalizacjaGlew();
	punkt.sprawdzenieWersji();
    punkt.stworzenieVAO();
	punkt.stworzenieVBO();
	glutDisplayFunc(punkt.wyswietl);
	
	glutCloseFunc(punkt.usun);

	glClearColor(0.2f, 0.1f, 0.0f, 0.0f); //kolor okna


	glutMainLoop();



	return 0;
}
