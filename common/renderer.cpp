#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>
#include <fstream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/euler_angles.hpp>

#include <common/config.h>
#include <common/camera.h>
#include <common/renderer.h>

Renderer::Renderer()
{
	grid = new Grid();
	// We get the camera from the scene later
	_camera = nullptr;

	// Create window with OpenGL context
	this->init();
}

Renderer::~Renderer()
{
	// Cleanup VBO and shader
	glDeleteProgram(_programID);
	// Close OpenGL window and terminate GLFW
	glfwTerminate();
}

int Renderer::init()
{
	// Initialise GLFW
	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		return -1;
	}

	// Set OpenGL version (2.1)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_SAMPLES, 4);

	// Open a window and create its OpenGL context
	_window = glfwCreateWindow( WIDTH, HEIGHT, "Demo", NULL, NULL);
	if( _window == NULL ){
		fprintf( stderr, "Failed to open GLFW window.\n" );
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(_window);

	// Initialize GLEW
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(_window, GLFW_STICKY_KEYS, GL_TRUE);

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	// Cull triangles which normal is not towards the camera
	glEnable(GL_CULL_FACE);

	// Create and compile our GLSL program from the shaders
	_programID = this->loadShaders("shaders/sprite.vert", "shaders/sprite.frag");

	// Use our shader
	glUseProgram(_programID);

	return 0;
}

float Renderer::updateDeltaTime() {
	// lastTime is initialised only the first time this function is called
	static double lastTime = glfwGetTime();
	// get the current time
	double currentTime = glfwGetTime();

	// Compute time difference between current and last time
	float deltaTime = float(currentTime - lastTime);

	// For the next frame, the "last time" will be "now"
	lastTime = currentTime;
	return deltaTime;
}

void Renderer::renderScene(Scene* scene)
{
	// get camera from scene and update
	_camera = scene->camera();

	// Clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Render all te sprites
	for (Sprite* sprite : scene->sprites())
	{
		this->renderSprite(sprite);
	}

	// Swap buffers
	glfwSwapBuffers(this->window());
	glfwPollEvents();
}

void Renderer::renderSprite(Sprite* sprite)
{
	// get view + projectionmatrix from camera
	glm::mat4 viewMatrix = _camera->getViewMatrix();
	glm::mat4 projectionMatrix = _camera->getProjectionMatrix();

	// Build the Model matrix from Sprite transform
	glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(sprite->position.x, sprite->position.y, 0.0f));
	glm::mat4 rotationMatrix    = glm::eulerAngleYXZ(0.0f, 0.0f, sprite->rotation);
	glm::mat4 scalingMatrix     = glm::scale(glm::mat4(1.0f), glm::vec3(sprite->scale.x, sprite->scale.y, 1.0f));

	glm::mat4 modelMatrix = translationMatrix * rotationMatrix * scalingMatrix;

	// Build MVP matrix
	glm::mat4 MVP = projectionMatrix * viewMatrix * modelMatrix;

	// Send our transformation to the currently bound shader, in the "MVP" uniform
	GLuint matrixID = glGetUniformLocation(_programID, "MVP");
	glUniformMatrix4fv(matrixID, 1, GL_FALSE, &MVP[0][0]);

	// Bind our texture in Texture Unit 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, sprite->texture());
	// Set our "textureSampler" sampler to use Texture Unit 0
	GLuint textureID  = glGetUniformLocation(_programID, "textureSampler");
	glUniform1i(textureID, 0);

	// 1st attribute buffer : vertices
	GLuint vertexPositionID = glGetAttribLocation(_programID, "vertexPosition");
	glEnableVertexAttribArray(vertexPositionID);
	glBindBuffer(GL_ARRAY_BUFFER, sprite->vertexbuffer());
	glVertexAttribPointer(
		vertexPositionID, // The attribute we want to configure
		3,          // size : x,y,z => 3
		GL_FLOAT,   // type
		GL_FALSE,   // normalized?
		0,          // stride
		(void*)0    // array buffer offset
	);

	// 2nd attribute buffer : UVs
	GLuint vertexUVID = glGetAttribLocation(_programID, "vertexUV");
	glEnableVertexAttribArray(vertexUVID);
	glBindBuffer(GL_ARRAY_BUFFER, sprite->uvbuffer());
	glVertexAttribPointer(
		vertexUVID, // The attribute we want to configure
		2,          // size : U,V => 2
		GL_FLOAT,   // type
		GL_FALSE,   // normalized?
		0,          // stride
		(void*)0    // array buffer offset
	);

	// Draw the triangles
	glDrawArrays(GL_TRIANGLES, 0, 2*3); // 2*3 indices starting at 0 -> 2 triangles

	// cleanup
	glDisableVertexAttribArray(vertexPositionID);
	glDisableVertexAttribArray(vertexUVID);
}

