#include "Shaders31.h"

Shaders::Shaders()
{
	wysokoscOkna = 768;
	szerokoscOkna = 1024;
	polozenieOknaX = 100;
	polozenieOknaY = 100;
}

Shaders::Shaders(int wysokoscOkna, int szerokoscOkna, int polozenieOknaX, int polozenieOknaY)
{
	this->wysokoscOkna = wysokoscOkna;
	this->szerokoscOkna = szerokoscOkna;
	this->polozenieOknaX = polozenieOknaX;
	this->polozenieOknaY = polozenieOknaY;
}

Shaders::~Shaders()
{

}

void Shaders::stworzenieOkna(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(szerokoscOkna, wysokoscOkna);
	glutInitWindowPosition(polozenieOknaX, polozenieOknaY);
	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow("Shaders");
}

void Shaders::inicjalizacjaGlew()
{
	GLenum wynik = glewInit();
	if (wynik != GLEW_OK)
	{
		std::cerr << "Nie udalo sie zainicjalizowac GLEW. Blad: " << glewGetErrorString(wynik) << std::endl;
		system("pause");
		exit(1);
	}
}

void  Shaders::wyswietl()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glDrawArrays(GL_TRIANGLES, 0, 3);

	glDisableVertexAttribArray(0);

	glutSwapBuffers();
}
void Shaders::usun()
{
	glDeleteShader(vertexShaderId);
	glDeleteShader(fragmentShaderId);
	glDeleteProgram(programZShaderami);
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
}
void Shaders::stworzenieVAO()
{
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
}

void Shaders::stworzenieVBO()
{
	glm::vec3 wierzcholek1 = glm::vec3(-1.0f, -1.0f, 0.0f);
	glm::vec3 wierzcholek2 = glm::vec3(1.0f, -1.0f, 0.0f);
	glm::vec3 wierzcholek3 = glm::vec3(0.0f, 1.0f, 0.0f);

	glm::vec3 wierzcholki[3] = {
		wierzcholek1,
		wierzcholek2,
		wierzcholek3
	};

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(wierzcholki), wierzcholki, GL_STATIC_DRAW);

}

void Shaders::stworzenieProgramu()
{
	programZShaderami = glCreateProgram();

	if (programZShaderami == 0)
	{
		std::cerr << "Blad podczas tworzenia progrmu shaderow." << std::endl;
		system("pause");
		exit(1);
	}

	//jako ze jest mnozone przez 0,5 to do polozenia x dodaje 2.0 - gdyby nie bylo mnozenia to bym dodal do x 1.0
	//dodawanie komentarzy niemozliwe "//" psuje program brak komunikatow o bledzie podczas kompilacji shadera
	//tylko niektore logi sa zbierane np glGetProgramInfoLog() zbiera o kompilacji shadera jesli uzyjemy czegos czego niepowinnismy
	static const GLchar *  vertexShader = {
		"	#version 330 core\n																	\
			in layout(location=0) vec3 polozenie;												\
			void main()																			\
			{																					\
				gl_Position = vec4(0.5 * (polozenie.x + 2.0), 0.5 * polozenie.y, polozenie.z, 1.0);		\
			}"
	};

	static const GLchar * fragmentShader = {
		"	#version 330 core\n							\
			out vec4 kolor;								\
			void main()									\
			{											\
				kolor = vec4(0.8, 0.8, 0.0, 1.0);		\
			}"
	};

	vertexShaderId = dodanieDoProgramu(programZShaderami, vertexShader, GL_VERTEX_SHADER);
	fragmentShaderId = dodanieDoProgramu(programZShaderami, fragmentShader, GL_FRAGMENT_SHADER);

	GLint linkowanieOK = 0;
	glLinkProgram(programZShaderami);
	glGetProgramiv(programZShaderami, GL_LINK_STATUS, &linkowanieOK);
	if (linkowanieOK == GL_FALSE) {
		GLint dlugoscLoga = 0;
		glGetProgramiv(programZShaderami, GL_INFO_LOG_LENGTH, &dlugoscLoga);
		std::vector<GLchar> log(dlugoscLoga);
		glGetProgramInfoLog(programZShaderami, dlugoscLoga, NULL, &log[0]);
		std::cerr << "Blad podczas linkowania programu shaderow." << std::endl;
		for (std::vector<GLchar>::const_iterator i = log.begin(); i != log.end(); ++i)
			std::cerr << *i;
		std::cerr << std::endl;
		glDeleteProgram(programZShaderami);
		system("pause");
		exit(1);
	}

	GLint walidacjaOK = 0;
	glValidateProgram(programZShaderami);
	glGetProgramiv(programZShaderami, GL_VALIDATE_STATUS, &walidacjaOK);
	if (walidacjaOK == GL_FALSE) {
		GLint dlugoscLoga = 0;
		glGetProgramiv(programZShaderami, GL_INFO_LOG_LENGTH, &dlugoscLoga);
		std::vector<GLchar> log(dlugoscLoga);
		glGetProgramInfoLog(programZShaderami, dlugoscLoga, NULL, &log[0]);
		std::cerr << "Blad podczas walidacji programu shaderow." << std::endl;
		for (std::vector<GLchar>::const_iterator i = log.begin(); i != log.end(); ++i)
			std::cerr << *i;
		std::cerr << std::endl;
		glDeleteProgram(programZShaderami);
		system("pause");
		exit(1);
	}

	glUseProgram(programZShaderami);
}

GLuint Shaders::dodanieDoProgramu(GLuint programZShaderami, const GLchar * tekstShadera, GLenum typShadera)
{
	GLuint shader = glCreateShader(typShadera);

	const GLchar * typShaderaTekst;
	typShaderaTekst = (typShadera == 35633 ? "vertex" : "fragment");
	if (shader == 0) {
		std::cerr << "Blad podczas tworzenia " << typShaderaTekst << " shadera." << std::endl;
		system("pause");
		exit(0);
	}

	const GLchar * tekstShaderaTab[1];
	tekstShaderaTab[0] = tekstShadera;
	GLint dlugoscShadera[1];
	dlugoscShadera[0] = strlen(tekstShadera);
	glShaderSource(shader, 1, tekstShaderaTab, dlugoscShadera);
	glCompileShader(shader);
	GLint kompilacjaOK = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &kompilacjaOK);
	if (kompilacjaOK == GL_FALSE)
	{
		GLint dlugoscLoga = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &dlugoscLoga);
		std::vector<GLchar> log(dlugoscLoga);
		glGetShaderInfoLog(shader, dlugoscLoga, NULL, &log[0]);
		std::cerr << std::endl;
		std::cerr << "Blad podczas kompilacji " << typShaderaTekst << " shadera." << std::endl;
		std::cerr << std::endl;
		std::cerr << "log: ";
		for (std::vector<GLchar>::const_iterator i = log.begin(); i != log.end(); ++i)
			std::cerr << *i;
		std::cerr << std::endl;
		glDeleteShader(shader);
		system("pause");
		exit(1);
	}

	glAttachShader(programZShaderami, shader);
	return shader;
}

int main(int argc, char** argv)
{
	Shaders shaders(786, 1024, 100, 100);

	shaders.stworzenieOkna(argc, argv);
	shaders.inicjalizacjaGlew();
	shaders.stworzenieVAO();
	shaders.stworzenieVBO();
	shaders.stworzenieProgramu();
	glutDisplayFunc(shaders.wyswietl);
	glutCloseFunc(shaders.usun);

	glClearColor(0.0f, 0.4f, 0.0f, 0.0f);

	glutMainLoop();

	return 0;
}