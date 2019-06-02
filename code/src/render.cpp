#include <GL\glew.h>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <cstdio>
#include <cassert>
#include <iostream>
#include <fstream>

#include "GL_framework.h"
#include <vector>

#include <imgui\imgui.h>
#include <imgui\imgui_impl_sdl_gl3.h>

glm::vec3 lightPos;
glm::vec3 actualLightPos{ 10,30,0 };
float currentTime = 0;
glm::vec3 firstCabinPosition = glm::vec3();
glm::vec3 offsetTrump = glm::vec3(-1.4f, 1.5f, 0.5f);
glm::vec3 offsetChicken = glm::vec3(1.3, 0.8f, -0.5f);
float timer = 0;
bool shootcountershoot = false;
bool enableShootCounteshoot = true;
bool scene1 = false;

#pragma region VARIABLES_SHADERS
std::ifstream file;
std::string vertexShaderString;
std::string vertexShaderStringChicken;
std::string fragShaderString;
#pragma endregion
#pragma region VARIABLES_CABINA
glm::vec3 cabinePosition = glm::vec3();
float cabineScale= 1.f;
glm::vec3 cabineRotation = glm::vec3();
#pragma endregion
#pragma region VARIABLES_WHEEL
glm::vec3 wheelPosition = glm::vec3(0, 4.1f, 0);
float wheelScale = 1.f;
glm::vec3 wheelRotation = glm::vec3();
#pragma endregion
#pragma region VARIABLES_LEGS
glm::vec3 legsPosition = glm::vec3(0, 3.62f, 0);
float legsScale = 1.f;
glm::vec3 legsRotation = glm::vec3();
#pragma endregion
#pragma region VARIABLES_TRUMP
glm::vec3 trumpPosition = glm::vec3(-1.2f, 1.f, 0);
float trumpScale = 1.f;
glm::vec3 trumpRotation = glm::vec3(0,90,0);
#pragma endregion
#pragma region VARIABLES_CHICKEN
glm::vec3 chickenPosition = glm::vec3(1.66f, 2.39f, 0.04f);
float chickenScale = 0.01f;
glm::vec3 chickenRotation = glm::vec3(0,-90,0);
#pragma endregion
#pragma region VARIABLES_CHICKEN_ARMY
glm::vec3 chickenArmyPosition = glm::vec3(-640, -400, -10);
float chickenArmyScale = 0.1f;
glm::vec3 chickenArmyRotation = glm::vec3(0,0,0);
float chickenArmyOffset = 60;
#pragma endregion
#pragma region VARIABLES_PENDULUM
glm::vec3 pendulumPosition = glm::vec3(0, 4, 0);
float pendulumScale = 0.15;
glm::vec3 pendulumRotation = glm::vec3(0,0,0);
float pendulumLenght = -6.7f;
float pendulumPower = 60;
float pendulumSpeed = 2;
#pragma endregion
#pragma region VARIABLES_NORIA
int numberCabines = 20;
float radius = 33.2f;
float frequency = 0.01f;
#pragma endregion

extern bool loadOBJ(const char * path,
	std::vector < glm::vec3 > & out_vertices,
	std::vector < glm::vec2 > & out_uvs,
	std::vector < glm::vec3 > & out_normals
);

#pragma region NAMESPACES
namespace Cube {
	const char* cube_vertShader;
	const char* cube_fragShader;
	void setupCube();
	void cleanupCube();
	void updateCube(const glm::mat4&);
	void drawCube(const glm::vec3, float, glm::vec3);
}

namespace Pendulum {
	const char* cube_vertShader;
	const char* cube_fragShader;
	void setupCube();
	void cleanupCube();
	void updateCube(const glm::mat4&);
	void drawCube(const glm::vec3, float, glm::vec3, glm::vec3);
}

namespace Cabina {
	const char* model_vertShader;
	const char* model_fragShader;
	void setupModel();
	void cleanupModel();
	void drawModel(glm::vec3, float, glm::vec3);
}

namespace Wheel {
	const char* model_vertShader;
	const char* model_fragShader;
	void setupModel();
	void cleanupModel();
	void drawModel(glm::vec3, float, glm::vec3);
}

namespace Legs {
	const char* model_vertShader;
	const char* model_fragShader;
	void setupModel();
	void cleanupModel();
	void drawModel(glm::vec3, float, glm::vec3);
}

namespace Trump {
	const char* model_vertShader;
	const char* model_fragShader;
	void setupModel();
	void cleanupModel();
	void drawModel(glm::vec3, float, glm::vec3);
}

namespace Chicken {
	const char* model_vertShader;
	const char* model_fragShader;
	void setupModel();
	void cleanupModel();
	void drawModel(glm::vec3, float, glm::vec3);
}

namespace ChickenArmy {
	const char* model_vertShader;
	const char* model_fragShader;
	void setupModel();
	void cleanupModel();
	void drawModel(glm::vec3, float, glm::vec3);
}
#pragma endregion

void reloadShaders() {

	file.open("shaders/vertexShaderChicken.txt");
	vertexShaderStringChicken = std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	ChickenArmy::model_vertShader = &vertexShaderStringChicken[0];

	file.close();

#pragma region VERTEX_SHADER
	file.open("shaders/vertexShader.txt");
	vertexShaderString = std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	Cabina::model_vertShader = &vertexShaderString[0];
	Wheel::model_vertShader = &vertexShaderString[0];
	Cube::cube_vertShader = &vertexShaderString[0];
	Legs::model_vertShader = &vertexShaderString[0];
	Chicken::model_vertShader = &vertexShaderString[0];
	Trump::model_vertShader = &vertexShaderString[0];
	Pendulum::cube_vertShader = &vertexShaderString[0];
	file.close();
#pragma endregion
#pragma region FRAGMENT_SHADER
	file.open("shaders/fragmentShader.txt");
	fragShaderString = std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	Cabina::model_fragShader = &fragShaderString[0];
	Wheel::model_fragShader = &fragShaderString[0];
	Cube::cube_fragShader = &fragShaderString[0];
	Legs::model_fragShader = &fragShaderString[0];
	Trump::model_fragShader = &fragShaderString[0];
	Chicken::model_fragShader = &fragShaderString[0];
	ChickenArmy::model_fragShader = &fragShaderString[0];
	Pendulum::cube_fragShader = &fragShaderString[0];

	file.close();
#pragma endregion
#pragma region SETUP_MODEL
	Cabina::setupModel();
	Wheel::setupModel();
	Cube::setupCube();
	Legs::setupModel();
	Trump::setupModel();
	Chicken::setupModel();
	ChickenArmy::setupModel();
	Pendulum::setupCube();
#pragma endregion
}

