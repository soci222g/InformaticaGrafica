#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <gtc/type_ptr.hpp>
#include <gtc/matrix_transform.hpp>
#include <iostream>
#include<string>
#include<fstream>
#include<vector>


#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480


struct ShaderProgram
{
	GLuint vertexShader = 0;
	GLuint geometryShader = 0;
	GLuint fragmentShader = 0;

};


struct GameObjects
{
	glm::vec3 position = glm::vec3(0.f);
	glm::vec3 rotation = glm::vec3(0.f);


	glm::vec3 forward = glm::vec3(1.f,0.f,0.f);
	glm::vec3 Up = glm::vec3(0.f,1.f,0.f);

	float velocity = 0.01f;
	float angularVelocity = 1;




};

void ResizeWindow(GLFWwindow* window, int iNewFrameBufferWidth, int iNewFrameBufferHeight) {
	//definir nou tamany
	glViewport(0, 0, iNewFrameBufferWidth, iNewFrameBufferHeight);

}

glm::mat4 GenerateTranslationMatrix(glm::vec3 translation) {
	return glm::translate(glm::mat4(1.0f), translation);

}
glm::mat4 GenerateRotationMatrix(glm::vec3 axi, float fdegrees) {
	return glm::rotate(glm::mat4(1.0f), glm::radians(fdegrees), glm::normalize(axi));

}




std::string LoadPath(const std::string& filePath) {

	std::ifstream file(filePath);
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

GLuint loadFragmentShader(const std::string& path) {

	GLuint FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	std::string sShaderCode = LoadPath(path);
	const char* cShaderSource = sShaderCode.c_str();


	//vinculem a la targeta grafica  (el 1 es el numero de archius que composan el shaders)
	glShaderSource(FragmentShader, 1, &cShaderSource, nullptr);


	//compilem el shader
	glCompileShader(FragmentShader);

	//verifiquem la compilacio del shader
	GLint succes;
	glGetShaderiv(FragmentShader, GL_COMPILE_STATUS, &succes);


	if (succes) {
		return FragmentShader;
	}
	else {
		std::cout << " error de carga!!" << std::endl;
		//primer he,m de saver la longitut del error
		GLint logLenght;
		glGetShaderiv(FragmentShader, GL_INFO_LOG_LENGTH, &logLenght);


		//get the log
		std::vector<GLchar> errorlog(logLenght);
		glGetShaderInfoLog(FragmentShader, logLenght, nullptr, errorlog.data());

		//mostrem el log

		std::cout << errorlog.data() << std::endl;
		std::exit(EXIT_FAILURE);
	}
}

GLuint loadGeometryShader(const std::string& path) {

	GLuint GeometryShader = glCreateShader(GL_GEOMETRY_SHADER);

	std::string sShaderCode = LoadPath(path);
	const char* cShaderSource = sShaderCode.c_str();


	//vinculem a la targeta grafica  (el 1 es el numero de archius que composan el shaders)
	glShaderSource(GeometryShader, 1, &cShaderSource, nullptr);


	//compilem el shader
	glCompileShader(GeometryShader);

	//verifiquem la compilacio del shader
	GLint succes;
	glGetShaderiv(GeometryShader, GL_COMPILE_STATUS, &succes);


	if (succes) {
		return GeometryShader;
	}
	else {
		std::cout << " error de carga!!" << std::endl;
		//primer he,m de saver la longitut del error
		GLint logLenght;
		glGetShaderiv(GeometryShader, GL_INFO_LOG_LENGTH, &logLenght);


		//get the log
		std::vector<GLchar> errorlog(logLenght);
		glGetShaderInfoLog(GeometryShader, logLenght, nullptr, errorlog.data());

		//mostrem el log

		std::cout << errorlog.data() << std::endl;
		std::exit(EXIT_FAILURE);
	}
}

GLuint loadVertexShader(const std::string& path) {

	//creat vertex shader a la GPU

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

	//load in memeori the shader

	std::string sShaderCode = LoadPath(path);
	const char* cShaderSource = sShaderCode.c_str();

	//vinculem a la targeta grafica  (el 1 es el numero de archius que composan el shaders)
	glShaderSource(vertexShader, 1, &cShaderSource, nullptr);


	//compilem el shader
	glCompileShader(vertexShader);

	//verifiquem la compilacio del shader
	GLint succes;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &succes);


	if (succes) {
		return vertexShader;
	}
	else {
		std::cout << " error de carga!!" << std::endl;
		//primer he,m de saver la longitut del error
		GLint logLenght;
		glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &logLenght);


		//get the log
		std::vector<GLchar> errorlog(logLenght);
		glGetShaderInfoLog(vertexShader, logLenght, nullptr, errorlog.data());

		//mostrem el log

		std::cout << errorlog.data() << std::endl;
		std::exit(EXIT_FAILURE);
	}
}


