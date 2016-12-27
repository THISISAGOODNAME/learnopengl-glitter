// Preprocessor Directives
#define STB_IMAGE_IMPLEMENTATION

// Local Headers
#include "glitter.hpp"
#include "shader.h"
#include "camera.h"
#include "model.h"

// Console Color
#include "consoleColor.hpp"

// System Headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

// Standard Headers
#include <iostream>

// 声明按键函数
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void do_movement();
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
GLuint loadTexture(GLchar* path);
GLuint loadCubemap(vector<const GLchar*> faces);

// 相机
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

// 光源
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

// 时间增量Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame

int main(int argc, char * argv[]) {

	// glfw初始化
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // 此行用来给Mac OS X系统做兼容
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// 创建窗口,获取窗口上上下文
	GLFWwindow* window = glfwCreateWindow(mWidth, mHeight, "LearnOpenGL", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << RED << "Failed to create GLFW window" << RESET << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// 通过glfw注册事件回调
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// glfw获取光标
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Load OpenGL Functions
	gladLoadGL();
	//    fprintf(stderr, "OpenGL %s\n", glGetString(GL_VERSION));
	std::cout << BLUE << "OpenGL " << glGetString(GL_VERSION) << RESET << std::endl;

	// 查询GPU最大支持顶点个数
	//    GLint nrAttributes;
	//    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	//    std::cout << GREEN << "Maximum nr of vertex attributes supported: " << nrAttributes << RESET << std::endl;

	// 获取视口
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	// 设置OpenGL可选项
	glEnable(GL_DEPTH_TEST); // 开启深度测试

	// 编译着色器程序
	Shader shader("vert.vert", "frag.frag");
	Shader normalShader("normals.vert", "normals.frag", "normals.geom");

#pragma region "object_initialization"
	// 读取模型
	Model nanosuit("nanosuit/nanosuit.obj");

	// Set projection matrix
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mWidth / (GLfloat)mHeight, 1.0f, 100.0f);
	shader.Use();
	glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	// Also one for normal shader
	normalShader.Use();
	glUniformMatrix4fv(glGetUniformLocation(normalShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

#pragma endregion

	// 使用线框模式进行渲染
	//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// 渲染循环
	while (!glfwWindowShouldClose(window))
	{
		// Calculate deltatime of current frame
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// 检查事件
		glfwPollEvents();
		do_movement();

		// 渲染指令
		glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Add transformation matrices
		shader.Use();
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(camera.GetViewMatrix()));
		glm::mat4 model;
		//model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f)); // 下移模型使模型位于屏幕中心
		//model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// 缩小模型使之在窗口内部更合适
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));

		// Draw model
		nanosuit.Draw(shader);

		
		// Now set transformation matrices for drawing normals
		normalShader.Use();
		glUniformMatrix4fv(glGetUniformLocation(normalShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(camera.GetViewMatrix()));
		model = glm::mat4();
		glUniformMatrix4fv(glGetUniformLocation(normalShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));

		// And draw model again, this time only drawing normal vectors using the geometry shaders (on top of previous model)
		nanosuit.Draw(normalShader);

		// 交换缓冲
		glfwSwapBuffers(window);
	}

	// 释放GLFW分配的内存
	glfwTerminate();

	return 0;
}


// Loads a cubemap texture from 6 individual texture faces
// Order should be:
// +X (right)
// -X (left)
// +Y (top)
// -Y (bottom)
// +Z (front)
// -Z (back)
GLuint loadCubemap(vector<const GLchar*> faces)
{
	GLuint textureID;
	glGenTextures(1, &textureID);

	int width, height, channels;
	unsigned char* image;

	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	for (GLuint i = 0; i < faces.size(); i++)
	{
		image = stbi_load(faces[i], &width, &height, &channels, 0);
		// Set the Correct Channel Format
		GLenum format;
		switch (channels)
		{
		case 1: format = GL_ALPHA;     break;
		case 2: format = GL_LUMINANCE; break;
		case 3: format = GL_RGB;       break;
		case 4: format = GL_RGBA;      break;
		}
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, image);
		stbi_image_free(image);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	return textureID;
}


// This function loads a texture from file. Note: texture loading functions like these are usually
// managed by a 'Resource Manager' that manages all resources (like textures, models, audio).
// For learning purposes we'll just define it as a utility function.
GLuint loadTexture(GLchar* path)
{
	//Generate texture ID and load texture data
	GLuint textureID;
	glGenTextures(1, &textureID);
	int width, height, channels;
	unsigned char* image = stbi_load(path, &width, &height, &channels, 0);
	// Set the Correct Channel Format
	GLenum format;
	switch (channels)
	{
	case 1: format = GL_ALPHA;     break;
	case 2: format = GL_LUMINANCE; break;
	case 3: format = GL_RGB;       break;
	case 4: format = GL_RGBA;      break;
	}
	// Assign texture to ID
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);

	// Parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(image);
	return textureID;
}

#pragma region "User input"

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	// 当用户按下ESC键,我们设置window窗口的WindowShouldClose属性为true
	// 关闭应用程序
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}
}

void do_movement()
{
	// 摄像机控制
	GLfloat cameraSpeed = 5.0f * deltaTime;
	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
		camera.ProcessKeyboard(RIGHT, deltaTime);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	GLfloat xoffset = xpos - lastX;
	GLfloat yoffset = lastY - ypos; // 注意这里是相反的，因为y坐标是从底部往顶部依次增大的
	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

#pragma endregion