void setupModels() {
	Cube::setupCube();
	Cabina::setupModel();
	Wheel::setupModel();
	Legs::setupModel();
	Trump::setupModel();
	Chicken::setupModel();
	Pendulum::setupCube();
}

void GUI() {
	bool show = true;
	ImGui::Begin("Simulation Parameters", &show, 0);
	{
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);//FrameRate
		if (ImGui::Button("Reload shaders")) {
			reloadShaders();
			printf("Shaders reloadeds");
		}
		ImGui::Checkbox("Enable Shoot-Countershoot", &enableShootCounteshoot);
		if(ImGui::Button("Scene1")){
			scene1 = !scene1;
			timer = 0;
			shootcountershoot = false;
		}
		if (scene1) {
			ImGui::DragFloat3("Light Pos", &actualLightPos.x, 0.1f);
			ImGui::DragFloat3("offset Trump", &offsetTrump[0], 0.1f);
			ImGui::DragFloat3("offset Chicken", &offsetChicken[0], 0.1f);
			if (ImGui::TreeNode("Cabine")) {
				ImGui::DragFloat3("Position", &cabinePosition.x, 0.01f);
				ImGui::DragFloat("Scale", &cabineScale, 0.01f);
				ImGui::DragFloat3("Rotation", &cabineRotation.x, 0.1f);
				ImGui::TreePop();
			}

			if (ImGui::TreeNode("Wheel")) {
				ImGui::DragFloat3("Position", &wheelPosition.x, 0.01f);
				ImGui::DragFloat("Scale", &wheelScale, 0.01f);
				ImGui::DragFloat3("Rotation", &wheelRotation.x, 0.1f);
				ImGui::TreePop();
			}

			if (ImGui::TreeNode("Legs")) {
				ImGui::DragFloat3("Position", &legsPosition.x, 0.01f);
				ImGui::DragFloat("Scale", &legsScale, 0.01f);
				ImGui::DragFloat3("Rotation", &legsRotation.x, 0.1f);
				ImGui::TreePop();
			}

			if (ImGui::TreeNode("Trump")) {
				ImGui::DragFloat3("Position", &trumpPosition.x, 0.01f);
				ImGui::DragFloat("Scale", &trumpScale, 0.01f);
				ImGui::DragFloat3("Rotation", &trumpRotation.x, 0.1f);
				ImGui::TreePop();
			}

			if (ImGui::TreeNode("Chicken")) {
				ImGui::DragFloat3("Position", &chickenPosition.x, 0.01f);
				ImGui::DragFloat("Scale", &chickenScale, 0.01f);
				ImGui::DragFloat3("Rotation", &chickenRotation.x, 0.1f);
				ImGui::TreePop();
			}

			if (ImGui::TreeNode("ChickenArmy")) {
				ImGui::DragFloat3("Position", &chickenArmyPosition.x, 0.01f);
				ImGui::DragFloat("Scale", &chickenArmyScale, 0.01f);
				ImGui::DragFloat3("Rotation", &chickenArmyRotation.x, 0.1f);
				ImGui::DragFloat("Offset", &chickenArmyOffset, 0.1f);
				ImGui::TreePop();
			}

			if (ImGui::TreeNode("Pendulum")) {
				ImGui::DragFloat3("Position", &pendulumPosition.x, 0.01f);
				ImGui::DragFloat("Scale", &pendulumScale, 0.01f);
				ImGui::DragFloat3("Rotation", &pendulumRotation.x, 0.1f);
				ImGui::DragFloat("Lenght", &pendulumLenght, 0.1f);
				ImGui::DragFloat("Pendulum power", &pendulumPower, 1.f);
				ImGui::DragFloat("Pendulum speed", &pendulumSpeed, 0.1f);
				ImGui::TreePop();
			}

			if (ImGui::TreeNode("Noria")) {
				ImGui::DragInt("NumberCabines", &numberCabines);
				ImGui::DragFloat("Radius", &radius, 0.1f);
				ImGui::DragFloat("Frequency", &frequency, 0.01f);
				ImGui::TreePop();
			}
		}
	}
	ImGui::End();
}

///////// fw decl
namespace ImGui {
	void Render();
}

namespace RenderVars {
	const float FOV = glm::radians(65.f);
	const float zNear = 1.f;
	const float zFar = 500.f;

	glm::mat4 _projection;
	glm::mat4 _modelView;
	glm::mat4 _MVP;
	glm::mat4 _inv_modelview;
	glm::vec4 _cameraPoint;

	struct prevMouse {
		float lastx, lasty;
		MouseEvent::Button button = MouseEvent::Button::None;
		bool waspressed = false;
	} prevMouse;

	float panv[3] = { 0.f, 0.f, -80.f };
	float rota[2] = { glm::radians(40.f), glm::radians(10.f) };
}
namespace RV = RenderVars;

void GLResize(int width, int height) {
	glViewport(0, 0, width, height);
	if(height != 0) RV::_projection = glm::perspective(RV::FOV, (float)width / (float)height, RV::zNear, RV::zFar);
	else RV::_projection = glm::perspective(RV::FOV, 0.f, RV::zNear, RV::zFar);
}

void GLmousecb(MouseEvent ev) {
	if(RV::prevMouse.waspressed && RV::prevMouse.button == ev.button) {
		float diffx = ev.posx - RV::prevMouse.lastx;
		float diffy = ev.posy - RV::prevMouse.lasty;
		switch(ev.button) {
		case MouseEvent::Button::Left: // ROTATE
			RV::rota[0] += diffx * 0.005f;
			RV::rota[1] += diffy * 0.005f;
			break;
		case MouseEvent::Button::Right: // MOVE XY
			RV::panv[0] += diffx * 0.03f;
			RV::panv[1] -= diffy * 0.03f;
			break;
		case MouseEvent::Button::Middle: // MOVE Z
			RV::panv[2] += diffy * 0.05f;
			break;
		default: break;
		}
	} else {
		RV::prevMouse.button = ev.button;
		RV::prevMouse.waspressed = true;
	}
	RV::prevMouse.lastx = ev.posx;
	RV::prevMouse.lasty = ev.posy;
}

