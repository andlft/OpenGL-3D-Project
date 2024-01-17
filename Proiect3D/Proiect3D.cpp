// PROIECT 3D (Lefter Andrei, Rosianu Robert, Udrea Robert, Enescu Irina)
// 
//  
//	Biblioteci
#include <windows.h>        //	Utilizarea functiilor de sistem Windows (crearea de ferestre, manipularea fisierelor si directoarelor);
#include <stdlib.h>         //  Biblioteci necesare pentru citirea shaderelor;
#include <stdio.h>
#include <math.h>			//	Biblioteca pentru calcule matematice;
#include <GL/glew.h>        //  Definește prototipurile functiilor OpenGL si constantele necesare pentru programarea OpenGL moderna; 
#include <GL/freeglut.h>    //	Include functii pentru:
#include <ctime>			//	- gestionarea ferestrelor si evenimentelor de tastatura si mouse, 
#include <vector>			//  - desenarea de primitive grafice precum dreptunghiuri, cercuri sau linii, 
							//  - crearea de meniuri si submeniuri;
#include "loadShaders.h"	//	Fisierul care face legatura intre program si shadere;
#include "glm/glm.hpp"		//	Bibloteci utilizate pentru transformari grafice;
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/geometric.hpp"

//  Identificatorii obiectelor de tip OpenGL; 
GLuint
VaoId,
VboId,
EboId,
ColorBufferId,
ProgramId,
myMatrixLocation,
matrUmbraLocation,
viewLocation,
projLocation,
matrRotlLocation,
lightColorLocation,
lightPosLocation,
viewPosLocation,
codColLocation;

int codCol;
float PI = 3.141592;
std::vector<int> colornumbers = { 0, 2, 3, 4 };
std::vector<int> xpos(10001);
std::vector<int> ypos(10001);
std::vector<int> xcpos(10001);
std::vector<int> ycpos(10001);
std::vector<int> rotationpos(10001);
float l = 0.0f, s = -100.0f, x = 1200.0f, y = 200.0f, l1 = -100.0f;
int resetThreshold;
int xposition, yposition, i, j, xmax, xmin, ymax, ymin, xcmax, xcmin, ycmax, ycmin, q;
// matrice utilizate
glm::mat4 myMatrix, matrRot, matrTransl, resizeMatrix, savedMatrix;

// elemente pentru matricea de vizualizare
float Refx = 0.0f, Refy = 0.0f, Refz = 4000.0f;
float alpha = PI, beta = PI, dist = 6000.0f;
float Obsx, Obsy, Obsz;
float Vx = 0.0, Vy = 0.0, Vz = 1.0;
glm::mat4 view;

// elemente pentru matricea de proiectie
float width = 800, height = 600, xwmin = -800.f, xwmax = 800, ywmin = -600, ywmax = 600, znear = 0.1, zfar = 1, fov = 45;
glm::mat4 projection;

// sursa de lumina
float xL = 5000000.f, yL = 1000000.f, zL = 4000000.f;

// matricea umbrei
float matrUmbra[4][4];

// Elemente pentru reprezentarea suprafetei sferei
// (1) intervalele pentru parametrii considerati (u si v)
float const U_MIN = -PI / 2, U_MAX = PI / 2, V_MIN = 0, V_MAX = 2 * PI;
// (2) numarul de paralele/meridiane, de fapt numarul de valori ptr parametri
int const NR_PARR = 6, NR_MERID = 20;
// (3) pasul cu care vom incrementa u, respectiv v
float step_u = (U_MAX - U_MIN) / NR_PARR, step_v = (V_MAX - V_MIN) / NR_MERID;
// alte variabile
float radius = 200;
int vertex, index_aux;
int startVertex = 270;
int startIndex = 68;

glm::quat rotQuat, rotQuat2;

