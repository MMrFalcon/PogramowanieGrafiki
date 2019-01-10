#include "ZwrotTrojkata.h"

ZwrotTrojkata::ZwrotTrojkata(void)
{
	wysokoscOkna = 768;
	szerokoscOkna = 1024;
	polozenieOknaX = 100;
	polozenieOknaY = 100;
}

ZwrotTrojkata::ZwrotTrojkata(int wysokoscOkna, int szerokoscOkna, int polozenieOknaX, int polozenieOknaY)
{
	this->wysokoscOkna = wysokoscOkna;
	this->szerokoscOkna = szerokoscOkna;
	this->polozenieOknaX = polozenieOknaX;
	this->polozenieOknaY = polozenieOknaY;
}

ZwrotTrojkata::~ZwrotTrojkata()
{

}

void ZwrotTrojkata::stworzenieOkna(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(szerokoscOkna, wysokoscOkna);
	glutInitWindowPosition(polozenieOknaX, polozenieOknaY);
	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow("ZwrotTrojkata");
}

void ZwrotTrojkata::inicjalizacjaGlew()
{
	GLenum wynik = glewInit();
	if (wynik != GLEW_OK)
	{
		std::cerr << "Nie udalo sie zainicjalizowac GLEW. Blad: " << glewGetErrorString(wynik) << std::endl;
		system("pause");
		exit(1);
	}
}



void ZwrotTrojkata::usun()
{
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
	glDeleteProgram(rendering_program);
}

//czesc pierwsza zadania -  sprawdzenie wzorem
void ZwrotTrojkata::jakiZwrot(glm::vec3 wierzcholki[3]) {
	double suma = 0.0f;
	double wynik = 0.0f;
	int j = 0;
	for (int i = 0; i < 3; i++) {
		j = i + 1;
		if (j == 3) {
			j = 0;
		}
		
		suma += (wierzcholki[i].x * (fmod(wierzcholki[j].y, 3)) - (fmod(wierzcholki[j].x,3) * wierzcholki[i].y));
		std::cout<<"wierzcholek x "<<wierzcholki[i].x<<" Wierzcholek y "<< wierzcholki[i].y << "WYNIK1: " << suma << std::endl;
	}

	wynik = suma * 0.5f;

	if (wynik > 0) {
		std::cout << "wynik dodatni " << wynik << " w przypadku domyslnego ccw trojkat jest zwrocony przodem, jesli cw tylem" << std::endl;
	}
	else {
		std::cout << "wynik ujemny " << wynik << " w przypadku domyslnego ccw trojkat jest zwrocony tylem, jesli cw przodem" << std::endl;
	}

}

void ZwrotTrojkata::stworzenieVAO()
{
	rendering_program = compile_shaders(); //kompilacja shadera
	glGenVertexArrays(1, &VAO); //dzia³a jak glGenBuffers, ale na wierzcho³ki, zapamietuje wszystkie vbo oraz ich layaut
	std::cout << "VAO: " << VAO << std::endl;
	glBindVertexArray(VAO); //uaktywanie wierzcho³ki
}

void ZwrotTrojkata::stworzenieVBO() //tworzy obiekty buffora oraz definiuje jego zawartosc
{
	//wyswietla dla cw RYSUJEMY zgodnie ze wskazowkami zegara
	glm::vec3 Wierzcholki[3]{
		glm::vec3(-0.5f,  0.3f, 1.0f),
		glm::vec3(0.2f,  0.5f, 0.0f),
		glm::vec3(0.5f,  -0.5f, 0.0f)
	};

	//wyswietla dla domyslnego ccw RYSUJEMY przeciwnie do wskazowek zegara
	/*glm::vec3 Wierzcholki[3]{
		glm::vec3(-0.5f,  0.3f, 1.0f),
		glm::vec3(0.5f,  -0.5f, 0.0f),
		glm::vec3(0.2f,  0.5f, 0.0f)
	
	};*/

	glGenBuffers(1, &VBO); //zapisuje ile jest wolnych buforów, podajemy ile chcemy buforów 



	glBindBuffer(GL_ARRAY_BUFFER, VBO); //podajemy cel, czyli co chcemy z t¹ zmienn¹ zrobiæ (chcemy zapisac do tablicy)
	std::cout << "VBO: " << VBO << std::endl;
	glBufferData(GL_ARRAY_BUFFER, sizeof(Wierzcholki), Wierzcholki, GL_STATIC_DRAW); //podajemy rozmiar, tablice, i co chcemy z tymi wierzcho³kami zrobiæ

	jakiZwrot(Wierzcholki);
}

void  ZwrotTrojkata::wyswietl()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnableVertexAttribArray(0); //uaktywniamy index

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0); //po kolei: indeks, ile wymiarów, typ danych, normalizacja
	

	glUseProgram(rendering_program); //u¿ycie shadera fragmentow do pokolorowania

	/*Do zadania podpunkt2*/
	glFrontFace(GL_CW);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	/*Do zadania podpunkt2*/

	glDrawArrays(GL_TRIANGLES, 0, 3); //co chcemy rysowac (ZwrotTrojkatay), index poczatkowy w wlaczonych tablicach
								   //liczba indexow do renderowania

	

	glDisableVertexAttribArray(0);

	glutSwapBuffers();
}


GLuint ZwrotTrojkata::compile_shaders(void)
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

void ZwrotTrojkata::sprawdzenieWersji()
{
	std::cout << "Wersja GLEW: " << glewGetString(GLEW_VERSION) << std::endl;
	std::cout << "Wersja VENDOR: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "Wersja REDERER: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "Wersja GL: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "Wersja GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
}



int main(int argc, char** argv)
{
	ZwrotTrojkata ZwrotTrojkata(786, 786, 100, 100);

	ZwrotTrojkata.stworzenieOkna(argc, argv);
	ZwrotTrojkata.inicjalizacjaGlew();
	ZwrotTrojkata.sprawdzenieWersji();
	ZwrotTrojkata.stworzenieVAO();
	ZwrotTrojkata.stworzenieVBO();
	glutDisplayFunc(ZwrotTrojkata.wyswietl);

	glutCloseFunc(ZwrotTrojkata.usun);

	glClearColor(0.2f, 0.1f, 0.0f, 0.0f); //kolor okna


	glutMainLoop();


	return 0;
}