void GLinit(int width, int height) {
	glViewport(0, 0, width, height);
	glClearColor(0.2f, 0.2f, 0.2f, 1.f);
	glClearDepth(1.f);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	RV::_projection = glm::perspective(RV::FOV, (float)width/(float)height, RV::zNear, RV::zFar);

	reloadShaders();
	setupModels();

	lightPos =  glm::vec3(40, 40, 0);
}

void GLcleanup() {
	Cabina::cleanupModel();
	Wheel::cleanupModel();
}

void Scene1(float dt) {
	lightPos = actualLightPos * glm::mat3(RV::_modelView);

	Cube::drawCube(lightPos, 1, glm::vec3(1, 1, 1));

	wheelRotation.z = 2 * 3.14159f * frequency * currentTime;
	Wheel::drawModel(wheelPosition, wheelScale, wheelRotation);


	Legs::drawModel(legsPosition, legsScale, legsRotation);

	for (int i = 0; i < numberCabines; i++) {
		float x = radius * cos((2 * 3.14159f * frequency * currentTime) + ((2 * 3.14159f * i) / numberCabines));
		float y = radius * sin((2 * 3.14159f * frequency * currentTime) + ((2 * 3.14159f * i)) / numberCabines);
		Cabina::drawModel(glm::vec3(x, y, 0) + cabinePosition, cabineScale, cabineRotation);
		if (i == 0) {
			firstCabinPosition = glm::vec3(x, y, 0);
		}
	}

	ChickenArmy::drawModel(chickenArmyPosition, chickenArmyScale, chickenArmyRotation);

	pendulumRotation.z = glm::cos(currentTime * pendulumSpeed) * pendulumPower;
	Pendulum::drawCube(pendulumPosition + firstCabinPosition, pendulumScale, pendulumRotation, glm::vec3(0, pendulumLenght, 0));

	Chicken::drawModel(chickenPosition + firstCabinPosition, chickenScale, chickenRotation);
	Trump::drawModel(trumpPosition + firstCabinPosition, trumpScale, trumpRotation);
	if (shootcountershoot && enableShootCounteshoot) {
		if (timer < 2.f)
			RV::_modelView = glm::lookAt(trumpPosition + firstCabinPosition + offsetTrump, chickenPosition + firstCabinPosition, glm::vec3(0, 1, 0));
		else if (timer < 4.f)
			RV::_modelView = glm::lookAt(chickenPosition + firstCabinPosition + offsetChicken, trumpPosition + firstCabinPosition, glm::vec3(0, 1, 0));
		else if (timer >= 4.f) {
			RV::_modelView = glm::lookAt(trumpPosition + firstCabinPosition + offsetTrump, chickenPosition + firstCabinPosition, glm::vec3(0, 1, 0));
			timer = 0;
		}
		timer += dt;
	}
	else
	{
		if (timer < 2.f) {
			timer += dt;
		}
		else {
			shootcountershoot = true;
			timer = 0;
		}
	}
}

void GLrender(float dt) {
	currentTime += dt;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	RV::_modelView = glm::mat4(1.f);
	RV::_modelView = glm::translate(RV::_modelView, glm::vec3(RV::panv[0], RV::panv[1], RV::panv[2]));
	RV::_modelView = glm::rotate(RV::_modelView, RV::rota[1], glm::vec3(1.f, 0.f, 0.f));
	RV::_modelView = glm::rotate(RV::_modelView, RV::rota[0], glm::vec3(0.f, 1.f, 0.f));
	
	if(scene1) Scene1(dt);

	RV::_MVP = RV::_projection * RV::_modelView;

	ImGui::Render();
}

//////////////////////////////////// COMPILE AND LINK
GLuint compileShader(const char* shaderStr, GLenum shaderType, const char* name="") {
	GLuint shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &shaderStr, NULL);
	glCompileShader(shader);
	GLint res;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &res);
	if (res == GL_FALSE) {
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &res);
		char *buff = new char[res];
		glGetShaderInfoLog(shader, res, &res, buff);
		fprintf(stderr, "Error Shader %s: %s", name, buff);
		delete[] buff;
		glDeleteShader(shader);
		return 0;
	}
	return shader;
}
void linkProgram(GLuint program) {
	glLinkProgram(program);
	GLint res;
	glGetProgramiv(program, GL_LINK_STATUS, &res);
	if (res == GL_FALSE) {
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &res);
		char *buff = new char[res];
		glGetProgramInfoLog(program, res, &res, buff);
		fprintf(stderr, "Error Link: %s", buff);
		delete[] buff;
	}
}

namespace Cube {
	GLuint cubeVao;
	GLuint cubeVbo[3];
	GLuint cubeShaders[2];
	GLuint cubeProgram;
	glm::mat4 objMat = glm::mat4(1.f);

	extern const float halfW = 0.5f;
	int numVerts = 24 + 6; // 4 vertex/face * 6 faces + 6 PRIMITIVE RESTART

						   //   4---------7
						   //  /|        /|
						   // / |       / |
						   //5---------6  |
						   //|  0------|--3
						   //| /       | /
						   //|/        |/
						   //1---------2
	glm::vec3 verts[] = {
		glm::vec3(-halfW, -halfW, -halfW),
		glm::vec3(-halfW, -halfW,  halfW),
		glm::vec3(halfW, -halfW,  halfW),
		glm::vec3(halfW, -halfW, -halfW),
		glm::vec3(-halfW,  halfW, -halfW),
		glm::vec3(-halfW,  halfW,  halfW),
		glm::vec3(halfW,  halfW,  halfW),
		glm::vec3(halfW,  halfW, -halfW)
	};
	glm::vec3 norms[] = {
		glm::vec3(0.f, -1.f,  0.f),
		glm::vec3(0.f,  1.f,  0.f),
		glm::vec3(-1.f,  0.f,  0.f),
		glm::vec3(1.f,  0.f,  0.f),
		glm::vec3(0.f,  0.f, -1.f),
		glm::vec3(0.f,  0.f,  1.f)
	};

