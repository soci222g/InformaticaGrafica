#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include<string>
#include<fstream>

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480


void ResizeWindow(GLFWwindow* window, int iNewFrameBufferWidth, int iNewFrameBufferHeight) {
	//definir nou tamany
	glViewport(0, 0, iNewFrameBufferWidth, iNewFrameBufferHeight);

}



std::string LoadPath(const std::string& filePath) {

	std::ifstream file (filePath);
	std::string fileContet;
	std::string line;


	if (!file.is_open()) {
		std::cout << "error de llegir el archiu puto " << filePath << std::endl;
		std::exit(EXIT_FAILURE);
	}

	while (std::getline(file, line))
	{
		fileContet += line + "\n";
	}
	file.close();

	return fileContet;
}

void main() {

	std::cout << "conectar a ficher: " << LoadPath("DeleteME.txt");


	//INICIALITZA GLFW per gestiona finestres
	glfwInit();



	//pillar drivers
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//vercio principal AKA OPenGl 4.4
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,4);

	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE); //configura si es escalabla la finestra



	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "WINDOW1", NULL, NULL);


	glfwSetFramebufferSizeCallback(window, ResizeWindow);
	
	//definim la finestra
	glfwMakeContextCurrent(window);


	//activem funcions experimentals per a totes les grafiques
	glewExperimental = GL_TRUE;

	//activem culling
	glEnable(GL_CULL_FACE);

	

	//indiquem el gl del culling
	glCullFace(GL_BACK);

	if (glewInit() == GLEW_OK) {
		std::cout << "ha funcionat" << std::endl;
		
		//set el color del buffer  (el de darrera)
		glClearColor(1.f, 0.f, 0.f, 1.f);





		GLuint vaoPuntos, vboPuntos;


		//Generop el VAO i m'el Guardo a vaoPuntos
		glGenVertexArrays(1, &vaoPuntos);

		//activo el vao per trevalla en ell
		glBindVertexArray(vaoPuntos);


		//amb generas un vbo i m'el guardes.
		glGenBuffers(1, &vboPuntos);

		//indico qun VBO es el actiu i que esta gaurdan arrays de dades
		glBindBuffer(GL_ARRAY_BUFFER, vboPuntos);



		//dibuixa geometries de debug (per la entrega true o false)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);



		//declarem un punt en el x i y
		GLfloat puntos[] = {
				0.5f,0.5f,
				-0.5f,0.5f,
				0.5f,-0.5f,
				-0.5f,-0.5f

		};


		//posu el array en el VBO
		glBufferData(GL_ARRAY_BUFFER, sizeof(puntos), puntos, GL_STATIC_DRAW);


		//definicm com llegir la info del VBO, 
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);


		//acties les dades de la gpu que els pugui utilitza
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindVertexArray(0);

		while (!glfwWindowShouldClose(window))
		{
			//fem un pull de events
			glfwPollEvents();


		
			//clear buffers
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
			


			//cridem a la geometria del VAO ( el VBO esta dintre de aquest)
			glBindVertexArray(vaoPuntos);

			//definim quiona info estem pintan del vao (en aquest cas punts , des del element 0 fins el 1)
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

			//desativem el VAO
			glBindVertexArray(0);

			//preven la dessincruintzacio de FPS amb Hz
			glFlush();

			//fas swap dels buffers
			glfwSwapBuffers(window);

		}

	}
	else {
		std::cout << "ha muerto" << std::endl;
		glfwTerminate();
	}



	glfwTerminate();



}