void processNormalKeys(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'l':
		Vx -= 0.1;
		break;
	case 'r':
		Vx += 0.1;
		break;
	case '+':
		dist += 5;
		break;
	case '-':
		dist -= 5;
		break;
	}
	if (key == 27)
		exit(0);
}
void processSpecialKeys(int key, int xx, int yy)
{
	switch (key)
	{
	case GLUT_KEY_LEFT:
		beta -= 0.01;
		break;
	case GLUT_KEY_RIGHT:
		beta += 0.01;
		break;
	case GLUT_KEY_UP:
		alpha += 0.0;
		break;
	case GLUT_KEY_DOWN:
		alpha -= 0.0;
		break;
	}
}
void CreateVBO(void)
{
	// varfurile 
	GLfloat Vertices[10000] =
	{
		// coordonate                   // culori			// normale
		// varfuri "ground"
	   -150000.0f,  -150000.0f, -600.0f, 1.0f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f, 1.0f,
		150000.0f,  -150000.0f, -600.0f, 1.0f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f, 1.0f,
		150000.0f,  150000.0f,  -600.0f, 1.0f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f, 1.0f,
	   -150000.0f,  150000.0f,  -600.0f, 1.0f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f, 1.0f,
	   // varfuri cub
		-50.0f,  -50.0f, -550.0f, 1.0f,   1.0f, 0.5f, 0.2f,  -1.0f, -1.0f, -1.0f,
		 50.0f,  -50.0f,  -550.0f, 1.0f,  1.0f, 0.5f, 0.2f,  1.0f, -1.0f, -1.0f,
		 50.0f,  50.0f,  -550.0f, 1.0f,   1.0f, 0.5f, 0.2f,  1.0f, 1.0f, -1.0f,
		-50.0f,  50.0f, -550.0f, 1.0f,    1.0f, 0.5f, 0.2f,  -1.0f, 1.0f, -1.0f,
		-50.0f,  -50.0f, -450.0f, 1.0f,  1.0f, 0.5f, 0.2f,  -1.0f, -1.0f, 1.0f,
		 50.0f,  -50.0f,  -450.0f, 1.0f, 1.0f, 0.5f, 0.2f,  1.0f, -1.0f, 1.0f,
		 50.0f,  50.0f,  -450.0f, 1.0f,  1.0f, 0.5f, 0.2f,  1.0f, 1.0f, 1.0f,
		-50.0f,  50.0f, -450.0f, 1.0f,   1.0f, 0.5f, 0.2f,  -1.0f, 1.0f, 1.0f,
		// varfuri con
		 -40.0f, -69.28f, -250.0f, 1.0f,   0.1f, 1.0f, 0.2f, -40.0f, -69.28f, -80.0f,
		 40.0f, -69.28f, -250.0f, 1.0f,    0.1f, 1.0f, 0.2f, 40.0f, -69.28f, -80.0f,
		 80.0f, 0.0f, -250.0f, 1.0f,       0.1f, 1.0f, 0.2f, 80.0f, 0.0f, -80.0f,
		 40.0f, 69.28f, -250.0f, 1.0f,     0.1f, 1.0f, 0.2f, 40.0f, 69.28f, -80.0f,
		-40.0f, 69.28f, -250.0f, 1.0f,     0.1f, 1.0f, 0.2f, -40.0f, 69.28f, -80.0f,
		-80.0f, 0.0f,  -250.0f, 1.0f,      0.1f, 1.0f, 0.2f, -80.0f, 0.0f, -80.0f,
		  0.0f, 0.0f, -350.0f, 1.0f,       0.3f, 1.0f, 0.2f, 0.0f, 0.0f, -1.0f,
		  // varfuri sfori
		  -50.0f,  -50.0f, -450.0f, 1.0f,   0.0f, 0.0f, 0.0f,  -1.0f, -1.0f, -1.0f,
		  -100.0f, -100.0f,-150.0f, 1.0f,   0.0f, 0.0f, 0.0f,  -1.0f, -1.0f, -1.0f,
		  50.0f,  -50.0f,  -450.0f, 1.0f,   0.0f, 0.0f, 0.0f,  1.0f, -1.0f, -1.0f,
		  100.0f, -100.0f ,-150.0f, 1.0f,   0.0f, 0.0f, 0.0f,  -1.0f, -1.0f, -1.0f,
		  50.0f,  50.0f,  -450.0f, 1.0f,   0.0f, 0.0f, 0.0f,  1.0f, 1.0f, -1.0f,
		  100.0f, 100.0f, -150.0f, 1.0f,   0.0f, 0.0f, 0.0f,  1.0f, 1.0f, -1.0f,
		  -50.0f,  50.0f, -450.0f, 1.0f,    0.0f, 0.0f, 0.0f,  -1.0f, 1.0f, -1.0f,
		  -100.0f, 100.0f, -150.0f, 1.0f,   0.0f, 0.0f, 0.0f,  1.0f, 1.0f, -1.0f,
	};

	// indicii pentru varfuri
	GLushort Indices[40000] =
	{
		// fetele "ground"
		 1, 2, 0,   2, 0, 3,
		 // fetele cubului
		 5, 6, 4,   6, 4, 7,
		 6, 7, 10, 10, 7, 11,
		 11, 7, 8,   8, 7, 4,
		 8, 4, 9,   9, 4, 5,
		 5, 6, 9,   9, 6, 10,
		 9, 10, 8,  8, 10, 11,
		 // fetele conului
		 12, 13, 18,
		 13, 14, 18,
		 14, 15, 18,
		 15, 16, 18,
		 16, 17, 18,
		 17, 12, 18,
		 // sfori
		19, 20,
		21, 22,
		23, 24,
		25, 26,

	};

	// adaugare varfuri sfera
	for (int merid = 0; merid < NR_MERID; merid++)
	{
		for (int parr = 0; parr < NR_PARR + 1; parr++)
		{
			float u = U_MIN + parr * step_u;
			float v = V_MIN + merid * step_v;
			float x_vf = radius * cosf(u) * cosf(v);
			float y_vf = radius * cosf(u) * sinf(v);
			float z_vf = radius * sinf(u);

			vertex = startVertex + (merid * (NR_PARR + 1) + parr) * 10;
			Vertices[vertex] = x_vf;
			Vertices[vertex + 1] = y_vf;
			Vertices[vertex + 2] = z_vf;
			Vertices[vertex + 3] = 1.0f;
			Vertices[vertex + 4] = 0.1f;
			Vertices[vertex + 5] = 1.0f;
			Vertices[vertex + 6] = 0.2f;
			Vertices[vertex + 7] = x_vf;
			Vertices[vertex + 8] = y_vf;
			Vertices[vertex + 9] = z_vf;

			Indices[(vertex - startVertex) / 10 + startIndex] = (vertex - startVertex) / 10 + 27;
			index_aux = parr * (NR_MERID)+merid;
			Indices[startIndex + (NR_PARR + 1) * NR_MERID + index_aux] = (vertex - startVertex) / 10 + 27;

			if ((parr + 1) % (NR_PARR + 1) != 0)
			{

				int AUX = 2 * (NR_PARR + 1) * NR_MERID + startIndex;
				int index1 = (vertex - startVertex) / 10 + 27; // varful v considerat
				int index2 = (vertex - startVertex) / 10 + 27 + (NR_PARR + 1); // dreapta lui v, pe meridianul urmator
				int index3 = index2 + 1;  // dreapta sus fata de v
				int index4 = (vertex - startVertex) / 10 + 27 + 1;  // deasupra lui v, pe acelasi meridian
				if (merid == NR_MERID - 1)  // la ultimul meridian, trebuie revenit la meridianul initial
				{
					index2 = (index2 - 27) % (NR_PARR + 1) + 27;
					index3 = (index3 - 27) % (NR_PARR + 1) + 27;
				}
				Indices[AUX + 4 * ((vertex - startVertex) / 10)] = index1;  // unele valori ale lui Indices, corespunzatoare Polului Nord, au valori neadecvate
				Indices[AUX + 4 * ((vertex - startVertex) / 10) + 1] = index2;
				Indices[AUX + 4 * ((vertex - startVertex) / 10) + 2] = index3;
				Indices[AUX + 4 * ((vertex - startVertex) / 10) + 3] = index4;

			}
			else {
				int AUX = 2 * (NR_PARR + 1) * NR_MERID + startIndex;
				int index1 = (vertex - startVertex) / 10 + 27; // varful v considerat
				int index2 = (vertex - startVertex) / 10 + 27 + (NR_PARR + 1); // dreapta lui v, pe meridianul urmator
				int index3 = index2 + 1;  // dreapta sus fata de v
				int index4 = (vertex - startVertex) / 10 + 27 + 1;  // deasupra lui v, pe acelasi meridian
				if (merid == NR_MERID - 1)  // la ultimul meridian, trebuie revenit la meridianul initial
				{
					index2 = (index2 - 27) % (NR_PARR + 1) + 27;
					index3 = (index3 - 27) % (NR_PARR + 1) + 27;
				}
				Indices[AUX + 4 * ((vertex - startVertex) / 10)] = -1;  // unele valori ale lui Indices, corespunzatoare Polului Nord, au valori neadecvate
				Indices[AUX + 4 * ((vertex - startVertex) / 10) + 1] = -1;
				Indices[AUX + 4 * ((vertex - startVertex) / 10) + 2] = -1;
				Indices[AUX + 4 * ((vertex - startVertex) / 10) + 3] = -1;
			}
		}
	}



	glGenVertexArrays(1, &VaoId);
	glGenBuffers(1, &VboId);
	glGenBuffers(1, &EboId);
	glBindVertexArray(VaoId);

	glBindBuffer(GL_ARRAY_BUFFER, VboId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EboId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);

	// atributul 0 = pozitie
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 10 * sizeof(GLfloat), (GLvoid*)0);
	// atributul 1 = culoare
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(GLfloat), (GLvoid*)(4 * sizeof(GLfloat)));
	// atributul 2 = normale
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(GLfloat), (GLvoid*)(7 * sizeof(GLfloat)));
}
void DestroyVBO(void)
{
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &VboId);
	glDeleteBuffers(1, &EboId);
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &VaoId);
}