	glm::vec3 cubeVerts[] = {
		verts[1], verts[0], verts[2], verts[3],
		verts[5], verts[6], verts[4], verts[7],
		verts[1], verts[5], verts[0], verts[4],
		verts[2], verts[3], verts[6], verts[7],
		verts[0], verts[4], verts[3], verts[7],
		verts[1], verts[2], verts[5], verts[6]
	};
	glm::vec3 cubeNorms[] = {
		norms[0], norms[0], norms[0], norms[0],
		norms[1], norms[1], norms[1], norms[1],
		norms[2], norms[2], norms[2], norms[2],
		norms[3], norms[3], norms[3], norms[3],
		norms[4], norms[4], norms[4], norms[4],
		norms[5], norms[5], norms[5], norms[5]
	};
	GLubyte cubeIdx[] = {
		0, 1, 2, 3, UCHAR_MAX,
		4, 5, 6, 7, UCHAR_MAX,
		8, 9, 10, 11, UCHAR_MAX,
		12, 13, 14, 15, UCHAR_MAX,
		16, 17, 18, 19, UCHAR_MAX,
		20, 21, 22, 23, UCHAR_MAX
	};

	void setupCube() {
		glGenVertexArrays(1, &cubeVao);
		glBindVertexArray(cubeVao);
		glGenBuffers(3, cubeVbo);

		glBindBuffer(GL_ARRAY_BUFFER, cubeVbo[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVerts), cubeVerts, GL_STATIC_DRAW);
		glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, cubeVbo[1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(cubeNorms), cubeNorms, GL_STATIC_DRAW);
		glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);

		glPrimitiveRestartIndex(UCHAR_MAX);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeVbo[2]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIdx), cubeIdx, GL_STATIC_DRAW);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		cubeShaders[0] = compileShader(cube_vertShader, GL_VERTEX_SHADER, "cubeVert");
		cubeShaders[1] = compileShader(cube_fragShader, GL_FRAGMENT_SHADER, "cubeFrag");

		cubeProgram = glCreateProgram();
		glAttachShader(cubeProgram, cubeShaders[0]);
		glAttachShader(cubeProgram, cubeShaders[1]);
		glBindAttribLocation(cubeProgram, 0, "in_Position");
		glBindAttribLocation(cubeProgram, 1, "in_Normal");
		linkProgram(cubeProgram);
	}

	void cleanupCube() {
		glDeleteBuffers(3, cubeVbo);
		glDeleteVertexArrays(1, &cubeVao);

		glDeleteProgram(cubeProgram);
		glDeleteShader(cubeShaders[0]);
		glDeleteShader(cubeShaders[1]);

	}

	void drawCube(const glm::vec3 position, float scaleObject, glm::vec3 rotation) {
		glm::mat4 translate = glm::translate(glm::mat4(1), position);
		glm::mat4 scale = glm::scale(glm::mat4(1), glm::vec3(scaleObject, scaleObject, scaleObject));
		glm::mat4 rotatex = glm::rotate(glm::mat4(1), glm::radians(rotation.x), glm::vec3(1, 0, 0));
		glm::mat4 rotatey = glm::rotate(glm::mat4(1), glm::radians(rotation.y), glm::vec3(0, 1, 0));
		glm::mat4 rotatez = glm::rotate(glm::mat4(1), glm::radians(rotation.z), glm::vec3(0, 0, 1));

		objMat = translate * rotatex * rotatey * rotatez * scale;

		glEnable(GL_PRIMITIVE_RESTART);
		glBindVertexArray(cubeVao);
		glUseProgram(cubeProgram);
		glUniformMatrix4fv(glGetUniformLocation(cubeProgram, "objMat"), 1, GL_FALSE, glm::value_ptr(objMat));
		glUniformMatrix4fv(glGetUniformLocation(cubeProgram, "mv_Mat"), 1, GL_FALSE, glm::value_ptr(RenderVars::_modelView));
		glUniformMatrix4fv(glGetUniformLocation(cubeProgram, "mvpMat"), 1, GL_FALSE, glm::value_ptr(RenderVars::_MVP));
		glUniform3f(glGetUniformLocation(cubeProgram, "lPos"), lightPos.x, lightPos.y, lightPos.z);
		glUniform4f(glGetUniformLocation(cubeProgram, "color"), .5f, .5f, 1.f, 0.f);

		glDrawElements(GL_TRIANGLE_STRIP, numVerts, GL_UNSIGNED_BYTE, 0);

		glUseProgram(0);
		glBindVertexArray(0);
		glDisable(GL_PRIMITIVE_RESTART);
	}
}

namespace Pendulum {
	GLuint cubeVao;
	GLuint cubeVbo[3];
	GLuint cubeShaders[2];
	GLuint cubeProgram;
	glm::mat4 objMat = glm::mat4(1.f);

	extern const float halfW = 0.5f;
	int numVerts = 24 + 6; // 4 vertex/face * 6 faces + 6 PRIMITIVE RESTART

						   //   4---------7
						   //  /|        /|
						   // / |       / |
						   //5---------6  |
						   //|  0------|--3
						   //| /       | /
						   //|/        |/
						   //1---------2
	glm::vec3 verts[] = {
		glm::vec3(-halfW, -halfW, -halfW),
		glm::vec3(-halfW, -halfW,  halfW),
		glm::vec3(halfW, -halfW,  halfW),
		glm::vec3(halfW, -halfW, -halfW),
		glm::vec3(-halfW,  halfW, -halfW),
		glm::vec3(-halfW,  halfW,  halfW),
		glm::vec3(halfW,  halfW,  halfW),
		glm::vec3(halfW,  halfW, -halfW)
	};
	glm::vec3 norms[] = {
		glm::vec3(0.f, -1.f,  0.f),
		glm::vec3(0.f,  1.f,  0.f),
		glm::vec3(-1.f,  0.f,  0.f),
		glm::vec3(1.f,  0.f,  0.f),
		glm::vec3(0.f,  0.f, -1.f),
		glm::vec3(0.f,  0.f,  1.f)
	};

	glm::vec3 cubeVerts[] = {
		verts[1], verts[0], verts[2], verts[3],
		verts[5], verts[6], verts[4], verts[7],
		verts[1], verts[5], verts[0], verts[4],
		verts[2], verts[3], verts[6], verts[7],
		verts[0], verts[4], verts[3], verts[7],
		verts[1], verts[2], verts[5], verts[6]
	};
	glm::vec3 cubeNorms[] = {
		norms[0], norms[0], norms[0], norms[0],
		norms[1], norms[1], norms[1], norms[1],
		norms[2], norms[2], norms[2], norms[2],
		norms[3], norms[3], norms[3], norms[3],
		norms[4], norms[4], norms[4], norms[4],
		norms[5], norms[5], norms[5], norms[5]
	};
	GLubyte cubeIdx[] = {
		0, 1, 2, 3, UCHAR_MAX,
		4, 5, 6, 7, UCHAR_MAX,
		8, 9, 10, 11, UCHAR_MAX,
		12, 13, 14, 15, UCHAR_MAX,
		16, 17, 18, 19, UCHAR_MAX,
		20, 21, 22, 23, UCHAR_MAX
	};

