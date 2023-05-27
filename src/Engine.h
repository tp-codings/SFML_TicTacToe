#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
using namespace sf;

class Engine
{
public:
	Engine();
	virtual ~Engine();

	void run();

	const bool running();

private:

	//window
	RenderWindow* window;
	VideoMode videomode;
	Event ev;
	bool close = false;

	//Grid
	int rows = 3;
	int columns = 3;
	int aktRow = 0;
	int aktCol = 0;

	//GUI
	Font font;
	Font font2;
	Text text[9];
	Text GUI;
	Text endText;
	Text startText;
	Text choiceText[3];

	//Mouse positions
	Vector2i mousePosWindow;
	Vector2f mousePosView;
	bool mouseheld = false;
	bool mousePressed = false;

	//Game logic
	Vector2f input = Vector2f(-1,-1);
	Vector2f aktKI = Vector2f(-1, -1);
	int zug;
	int player;
	bool isFull;
	char raster[3][3];
	bool endGame;
	bool start = true;
	bool newGame = false;
	int winner;
	int choice;

	void update();
	void updateGUI();
	void updateMousePosition();
	void updateFieldPosition();
	void updateInput();
	void updateGame(Vector2f);

	void render();
	void renderGUI(RenderTarget*);
	void renderConsole();

	void initWindow();
	void initGUI();
	void initVariables();
	void initGame();

	void pollEvent();

	void drawGrid(RenderTarget*);

	bool pruefeBelegt(unsigned zeile, unsigned spalte);
	int pruefeEndGame();
	int countZaehler(int a, int b, int* cA, int* cB);
	int pruefeNichtMehrGewinnbar();

	void KI();
	void pruefeUndSetze();
	bool wennGehtGewinnen();

	unsigned zufall();

};