GLuint CreateProgram(const ShaderProgram& shader) {

	//creamos programa

	GLuint program = glCreateProgram();

	//verifiquem si hi ha un vertex shader o no
	if (shader.vertexShader != 0) {
		glAttachShader(program, shader.vertexShader);
	}
	if (shader.geometryShader != 0) {
		glAttachShader(program, shader.geometryShader);
	}
	if (shader.fragmentShader != 0) {
		glAttachShader(program, shader.fragmentShader);
	}

	//Linkear el programa
	glLinkProgram(program);

	//comprobem estat del programa

	GLint succes;
	glGetProgramiv(program, GL_LINK_STATUS, &succes);

	if (succes) {
		//llibarem recursos
		if (shader.vertexShader != 0) {
			glDetachShader(program, shader.vertexShader);

		}
		if (shader.geometryShader != 0) {
			glDetachShader(program, shader.geometryShader);

		}
		if (shader.fragmentShader != 0) {
			glDetachShader(program, shader.fragmentShader);

		}

		return program;
	}
	else {
		std::cout << " error del programa!!" << std::endl;
		//pillem el lenght del log
		GLint logLenght;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLenght);

		//guardem el log
		std::vector<GLchar> errorlog(logLenght);
		glGetProgramInfoLog(program, logLenght, nullptr, errorlog.data());


		//el printagem i surtim del programa
		std::cout << errorlog.data() << std::endl;
		std::exit(EXIT_FAILURE);
	}


}