	void setupCube() {
		glGenVertexArrays(1, &cubeVao);
		glBindVertexArray(cubeVao);
		glGenBuffers(3, cubeVbo);

		glBindBuffer(GL_ARRAY_BUFFER, cubeVbo[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVerts), cubeVerts, GL_STATIC_DRAW);
		glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, cubeVbo[1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(cubeNorms), cubeNorms, GL_STATIC_DRAW);
		glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);

		glPrimitiveRestartIndex(UCHAR_MAX);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeVbo[2]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIdx), cubeIdx, GL_STATIC_DRAW);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		cubeShaders[0] = compileShader(cube_vertShader, GL_VERTEX_SHADER, "cubeVert");
		cubeShaders[1] = compileShader(cube_fragShader, GL_FRAGMENT_SHADER, "cubeFrag");

		cubeProgram = glCreateProgram();
		glAttachShader(cubeProgram, cubeShaders[0]);
		glAttachShader(cubeProgram, cubeShaders[1]);
		glBindAttribLocation(cubeProgram, 0, "in_Position");
		glBindAttribLocation(cubeProgram, 1, "in_Normal");
		linkProgram(cubeProgram);
	}

	void cleanupCube() {
		glDeleteBuffers(3, cubeVbo);
		glDeleteVertexArrays(1, &cubeVao);

		glDeleteProgram(cubeProgram);
		glDeleteShader(cubeShaders[0]);
		glDeleteShader(cubeShaders[1]);

	}

	void drawCube(const glm::vec3 position, float scaleObject, glm::vec3 rotation, glm::vec3 pendulumDistance) {
		glm::mat4 translate = glm::translate(glm::mat4(1), position);
		glm::mat4 scale = glm::scale(glm::mat4(1), glm::vec3(scaleObject, scaleObject, scaleObject));
		glm::mat4 rotatex = glm::rotate(glm::mat4(1), glm::radians(rotation.x), glm::vec3(1, 0, 0));
		glm::mat4 rotatey = glm::rotate(glm::mat4(1), glm::radians(rotation.y), glm::vec3(0, 1, 0));
		glm::mat4 rotatez = glm::rotate(glm::mat4(1), glm::radians(rotation.z), glm::vec3(0, 0, 1));
		glm::mat4 pendulumDist = glm::translate(glm::mat4(1), pendulumDistance);

		objMat = translate * rotatex * rotatey * rotatez * scale * pendulumDist;

		glEnable(GL_PRIMITIVE_RESTART);
		glBindVertexArray(cubeVao);
		glUseProgram(cubeProgram);
		glUniformMatrix4fv(glGetUniformLocation(cubeProgram, "objMat"), 1, GL_FALSE, glm::value_ptr(objMat));
		glUniformMatrix4fv(glGetUniformLocation(cubeProgram, "mv_Mat"), 1, GL_FALSE, glm::value_ptr(RenderVars::_modelView));
		glUniformMatrix4fv(glGetUniformLocation(cubeProgram, "mvpMat"), 1, GL_FALSE, glm::value_ptr(RenderVars::_MVP));
		glUniform3f(glGetUniformLocation(cubeProgram, "lPos"), lightPos.x, lightPos.y, lightPos.z);
		glUniform4f(glGetUniformLocation(cubeProgram, "color"), .5f, .5f, 1.f, 0.f);

		glDrawElements(GL_TRIANGLE_STRIP, numVerts, GL_UNSIGNED_BYTE, 0);

		glUseProgram(0);
		glBindVertexArray(0);
		glDisable(GL_PRIMITIVE_RESTART);
	}
}

namespace Cabina {
	GLuint modelVao;
	GLuint modelVbo[3];
	GLuint modelShaders[2];
	GLuint modelProgram;
	glm::mat4 objMat = glm::mat4(1.f);
	std::string modelPath = "models/Cabine.obj";
	int nverts;

	void setupModel() {
		std::vector<glm::vec3> verts, norms;
		std::vector<glm::vec2> uvs;

		loadOBJ(&modelPath[0], verts, uvs, norms);
		nverts = verts.size();

		glGenVertexArrays(1, &modelVao);
		glBindVertexArray(modelVao);
		glGenBuffers(3, modelVbo);

		glBindBuffer(GL_ARRAY_BUFFER, modelVbo[0]);
		glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(glm::vec3), &verts[0], GL_STATIC_DRAW);
		glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, modelVbo[1]);
		glBufferData(GL_ARRAY_BUFFER, norms.size() * sizeof(glm::vec3), &norms[0], GL_STATIC_DRAW);
		glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		modelShaders[0] = compileShader(model_vertShader, GL_VERTEX_SHADER, "cubeVert");
		modelShaders[1] = compileShader(model_fragShader, GL_FRAGMENT_SHADER, "cubeFrag");

		modelProgram = glCreateProgram();
		glAttachShader(modelProgram, modelShaders[0]);
		glAttachShader(modelProgram, modelShaders[1]);
		glBindAttribLocation(modelProgram, 0, "in_Position");
		glBindAttribLocation(modelProgram, 1, "in_Normal");
		linkProgram(modelProgram);
	}

	void cleanupModel() {
	
		glDeleteBuffers(2, modelVbo);
		glDeleteVertexArrays(1, &modelVao);

		glDeleteProgram(modelProgram);
		glDeleteShader(modelShaders[0]);
		glDeleteShader(modelShaders[1]);
	}

	void drawModel(glm::vec3 position, float scaleObject, glm::vec3 rotation) {
		glm::mat4 translate = glm::translate(glm::mat4(1), position);
		glm::mat4 scale = glm::scale(glm::mat4(1), glm::vec3(scaleObject, scaleObject, scaleObject));
		glm::mat4 rotatex = glm::rotate(glm::mat4(1), glm::radians(rotation.x), glm::vec3(1, 0, 0));
		glm::mat4 rotatey = glm::rotate(glm::mat4(1), glm::radians(rotation.y), glm::vec3(0, 1, 0));
		glm::mat4 rotatez = glm::rotate(glm::mat4(1), glm::radians(rotation.z), glm::vec3(0, 0, 1));

		objMat = translate * rotatex * rotatey * rotatez * scale;

		glBindVertexArray(modelVao);
		glUseProgram(modelProgram);
		glUniformMatrix4fv(glGetUniformLocation(modelProgram, "objMat"), 1, GL_FALSE, glm::value_ptr(objMat));
		glUniformMatrix4fv(glGetUniformLocation(modelProgram, "mv_Mat"), 1, GL_FALSE, glm::value_ptr(RenderVars::_modelView));
		glUniformMatrix4fv(glGetUniformLocation(modelProgram, "mvpMat"), 1, GL_FALSE, glm::value_ptr(RenderVars::_MVP));
		glUniform3f(glGetUniformLocation(modelProgram, "lPos"), lightPos.x, lightPos.y, lightPos.z);
		glUniform4f(glGetUniformLocation(modelProgram, "color"), 0.5f, .5f, 1.f, 0.f);
	
		glDrawArrays(GL_TRIANGLES, 0, nverts);

		glUseProgram(0);
		glBindVertexArray(0);

	}
}

