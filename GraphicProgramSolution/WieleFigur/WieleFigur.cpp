#include "WieleFigur.h"

WieleFigur::WieleFigur(void)
{
	wysokoscOkna = 768;
	szerokoscOkna = 1024;
	polozenieOknaX = 100;
	polozenieOknaY = 100;
}

WieleFigur::WieleFigur(int wysokoscOkna, int szerokoscOkna, int polozenieOknaX, int polozenieOknaY)
{
	this->wysokoscOkna = wysokoscOkna;
	this->szerokoscOkna = szerokoscOkna;
	this->polozenieOknaX = polozenieOknaX;
	this->polozenieOknaY = polozenieOknaY;
}

WieleFigur::~WieleFigur()
{

}

void WieleFigur::stworzenieOkna(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(szerokoscOkna, wysokoscOkna);
	glutInitWindowPosition(polozenieOknaX, polozenieOknaY);
	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow("WieleFigur");
}

void WieleFigur::inicjalizacjaGlew()
{
	GLenum wynik = glewInit();
	if (wynik != GLEW_OK)
	{
		std::cerr << "Nie udalo sie zainicjalizowac GLEW. Blad: " << glewGetErrorString(wynik) << std::endl;
		system("pause");
		exit(1);
	}
}



void WieleFigur::usun()
{
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
	glDeleteProgram(rendering_program);
}

void WieleFigur::stworzenieVAO()
{
	rendering_program = compile_shaders(); //kompilacja shadera
	glGenVertexArrays(1, &VAO); //dzia³a jak glGenBuffers, ale na wierzcho³ki, zapamietuje wszystkie vbo oraz ich layaut
	std::cout << "VAO: " << VAO << std::endl;
	glBindVertexArray(VAO); //uaktywanie wierzcho³ki
}

void WieleFigur::stworzenieVBO() //tworzy obiekty buffora oraz definiuje jego zawartosc
{


	glm::vec3 WierzcholkiKwadrat[4]{
		glm::vec3(-0.1f,  0.1f, 0.4f),
		glm::vec3(0.1f,  0.1f, 0.0f),
		glm::vec3(0.1f,  -0.1f, 0.0f),
		glm::vec3(-0.1f,  -0.1f, 0.4f)
	};

	glm::vec3 WierzcholkiTrojkata[3]{
	    glm::vec3(-1.0f,  1.0f, 1.0f),
		glm::vec3(-0.8f,  1.0f, 0.0f),
		glm::vec3(-0.9f,  0.7f, 0.0f)
	};

	glm::vec3 KoloWierzcholki[361]{
		glm::vec3(-0.5f,  -0.5f, 1.0f), //srodek
		glm::vec3(-0.5f, -0.4f, 0.0f),
	};



	glGenBuffers(1, &VBO); //zapisuje ile jest wolnych buforów, podajemy ile chcemy buforów 
	glBindBuffer(GL_ARRAY_BUFFER, VBO); //podajemy cel, czyli co chcemy z t¹ zmienn¹ zrobiæ (chcemy zapisac do tablicy)
	std::cout << "VBO: " << VBO << std::endl;
	glBufferData(GL_ARRAY_BUFFER, sizeof(WierzcholkiKwadrat), WierzcholkiKwadrat, GL_STATIC_DRAW); //podajemy rozmiar, tablice, i co chcemy z tymi wierzcho³kami zrobiæ


	GLuint elements[] = {
			0, 1, 2,
			2, 3, 0
	};

	glGenBuffers(1, &EBO); //generowaie buffa dla tablicy elemntow (Elements Buffer Object)
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

	//generowanie buffora dla trojkatow
	glGenBuffers(1, &TROJKAT);
	glBindBuffer(GL_ARRAY_BUFFER, TROJKAT);
	glBufferData(GL_ARRAY_BUFFER, sizeof(WierzcholkiTrojkata), WierzcholkiTrojkata, GL_STATIC_DRAW);

	glGenBuffers(1, &KOLO);
	glBindBuffer(GL_ARRAY_BUFFER, KOLO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(KoloWierzcholki), KoloWierzcholki, GL_STATIC_DRAW);
}

void  WieleFigur::wyswietl()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnableVertexAttribArray(0); //uaktywniamy index
	glUseProgram(rendering_program); //u¿ycie shadera fragmentow do pokolorowania
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindBuffer(GL_ARRAY_BUFFER, TROJKAT);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	glBindBuffer(GL_ARRAY_BUFFER, KOLO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 362);

	glDisableVertexAttribArray(0);
	glutSwapBuffers();
}


GLuint WieleFigur::compile_shaders(void)
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

void WieleFigur::sprawdzenieWersji()
{
	std::cout << "Wersja GLEW: " << glewGetString(GLEW_VERSION) << std::endl;
	std::cout << "Wersja VENDOR: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "Wersja REDERER: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "Wersja GL: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "Wersja GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
}



int main(int argc, char** argv)
{
	WieleFigur WieleFigur(786, 786, 100, 100);

	WieleFigur.stworzenieOkna(argc, argv);
	WieleFigur.inicjalizacjaGlew();
	WieleFigur.sprawdzenieWersji();
	WieleFigur.stworzenieVAO();
	WieleFigur.stworzenieVBO();
	glutDisplayFunc(WieleFigur.wyswietl);

	glutCloseFunc(WieleFigur.usun);

	glClearColor(0.2f, 0.1f, 0.0f, 0.0f); //kolor okna


	glutMainLoop();

	return 0;
}