void CreateShaders(void)
{
	ProgramId = LoadShaders("Shader.vert", "Shader.frag");
	glUseProgram(ProgramId);
}

void DestroyShaders(void)
{
	glDeleteProgram(ProgramId);
}

int getRandomNumber(const std::vector<int>& numbers) {
	int randomIndex = std::rand() % numbers.size();
	return numbers[randomIndex];
}

void Initialize(void)
{
	myMatrix = glm::mat4(1.0f);
	
	// vector coordonate baloane 
	xmax = 2000;
	xmin = -2000;
	ymax = 2000;
	ymin = -2000;
	for (j = 1; j <= 21; j++)
	{
		xpos[j] = rand() % (xmax - xmin + 1) + xmin;
		ypos[j] = rand() % (ymax - ymin + 1) + ymin;
	}

	// vector coordonate norisori
	xcmax = 15000;
	xcmin = -15000;
	ycmax = 15000;
	ycmin = -15000;
	for (j = 1; j <= 450; j++)
	{
		xcpos[j] = rand() % (xcmax - xcmin + 1) + xcmin;
		ycpos[j] = rand() % (ycmax - ycmin + 1) + ycmin;
		rotationpos[j] = rand() % 100;
	}

	matrRot = glm::rotate(glm::mat4(1.0f), PI / 8, glm::vec3(0.0, 0.0, 1.0));
	glClearColor(0.0f, 255.0f, 255.0f, 0.0f);
	CreateVBO();
	CreateShaders();
	// locatii pentru shader-e
	myMatrixLocation = glGetUniformLocation(ProgramId, "myMatrix");
	matrUmbraLocation = glGetUniformLocation(ProgramId, "matrUmbra");
	viewLocation = glGetUniformLocation(ProgramId, "view");
	projLocation = glGetUniformLocation(ProgramId, "projection");
	lightColorLocation = glGetUniformLocation(ProgramId, "lightColor");
	lightPosLocation = glGetUniformLocation(ProgramId, "lightPos");
	viewPosLocation = glGetUniformLocation(ProgramId, "viewPos");
	codColLocation = glGetUniformLocation(ProgramId, "codCol");
}