namespace Wheel {
	GLuint modelVao;
	GLuint modelVbo[3];
	GLuint modelShaders[2];
	GLuint modelProgram;
	glm::mat4 objMat = glm::mat4(1.f);
	std::string modelPath = "models/Wheel.obj";
	int nverts;

	void setupModel() {
		std::vector<glm::vec3> verts, norms;
		std::vector<glm::vec2> uvs;

		loadOBJ(&modelPath[0], verts, uvs, norms);
		nverts = verts.size();

		glGenVertexArrays(1, &modelVao);
		glBindVertexArray(modelVao);
		glGenBuffers(3, modelVbo);

		glBindBuffer(GL_ARRAY_BUFFER, modelVbo[0]);
		glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(glm::vec3), &verts[0], GL_STATIC_DRAW);
		glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, modelVbo[1]);
		glBufferData(GL_ARRAY_BUFFER, norms.size() * sizeof(glm::vec3), &norms[0], GL_STATIC_DRAW);
		glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		modelShaders[0] = compileShader(model_vertShader, GL_VERTEX_SHADER, "cubeVert");
		modelShaders[1] = compileShader(model_fragShader, GL_FRAGMENT_SHADER, "cubeFrag");

		modelProgram = glCreateProgram();
		glAttachShader(modelProgram, modelShaders[0]);
		glAttachShader(modelProgram, modelShaders[1]);
		glBindAttribLocation(modelProgram, 0, "in_Position");
		glBindAttribLocation(modelProgram, 1, "in_Normal");
		linkProgram(modelProgram);
	}

	void cleanupModel() {

		glDeleteBuffers(2, modelVbo);
		glDeleteVertexArrays(1, &modelVao);

		glDeleteProgram(modelProgram);
		glDeleteShader(modelShaders[0]);
		glDeleteShader(modelShaders[1]);
	}

	void drawModel(glm::vec3 position, float scaleObject, glm::vec3 rotation) {

		glm::mat4 translate = glm::translate(glm::mat4(1), position);
		glm::mat4 scale = glm::scale(glm::mat4(1), glm::vec3(scaleObject, scaleObject, scaleObject));
		glm::mat4 rotatex = glm::rotate(glm::mat4(1), rotation.x, glm::vec3(1, 0, 0));
		glm::mat4 rotatey = glm::rotate(glm::mat4(1), rotation.y, glm::vec3(0, 1, 0));
		glm::mat4 rotatez = glm::rotate(glm::mat4(1), rotation.z, glm::vec3(0, 0, 1));

		objMat = translate * rotatex * rotatey * rotatez * scale;

		glBindVertexArray(modelVao);
		glUseProgram(modelProgram);
		glUniformMatrix4fv(glGetUniformLocation(modelProgram, "objMat"), 1, GL_FALSE, glm::value_ptr(objMat));
		glUniformMatrix4fv(glGetUniformLocation(modelProgram, "mv_Mat"), 1, GL_FALSE, glm::value_ptr(RenderVars::_modelView));
		glUniformMatrix4fv(glGetUniformLocation(modelProgram, "mvpMat"), 1, GL_FALSE, glm::value_ptr(RenderVars::_MVP));
		glUniform3f(glGetUniformLocation(modelProgram, "lPos"), lightPos.x, lightPos.y, lightPos.z);
		glUniform4f(glGetUniformLocation(modelProgram, "color"), 0.5f, .5f, 1.f, 0.f);

		glDrawArrays(GL_TRIANGLES, 0, nverts);


		glUseProgram(0);
		glBindVertexArray(0);

	}
}

namespace Legs {
	GLuint modelVao;
	GLuint modelVbo[3];
	GLuint modelShaders[2];
	GLuint modelProgram;
	glm::mat4 objMat = glm::mat4(1.f);
	std::string modelPath = "models/Legs.obj";
	int nverts;

