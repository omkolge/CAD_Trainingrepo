#include<iostream>
#include<sstream>

#define GLEW_STATIC
#include"GL/glew.h"
#include"GLFW/glfw3.h"

const char* APP_TITLE = "Introduction to Modern OpenGL -- Hello Pentagon";
const int gWindowWidth = 800;
const int gWindowHeight = 600;
bool gWireFrame = false;

// Shaders
const GLchar* vertexShaderSrc =
"#version 330 core\n"
"layout (location = 0) in vec3 pos;"
"void main()"
"{"
"    gl_Position = vec4(pos.x, pos.y, pos.z, 1.0);"
"}";

const GLchar* fragmentShaderSrc =
"#version 330 core\n"
"out vec4 frag_color;"
"void main()"
"{"
"    frag_color = vec4(0.35f,0.96f,0.3f, 1.0f);"
"}";


GLFWwindow* gWindow = NULL;

//close window after pressing esc key
void glfw_onKey(GLFWwindow* window, int key, int scancode, int action, int mode);//this func have mandate to with this parameters
void showFPS(GLFWwindow* window);
bool initOpenGL();

int main()
{
	//cout << "Welcome To OpenGL" << endl;
	if (!initOpenGL())
	{
		std::cerr << "GLFW initialization failed" << std::endl;
		return false;
	}
	/*GLfloat vertices[] = {
		  0.0f,0.5f,0.0f,
		  0.5f,0.25f,0.0f,
		  0.25f,-0.5f,0.0f,
		  -0.25f,-0.5f,0.0f,
		  -0.5f,0.25f,0.0f

	};*/

	//Hexagon
	GLfloat vertices[] = {
		  0.0f,0.5f,0.0f,
		  0.5f,0.25f,0.0f,
		  0.5f,-0.25f,0.0f,
		  0.0f,-0.5f,0.0f,
		  -0.5f,-0.25f,0.0f,
		  -0.5f,0.25f,0.0f

	};

	GLuint indices[] = {
		0,1,2,
		0,2,3,
		0,3,4,
		0,4,5
	};

// 2. Set up buffers on the GPU
	GLuint vbo, vao, ibo;

	glGenBuffers(1, &vbo);					// Generate an empty vertex buffer on the GPU
	glBindBuffer(GL_ARRAY_BUFFER, vbo);		// "bind" or set as the current buffer we are working with
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);// copy the data from CPU to GPU "GL_STATIC_DRAW" "GL_DYNAMIC_DRAW" GL_STREAM_DRAW

	// The vertex array object (VAO) is a little descriptor that defines which data from vertex buffer objects should be used as input 
	// variables to vertex shaders. in our case - use our only VBO, and say 'every three floats is a variable'
	// Modern OGL requires that we use a vertex array object
	glGenVertexArrays(1, &vao);				// Tell OpenGL to create new Vertex Array Object
	glBindVertexArray(vao);					// Make it the current one
			// Offset inside the structure to find the attribute*/

		//position

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);	// Enable the first attribute or attribute "0"

	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

// 3. Create vertex shader
	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vertexShaderSrc, NULL);
	glCompileShader(vs);

	// Check for compile errors
	GLint result;
	GLchar infoLog[512];
	glGetShaderiv(vs, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(vs, sizeof(infoLog), NULL, infoLog);
		std::cout << "Error! Vertex shader failed to compile. " << infoLog << std::endl;
	}

// 4. Create fragment shader
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fragmentShaderSrc, NULL);
	glCompileShader(fs);

	// Check for compile errors
	glGetShaderiv(fs, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(fs, sizeof(infoLog), NULL, infoLog);
		std::cout << "Error! Fragment shader failed to compile. " << infoLog << std::endl;
	}

	// 5. Create shader program and link shaders to program
	GLint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vs);
	glAttachShader(shaderProgram, fs);
	glLinkProgram(shaderProgram);

	// Check for linker errors
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shaderProgram, sizeof(infoLog), NULL, infoLog);
		std::cout << "Error! Shader program linker failure " << infoLog << std::endl;
	}

	// Clean up shaders, do not need them anymore since they are linked to a shader program
	glDeleteShader(vs);
	glDeleteShader(fs);


	while (!glfwWindowShouldClose(gWindow))
	{
		showFPS(gWindow);

		// Poll for and process events
		glfwPollEvents();

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT);

		// Render the triangle
		glUseProgram(shaderProgram);
		glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);


		// Swap front and back buffers
		glfwSwapBuffers(gWindow);
	}

	// Clean up
	glDeleteProgram(shaderProgram);
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ibo);

	glfwTerminate();
	return 0;
}