void main() {

	srand(time(NULL));


	//INICIALITZA GLFW per gestiona finestres
	glfwInit();



	//pillar drivers
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//vercio principal AKA OPenGl 4.4
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);

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

		//compilem shader
		ShaderProgram myFirstProgram;

		myFirstProgram.vertexShader = loadVertexShader("MyFistVertexShader.glsl");
		myFirstProgram.geometryShader = loadGeometryShader("MyFirstGeometryShader.glsl");
		myFirstProgram.geometryShader = loadFragmentShader("MyFirstFragmentShader.glsl");

		//compila el programa un cop el shader esta compilat

		GLuint myfirstCompiledProgram;
		myfirstCompiledProgram = CreateProgram(myFirstProgram);


		//declarem struc game object
		GameObjects cube;




		//optenim referencia del element dins del shader

		GLint offsetReference = glGetUniformLocation(myfirstCompiledProgram, "offset");
		 


		//set el color del buffer  (el de darrera)
		glClearColor(0.f, 0.f, 0.f, 1.f);





		GLuint vaoPuntos, vboPuntos;

		GLuint vboPuntos2;
		//Generop el VAO i m'el Guardo a vaoPuntos
		glGenVertexArrays(1, &vaoPuntos);

		//activo el vao per trevalla en ell
		glBindVertexArray(vaoPuntos);


		//amb generas un vbo i m'el guardes.
		glGenBuffers(1, &vboPuntos);

		glGenBuffers(1, &vboPuntos2);

		//indico quin VBO es el actiu i que esta gaurdan arrays de dades
		glBindBuffer(GL_ARRAY_BUFFER, vboPuntos);

		glEnableVertexAttribArray(0);

		//dibuixa geometries de debug (per la entrega true o false)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);



		//declarem un punt en el x i y
		GLfloat puntos[] = {
				-0.5f,0.5f,-0.5f,
				0.5f,0.5f, -0.5f,
				-0.5f,-0.5f, -0.5f,
				0.5f,-0.5f,-0.5f,
				0.5f,-0.5f,0.5f,
				0.5f,0.5f, -0.5f,
				0.5f,0.5f,0.5f,
				-0.5f,0.5f,-0.5f,
				-0.5f,0.5f,0.5f,
				-0.5f,-0.5f, -0.5f,
				-0.5f,-0.5f, 0.5f,
				0.5f,-0.5f,0.5f,
				-0.5f,0.5f,0.5f,
				0.5f,0.5f,0.5f


		};

		//posu el array en el VBO
		glBufferData(GL_ARRAY_BUFFER, sizeof(puntos), puntos, GL_STATIC_DRAW);


		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

		//nategem el buffer amb el vbo
		glBindBuffer(GL_ARRAY_BUFFER, 0);




		//bufer 2


		GLfloat puntosRandom[] = {
				static_cast<GLfloat>(rand()) / RAND_MAX * 0.5f,
				static_cast<GLfloat>(rand()) / RAND_MAX * 0.5f,
				static_cast<GLfloat>(rand()) / RAND_MAX * 0.5f,
				static_cast<GLfloat>(rand()) / RAND_MAX * 0.5f,

		};


		glBindBuffer(GL_ARRAY_BUFFER, vboPuntos2);


		glBufferData(GL_ARRAY_BUFFER, sizeof(puntosRandom), puntosRandom, GL_STATIC_DRAW);
		//el primer numero es el id del vbo que corespon
		glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 1 * sizeof(GLfloat), (GLvoid*)0);
		//actia el vbo amb el index coresponenet
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, 0);


		//desactivem el vao 
		glBindVertexArray(0);





		//generem el model de la matriu MVP
		


		//indica a la cpi que utilitzi el programa 
		glUseProgram(myfirstCompiledProgram);


		glUniform2f(glGetUniformLocation(myfirstCompiledProgram, "WindowSize"), WINDOW_WIDTH, WINDOW_HEIGHT);

		//qui si podem modifica la variable (nomes es pot amb us)
		glm::vec2 offset = glm::vec2(0.0f, 0.0f);



		unsigned int LastFremeTime = glfwGetTime();




		while (!glfwWindowShouldClose(window))
		{
			unsigned int currentTime = glfwGetTime();



			unsigned int DeltaTime = currentTime - LastFremeTime;


			LastFremeTime = glfwGetTime();



			if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
				offset.y += 0.01 * DeltaTime;
			}
			else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
				offset.y -= 0.01 * DeltaTime;
			}

			if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
				offset.x -= 0.01 * DeltaTime;
			}
			else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
				offset.x += 0.01 * DeltaTime;
			}

			
			glm::mat4 cubeModelMatrix = glm::mat4(1.0f);

			cube.position = cube.position + cube.forward * cube.velocity;
			cube.rotation = cube.rotation + cube.Up * cube.angularVelocity;


			//invertim direccio si surt dels limits

			if (cube.position.x >= 0.5f || cube.position.x <= -0.5f) {
				cube.forward = cube.forward * -1.f;

			}

		



			glm::mat4 cubeTranslacioMatrix = GenerateTranslationMatrix(cube.position);
			glm::mat4 rotationMatrix = GenerateRotationMatrix(glm::vec3(1.f,1.f,0.f), cube.rotation.y);


			cubeModelMatrix = cubeTranslacioMatrix * rotationMatrix * cubeModelMatrix;

			glUniformMatrix4fv(glGetUniformLocation(myfirstCompiledProgram, "transform"), 1, GL_FALSE, glm::value_ptr(cubeModelMatrix));
			


		

			//una ariable per cada tipo, (2, floats, vector)
			glUniform2fv(offsetReference, 1, &offset[0]);


			//fem un pull de events
			glfwPollEvents();



			//clear buffers
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);



			//cridem a la geometria del VAO ( el VBO esta dintre de aquest)
			glBindVertexArray(vaoPuntos);

			//definim quiona info estem pintan del vao (en aquest cas punts , des del element 0 fins el 1)
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 14);

			//desativem el VAO
			glBindVertexArray(0);

			//preven la dessincruintzacio de FPS amb Hz
			glFlush();

			//fas swap dels buffers
			glfwSwapBuffers(window);

		}


		//desactivem i alliverem recursos del programa
		glUseProgram(0); //casquem que el programa que estem utilitzan sigi inactiu per poder modifica
		glDeleteProgram(myfirstCompiledProgram);
	}
	else {
		std::cout << "ha muerto" << std::endl;
		glfwTerminate();
	}



	glfwTerminate();



}



