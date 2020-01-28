#include "stdafx.h"

using std::ifstream;
using std::string;


//#define SINGLE_BUFFER

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

RGBCubeScene::RGBCubeScene() { }

void RGBCubeScene::initScene()
{
	//////////////////////////////////////////////////////
	/////////// Vertex shader //////////////////////////
	//////////////////////////////////////////////////////

	// Load contents of file
	ifstream inFile("shader/basic.vert");
	if (!inFile) {
		fprintf(stderr, "Error opening file: shader/basic.vert\n");
		exit(1);
	}

	std::stringstream code;
	code << inFile.rdbuf();
	inFile.close();
	string codeStr(code.str());

	// Create the shader object
	GLuint vertShader = gl::CreateShader(gl::VERTEX_SHADER);
	if (0 == vertShader) {
		fprintf(stderr, "Error creating vertex shader.\n");
		exit(EXIT_FAILURE);
	}

	// Load the source code into the shader object
	const GLchar* codeArray[] = { codeStr.c_str() };
	gl::ShaderSource(vertShader, 1, codeArray, NULL);

	// Compile the shader
	gl::CompileShader(vertShader);

	// Check compilation status
	GLint result;
	gl::GetShaderiv(vertShader, gl::COMPILE_STATUS, &result);
	if (FALSE == result) {
		fprintf(stderr, "Vertex shader compilation failed!\n");

		GLint logLen;
		gl::GetShaderiv(vertShader, gl::INFO_LOG_LENGTH, &logLen);

		if (logLen > 0) {
			char * log = (char *)malloc(logLen);

			GLsizei written;
			gl::GetShaderInfoLog(vertShader, logLen, &written, log);

			fprintf(stderr, "Shader log: \n%s", log);

			free(log);
		}
	}

	//////////////////////////////////////////////////////
	/////////// Fragment shader //////////////////////////
	//////////////////////////////////////////////////////

	// Load contents of file into shaderCode here
	ifstream fragFile("shader/basic.frag");
	if (!fragFile) {
		fprintf(stderr, "Error opening file: shader/basic.frag\n");
		exit(1);
	}

	std::stringstream fragCode;
	fragCode << fragFile.rdbuf();
	fragFile.close();
	codeStr = fragCode.str();

	// Create the shader object
	GLuint fragShader = gl::CreateShader(gl::FRAGMENT_SHADER);
	if (0 == fragShader) {
		fprintf(stderr, "Error creating fragment shader.\n");
		exit(1);
	}

	// Load the source code into the shader object
	codeArray[0] = codeStr.c_str();
	gl::ShaderSource(fragShader, 1, codeArray, NULL);

	// Compile the shader
	gl::CompileShader(fragShader);

	// Check compilation status
	gl::GetShaderiv(fragShader, gl::COMPILE_STATUS, &result);
	if (FALSE == result) {
		fprintf(stderr, "Fragment shader compilation failed!\n");

		GLint logLen;
		gl::GetShaderiv(fragShader, gl::INFO_LOG_LENGTH, &logLen);

		if (logLen > 0) {
			char * log = (char *)malloc(logLen);

			GLsizei written;
			gl::GetShaderInfoLog(fragShader, logLen, &written, log);

			fprintf(stderr, "Shader log: \n%s", log);

			free(log);
		}
	}

	linkMe(vertShader, fragShader);


	//Indexed
	float positionData[] = {
		-0.2f, 0.8f, 0.0f,		//0  
		-0.2f, 0.4f, 0.0f,		//1
		 0.2f, 0.8f, 0.0f,		//2
		 0.2f, 0.4f, 0.0f,		//3
		-0.2f, 0.0f, 0.0f,		//4
		-0.2f,-0.4f, 0.0f,		//5
		 0.2f, 0.0f, 0.0f,		//6
		 0.2f,-0.4f, 0.0f,		//7
		 -0.2f,-0.8f,0.0f,		//8
		 0.2f,-0.8f,0.0f,		//9
		 -0.6f,-0.4f,0.0f,		//10
		 -0.6f,-0.8f, 0.0f,		//11
		 0.6f,-0.4f,0.0f,		//12
		 0.6f,-0.8f, 0.0f		//13



	};

	//Indexed
	float colourData[] = {
		0.0f, 0.0f, 0.0f,	//0 - Black
		0.0f, 0.0f, 1.0f,	//1 - Blue
		1.0f, 0.0f, 0.0f,	//2 - Red
		1.0f, 0.0f, 1.0f,	//3 - Magenta
		0.0f, 1.0f, 1.0f,	//4 - Cyan
		0.0f, 1.0f, 0.0f,	//5 - Green
		1.0f, 1.0f, 1.0f,	//6 - White
		1.0f, 1.0f, 0.0f,	//7 - Yellow
		0.0f, 0.0f, 0.0f,	//8 - Black
		1.0f, 0.0f, 0.0f,	//9 - Red
		0.0f, 1.0f, 1.0f,	//10 - Cyan
		0.0f, 0.0f, 1.0f,	//11 - Blue
		1.0f, 1.0f, 1.0f,	//12 - White
		1.0f, 0.0f, 1.0f,	//13 - Magenta
	};

	GLuint indices[] = {
		0,1,2, 
		1,3,2, 
		1,4,3,
		4,6,3,
		4,5,6,
		5,7,6,
		5,8,7,
		7,9,8,
		8,10,5,
		10,11,8,
		9,12,7,
		12,13,9

	};

#ifdef SINGLE_BUFFER
	//Create and load buffer
	gl::GenBuffers(1, &vbo);
	gl::BindBuffer(gl::ARRAY_BUFFER, vbo);
	gl::BufferData(gl::ARRAY_BUFFER, 4 * 3 * sizeof(GLfloat), NULL, gl::STATIC_DRAW);
	gl::BufferSubData(gl::ARRAY_BUFFER, 0, 4 * 3 * sizeof(GLfloat), positionData);
	gl::BufferSubData(gl::ARRAY_BUFFER, 4 * 3 * sizeof(GLfloat), 4 * 3 * sizeof(GLfloat), colourData);


	//Get the VertexPosition attrib handle
	GLuint loc1;
	loc1 = gl::GetAttribLocation(programHandle, "VertexPosition");
	gl::EnableVertexAttribArray(loc1);
	//Set the location and tell it the data format
	gl::VertexAttribPointer(loc1, 3, gl::FLOAT, FALSE, 0, 0);

	//Get the VertexColour attrib handle
	GLuint loc2;
	loc2 = gl::GetAttribLocation(programHandle, "VertexColour");
	gl::EnableVertexAttribArray(loc2);
	//Set the location and tell it the data format
	gl::VertexAttribPointer(loc2, 3, gl::FLOAT, FALSE, 0, BUFFER_OFFSET(3 * 3 * sizeof(GLfloat)));

#else

	// Create and set-up the vertex array object using an interlaced buffer
	gl::GenVertexArrays(1, &vaoHandle);
	gl::BindVertexArray(vaoHandle);


	GLuint numOfIndices = sizeof(indices) / sizeof(GLuint);

	indexSize = sizeof(indices);

	// Create and populate the buffer objects using separate buffers
	//GLuint vboHandles[3]; //Position/Colour/Indices
	gl::GenBuffers(3, vboHandles);
	GLuint positionBufferHandle = vboHandles[0];
	GLuint colorBufferHandle = vboHandles[1];
	GLuint indexBufferHandle = vboHandles[2];

	gl::BindBuffer(gl::ARRAY_BUFFER, positionBufferHandle);
	gl::BufferData(gl::ARRAY_BUFFER, sizeof(positionData), positionData, gl::STATIC_DRAW);

	gl::BindBuffer(gl::ARRAY_BUFFER, colorBufferHandle);
	gl::BufferData(gl::ARRAY_BUFFER, sizeof(colourData), colourData, gl::STATIC_DRAW);

	gl::BindBuffer(gl::ELEMENT_ARRAY_BUFFER, indexBufferHandle);
	gl::BufferData(gl::ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, gl::STATIC_DRAW);

	// Create and set-up the vertex array object
	gl::GenVertexArrays(1, &vaoHandle);
	gl::BindVertexArray(vaoHandle);

	gl::EnableVertexAttribArray(0);  // Vertex position
	gl::EnableVertexAttribArray(1);  // Vertex color

	gl::BindBuffer(gl::ARRAY_BUFFER, positionBufferHandle);
	gl::VertexAttribPointer(0, 3, gl::FLOAT, FALSE, 0, (GLubyte *)NULL);

	gl::BindBuffer(gl::ARRAY_BUFFER, colorBufferHandle);
	gl::VertexAttribPointer(1, 3, gl::FLOAT, FALSE, 0, (GLubyte *)NULL);

#endif

	}

