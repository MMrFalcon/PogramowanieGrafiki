#include "kwadrat.h"

Kwadrat::Kwadrat(void)
{
	wysokoscOkna = 768;
	szerokoscOkna = 1024;
	polozenieOknaX = 100;
	polozenieOknaY = 100;
}

Kwadrat::Kwadrat(int wysokoscOkna, int szerokoscOkna, int polozenieOknaX, int polozenieOknaY)
{
	this->wysokoscOkna = wysokoscOkna;
	this->szerokoscOkna = szerokoscOkna;
	this->polozenieOknaX = polozenieOknaX;
	this->polozenieOknaY = polozenieOknaY;
}

Kwadrat::~Kwadrat()
{

}

void Kwadrat::stworzenieOkna(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(szerokoscOkna, wysokoscOkna);
	glutInitWindowPosition(polozenieOknaX, polozenieOknaY);
	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow("Kwadrat");
}

void Kwadrat::inicjalizacjaGlew()
{
	GLenum wynik = glewInit();
	if (wynik != GLEW_OK)
	{
		std::cerr << "Nie udalo sie zainicjalizowac GLEW. Blad: " << glewGetErrorString(wynik) << std::endl;
		system("pause");
		exit(1);
	}
}



void Kwadrat::usun()
{
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
	glDeleteProgram(rendering_program);
}

void Kwadrat::stworzenieVAO()
{
	rendering_program = compile_shaders(); //kompilacja shadera
	glGenVertexArrays(1, &VAO); //dzia³a jak glGenBuffers, ale na wierzcho³ki, zapamietuje wszystkie vbo oraz ich layaut
	std::cout << "VAO: " << VAO << std::endl;
	glBindVertexArray(VAO); //uaktywanie wierzcho³ki
}

void Kwadrat::stworzenieVBO() //tworzy obiekty buffora oraz definiuje jego zawartosc
{
	glm::vec3 Wierzcholki[4]{
		glm::vec3(-0.5f,  0.5f, 1.0f),
		glm::vec3(0.5f,  0.5f, 0.0f),
		glm::vec3(0.5f,  -0.5f, 0.0f),
		glm::vec3(-0.5f,  -0.5f, 1.0f)
	};


	glGenBuffers(1, &VBO); //zapisuje ile jest wolnych buforów, podajemy ile chcemy buforów 

	glBindBuffer(GL_ARRAY_BUFFER, VBO); //podajemy cel, czyli co chcemy z t¹ zmienn¹ zrobiæ (chcemy zapisac do tablicy)
	std::cout << "VBO: " << VBO << std::endl;
	glBufferData(GL_ARRAY_BUFFER, sizeof(Wierzcholki), Wierzcholki, GL_STATIC_DRAW); //podajemy rozmiar, tablice, i co chcemy z tymi wierzcho³kami zrobiæ


	GLuint elements[] = {
			0, 1, 2,
			2, 3, 0
	};

	glGenBuffers(1, &EBO); //generowaie buffa dla tablicy elemntow (Elements Buffer Object)

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);
}

void  Kwadrat::wyswietl()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnableVertexAttribArray(0); //uaktywniamy index

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0); //po kolei: indeks, ile wymiarów, typ danych, normalizacja

	glUseProgram(rendering_program); //u¿ycie shadera fragmentow do pokolorowania

	//glDrawArrays(GL_TRIANGLES, 0, 3); //co chcemy rysowac (Kwadraty), index poczatkowy w wlaczonych tablicach
								   //liczba indexow do renderowania

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray(0);

	glutSwapBuffers();
}


GLuint Kwadrat::compile_shaders(void)
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

void Kwadrat::sprawdzenieWersji()
{
	std::cout << "Wersja GLEW: " << glewGetString(GLEW_VERSION) << std::endl;
	std::cout << "Wersja VENDOR: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "Wersja REDERER: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "Wersja GL: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "Wersja GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
}



int main(int argc, char** argv)
{
	Kwadrat Kwadrat(786, 786, 100, 100);

	Kwadrat.stworzenieOkna(argc, argv);
	Kwadrat.inicjalizacjaGlew();
	Kwadrat.sprawdzenieWersji();
	Kwadrat.stworzenieVAO();
	Kwadrat.stworzenieVBO();
	glutDisplayFunc(Kwadrat.wyswietl);

	glutCloseFunc(Kwadrat.usun);

	glClearColor(0.2f, 0.1f, 0.0f, 0.0f); //kolor okna


	glutMainLoop();

	//Odpowiedzia na pytanie jest - mozna stworzyæ kwadrat korzystajac z 6 wierzcholkow (potrzebne sa dwa trojkaty) lub 
	//uzywajac 4 wierzcholkow oraz dodatkowego buffora odpowiadajacego za kolejnosc wyswietlania wierzcholkow
	//co w rezultacie pozwolilo znacznie zaoszczedzic pamiec

	return 0;
}