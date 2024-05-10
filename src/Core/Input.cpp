#include "Input.h"
#include "Component/Camera.h"
#include "Managers/GameManager.h"

Input::Input()
{
}

Input& Input::Instance() {
	static Input instance;
	return instance;
}

void Input::Init(GLFWwindow* window, const unsigned int SCR_WIDTH, const unsigned int SCR_HEIGHT)
{
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetMouseButtonCallback(window, MouseButtonCallback);
	glfwSetCursorPosCallback(window, MouseCallback);
	float lastX = SCR_WIDTH / 2.0f;
	float lastY = SCR_HEIGHT / 2.0f;
	_mousePos = glm::vec2(lastX, lastY);
	_window = window;
}

void Input::SetKeyFlagState(int keyCode, int action)
{
	_lastKeys.push_back(keyCode);
	switch (action)
	{
	case GLFW_PRESS:
		_keyPressFlagArray[keyCode] = true;
		break;
	case GLFW_REPEAT:
		_keyHoldFlagArray[keyCode] = true;
		_keyPressFlagArray[keyCode] = false;
		break;
	case GLFW_RELEASE:
		_keyPressFlagArray[keyCode] = false;
		_keyHoldFlagArray[keyCode] = false;
		break;
	}
}

void Input::SetMouseButtonFlagState(int mouseCode, int action)
{
	_mouseButtonPreviousPressFlagArray[mouseCode] = _mouseButtonPressFlagArray[mouseCode];
	_mouseButtonPressFlagArray[mouseCode] = action;
}

bool Input::GetKeyDown(int keyCode)
{
	return _keyHoldFlagArray[keyCode] || _keyPressFlagArray[keyCode];
}

bool Input::IsKeyPressed(int keyCode)
{
	if (!_oldKeyPressFlagArray[keyCode] && _keyPressFlagArray[keyCode])
		return true;
	else return false;
}

bool Input::IsKeyReleased(int keyCode)
{
	return !_keyPressFlagArray[keyCode];
}

bool Input::GetMouseButtonState(int mouseCode)
{
	return _mouseButtonPressFlagArray[mouseCode];
}

bool Input::GetMouseButtonDown(int mouseCode)
{
	return _mouseButtonPressFlagArray[mouseCode];
}

bool Input::IsMousePressed(int mouseCode)
{
	return !_mouseButtonPreviousPressFlagArray[mouseCode] && _mouseButtonPressFlagArray[mouseCode];
}

void Input::SetMousePos(float xpos, float ypos)
{
	_mousePos = glm::vec2(xpos, ypos);
}

glm::vec2 Input::GetMousePos()
{
	return _mousePos;
}

void Input::SetMouseOffset(float xOffset, float yOffset)
{
	_mouseOffset = glm::vec2(xOffset, yOffset);
}

glm::vec2 Input::GetMouseOffset()
{
	return _mouseOffset;
}

void Input::SetFirstMouse(bool firstMouse)
{
	_firstMouse = firstMouse;
}

bool Input::GetFirstMouse()
{
	return _firstMouse;
}

void Input::UpdateOldStates()
{
	while (_lastKeys.size())
	{
		_oldKeyPressFlagArray[_lastKeys.back()] = _keyPressFlagArray[_lastKeys.back()];
		_lastKeys.pop_back();
	}
	for (int i = 0; i < 3; i++)
	{
		_mouseButtonPreviousPressFlagArray[i] = _mouseButtonPressFlagArray[i];
	}
	_mouseOffset = glm::vec2(0.0f);
}

void Input::SetCursorMode(bool editMode) {
	int cursorMode = editMode ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED;
	glfwSetInputMode(_window, GLFW_CURSOR, cursorMode);
}

int Input::ImGuiKeyCorrelation(int keyCode)
{
	switch (keyCode)
	{
	case GLFW_KEY_W:
		return 3;
	case 3:
		return GLFW_KEY_W;

	case GLFW_KEY_A:
		return 0;
	case 0:
		return GLFW_KEY_A;

	case GLFW_KEY_D:
		return 1;
	case 1:
		return GLFW_KEY_D;

	case GLFW_KEY_S:
		return 2;
	case 2:
		return GLFW_KEY_S;

	case GLFW_KEY_E:
		return 4;
	case 4:
		return GLFW_KEY_E;

	case GLFW_KEY_SPACE:
		return 5;
	case 5:
		return GLFW_KEY_SPACE;
	default:
		return 0;
	}
	
}


void Input::FramebufferSizeCallbackIMP(int width, int height)
{
	// TODO
	/*
		CAMERA.SCR_WIDTH = width;
		CAMERA.SCR_HEIGHT = height;
	*/

}

void Input::KeyCallbackIMP(int key, int scancode, int action, int mods)
{
	SetKeyFlagState(key, action);
}

void Input::MouseButtonCallbackIMP(int button, int action, int mods)
{
	SetMouseButtonFlagState(button, action);
}

void Input::MouseCallbackIMP(double xpos, double ypos)
{
	if (_firstMouse)
	{
		_mousePos = glm::vec2(xpos, ypos);
		_firstMouse = false;
	}
	float xoffset = xpos - _mousePos.x;
	float yoffset = _mousePos.y - ypos;

	_mouseOffset = glm::vec2(xoffset, yoffset);
	_mousePos = glm::vec2(xpos, ypos);
}

void Input::SetMouseFixedPos(float xpos, float ypos) {

}

glm::vec2 Input::GetMouseFixedPos() {

    glm::vec2 fixedPos;
    fixedPos.x = (_mousePos.x / GAMEMANAGER._screenWidth) * 2.0f - 1.0f;
    fixedPos.y = (_mousePos.y / GAMEMANAGER._screenHeight) * 2.0f - 1.0f;
    fixedPos.y = -fixedPos.y;

    return fixedPos;
}