GLuint Renderer::loadShaders(const std::string& vertex_file_path, const std::string& fragment_file_path)
{
	// Create the shaders
	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string vertexShaderCode;
	std::ifstream vertexShaderStream(vertex_file_path.c_str(), std::ios::in);
	if (vertexShaderStream.is_open()){
		std::string line = "";
		while (getline(vertexShaderStream, line)) {
			vertexShaderCode += "\n" + line;
		}
		vertexShaderStream.close();
	} else {
		printf("Can't to open %s.\n", vertex_file_path.c_str());
		return 0;
	}

	// Read the Fragment Shader code from the file
	std::string fragmentShaderCode;
	std::ifstream fragmentShaderStream(fragment_file_path.c_str(), std::ios::in);
	if (fragmentShaderStream.is_open()){
		std::string line = "";
		while (getline(fragmentShaderStream, line)) {
			fragmentShaderCode += "\n" + line;
		}
		fragmentShaderStream.close();
	} else {
		printf("Can't to open %s.\n", fragment_file_path.c_str());
		return 0;
	}

	GLint result = GL_FALSE;
	int infoLogLength;

	// Compile Vertex Shader
	printf("Compiling shader : %s\n", vertex_file_path.c_str());
	char const * vertexSourcePointer = vertexShaderCode.c_str();
	glShaderSource(vertexShaderID, 1, &vertexSourcePointer , NULL);
	glCompileShader(vertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &result);
	glGetShaderiv(vertexShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
	if ( infoLogLength > 0 ){
		std::vector<char> vertexShaderErrorMessage(infoLogLength+1);
		glGetShaderInfoLog(vertexShaderID, infoLogLength, NULL, &vertexShaderErrorMessage[0]);
		printf("%s\n", &vertexShaderErrorMessage[0]);
	}

	// Compile Fragment Shader
	printf("Compiling shader : %s\n", fragment_file_path.c_str());
	char const * fragmentSourcePointer = fragmentShaderCode.c_str();
	glShaderSource(fragmentShaderID, 1, &fragmentSourcePointer , NULL);
	glCompileShader(fragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &result);
	glGetShaderiv(fragmentShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
	if ( infoLogLength > 0 ){
		std::vector<char> fragmentShaderErrorMessage(infoLogLength+1);
		glGetShaderInfoLog(fragmentShaderID, infoLogLength, NULL, &fragmentShaderErrorMessage[0]);
		printf("%s\n", &fragmentShaderErrorMessage[0]);
	}

	// Link the program
	printf("Linking program\n");
	GLuint programID = glCreateProgram();
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);
	glLinkProgram(programID);

	// Check the program
	glGetProgramiv(programID, GL_LINK_STATUS, &result);
	glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
	if ( infoLogLength > 0 ){
		std::vector<char> programErrorMessage(infoLogLength+1);
		glGetProgramInfoLog(programID, infoLogLength, NULL, &programErrorMessage[0]);
		printf("%s\n", &programErrorMessage[0]);
	}

	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);

	return programID;
}