	void setupModel() {
		std::vector<glm::vec3> verts, norms;
		std::vector<glm::vec2> uvs;

		loadOBJ(&modelPath[0], verts, uvs, norms);
		nverts = verts.size();

		glGenVertexArrays(1, &modelVao);
		glBindVertexArray(modelVao);
		glGenBuffers(3, modelVbo);

		glBindBuffer(GL_ARRAY_BUFFER, modelVbo[0]);
		glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(glm::vec3), &verts[0], GL_STATIC_DRAW);
		glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, modelVbo[1]);
		glBufferData(GL_ARRAY_BUFFER, norms.size() * sizeof(glm::vec3), &norms[0], GL_STATIC_DRAW);
		glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		modelShaders[0] = compileShader(model_vertShader, GL_VERTEX_SHADER, "cubeVert");
		modelShaders[1] = compileShader(model_fragShader, GL_FRAGMENT_SHADER, "cubeFrag");

		modelProgram = glCreateProgram();
		glAttachShader(modelProgram, modelShaders[0]);
		glAttachShader(modelProgram, modelShaders[1]);
		glBindAttribLocation(modelProgram, 0, "in_Position");
		glBindAttribLocation(modelProgram, 1, "in_Normal");
		linkProgram(modelProgram);
	}

	void cleanupModel() {

		glDeleteBuffers(2, modelVbo);
		glDeleteVertexArrays(1, &modelVao);

		glDeleteProgram(modelProgram);
		glDeleteShader(modelShaders[0]);
		glDeleteShader(modelShaders[1]);
	}

	void drawModel(glm::vec3 position, float scaleObject, glm::vec3 rotation) {

		glm::mat4 translate = glm::translate(glm::mat4(1), position);
		glm::mat4 scale = glm::scale(glm::mat4(1), glm::vec3(scaleObject, scaleObject, scaleObject));
		glm::mat4 rotatex = glm::rotate(glm::mat4(1), rotation.x, glm::vec3(1, 0, 0));
		glm::mat4 rotatey = glm::rotate(glm::mat4(1), rotation.y, glm::vec3(0, 1, 0));
		glm::mat4 rotatez = glm::rotate(glm::mat4(1), rotation.z, glm::vec3(0, 0, 1));

		objMat = translate * rotatex * rotatey * rotatez * scale;

		glBindVertexArray(modelVao);
		glUseProgram(modelProgram);
		glUniformMatrix4fv(glGetUniformLocation(modelProgram, "objMat"), 1, GL_FALSE, glm::value_ptr(objMat));
		glUniformMatrix4fv(glGetUniformLocation(modelProgram, "mv_Mat"), 1, GL_FALSE, glm::value_ptr(RenderVars::_modelView));
		glUniformMatrix4fv(glGetUniformLocation(modelProgram, "mvpMat"), 1, GL_FALSE, glm::value_ptr(RenderVars::_MVP));
		glUniform3f(glGetUniformLocation(modelProgram, "lPos"), lightPos.x, lightPos.y, lightPos.z);
		glUniform4f(glGetUniformLocation(modelProgram, "color"), 0.5f, .5f, 1.f, 0.f);

		glDrawArrays(GL_TRIANGLES, 0, nverts);


		glUseProgram(0);
		glBindVertexArray(0);

	}
}

namespace Trump {
	GLuint modelVao;
	GLuint modelVbo[3];
	GLuint modelShaders[2];
	GLuint modelProgram;
	glm::mat4 objMat = glm::mat4(1.f);
	std::string modelPath = "models/Trump.obj";
	int nverts;

	void setupModel() {
		std::vector<glm::vec3> verts, norms;
		std::vector<glm::vec2> uvs;

		loadOBJ(&modelPath[0], verts, uvs, norms);
		nverts = verts.size();

		glGenVertexArrays(1, &modelVao);
		glBindVertexArray(modelVao);
		glGenBuffers(3, modelVbo);

		glBindBuffer(GL_ARRAY_BUFFER, modelVbo[0]);
		glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(glm::vec3), &verts[0], GL_STATIC_DRAW);
		glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, modelVbo[1]);
		glBufferData(GL_ARRAY_BUFFER, norms.size() * sizeof(glm::vec3), &norms[0], GL_STATIC_DRAW);
		glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		modelShaders[0] = compileShader(model_vertShader, GL_VERTEX_SHADER, "cubeVert");
		modelShaders[1] = compileShader(model_fragShader, GL_FRAGMENT_SHADER, "cubeFrag");

		modelProgram = glCreateProgram();
		glAttachShader(modelProgram, modelShaders[0]);
		glAttachShader(modelProgram, modelShaders[1]);
		glBindAttribLocation(modelProgram, 0, "in_Position");
		glBindAttribLocation(modelProgram, 1, "in_Normal");
		linkProgram(modelProgram);
	}

	void cleanupModel() {

		glDeleteBuffers(2, modelVbo);
		glDeleteVertexArrays(1, &modelVao);

		glDeleteProgram(modelProgram);
		glDeleteShader(modelShaders[0]);
		glDeleteShader(modelShaders[1]);
	}

	void drawModel(glm::vec3 position, float scaleObject, glm::vec3 rotation) {

		glm::mat4 translate = glm::translate(glm::mat4(1), position);
		glm::mat4 scale = glm::scale(glm::mat4(1), glm::vec3(scaleObject, scaleObject, scaleObject));
		glm::mat4 rotatex = glm::rotate(glm::mat4(1), glm::radians(rotation.x), glm::vec3(1, 0, 0));
		glm::mat4 rotatey = glm::rotate(glm::mat4(1), glm::radians(rotation.y), glm::vec3(0, 1, 0));
		glm::mat4 rotatez = glm::rotate(glm::mat4(1), glm::radians(rotation.z), glm::vec3(0, 0, 1));

		objMat = translate * rotatex * rotatey * rotatez * scale;

		glBindVertexArray(modelVao);
		glUseProgram(modelProgram);
		glUniformMatrix4fv(glGetUniformLocation(modelProgram, "objMat"), 1, GL_FALSE, glm::value_ptr(objMat));
		glUniformMatrix4fv(glGetUniformLocation(modelProgram, "mv_Mat"), 1, GL_FALSE, glm::value_ptr(RenderVars::_modelView));
		glUniformMatrix4fv(glGetUniformLocation(modelProgram, "mvpMat"), 1, GL_FALSE, glm::value_ptr(RenderVars::_MVP));
		glUniform3f(glGetUniformLocation(modelProgram, "lPos"), lightPos.x, lightPos.y, lightPos.z);
		glUniform4f(glGetUniformLocation(modelProgram, "color"), 0.5f, .5f, 1.f, 0.f);

		glDrawArrays(GL_TRIANGLES, 0, nverts);


		glUseProgram(0);
		glBindVertexArray(0);

	}
}

namespace Chicken {
	GLuint modelVao;
	GLuint modelVbo[3];
	GLuint modelShaders[2];
	GLuint modelProgram;
	glm::mat4 objMat = glm::mat4(1.f);
	std::string modelPath = "models/Chicken.obj";
	int nverts;

