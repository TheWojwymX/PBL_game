#pragma once

#define INPUT Input::Instance()

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <Core/Time.h>
#include <iostream>
#include <vector>

class Input
{
public:

	//Daje instancje singletona
	static Input& Instance();

	//Ustawia okno dla glfw oraz klawisze które nasłuchujemy
	void Init(GLFWwindow* window, const unsigned int SCR_WIDTH, const unsigned int SCR_HEIGHT);

	//Ustawia stan klawisza w _keyFlagArray
	void SetKeyFlagState(int keyCode, int action);

	//Ustawia stan przycisku myszy w _mouseButtonFlagArray
	void SetMouseButtonFlagState(int mouseCode, int action);

	//Zwraca flage danego klawisza
	bool GetKeyDown(int keyCode);
	bool IsKeyPressed(int keyCode);
	bool IsKeyReleased(int keyCode);

	//Zwraca flage danego przycisku myszy
	bool GetMouseButtonState(int mouseCode);
	bool GetMouseButtonDown(int mouseCode);
	bool IsMousePressed(int mouseCode);

	//Seter i geter do pozycji myszki
	void SetMousePos(float xpos, float ypos);
	glm::vec2 GetMousePos();

    //Setter i getter do pozycji myszki zmienionej na 0, 0 w środku okna
    void SetMouseFixedPos(float xpos, float ypos);
    glm::vec2 GetMouseFixedPos();

	//Seter i geter do offsetu myszki
	void SetMouseOffset(float xOffset, float yOffset);
	glm::vec2 GetMouseOffset();

	void SetFirstMouse(bool firstMouse);
	bool GetFirstMouse();
	void UpdateOldStates();

	void SetCursorMode(bool editMode);

	static int ImGuiKeyCorrelation(int keyCode);

	//Callback metoda dla glfw od zmiany wielkości okna
	static void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
	{
		Instance().FramebufferSizeCallbackIMP(width, height);
	}
	//Implementacja callbacka od zmiany wielkości okna
	void FramebufferSizeCallbackIMP(int width, int height);


	//Callback metoda dla glfw od klikniecia klawisza
	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		Instance().KeyCallbackIMP(key, scancode, action, mods);
	}
	//Implementacja callbacka od klikniecia klawisza
	void KeyCallbackIMP(int key, int scancode, int action, int mods);


	//Callback metoda dla glfw od klikniecia przycisków myszy
	static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
	{
		Instance().MouseButtonCallbackIMP(button, action, mods);
	}
	//Implementacja callbacka od klikniecia przycisków myszy
	void MouseButtonCallbackIMP(int button, int action, int mods);


	//Callback metoda dla glfw od ruszania myszki
	static void MouseCallback(GLFWwindow* window, double xpos, double ypos)
	{
		Instance().MouseCallbackIMP(xpos, ypos);
	}
	//Implementacja callbacka od ruszania myszki
	void MouseCallbackIMP(double xpos, double ypos);



private:
	Input();
	Input(Input const&);

	//Trzyma stare stany przycisku
	bool _oldKeyPressFlagArray[349] = { 0 };

	//Trzyma stany wszystkich klawiszy ich kod odpowiada lokalizacji
	bool _keyPressFlagArray[349] = { 0 };

	//Trzyma stany wszystkich klawiszy ich kod odpowiada lokalizacji z ostatniej klatki
	bool _keyHoldFlagArray[349] = { 0 };

	//Trzyma stany przycisków myszy
	bool _mouseButtonPreviousPressFlagArray[3] = { 0 };
	bool _mouseButtonPressFlagArray[3] = { 0 };

	//Pozycja myszki
	glm::vec2 _mousePos;

	//Offset ruchu myszki
	glm::vec2 _mouseOffset;

	bool _firstMouse = true;

	GLFWwindow* _window;

	std::vector<int> _lastKeys;
};
