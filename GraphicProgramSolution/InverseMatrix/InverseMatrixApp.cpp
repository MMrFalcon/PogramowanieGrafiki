#include <iostream>
#include <conio.h>
#include <vector>
using namespace std;

int endProgram() {
	char end = 'p';
	cout <<endl<< "q aby zakonczyc" << endl;
	while (true) {
		cin >> end;
		if (end == 'q')
			break;
		else
			continue;
	}
	return 0;
}


vector<float> buildMatrix(int size) {

	vector<float> matrix(size*size);

	int counter = 0;
	int index = 0;
	cout << "Budowa macierzy " << size << "x" << size << endl;
	cout << "q aby przerwac" << endl;

	while (counter < size) {
		int column = 1;
		while (column < size + 1) {
			float input;
			cout << "podaj " << column << " liczbe " << counter + 1 << " wiersza macierzy" << endl;
			cin >> input;

			if (cin.fail()) {
				cin.clear();
				break;
			}


			matrix[index] = input;
			++index;

			++column;
		}
		++counter;
	}

	return matrix;
}

vector <float> inverseMatrix2(vector <float> matrix) {
	
	float determinant;

	determinant = (matrix[0] - matrix[3]) * (matrix[1] - matrix[2]);

	if (determinant == 0) {
		cout << "wyznacznik rowny 0 nie mozna policzyc macierzy odwrotnej" << endl;
		endProgram();
	}

	vector <float> matrixComplements(matrix.size());

	int index = matrix.size() - 1; //ostatni index macierzy jako pierwszy wyznacznik
	for (size_t i = 0; i < matrixComplements.size(); i++) {

		i % 2 == 0 ? matrixComplements[i] = matrix[index] * 1 : matrixComplements[i] = matrix[index] * -1;
		--index;
	}

	vector <float> matrixTranspozition(matrix.size());

	matrixTranspozition[0] = matrixComplements[0];
	matrixTranspozition[1] = matrixComplements[2];
	matrixTranspozition[2] = matrixComplements[1];
	matrixTranspozition[3] = matrixComplements[3];

	vector <float> inversedMatrix(matrix.size());

	for (size_t i = 0; i < matrix.size(); i++)
		inversedMatrix[i] = matrixTranspozition[i] / determinant;
		
	return inversedMatrix;
}


