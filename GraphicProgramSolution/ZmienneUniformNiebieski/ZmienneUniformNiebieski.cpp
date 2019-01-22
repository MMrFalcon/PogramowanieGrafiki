#include "ZmienneUniformNiebieski.h"

ZmienneUniform::ZmienneUniform(void)
{
	wysokoscOkna = 768;
	szerokoscOkna = 1024;
	polozenieOknaX = 100;
	polozenieOknaY = 100;
}

ZmienneUniform::ZmienneUniform(int wysokoscOkna, int szerokoscOkna, int polozenieOknaX, int polozenieOknaY)
{
	this->wysokoscOkna = wysokoscOkna;
	this->szerokoscOkna = szerokoscOkna;
	this->polozenieOknaX = polozenieOknaX;
	this->polozenieOknaY = polozenieOknaY;
}

ZmienneUniform::~ZmienneUniform()
{

}

void ZmienneUniform::stworzenieOkna(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(szerokoscOkna, wysokoscOkna);
	glutInitWindowPosition(polozenieOknaX, polozenieOknaY);
	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow("ZmienneUniform");
}

void ZmienneUniform::inicjalizacjaGlew()
{
	GLenum wynik = glewInit();
	if (wynik != GLEW_OK)
	{
		std::cerr << "Nie udalo sie zainicjalizowac GLEW. Blad: " << glewGetErrorString(wynik) << std::endl;
		system("pause");
		exit(1);
	}
}

void  ZmienneUniform::wyswietl()
{
	//metoda czysci buffor dla prezentowanych wartosci
	//gdyby nie bylo stalej uniform zapewne kolorem bufforowanym byl by kolor czarny
	//glClear(GL_COLOR_BUFFER_BIT);

	static GLfloat zmiana = 0.0f;
	zmiana += 0.0005f;
	glUniform1f(zmianaShader, abs(sinf(zmiana)));
	glUniform4f(kolorowanko, 0.0f, 0.0f, 1.0f, 1.0f);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glDrawArrays(GL_TRIANGLES, 0, 3);

	glDisableVertexAttribArray(0);

	glutSwapBuffers();
}

void ZmienneUniform::usun()
{
	glDeleteShader(vertexShaderId);
	glDeleteShader(fragmentShaderId);
	glDeleteProgram(programZShaderami);
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
}

void ZmienneUniform::stworzenieVAO()
{
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
}

void ZmienneUniform::stworzenieVBO()
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

void ZmienneUniform::stworzenieProgramu()
{
	programZShaderami = glCreateProgram();

	if (programZShaderami == 0)
	{
		std::cerr << "Blad podczas tworzenia programu shaderow." << std::endl;
		system("pause");
		exit(1);
	}

	static const GLchar * vertexShader = {
		"	#version 330 core \n																	\
			in layout(location=0) vec3 polozenie;													\
			uniform float zmiana;																	\
			void main()																				\
			{																						\
				gl_Position = vec4(zmiana * polozenie.x, zmiana * polozenie.y, polozenie.z, 1.0);	\
			}"
	};

	static const GLchar * fragmentShader = {
	"	#version 330 core \n						\
                            \
                                                          \
            uniform vec4 color;                          \
			void main()									\
			{											\
						\
                 gl_FragColor = color; \
             \
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
	zmianaShader = glGetUniformLocation(programZShaderami, "zmiana");
	if (zmianaShader == -1)
	{
		std::cerr << "Nie znalezion zmiennej uniform." << std::endl;
		system("pause");
		exit(1);
	}

	kolorowanko = glGetUniformLocation(programZShaderami, "color");
	if (zmianaShader == -1)
	{
		std::cerr << "Nie znalezion zmiennej uniform kolorowanko." << std::endl;
		system("pause");
		exit(1);
	}
}

GLuint ZmienneUniform::dodanieDoProgramu(GLuint programZShaderami, const GLchar * tekstShadera, GLenum typShadera)
{
	GLuint shader = glCreateShader(typShadera);

	const GLchar * typShaderaTekst = typShadera == 35633 ? "vertex" : "fragment";
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

void ZmienneUniform::sprawdzenieWersji()
{
	std::cout << "Wersja GLEW: " << glewGetString(GLEW_VERSION) << std::endl;
	std::cout << "Wersja VENDOR: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "Wersja REDERER: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "Wersja GL: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "Wersja GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
}

int main(int argc, char** argv)
{
	ZmienneUniform zmienneUniform(786, 786, 100, 100);

	zmienneUniform.stworzenieOkna(argc, argv);
	zmienneUniform.inicjalizacjaGlew();
	zmienneUniform.sprawdzenieWersji();
	zmienneUniform.stworzenieVAO();
	zmienneUniform.stworzenieVBO();
	zmienneUniform.stworzenieProgramu();
	glutDisplayFunc(zmienneUniform.wyswietl);
	glutIdleFunc(zmienneUniform.wyswietl);
	glutCloseFunc(zmienneUniform.usun);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glutMainLoop();

	return 0;
}