void Renderer::_renderLine(const glm::mat4 modelMatrix, Grid* line)
{
	// This will identify our vertex buffer
	GLuint vertexbuffer;
	// Generate 1 buffer, put the resulting identifier in vertexbuffer
	glGenBuffers(1, &vertexbuffer);
	// The following commands will talk about our 'vertexbuffer' buffer
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	// Give the vertices to OpenGL.
	glBufferData(GL_ARRAY_BUFFER, grid->linePoints.size() * sizeof(glm::vec3), grid->vertexLinePoints, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);
	// Draw the triangle !
	glDrawArrays(GL_LINES, 0, 2); // Starting from vertex 0; 2 vertices total -> 1 line
	glDisableVertexAttribArray(0);


	/*int linepoints = (line->points().size() * 2) - 1;
	if (line->closed()) {
		linepoints += 1;
	}*/

	/*if (line->dynamic()) {
		mesh = new Mesh();
		mesh->generateLine(line);
	}
	else {
		mesh = _resman.getLineMesh(line);
	}

	if (line->dynamic()) {
		delete mesh;
	}*/
}

void Renderer::_renderMap() {
	/*for (int c = 0; c < tilemap.cols; c++) {
		for (int r = 0; r < tilemap.rows; r++) {
			TileMap* tile = tilemap.getTile(c, r);
			if (tile != = 0) { // 0 => empty tile
				this.ctx.drawImage(
					this.tileAtlas, // image
					(tile - 1) * map.tsize, // source x
					0, // source y
					map.tsize, // source width
					map.tsize, // source height
					c * map.tsize,  // target x
					r * map.tsize, // target y
					map.tsize, // target width
					map.tsize // target height
				);
			}
		}
	}*/
}

void Renderer::_renderMesh(const glm::mat4 modelMatrix, int numverts, GLuint mode)
{
	// use our shader program
	//glUseProgram(shader->programID());

	// generate the ModelMatrix
	//glm::mat4 MVP = _projectionMatrix * _viewMatrix * modelMatrix;

	// ... and send our transformation to the currently bound shader, in the "MVP" uniform
	//glUniformMatrix4fv(shader->matrixID(), 1, GL_FALSE, &MVP[0][0]);

	// _blendColorID
	//glUniform4f(shader->blendColorID(), (float)blendcolor.r / 255.0f, (float)blendcolor.g / 255.0f, (float)blendcolor.b / 255.0f, (float)blendcolor.a / 255.0f);

	// Set our "textureSampler" sampler to user Texture Unit 0
	//glUniform1i(shader->textureID(), 0);

	// Set our "paletteSampler" sampler to user Texture Unit 1
	/*if (shader->paletteID() != -1) {
		glUniform1i(shader->paletteID(), 1);
	}*/

	// Note: We generated vertices in the correct order, with normals facing the camera.
	// We can also get the normalbuffer from the Mesh, but that's ignored here.
	// Use the normalbuffer (with links to the Shader) if you want to use lighting on your Sprites.
	// TODO: implement
	//GLuint vertexPositionID = glGetAttribLocation(shader->programID(), "vertexPosition"); // Mesh::_vertexbuffer
	//GLuint vertexUVID = glGetAttribLocation(shader->programID(), "vertexUV"); // Mesh::_uvbuffer

	//// 1st attribute buffer : vertices
	//glEnableVertexAttribArray(vertexPositionID);
	////glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexbuffer());
	//glVertexAttribPointer(
	//	vertexPositionID,             // The attribute we want to configure
	//	3,                            // size : x+y+z => 3
	//	GL_FLOAT,                     // type
	//	GL_FALSE,                     // normalized?
	//	0,                            // stride
	//	(void*)0                      // array buffer offset
	//);

	//// 2nd attribute buffer : UVs
	//glEnableVertexAttribArray(vertexUVID);
	////glBindBuffer(GL_ARRAY_BUFFER, mesh->uvbuffer());
	//glVertexAttribPointer(
	//	vertexUVID,                   // The attribute we want to configure
	//	2,                            // size : U+V => 2
	//	GL_FLOAT,                     // type
	//	GL_FALSE,                     // normalized?
	//	0,                            // stride
	//	(void*)0                      // array buffer offset
	//);

	//// Draw the triangles or lines
	//glDrawArrays(mode, 0, numverts);

	//glDisableVertexAttribArray(vertexPositionID);
	//glDisableVertexAttribArray(vertexUVID);
}