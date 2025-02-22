#include "..\include\create_window.h"

//camera
GLCamera camera(glm::vec3(0.0f, -3.0f, 10.0f));
//static
float WindowManager::lastX;
float WindowManager::lastY;
bool WindowManager::firstMouse;

//---------------------------------------------------------------
//public
//---------------------------------------------------------------

//init
WindowManager::WindowManager(const unsigned int& width, const unsigned int& height, const std::string& title, GLFWmonitor* monitor, GLFWwindow* share, const bool captureMouse)
	:scrWidth(width), scrHeight(height), windowTitle(title)
{
	//camera
	this->lastX = width / 2.0f;
	this->lastY = height / 2.0f;
	this->firstMouse = true;
	//timing
	this->deltaTime = 0.0f;		// time between current frame and last frame
	this->lastTime = 0.0f;

	this->window = CreateWindow(monitor, share, captureMouse);
	
	//std::cout << "init" << std::endl;
}
WindowManager::~WindowManager() { }

void WindowManager::SetCallback()
{
	glfwSetFramebufferSizeCallback(window, FrameBufferSizeCallbackFunc);
	glfwSetCursorPosCallback(window, MouseCallbackFunc);
	glfwSetScrollCallback(window, ScrollCallbackFunc);
}
void WindowManager::SetPerFrameTimeLogic()
{
	float currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastTime;
	lastTime = currentFrame;
}
void WindowManager::ProcessInput()
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(CM::FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(CM::BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(CM::LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(CM::RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		camera.ProcessKeyboard(CM::RISE, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_CAPS_LOCK) == GLFW_PRESS)
		camera.ProcessKeyboard(CM::FALL, deltaTime);

}

unsigned int WindowManager::GetScrWidth() { return scrWidth; }
unsigned int WindowManager::GetScrHeight() { return scrHeight; }
std::string WindowManager::GetWindowTitle() { return windowTitle; }

//---------------------------------------------------------------
//private
//---------------------------------------------------------------

//init glfw
void WindowManager::GLFWInit()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
#ifdef _APPLE_
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif // _APPLE_
}

//create window
GLFWwindow* WindowManager::CreateWindow(GLFWmonitor* monitor, GLFWwindow* share, bool captureMouse)
{
	GLFWInit();
	GLFWwindow* window = glfwCreateWindow(scrWidth, scrHeight, windowTitle.c_str(), monitor, share);
	if(window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return nullptr;
	}
	glfwMakeContextCurrent(window);

	//tell GLFW to capture our mouse
	if(captureMouse == true)
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//glad
	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return nullptr;
	}
	return window;
}

//framebuffer size callback function
WindowManager* WindowManager::FBSCb = nullptr;
void WindowManager::FrameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
	
	glViewport(0, 0, width, height);
}
void WindowManager::FrameBufferSizeCallbackFunc(GLFWwindow* window, int width, int height)
{
	FBSCb->FrameBufferSizeCallback(window, width, height);
}

//mouse callback function
WindowManager* WindowManager::MCb = nullptr;
void WindowManager::MouseCallback(GLFWwindow* window, double xPos, double yPos)
{

	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}
	float xoffset = xPos - lastX;
	float yoffset = lastY - yPos;
	lastX = xPos;
	lastY = yPos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}
void WindowManager::MouseCallbackFunc(GLFWwindow* window, double xPos, double yPos)
{
	MCb->MouseCallback(window, xPos, yPos);
}


//scroll callback function
WindowManager* WindowManager::SCb = nullptr;
void WindowManager::ScrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{
	camera.ProcessMouseScroll(yOffset);
}
void WindowManager::ScrollCallbackFunc(GLFWwindow* window, double xOffset, double yOffset)
{
	SCb->ScrollCallback(window, xOffset, yOffset);
}
