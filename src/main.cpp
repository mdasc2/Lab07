/* Lab 5 base code - transforms using local matrix functions
	to be written by students -
	CPE 471 Cal Poly Z. Wood + S. Sueda
*/
#include <iostream>
#include <glad/glad.h>
#include "MatrixStack.h"
#include "GLSL.h"
#include "Program.h"
#include "Shape.h"
#include "glm/glm.hpp"
#include "WindowManager.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

 #define PI 3.1415926535897932384626433832795


void initializeVertexArray(GLfloat *);

// The best I got for right now.

//Andrew Chow
// CPE 471
// Program 2B
using namespace std;
using namespace glm;

class Application : public EventCallbacks
{

public:

	WindowManager * windowManager = nullptr;

	float globalRotate = 0.0;

	// Our shader program
	std::shared_ptr<Program> prog;
	std::shared_ptr<Program> otherProg;

	std::shared_ptr<Program> globeProg;

	// Shape to be used (from obj file)
	shared_ptr<Shape> shape;

	// Contains vertex information for OpenGL
	GLuint VertexArrayID;

	// Data necessary to give our triangle to OpenGL
	GLuint VertexBufferID;
	GLuint IndexBufferID;
	GLuint ColorBuffer;

	void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
	{
		if (key == GLFW_KEY_A && action == GLFW_PRESS)
		{
			globalRotate += -.1;
		}

		if (key == GLFW_KEY_D && action == GLFW_PRESS)
		{
			globalRotate += .1;
		}
	}

	void mouseCallback(GLFWwindow *window, int button, int action, int mods)
	{
		double posX, posY;

		if (action == GLFW_PRESS)
		{
			glfwGetCursorPos(window, &posX, &posY);
			cout << "Pos X " << posX <<  " Pos Y " << posY << endl;
		}
	}

	void resizeCallback(GLFWwindow *window, int width, int height)
	{
		glViewport(0, 0, width, height);
	}

	void init(const std::string& resourceDirectory)
	{
		GLSL::checkVersion();

		// Set background color.
		glClearColor(1.0f, 1.0f, 1.0f , 1.0f);

		// Enable z-buffer test.
		glEnable(GL_DEPTH_TEST);

		// Initialize the GLSL program.
		prog = make_shared<Program>();
		prog->setVerbose(true);
		prog->setShaderNames(resourceDirectory + "/simple_vert.glsl", resourceDirectory + "/simple_frag.glsl");
		if (! prog->init())
		{
			std::cerr << "One or more shaders failed to compile... exiting!" << std::endl;
			exit(1);
		}
		prog->init();
		prog->addUniform("P");
		prog->addUniform("MV");
		prog->addUniform("uColor");
		prog->addUniform("LightVector");
		prog->addAttribute("vertPos");
		prog->addAttribute("vertNor");

		otherProg = make_shared<Program>();
		otherProg->setVerbose(true);
		otherProg->setShaderNames(resourceDirectory + "/cylinder_vert.glsl", resourceDirectory + "/cylinder_frag.glsl");
		if (!otherProg->init())
		{
			std::cerr << "One or more shaders failed to compile... exiting!" << std::endl;
			exit(1);
		}
		otherProg->init();
		otherProg->addUniform("P");
		otherProg->addUniform("MV");
		otherProg->addUniform("uColor");
		otherProg->addUniform("LightVector");
		otherProg->addAttribute("vertPos");
		otherProg->addAttribute("vertNor");

		
	}

	void initGeom(const std::string& resourceDirectory)
	{
		//generate the VAO
		glGenVertexArrays(1, &VertexArrayID);
		glBindVertexArray(VertexArrayID);

		//generate vertex buffer to hand off to OGL
		glGenBuffers(1, &VertexBufferID);

		//set the current state to focus on our vertex buffer
		glBindBuffer(GL_ARRAY_BUFFER, VertexBufferID);


		GLfloat  g_vertex_buffer_data[66];

		initializeVertexArray(g_vertex_buffer_data);

		for (int i = 0; i < 66; i++)
		{
			cout << "Number" << i << " : " << g_vertex_buffer_data[i] << endl;
		}
		//actually memcopy the data - only do this once
		glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_DYNAMIC_DRAW);
		//we need to set up the vertex array
		glEnableVertexAttribArray(0);
		//key function to get up how many elements to pull out at a time (3)
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glGenBuffers(1, &IndexBufferID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferID);