void DrawSphere()
{
	for (int patr = 0; patr < (NR_PARR + 1) * NR_MERID; patr++)
	{
		if ((patr + 1) % (NR_PARR + 1) != 0) // nu sunt considerate fetele in care in stanga jos este Polul Nord
			glDrawElements(
				GL_QUADS,
				4,
				GL_UNSIGNED_SHORT,
				(void*)(((2 * (NR_PARR + 1) * (NR_MERID)+4 * patr) + 68) * sizeof(GLushort)));
	}
}

void DrawBalloon(glm::mat4 balloonMatrix, int codColor) //codCol selecteaza culoarea balonului => 0 - verde, 1 - umbra, 2 - rosu, 3 - albastru, 4 - galben
{
	glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
	// desenare cub
	codCol = 0;
	glUniform1i(codColLocation, codCol);
	myMatrix = balloonMatrix;
	glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, (void*)(6 * sizeof(GLushort)));

	// desenare sfori
	myMatrix = glm::translate(balloonMatrix, glm::vec3(0.f, 0.f, 0.0f));
	myMatrixLocation = glGetUniformLocation(ProgramId, "myMatrix");
	glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
	glLineWidth(5.0f);
	glDrawElements(GL_LINES, 8, GL_UNSIGNED_SHORT, (void*)(60 * sizeof(GLushort)));

	codCol = codColor;
	glUniform1i(codColLocation, codCol);
	// desenare con
	myMatrix = glm::translate(balloonMatrix, glm::vec3(0.f, 0.f, 0.0));
	myMatrixLocation = glGetUniformLocation(ProgramId, "myMatrix");
	glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
	glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_SHORT, (void*)(42 * sizeof(GLushort)));

	// desenare balon
	myMatrix = glm::translate(balloonMatrix, glm::vec3(0.f, 0.f, -75.0f));
	myMatrixLocation = glGetUniformLocation(ProgramId, "myMatrix");
	glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
	DrawSphere();

	// desenare umbra cub
	codCol = 1;
	glUniform1i(codColLocation, codCol);
	myMatrix = balloonMatrix;
	glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, (void*)(6 * sizeof(GLushort)));

	// desenare umbra con
	myMatrix = glm::translate(balloonMatrix, glm::vec3(0.f, 0.f, 0.0));
	glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
	glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_SHORT, (void*)(42 * sizeof(GLushort)));

	//desenare umbra sfori
	myMatrix = glm::translate(balloonMatrix, glm::vec3(0.f, 0.f, 0.0f));
	glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
	glLineWidth(5.0f);
	glDrawElements(GL_LINES, 8, GL_UNSIGNED_SHORT, (void*)(60 * sizeof(GLushort)));

	//desenare umbra balon
	myMatrix = glm::translate(balloonMatrix, glm::vec3(0.f, 0.f, -75.0f));
	glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
	DrawSphere();
}