vector <float> inverseMatrix3(vector <float> matrix, int numberOfItemInRow) {

	float determinant;
	float leftSide = 0; //najpierw z lewej strony dodawanie
	float rightSide = 0; //potem z prawej strony dodawanie
	//metoda Sarrusa
	vector<float> additionalMatrix(matrix.size() + 6);


	for (size_t i = 0; i < matrix.size(); i++) {
		additionalMatrix[i] = matrix[i];
	}

	int index = 0;
	for (size_t i = matrix.size(); i < additionalMatrix.size(); i++) {
		additionalMatrix[i] = matrix[index];
		++index;
	}

	cout << "Dodatkowa macierz do metody Sarrusa" << endl << endl;
	int breakLine = 1;
	for (size_t i = 0; i < additionalMatrix.size(); i++) {
		if (breakLine % 3 == 0)
			cout << "\n";
		cout << additionalMatrix[i] << " ";
		++breakLine;
	}

	//liczymy z lewej strony po przekatnych dlatego skoki o 3 
	for (size_t i = 0; i < matrix.size(); i= i + 3) { //ograniczenie macierzy podstawowej poniewa¿ ostatni indeks pobierany jest z niej
		leftSide +=  (additionalMatrix[i] * additionalMatrix[i + 4] * additionalMatrix[i + 8]);
	}

	for (size_t i = 2; i < matrix.size(); i = i + 3) {
		rightSide += (additionalMatrix[i] * additionalMatrix[i + 2] * additionalMatrix[i + 4]);
	}

	determinant = leftSide - rightSide;

	if (determinant == 0) {
		cout << "wyznacznik rowny 0 nie mozna policzyc macierzy odwrotnej" << endl;
		endProgram();
	}

	cout << endl << "wyznacznik " << determinant << endl << endl;

	vector <float> matrixComplements(matrix.size());

	//obliczanie dopelnien za pomoca minorow
	matrixComplements[0] = 1 * (matrix[4] * matrix[8] - matrix[5] * matrix[7]);
	matrixComplements[1] = -1 * (matrix[3] * matrix[8] - matrix[5] * matrix[6]);
	matrixComplements[2] = 1 * (matrix[3] * matrix[7] - matrix[4] * matrix[6]);
	matrixComplements[3] = -1 * (matrix[1] * matrix[8] - matrix[2] * matrix[7]);
	matrixComplements[4] = 1 * (matrix[0] * matrix[8] - matrix[2] * matrix[6]);
	matrixComplements[5] = -1 * (matrix[0] * matrix[7] - matrix[1] * matrix[6]);
	matrixComplements[6] = 1 * (matrix[1] * matrix[5] - matrix[2] * matrix[4]);
	matrixComplements[7] = -1 * (matrix[0] * matrix[5] - matrix[2] * matrix[3]);
	matrixComplements[8] = 1 * (matrix[0] * matrix[4] - matrix[1] * matrix[3]);

	cout << "macierz dopelnien:" << endl;
	breakLine = 0;
	for (size_t i = 0; i < matrixComplements.size(); i++) {
		if (breakLine % 3 == 0)
			cout << "\n";
		cout << matrixComplements[i] << " ";
		++breakLine;
	}

	//transponowanie
	vector <float> matrixTranspozition(matrix.size());

	matrixTranspozition[0] = matrixComplements[0];
	matrixTranspozition[1] = matrixComplements[3];
	matrixTranspozition[2] = matrixComplements[6];
	matrixTranspozition[3] = matrixComplements[1];
	matrixTranspozition[4] = matrixComplements[4];
	matrixTranspozition[5] = matrixComplements[7];
	matrixTranspozition[6] = matrixComplements[2];
	matrixTranspozition[7] = matrixComplements[5];
	matrixTranspozition[8] = matrixComplements[8];

	cout << endl << "macierz transponowana:" << endl;

	breakLine = 1;
	for (size_t i = 0; i < matrixTranspozition.size(); i++) {
		if (breakLine % 3 == 0) {
			cout << "\n";
		}

		cout << matrixTranspozition[i] << " ";
		++breakLine
	}

	//macierz odwrotna
	vector <float> inversedMatrix(matrix.size());


	for (size_t i = 0; i < matrix.size(); i++) {
		inversedMatrix[i] = matrixTranspozition[i] / determinant;
	}

	return inversedMatrix;
}


int main() {

	
	int matrixSize;
	cout << "Podaj rozmiar macierzy, mozliwa jedynie kwadratowa q konczy program" << endl;
	cin >> matrixSize;

	if (cin.fail()) {
		cin.clear();
	}
	vector <float> matrix(matrixSize*matrixSize);
	matrix = buildMatrix(matrixSize);

	int breakPoint = 1;
	
	for (int i = 0; i < matrixSize * matrixSize; i++) {
		cout << matrix[i] << " ";
		
		if (breakPoint == matrixSize)
		{
			cout << endl;
			breakPoint = 0;
		}
		++breakPoint;
	}

	if (matrixSize == 1) {
		cout << matrix[1] << endl;
		endProgram();
	}
	if (matrixSize == 2) {
		matrix = inverseMatrix2(matrix);
		int breakLine = 1;
			for (size_t i = 0; i < matrix.size(); i++) {
				if (breakLine % 2 == 0)
					cout << endl;
				cout << matrix[i] << " ";
				++breakLine;
			}

		endProgram();
	}
	else if(matrixSize == 3) {
		matrix = inverseMatrix3(matrix, matrixSize);
		int breakLine = 1;
		for (size_t i = 0; i < matrix.size(); i++) {
			if (breakLine % 2 == 0)
				cout << endl;
			cout << matrix[i] << " ";
			++breakLine;
		}

		endProgram();
	}
	else {
		endProgram();
	}

	return 0;
}