		GLuint g_index_buffer_data[] = 
		{
			//One top face
			0,1,2,
			0,2,3,
			0,3,4,
			0,4,5,
			0,5,6,
			0,7,6,
			0,7,8,
			0,8,9,
			0,9,10,
			0,1,10,
			
			// Other top face
			11,12,13,
			11,13,14,
			11,14,15,
			11,15,16,
			11,16,17,
			11,17,18,
			11,18,19,
			11,19,20,
			11,20,21,
			11,12,21,

			// Sides of cylinder
			1,2,12,
			2,12,13,
			2,3,13,
			3,13,14,
			3,4,14,
			4,14,15,
			4,5,15,
			5,15,16,
			5,6,16,
			6,16,17,
			6,7,17,
			7,17,18,
			7,8,18,
			8,18,19,
			8,9,19,
			9,19,20,
			9,10,20,
			10,20,21,
			10,1,21,
			1,21,12,
		};

		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(g_index_buffer_data), g_index_buffer_data, GL_DYNAMIC_DRAW);

		GLfloat colorBuffer_Data[66];

		for (int i = 0; i < 66; i++)
		{
			colorBuffer_Data[i] = g_vertex_buffer_data[i];
		}

		glGenBuffers(1, &ColorBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, ColorBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(colorBuffer_Data), colorBuffer_Data, GL_STATIC_DRAW);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glBindVertexArray(0);
		// Initialize mesh.
		shape = make_shared<Shape>();
		shape->loadMesh(resourceDirectory + "/sphere.obj");
		shape->resize();
		shape->init();
	}

	void render()
	{
		// Local modelview matrix use this for lab 5
		// Get current frame buffer size.
		int width, height;
		glfwGetFramebufferSize(windowManager->getHandle(), &width, &height);
		glViewport(0, 0, width, height);

		// Clear framebuffer.

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		float aspect = width / (float)height;
		// Use the local matrices for lab 5
		auto P = std::make_shared<MatrixStack>();
		auto MV = std::make_shared<MatrixStack>();

		// Apply orthographic projection.
		P->pushMatrix();
		if (width > height)
		{
			P->ortho(-1 * aspect, 1 * aspect, -1, 1, -2, 100.0f);
		}
		else
		{
			P->ortho(-1, 1, -1 * 1 / aspect, 1 * 1 / aspect, -2, 100.0f);
		}
		// Left Circle
		MV->pushMatrix();
		MV->scale(.25);
		MV->translate(vec3(-2.0, 0, 0));
		MV->rotate((float)glfwGetTime(), vec3(0.0, 1.0, 0.0));

		prog->bind();

		glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, glm::value_ptr(P->topMatrix()));

		
		glUniformMatrix4fv(prog->getUniform("MV"), 1, GL_FALSE, glm::value_ptr(MV->topMatrix()));
		glUniform3f(prog->getUniform("uColor"), 1.0f, (216.0f / 255.0f), (112.0f / 255.0f));
		glUniform3f(prog->getUniform("LightVector"), -2.0, 4.0, 0.0);

		glBindVertexArray(VertexArrayID);

		glBindVertexArray(0);
		shape->draw(prog);

		prog->unbind();

		MV->popMatrix();

		//Right Circle
		MV->pushMatrix();
		MV->scale(.25);
		MV->translate(vec3(3.0, 0, 0));
		MV->rotate((float)glfwGetTime(), vec3(0.0, 1.0, 0.0));

		otherProg->bind();

		glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, glm::value_ptr(P->topMatrix()));
		glUniformMatrix4fv(prog->getUniform("MV"), 1, GL_FALSE, glm::value_ptr(MV->topMatrix()));
		glUniform3f(prog->getUniform("uColor"), 24.0f / 255.0f, 184.0f / 255.0f, 255.0f / 255.0f);
		glUniform3f(prog->getUniform("LightVector"), -5.0, 10.0 , 0.0);

		glBindVertexArray(VertexArrayID);

		glBindVertexArray(0);
		shape->draw(prog);

		otherProg->unbind();

		// Pop matrix stacks.
		MV->popMatrix();
		P->popMatrix();
	}
};

int main(int argc, char **argv)
{
	// Where the resources are loaded from
	std::string resourceDir = "../resources";

	if (argc >= 2)
	{
		resourceDir = argv[1];
	}

	Application *application = new Application();

	// Your main will always include a similar set up to establish your window
	// and GL context, etc.

	WindowManager *windowManager = new WindowManager();
	windowManager->init(640, 480);
	windowManager->setEventCallbacks(application);
	application->windowManager = windowManager;

	// This is the code that will likely change program to program as you
	// may need to initialize or set up different data and state

	application->init(resourceDir);
	application->initGeom(resourceDir);

	// Loop until the user closes the window.
	while (! glfwWindowShouldClose(windowManager->getHandle()))
	{
		// Render scene.
		application->render();

		// Swap front and back buffers.
		glfwSwapBuffers(windowManager->getHandle());
		// Poll for and process events.
		glfwPollEvents();
	}

	// Quit program.
	windowManager->shutdown();
	return 0;
}

void initializeVertexArray(GLfloat *array)
{
	float radius = .5;
	array[0] = 0.0;
	array[1] = 0.0;
	array[2] = 1.0;


	for (int i = 1; i <= 10; i++)
	{
			array[i * 3] = radius * cos(2 * PI * i / 10);
			array[i * 3 + 1] = radius * sin(2 * PI * i / 10);
			array[i * 3 + 2] = 1.0;
	}

	array[33] = 0.0;
	array[34] = 0.0;
	array[35] = -1.0;

	for (int i = 1; i <= 10; i++)
	{
		array[i * 3 +33] = radius * cos(2 * PI * i / 10);
		array[i * 3 + 1 + 33] = radius * sin(2 * PI * i / 10);
		array[i * 3 + 2 + 33] = -1.0;
	}

}