	void setupModel() {
		std::vector<glm::vec3> verts, norms;
		std::vector<glm::vec2> uvs;

		loadOBJ(&modelPath[0], verts, uvs, norms);
		nverts = verts.size();

		glGenVertexArrays(1, &modelVao);
		glBindVertexArray(modelVao);
		glGenBuffers(3, modelVbo);

		glBindBuffer(GL_ARRAY_BUFFER, modelVbo[0]);
		glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(glm::vec3), &verts[0], GL_STATIC_DRAW);
		glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, modelVbo[1]);
		glBufferData(GL_ARRAY_BUFFER, norms.size() * sizeof(glm::vec3), &norms[0], GL_STATIC_DRAW);
		glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		modelShaders[0] = compileShader(model_vertShader, GL_VERTEX_SHADER, "cubeVert");
		modelShaders[1] = compileShader(model_fragShader, GL_FRAGMENT_SHADER, "cubeFrag");

		modelProgram = glCreateProgram();
		glAttachShader(modelProgram, modelShaders[0]);
		glAttachShader(modelProgram, modelShaders[1]);
		glBindAttribLocation(modelProgram, 0, "in_Position");
		glBindAttribLocation(modelProgram, 1, "in_Normal");
		linkProgram(modelProgram);
	}

	void cleanupModel() {

		glDeleteBuffers(2, modelVbo);
		glDeleteVertexArrays(1, &modelVao);

		glDeleteProgram(modelProgram);
		glDeleteShader(modelShaders[0]);
		glDeleteShader(modelShaders[1]);
	}

	void drawModel(glm::vec3 position, float scaleObject, glm::vec3 rotation) {

		glm::mat4 translate = glm::translate(glm::mat4(1), position);
		glm::mat4 scale = glm::scale(glm::mat4(1), glm::vec3(scaleObject, scaleObject, scaleObject));
		glm::mat4 rotatex = glm::rotate(glm::mat4(1), glm::radians(rotation.x), glm::vec3(1, 0, 0));
		glm::mat4 rotatey = glm::rotate(glm::mat4(1), glm::radians(rotation.y), glm::vec3(0, 1, 0));
		glm::mat4 rotatez = glm::rotate(glm::mat4(1), glm::radians(rotation.z), glm::vec3(0, 0, 1));

		objMat = translate * rotatex * rotatey * rotatez * scale;

		glBindVertexArray(modelVao);
		glUseProgram(modelProgram);
		glUniformMatrix4fv(glGetUniformLocation(modelProgram, "objMat"), 1, GL_FALSE, glm::value_ptr(objMat));
		glUniformMatrix4fv(glGetUniformLocation(modelProgram, "mv_Mat"), 1, GL_FALSE, glm::value_ptr(RenderVars::_modelView));
		glUniformMatrix4fv(glGetUniformLocation(modelProgram, "mvpMat"), 1, GL_FALSE, glm::value_ptr(RenderVars::_MVP));
		glUniform3f(glGetUniformLocation(modelProgram, "lPos"), lightPos.x, lightPos.y, lightPos.z);
		glUniform4f(glGetUniformLocation(modelProgram, "color"), 0.5f, .5f, 1.f, 0.f);

		glDrawArrays(GL_TRIANGLES, 0, nverts);


		glUseProgram(0);
		glBindVertexArray(0);

	}
}

namespace ChickenArmy {
	GLuint modelVao;
	GLuint modelVbo[3];
	GLuint modelShaders[2];
	GLuint modelProgram;
	glm::mat4 objMat = glm::mat4(1.f);
	std::string modelPath = "models/Chicken.obj";
	int nverts;

	void setupModel() {
		std::vector<glm::vec3> verts, norms;
		std::vector<glm::vec2> uvs;

		loadOBJ(&modelPath[0], verts, uvs, norms);
		nverts = verts.size();

		glGenVertexArrays(1, &modelVao);
		glBindVertexArray(modelVao);
		glGenBuffers(3, modelVbo);

		glBindBuffer(GL_ARRAY_BUFFER, modelVbo[0]);
		glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(glm::vec3), &verts[0], GL_STATIC_DRAW);
		glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, modelVbo[1]);
		glBufferData(GL_ARRAY_BUFFER, norms.size() * sizeof(glm::vec3), &norms[0], GL_STATIC_DRAW);
		glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		modelShaders[0] = compileShader(model_vertShader, GL_VERTEX_SHADER, "cubeVert");
		modelShaders[1] = compileShader(model_fragShader, GL_FRAGMENT_SHADER, "cubeFrag");

		modelProgram = glCreateProgram();
		glAttachShader(modelProgram, modelShaders[0]);
		glAttachShader(modelProgram, modelShaders[1]);
		glBindAttribLocation(modelProgram, 0, "in_Position");
		glBindAttribLocation(modelProgram, 1, "in_Normal");
		linkProgram(modelProgram);
	}

	void cleanupModel() {

		glDeleteBuffers(2, modelVbo);
		glDeleteVertexArrays(1, &modelVao);

		glDeleteProgram(modelProgram);
		glDeleteShader(modelShaders[0]);
		glDeleteShader(modelShaders[1]);
	}

	void drawModel(glm::vec3 position, float scaleObject, glm::vec3 rotation) {

		glm::mat4 translate = glm::translate(glm::mat4(1), position);
		glm::mat4 scale = glm::scale(glm::mat4(1), glm::vec3(scaleObject, scaleObject, scaleObject));
		glm::mat4 rotatex = glm::rotate(glm::mat4(1), glm::radians(rotation.x), glm::vec3(1, 0, 0));
		glm::mat4 rotatey = glm::rotate(glm::mat4(1), glm::radians(rotation.y), glm::vec3(0, 1, 0));
		glm::mat4 rotatez = glm::rotate(glm::mat4(1), glm::radians(rotation.z), glm::vec3(0, 0, 1));

		objMat = translate * rotatex * rotatey * rotatez * scale;

		glBindVertexArray(modelVao);
		glUseProgram(modelProgram);
		glUniformMatrix4fv(glGetUniformLocation(modelProgram, "objMat"), 1, GL_FALSE, glm::value_ptr(objMat));
		glUniformMatrix4fv(glGetUniformLocation(modelProgram, "mv_Mat"), 1, GL_FALSE, glm::value_ptr(RenderVars::_modelView));
		glUniformMatrix4fv(glGetUniformLocation(modelProgram, "mvpMat"), 1, GL_FALSE, glm::value_ptr(RenderVars::_MVP));
		glUniform3f(glGetUniformLocation(modelProgram, "lPos"), lightPos.x, lightPos.y, lightPos.z);
		glUniform1f(glGetUniformLocation(modelProgram, "chickenOffset"), chickenArmyOffset);
		glUniform4f(glGetUniformLocation(modelProgram, "color"), 0.5f, .5f, 1.f, 0.f);

		glDrawArraysInstanced(GL_TRIANGLES, 0, nverts, 10000);


		glUseProgram(0);
		glBindVertexArray(0);

	}
}