//close window after pressing esc key
void glfw_onKey(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	if (key == GLFW_KEY_W && action == GLFW_PRESS)
	{
		gWireFrame = !gWireFrame;
		if (gWireFrame)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
	}

}
void showFPS(GLFWwindow* window)
{
	static double previousSeconds = 0.0;
	static int frameCount = 0;
	double elapsedSeconds;
	double currentSeconds = glfwGetTime(); //  returns no of seconds since GLFW started as double
	elapsedSeconds = currentSeconds - previousSeconds;
	//limit text update 4 times per second
	if (elapsedSeconds > 0.25)
	{
		previousSeconds = currentSeconds;
		double fps = (double)frameCount / elapsedSeconds;
		double msPerFrame = 1000.0 / fps;

		std::ostringstream outs;
		outs.precision(3);
		outs << std::fixed << APP_TITLE << "  " << "FPS: " << fps << "  " << "Frame Time: " << msPerFrame << " (ms)" << std::endl;
		glfwSetWindowTitle(window, outs.str().c_str());
		frameCount = 0;

	}
	frameCount++;
}

// Initialize GLFW and OpenGL
bool initOpenGL()
{
	if (!glfwInit())
	{
		std::cerr << "GLFW intialization failed " << std::endl;
		return false;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	gWindow = glfwCreateWindow(gWindowWidth, gWindowHeight, APP_TITLE, NULL, NULL);

	if (gWindow == NULL)
	{
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(gWindow);
	glfwSetKeyCallback(gWindow, glfw_onKey);
	glewExperimental = GL_TRUE;
	if (glewInit() != 0)
	{
		std::cerr << "GLEW intialization failed " << std::endl;
		return false;

	}
	glClearColor(0.11f, 1.10f, 6.60f, 0.16f);
	return true;
}

//
//#define GLEW_STATIC
//#include"GL/glew.h"
//#include <iostream>
//#include <cmath>
//
//
//
//const GLfloat triangleVertices[] = {
//    0.0f, 0.5f, 1.0f, 0.0f, 0.0f,
//    -0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
//    0.5f, -0.5f, 0.0f, 0.0f, 1.0f
//};
//
//void rotateVerticesColor(GLfloat* vertices, int count, GLfloat speed)
//{
//    GLfloat t = glfwGetTime() * speed;
//    for (int i = 0; i < count; i += 5)
//    {
//        GLfloat r = 0.5f + 0.5f * sin(t + i);
//        GLfloat g = 0.5f + 0.5f * sin(t + i + 2);
//        GLfloat b = 0.5f + 0.5f * sin(t + i + 4);
//        vertices[i + 2] = r;
//        vertices[i + 3] = g;
//        vertices[i + 4] = b;
//    }
//}
//
//void drawTriangle()
//{
//    GLuint vao, vbo;
//    glGenVertexArrays(1, &vao);
//    glBindVertexArray(vao);
//
//    glGenBuffers(1, &vbo);
//    glBindBuffer(GL_ARRAY_BUFFER, vbo);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices), triangleVertices, GL_STATIC_DRAW);
//
//    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
//    glEnableVertexAttribArray(0);
//
//    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
//    glEnableVertexAttribArray(1);
//
//    glBindVertexArray(vao);
//    glDrawArrays(GL_TRIANGLES, 0, 3);
//    glBindVertexArray(0);
//
//    glDeleteVertexArrays(1, &vao);
//    glDeleteBuffers(1, &vbo);
//}
//
//int main()
//{
//    glfwInit();
//    GLFWwindow* window = glfwCreateWindow(800, 600, "Triangle", nullptr, nullptr);
//    glfwMakeContextCurrent(window);
//
//    glewExperimental = GL_TRUE;
//    glewInit();
//
//    while (!glfwWindowShouldClose(window))
//    {
//        glClear(GL_COLOR_BUFFER_BIT);
//        rotateVerticesColor((GLfloat*)triangleVertices, 15, 2.0f);
//        drawTriangle();
//        glfwSwapBuffers(window);
//        glfwPollEvents();
//    }
//
//    glfwTerminate();
//    return 0;
//}