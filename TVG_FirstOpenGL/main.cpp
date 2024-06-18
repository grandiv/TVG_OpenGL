#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>

// Define the source code for the vertex shader
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

// Define the source code for the fragment shader
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";

int main()
{
	// Initialize GLFW
	glfwInit();

	// Tell GLFW what version of OpenGL we are using
	// In this case we are using OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Tell GLFW we are using the CORE profile
	// So that means we only have the modern functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Normal floats may differ in size of the floats that OpenGL uses, so we use GLfloat
	// Define the triangle vertices, must be between -1 and 1 because it's been normalized
	// Meaning that the most left point of the window is -1, the most right point is 1, the top is 1, and the bottom is -1
	GLfloat vertices[] = {
		-0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,
		0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,
		0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f
	};

	// Create a GLFW window object of 800 by 800 pixels, naming it "First OpenGL", NOT full screen, and NOT shared
	GLFWwindow* window = glfwCreateWindow(800, 800, "First OpenGL", NULL, NULL);
	// Error check if the window fails to create
	if (window == NULL)
	{
		std::cout << "Failed to create window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// Introduce the window into the current context
	glfwMakeContextCurrent(window);

	// Load GLAD so it configures OpenGL
	gladLoadGL();

	// Specify the viewport of OpenGL in the Window
	// In this case the viewport goes from x=0, y=0, to x=800, y=800
	glViewport(0, 0, 800, 800);

	// Creating a vertex shader's value (a reference) to store the Vertex Shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	// Creating a fragment shader's value (a reference) to store the Fragment Shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	// To use the shaders we must create a shader program
	GLuint shaderProgram = glCreateProgram();
	// Attach the compiled shaders to the shader program
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	// Wrap up the Shader Program
	glLinkProgram(shaderProgram);
	// Delete the shaders we've created before because they're already in the program itself
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);


	// ~~ Vertex Buffer Object (VBO) ~~
	// We have to tell OpenGL how to interpret the vertices
	// Sending stuff between the CPU and the GPU is slow so we have to send it into big batches
	// These big batches are called buffers 
	// Create a Vertex Buffer Object (VBO) to store the vertex data 
	// VBO is usually an array of references, but we'll use a reference integer since we only have one object

	// ~~ Vertex Array Object (VAO) ~~
	// Is an object to store pointers to one or more VBO objects
	// and tells OpenGL how to interpret them
	// VAO exists in order to quickly switch between different VBOs

	// ~~ Binding ~~
	// Meaning that we make a certain object the current object
	// Whenever we fire a function that modifies the object, it modifies the currently binded object

	// !! IMPORTANT !! 
	// Generate the VAO BEFORE the VBO
	GLuint VAO, VBO;
	glGenVertexArrays(1, &VAO); // Generate VAO
	glGenBuffers(1, &VBO); // Generate VBO, 1 is for the first argument cus we only have one 3D object

	glBindVertexArray(VAO); // Bind the VAO
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // GL_ARRAY_BUFFER is a buffer type we need to use for the Vertex Buffer

	// Store our vertices in the VBO
	// STREAM means that the vertices will be modified once and used a few times
	// STATIC means that the vertices will be modified once and used many times
	// DYNAMIC means that the vertices will be modified many times and used many times
	// DRAW means that the vertices will be modified and be used to draw an image on the screen
	// You can imagine what the READ and COPY will do
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Configure Vertex Attribute to let OpenGL know how to read the VBO
	// Vertex Attribute is a way of communicating with a Vertex Shader from the outside
	// 0 is the location of the vertex attribute 
	// 3 is how many values we have per vertex, in our case that's 3 cus we have 3 floats
	// GL_FLOAT is the type of the values
	// GL_FALSE means that we don't want to normalize the values
	// Give stride of our vertices: the amount of data between each vertex. 
	// We have 3 floats, so that's just 3 times the size of one float
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	
	// Enable Vertex Attribute 
	// 0 is the position of the Vertex Attribute
	glEnableVertexAttribArray(0);

	// The following is not mandatory, but to make sure we don't accidentally change a VBO or VAO with a function
	// Basically just unbind both the VBO and VAO by binding to zero
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);




	// Specify the color of the background 
	glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
	// CLean the back buffer and assign the new color to it
	glClear(GL_COLOR_BUFFER_BIT);
	// Swap the back buffer with the front buffer
	glfwSwapBuffers(window);

	// Main while loop, only closes when we press the CLOSE button or if another function tells it to close 
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		// Activate the Shader Program 
		glUseProgram(shaderProgram);
		// Bind the VAO so that we're telling OpenGL that we want to use this one
		glBindVertexArray(VAO);
		// Drawing function, TRIANGLES is the primitive we want to use
		// 0 is the starting index of the vertices we want to draw
		// 3 is the amount of vertices we want to draw
		glDrawArrays(GL_TRIANGLES, 0, 3);
		// Swap the back buffer with the front buffer to make sure the image gets updated each frame
		glfwSwapBuffers(window);
		// Take care of all GLFW events, such as resizing window, mouse input, keyboard input, etc...
		glfwPollEvents();
	}

	// Delete all the objects we've created
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);

	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();
	return 0;
}