void RGBCubeScene::linkMe(GLint vertShader, GLint fragShader)
{
	// Create the program object
	programHandle = gl::CreateProgram();
	if (0 == programHandle) {
		fprintf(stderr, "Error creating program object.\n");
		exit(1);
	}


	// Attach the shaders to the program object
	gl::AttachShader(programHandle, vertShader);
	gl::AttachShader(programHandle, fragShader);

	// Link the program
	gl::LinkProgram(programHandle);

	// Check for successful linking
	GLint status;
	gl::GetProgramiv(programHandle, gl::LINK_STATUS, &status);
	if (FALSE == status) {

		fprintf(stderr, "Failed to link shader program!\n");

		GLint logLen;
		gl::GetProgramiv(programHandle, gl::INFO_LOG_LENGTH, &logLen);

		if (logLen > 0) {
			char * log = (char *)malloc(logLen);

			GLsizei written;
			gl::GetProgramInfoLog(programHandle, logLen, &written, log);

			fprintf(stderr, "Program log: \n%s", log);

			free(log);
		}
	}
	else {
		gl::UseProgram(programHandle);
	}

}

void RGBCubeScene::update(float t)
{

}

void RGBCubeScene::render()
{
	gl::Clear(gl::COLOR_BUFFER_BIT);
	GLuint indexBufferHandle = vboHandles[2];
	gl::BindVertexArray(vaoHandle);
	gl::BindBuffer(gl::ELEMENT_ARRAY_BUFFER, indexBufferHandle);
	gl::DrawElements(gl::TRIANGLES, indexSize, gl::UNSIGNED_INT, (void*)0);
	//gl::DrawArrays(gl::TRIANGLES, 0, 3);
}

void RGBCubeScene::resize(int w, int h)
{
	gl::Viewport(0, 0, w, h);
}