void DrawCloud(glm::mat4 cloudMatrix) 
{	
	codCol = 5;
	glUniform1i(codColLocation, codCol);

	myMatrix = glm::translate(cloudMatrix, glm::vec3(0.f, 0.f, 4000.0f));
	myMatrix = glm::scale(myMatrix, glm::vec3(0.5f, 0.5f, 0.5f));
	savedMatrix = myMatrix;
	myMatrixLocation = glGetUniformLocation(ProgramId, "myMatrix");
	glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
	for (int i = 0; i <= 3; i++)
	{
		codCol = 5;
		glUniform1i(codColLocation, codCol);
		myMatrix = glm::translate(savedMatrix, glm::vec3(0.f, 200.f, 0.0f));
		myMatrixLocation = glGetUniformLocation(ProgramId, "myMatrix");
		glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
		savedMatrix = myMatrix;
		DrawSphere();

		codCol = 6;
		glUniform1i(codColLocation, codCol);
		myMatrix = glm::translate(myMatrix, glm::vec3(0.f, 0.f, -75.0f));
		glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
		DrawSphere();
	}

	myMatrix = glm::translate(cloudMatrix, glm::vec3(0.f, 0.f, 4000.0f));
	myMatrix = glm::scale(myMatrix, glm::vec3(0.5f, 0.5f, 0.5f));
	savedMatrix = myMatrix;
	myMatrixLocation = glGetUniformLocation(ProgramId, "myMatrix");
	glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
	for (int i = 0; i < 2; i++)
	{
		codCol = 5;
		glUniform1i(codColLocation, codCol);
		myMatrix = glm::translate(savedMatrix, glm::vec3(0.f, 250.f, 100.0f));
		myMatrixLocation = glGetUniformLocation(ProgramId, "myMatrix");
		glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
		savedMatrix = myMatrix;
		DrawSphere();

		codCol = 6;
		glUniform1i(codColLocation, codCol);
		myMatrix = glm::translate(savedMatrix, glm::vec3(0.f, 0.f, -75.0f));
		glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
		savedMatrix = myMatrix;
		DrawSphere();
	}
}

void RenderFunction(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	//pozitia observatorului
	Obsx = Refx + dist * cos(alpha) * cos(beta);
	Obsy = Refy + dist * cos(alpha) * sin(beta);
	Obsz = Refz + dist * sin(alpha);

	// matrice de vizualizare + proiectie
	glm::vec3 Obs = glm::vec3(Obsx, Obsy, Obsz);   // se schimba pozitia observatorului	
	glm::vec3 PctRef = glm::vec3(Refx, Refy, Refz); // pozitia punctului de referinta
	glm::vec3 Vert = glm::vec3(Vx, Vy, Vz); // verticala din planul de vizualizare 
	view = glm::lookAt(Obs, PctRef, Vert);
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);
	projection = glm::infinitePerspective(fov, GLfloat(width) / GLfloat(height), znear);
	glUniformMatrix4fv(projLocation, 1, GL_FALSE, &projection[0][0]);

	// matricea pentru umbra
	float D = 1000.0f;
	matrUmbra[0][0] = zL + D; matrUmbra[0][1] = 0; matrUmbra[0][2] = 0; matrUmbra[0][3] = 0;
	matrUmbra[1][0] = 0; matrUmbra[1][1] = zL + D; matrUmbra[1][2] = 0; matrUmbra[1][3] = 0;
	matrUmbra[2][0] = -xL; matrUmbra[2][1] = -yL; matrUmbra[2][2] = D; matrUmbra[2][3] = -1;
	matrUmbra[3][0] = -D * xL; matrUmbra[3][1] = -D * yL; matrUmbra[3][2] = -D * zL; matrUmbra[3][3] = zL;
	glUniformMatrix4fv(matrUmbraLocation, 1, GL_FALSE, &matrUmbra[0][0]);

	// Variabile uniforme pentru iluminare
	glUniform3f(lightColorLocation, 1.0f, 1.0f, 1.0f);
	glUniform3f(lightPosLocation, xL, yL, zL);
	glUniform3f(viewPosLocation, Obsx, Obsy, Obsz);

	myMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -1100.f));
	glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
	codCol = 0;
	glUniform1i(codColLocation, codCol);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
	
	//vector random pt x,y
	l = l + 0.8;

	for (int i = 1; i <= 450; i++) 
	{
		myMatrix = glm::rotate(glm::mat4(1.0f), (PI / 8) * rotationpos[i], glm::vec3(0.0f, 0.0f, 1.0f));
		myMatrix = glm::translate(myMatrix, glm::vec3(xcpos[i], ycpos[i], std::fmod((50.0f + i * 1000), 10000.0f)));
		glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
		DrawCloud(myMatrix);
	}	
	
	for (int i = 1; i <= 10; i++)
	{
		matrTransl = glm::translate(glm::mat4(1.0f), glm::vec3(xpos[i], ypos[i], std::fmod((l + i * 1000), 10000.0f)));
		rotQuat = glm::angleAxis((0.2f * sinf(l * (i % 3 - 1.5f) / 100) - 0.1f), glm::vec3(1.0f, 0.0f, 0.0f));
		rotQuat2 = glm::angleAxis((l * (i % 5 - 1.5f) / 300), glm::vec3(0.0f, 0.0f, 1.0f));
		matrRot = glm::mat4_cast(rotQuat * rotQuat2);
		myMatrix = matrTransl * matrRot;
		glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
		DrawBalloon(myMatrix, colornumbers[i % 4]);
	}

	glutSwapBuffers();
	glFlush();
}
void Cleanup(void)
{
	DestroyShaders();
	DestroyVBO();
}

int main(int argc, char* argv[])
{
	srand(time(0));
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(1200, 900);
	glutCreateWindow("Proiect 3D");
	glewInit();
	Initialize();
	glutIdleFunc(RenderFunction);
	glutDisplayFunc(RenderFunction);
	glutKeyboardFunc(processNormalKeys);
	glutSpecialFunc(processSpecialKeys);
	glutCloseFunc(Cleanup);
	glutMainLoop();
}

