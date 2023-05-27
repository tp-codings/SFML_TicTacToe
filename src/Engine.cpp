#include "Engine.h"
#include <iostream>
#include <math.h>
#include <cmath>
#include <sstream>
#include <iomanip>

Engine::Engine()
{
	this->initWindow();
	this->initGUI();
	this->initVariables();
}

Engine::~Engine()
{
	delete this->window;
}

void Engine::run()
{
	while (this->running())
	{
		this->render();
		this->update();
	}
}

const bool Engine::running()
{
	return this->window->isOpen();
}

void Engine::update()
{
	if (endGame)
	{
		start = false;
	}
	if (!Mouse::isButtonPressed(Mouse::Left)) this->mouseheld = false;
	if (this->endGame && (Keyboard::isKeyPressed(Keyboard::Enter) || Mouse::isButtonPressed(Mouse::Left) && !mouseheld))
	{
		this->start = true;
		this->mouseheld = true;
	}
	if (this->start)
	{
		this->initGame();
	}
	//this->updateGUI();
	this->pollEvent();
	this->updateMousePosition();
	this->updateFieldPosition();
	if (!start)
	{
		this->updateInput();
	}
}

void Engine::updateGUI()
{
	std::stringstream ss;
	std::stringstream end;
	std::cout << this->start << this->newGame << "\n";
	if (!isFull && !endGame && !newGame && !start)
	{
		ss << "Spieler: " << this->player%2 +1;
	}
	else if(isFull && !endGame && !newGame && !start)
	{
		ss << "belegt";
	}
	if (newGame && choice == 2)
	{
		std::cout << "\nhieeer\n";
		ss << "gegen Spieler ausgewählt";
	}
	if (newGame && choice == 1)
	{
		ss << "gegen KI ausgewählt";
	}
	else if (endGame && winner != 4)
	{
		if (choice == 2)
		{
			end << "Player: " << this->winner << " hat gewonnen";
		}
		else if (choice == 1 && winner == 2)
		{
			end << "KI hat gewonnen";
		}
		else if (choice == 1 && winner == 1)
		{
			end << "Player hat gewonnen :)";
		}
		ss << "";
	}
	else if (endGame && winner == 4)
	{
		end << "Unentschieden";
		ss << "";
	}
	this->GUI.setString(ss.str());
	this->endText.setString(end.str());
	this->GUI.setPosition(0.5 * this->window->getSize().x - 0.5 * this->GUI.getGlobalBounds().width, 0);
	this->endText.setPosition(0.5 * this->window->getSize().x - 0.5 * this->endText.getGlobalBounds().width, 0.5 * this->window->getSize().y);
	
	if (zug < 9)
	{
		if (choice == 2 || choice == 1 && player == 1 &&!isFull)
		{
			if (this->mousePressed)
			{
				this->text[zug].setPosition(Vector2f(input.y * this->window->getSize().x / this->columns, input.x * this->window->getSize().y / this->rows));
			}
			std::stringstream ss1;
			std::stringstream ss2;

			if (this->player == 1 && this->zug >= 0 && !isFull)
			{
				ss1 << "X";
				this->text[zug].setString(ss1.str());
			}
			else if (this->player == 2 && this->zug >= 0 &&!isFull)
			{
				ss2 << "O";
				this->text[zug].setString(ss2.str());
			}
		}
		else if (choice == 1 && player == 2&&!isFull)
		{
			if (zug < 9)
			{

				this->text[zug].setPosition(Vector2f(aktKI.y * this->window->getSize().x / this->columns, aktKI.x * this->window->getSize().y / this->rows));
				std::stringstream ss2;

				if (this->player == 2 && this->zug >= 0 && !isFull)
				{
					ss2 << "O";
					this->text[zug].setString(ss2.str());
				}
			}
		}
	}
}

void Engine::updateMousePosition()
{
	this->mousePosWindow = Mouse::getPosition(*this->window);
	this->mousePosView = this->window->mapPixelToCoords(this->mousePosWindow);
	//std::cout << mousePosView.x << " " << mousePosView.y << "\n";
}

void Engine::updateFieldPosition()
{
	
	if (this->mousePosView.x >= 0 && this->mousePosView.x < this->window->getSize().x / this->columns) this->aktCol = 0;
	else if (this->mousePosView.x >= this->window->getSize().x/this->columns && this->mousePosView.x < 2*this->window->getSize().x / this->columns) this->aktCol = 1;
	else if (this->mousePosView.x >= 2*this->window->getSize().x / this->columns && this->mousePosView.x <this->window->getSize().x)this->aktCol = 2;

	if (this->mousePosView.y >= 0 && this->mousePosView.y < this->window->getSize().y / this->rows) this->aktRow = 0;
	else if (this->mousePosView.y >= this->window->getSize().y / this->rows && this->mousePosView.y < 2 * this->window->getSize().y / this->rows) this->aktRow = 1;
	else if (this->mousePosView.y >= 2 * this->window->getSize().y / this->rows && this->mousePosView.y < this->window->getSize().y) this->aktRow = 2;
	//std::cout << this->aktRow << "\n";
	//std::cout << this->aktCol << "\n";
}

void Engine::updateInput()
{
	if (!endGame)
	{
		if (choice == 2 || choice == 1 && player == 1)
		{
			if (!Mouse::isButtonPressed(Mouse::Left))
			{
				this->mouseheld = false;
			}
			if (Mouse::isButtonPressed(Mouse::Left) && !this->mouseheld && !this->pruefeBelegt(this->aktRow, this->aktCol))
			{
				this->isFull = false;
				input = Vector2f(this->aktRow, this->aktCol);
				this->mousePressed = true;
				this->mouseheld = true;
				this->newGame = false;
				this->zug++;
				this->updateGame(input);
				this->updateGUI();
				if (this->player == 2)
				{
					this->player = 1;
				}
				else
				{
					this->player++;
				}
			}
			else if (Mouse::isButtonPressed(Mouse::Left) && !this->mouseheld && this->pruefeBelegt(this->aktRow, this->aktCol))
			{
				std::cout << "belegt\n\n";
				isFull = true;
				this->updateGUI();
			}
		}
		else if (choice == 1 && player == 2)
		{
			this->KI();
			this->zug++;
			this->updateGUI();
			this->player = 1;
		}
	}
}

void Engine::updateGame(Vector2f in)
{
	if (player == 1)
	{
		raster[int(in.x)][int(in.y)] = 'X';
	}
	else
	{
		raster[int(in.x)][int(in.y)] = 'O';
		aktKI = in;
	}
	this->winner = this->pruefeEndGame();
}

void Engine::render()
{
	this->window->clear(Color(153, 205, 178));
	this->drawGrid(this->window);
	this->renderGUI(this->window);
	this->renderConsole();
	this->window->display();
}

void Engine::renderGUI(RenderTarget* target)
{
	for (int i = 0; i<9; i++)
	{
		target->draw(text[i]);
	}
	if (!endGame)
	{
		target->draw(this->GUI);
	}
	else if(endGame && !start)
	{
		target->draw(this->endText);
	}
	
	if (start)
	{
		target->draw(this->startText);
		target->draw(this->choiceText[0]);
		target->draw(this->choiceText[1]);
		target->draw(this->choiceText[2]);
	}
	//std::cout << this->start;
}

void Engine::renderConsole()
{
	for (size_t i = 0; i < 3; i++)
	{
		for (size_t k = 0; k < 3; k++)
		{
			std::cout << raster[i][k] << "      ";
		} 
		std::cout  << input.x << " " << input.y << " " << mousePosView.x << " " << mousePosView.y << "\n";
		//std::cout << input.x << " " << input.y << " " << aktKI.x << " " << aktKI.y << "\n";
	}
	//std::cout << this->choice;
	std::cout << "\n";
	std::cout << "\n";
}

void Engine::initWindow()
{
	this->videomode.height = 700;
	this->videomode.width = 700;
	this->window = new RenderWindow(this->videomode, "TicTacToe", Style::Titlebar | Style::Close);
	this->window->setFramerateLimit(60);
	this->window->setVerticalSyncEnabled(false);
}

void Engine::initGUI()
{
	if (!this->font.loadFromFile("Fonts/Silkscreen-Bold.ttf")) std::cout << "fuck it";
	if (!this->font2.loadFromFile("Fonts/Silkscreen-Regular.ttf")) std::cout << "fuck it";
	
	for (int i = 0; i < 9; i++)
	{
		this->text[i].setFont(font);
		this->text[i].setCharacterSize(70);
		this->text[i].setFillColor(Color::White);
		this->text[i].setPosition(100, 300);
	}
	this->GUI.setFont(font2);
	this->GUI.setCharacterSize(20);
	this->GUI.setFillColor(Color::White);
	this->GUI.setPosition(280,0);

	this->endText.setFont(font2);
	this->endText.setCharacterSize(40);
	this->endText.setString("Player: x hat gewonnen");
	this->endText.setFillColor(Color::White);
	this->endText.setPosition(0.5 * this->window->getSize().x - 0.5 * this->endText.getGlobalBounds().width, 0.5 * this->window->getSize().y);this->endText.setFont(font2);
	
	this->startText.setFont(font2);
	this->choiceText[0].setFont(font2);
	this->choiceText[1].setFont(font2);
	this->choiceText[2].setFont(font2);

	this->startText.setCharacterSize(60);
	this->startText.setString("Bitte auswählen:");
	this->startText.setFillColor(Color::White);
	this->startText.setPosition(0.5 * this->window->getSize().x - 0.5 * this->endText.getGlobalBounds().width, 0.24 * this->window->getSize().y);
	
	this->choiceText[0].setCharacterSize(25);
	this->choiceText[0].setString("gegen Spieler");
	this->choiceText[0].setFillColor(Color::White);
	this->choiceText[0].setPosition(0, 0.5 * this->window->getSize().y);

	this->choiceText[1].setCharacterSize(25);
	this->choiceText[1].setString("gegen KI");
	this->choiceText[1].setFillColor(Color::White);
	this->choiceText[1].setPosition(this->window->getSize().x - 1.3*this->choiceText[1].getGlobalBounds().width, 0.5 * this->window->getSize().y);

	this->choiceText[2].setCharacterSize(25);
	this->choiceText[2].setString("beenden");
	this->choiceText[2].setFillColor(Color::White);
	this->choiceText[2].setPosition(0.5*this->window->getSize().x-0.5*this->choiceText[2].getGlobalBounds().width, this->window->getSize().y - 4 * this->choiceText[2].getGlobalBounds().height);
}

void Engine::initVariables()
{
	for (size_t i = 0; i < 3; i++)
	{
		for (size_t k = 0; k < 3; k++)
		{
			this->raster[i][k] = '-';
		}
	}
	this->zug = -1;
	this->player = 1;
	this->isFull = false;
	this->endGame = false;
	this->winner = 0;
	this->choice = 0;
	for (int i = 0; i < 9; i++)
	{
		this->text[i].setString("");
	}
}

void Engine::initGame()
{
	this->initVariables();
	if (!Mouse::isButtonPressed(Mouse::Left))
	{
		this->mouseheld = false;
	}
	if (Mouse::isButtonPressed(Mouse::Left) && !this->mouseheld)
	{
		if (Vector2f(this->aktRow, this->aktCol) == Vector2f(1, 0))
		{
			choice = 2;
			this->start = false;
			this->mouseheld = true;
			this->newGame = true;
			//this->updateGUI();
		}
		else if (Vector2f(this->aktRow, this->aktCol) == Vector2f(1, 2))
		{
			choice = 1;
			this->start = false;
			this->mouseheld = true;
			this->newGame = true;
			//this->updateGUI();
		}
		else if (Vector2f(this->aktRow, this->aktCol) == Vector2f(2, 1))
		{
			this->close = true;
		}
	}
}

void Engine::pollEvent()
{
	while (this->window->pollEvent(this->ev))
	{
		switch (this->ev.type)
		{
		case Event::Closed:
			this->window->close();
			break;
		case Event::KeyPressed:
			if (this->ev.key.code == Keyboard::F) this->window->close();
			break;
		}
	}
	if (close) this->window->close();
}

void Engine::drawGrid(RenderTarget* win) {
	// initialize values
	int numLines = this->rows + this->columns - 2;
	sf::VertexArray grid(Lines, 2*(numLines));
	win->setView(win->getDefaultView());
	auto size = win->getView().getSize();
	float rowH = size.y / this->rows;
	float colW = size.x / this->columns;
	// row separators
	for (int i = 0; i < this->rows - 1; i++) {
		int r = i + 1;
		float rowY = rowH * r;
		grid[i*2].position = { 0, rowY };
		grid[i*2+1].position = { size.x, rowY };
	}
	// column separators
	for (int i = this->rows - 1; i < numLines; i++) {
		int c = i - this->rows + 2;
		float colX = colW * c;
		grid[i*2].position = { colX, 0 };
		grid[i*2+1].position = { colX, size.y };
	}
	// draw it
	win->draw(grid);
}

bool Engine::pruefeBelegt(unsigned zeile, unsigned spalte)
{
	if (raster[zeile][spalte] == 'X' || raster[zeile][spalte] == 'O')
		return true;
	else
		return false;
}

int Engine::pruefeEndGame()
{
	int countX = 0;
	int countO = 0;
	int winner = 0;

	//Zeilen Prüfen
	for (size_t i = 0; i < 3; i++)
	{
		for (size_t k = 0; k < 3; k++)
		{
			if (raster[i][k] == 'O')
				countO++;
			else if (raster[i][k] == 'X')
				countX++;
		}
		winner = countZaehler(countO, countX, &countO, &countX);
		if (winner != 0) return winner;
	}
	//Spalten Prüfen
	for (size_t i = 0; i < 3; i++)
	{
		for (size_t k = 0; k < 3; k++)
		{
			if (raster[k][i] == 'O')
				countO++;
			else if (raster[k][i] == 'X')
				countX++;
		}
		winner = countZaehler(countO, countX, &countO, &countX);
		if (winner != 0) return winner;
	}

	//erste Hauptdiagonale
	for (size_t i = 0; i < 3; i++)
	{
		if (raster[i][i] == 'O')
			countO++;
		else if (raster[i][i] == 'X')
			countX++;
	}

	winner = countZaehler(countO, countX, &countO, &countX);
	if (winner != 0) return winner;

	//zweite Hauptdiagonale
	int i = 2;
	int k = 0;
	while (i >= 0 && k < 3)
	{
		if (raster[i][k] == 'O')
			countO++;
		else if (raster[i][k] == 'X')
			countX++;
		i--;
		k++;
	}
	winner = countZaehler(countO, countX, &countO, &countX);
	if (winner != 0) return winner;
	if (pruefeNichtMehrGewinnbar() == 4) return 4;
	return 0;
}

int Engine::countZaehler(int a, int b, int* cA, int* cB)
{
	int winner = 0;
	if (a == 3)
	{
		endGame = true;
		winner = 2;
	}
	else if (b == 3)
	{
		endGame = true;
		winner = 1;
	}
	*cA = 0;
	*cB = 0;
	return winner;
}

int Engine::pruefeNichtMehrGewinnbar()
{
	int count = 0;
	int k = 0;
	int i = 0;
	bool zeileVoll = false;
	bool spalteVoll = false;
	bool ersteHD = false;
	bool zweiteHD = false;

	//Zeilen prüfen

	for (size_t i = 0; i < 3; i++)
	{
		if (
			raster[i][k] == 'O' && raster[i][k + 1] == 'X' ||
			raster[i][k] == 'X' && raster[i][k + 1] == 'O' ||
			raster[i][k] == 'O' && raster[i][k + 2] == 'X' ||
			raster[i][k] == 'X' && raster[i][k + 2] == 'O' ||
			raster[i][k + 1] == 'O' && raster[i][k + 2] == 'X' ||
			raster[i][k + 1] == 'X' && raster[i][k + 2] == 'O'
			)
			count++;
	}
	if (count == 3) zeileVoll = true;
	count = 0;

	//Spalten prüfen
	for (size_t k = 0; k < 3; k++)
	{
		if (
			raster[i][k] == 'O' && raster[i + 1][k] == 'X' ||
			raster[i][k] == 'X' && raster[i + 1][k] == 'O' ||
			raster[i][k] == 'O' && raster[i + 2][k] == 'X' ||
			raster[i][k] == 'X' && raster[i + 2][k] == 'O' ||
			raster[i + 1][k] == 'O' && raster[i + 2][k] == 'X' ||
			raster[i + 1][k] == 'X' && raster[i + 2][k] == 'O'
			)
			count++;
	}
	if (count == 3) spalteVoll = true;
	count = 0;

	//erste Hauptdiagonale
	i = 0;
	k = 0;
	if (
		raster[i][k] == 'O' && raster[i + 1][k + 1] == 'X' ||
		raster[i][k] == 'X' && raster[i + 1][k + 1] == 'O' ||
		raster[i + 1][k + 1] == 'O' && raster[i + 2][k + 2] == 'X' ||
		raster[i + 1][k + 1] == 'X' && raster[i + 2][k + 2] == 'O' ||
		raster[i][k] == 'O' && raster[i + 2][k + 2] == 'X' ||
		raster[i][k] == 'X' && raster[i + 2][k + 2] == 'O'
		)
		ersteHD = true;

	//zweite Hauptdiagonale
	i = 2;
	k = 0;
	if (
		raster[i][k] == 'O' && raster[i - 1][k + 1] == 'X' ||
		raster[i][k] == 'X' && raster[i - 1][k + 1] == 'O' ||
		raster[i - 1][k + 1] == 'O' && raster[i - 2][k + 2] == 'X' ||
		raster[i - 1][k + 1] == 'X' && raster[i - 2][k + 2] == 'O' ||
		raster[i][k] == 'O' && raster[i - 2][k + 2] == 'X' ||
		raster[i][k] == 'X' && raster[i - 2][k + 2] == 'O'
		)
		zweiteHD = true;


	if (zeileVoll && spalteVoll && ersteHD && zweiteHD)
	{
		endGame = true;
		return 4;
	}
	return 0;
}

void Engine::KI()
{
	this->pruefeUndSetze();
}

void Engine::pruefeUndSetze()
{
	int k = 0;
	int i = 0;
	unsigned zeilenWahl = 3;
	unsigned spaltenWahl = 3;

	//Gewinnen prüfen
	if (wennGehtGewinnen())
	{
		return;
	}

	//Zeilen prüfen
	for (size_t i = 0; i < 3; i++)
	{
		if (raster[i][k] == 'O' && raster[i][k + 1] == 'O' && raster[i][k + 2] != 'X')
		{
			updateGame(Vector2f(i, k + 2));
			return;
		}
		if (raster[i][k] == 'O' && raster[i][k + 2] == 'O' && raster[i][k + 1] != 'X')
		{
			updateGame(Vector2f(i, k + 1));
			return;
		}
		if (raster[i][k + 1] == 'O' && raster[i][k + 2] == 'O' && raster[i][k] != 'X')
		{
			updateGame(Vector2f(i, k));
			return;
		}
	}

	//Spalten prüfen
	for (size_t k = 0; k < 3; k++)
	{
		if (raster[i][k] == 'O' && raster[i + 1][k] == 'O' && raster[i + 2][k] != 'X')
		{
			updateGame(Vector2f(i + 2, k));
			return;
		}
		if (raster[i][k] == 'O' && raster[i + 2][k] == 'O' && raster[i + 1][k] != 'X')
		{
			updateGame(Vector2f(i + 1, k));
			return;
		}
		if (raster[i + 1][k] == 'O' && raster[i + 2][k] == 'O' && raster[i][k] != 'X')
		{
			updateGame(Vector2f(i, k));
			return;
		}
	}

	//Erste HD prüfen
	{
		if (raster[i][k] == 'O' && raster[i + 1][k + 1] == 'O' && raster[i + 2][k + 2] != 'X')
		{
			updateGame(Vector2f(i + 2, k + 2));
			return;
		}
		if (raster[i][k] == 'O' && raster[i + 2][k + 2] == 'O' && raster[i + 1][k + 1] != 'X')
		{
			updateGame(Vector2f(i + 1, k + 1));
			return;
		}
		if (raster[i + 2][k + 2] == 'O' && raster[i + 1][k + 1] == 'O' && raster[i][k] != 'X')
		{
			updateGame(Vector2f(i, k));
			return;
		}
	}

	//Zweite HD prüfen
	{

		if (raster[i + 2][k] == 'O' && raster[i + 1][k + 1] == 'O' && raster[i][k + 2] != 'X')
		{
			updateGame(Vector2f(i, k + 2));
			return;
		}
		if (raster[i + 2][k] == 'O' && raster[i][k + 2] == 'O' && raster[i + 1][k + 1] != 'X')
		{
			updateGame(Vector2f(i + 1, k + 1));
			return;
		}
		if (raster[i + 1][k + 1] == 'O' && raster[i][k + 2] == 'O' && raster[i + 2][k] != 'X')
		{
			updateGame(Vector2f(i + 2, k));
			return;
		}
	}

	//sonst ein Zug
	do
	{
		zeilenWahl = zufall();
		spaltenWahl = zufall();
	} while (pruefeBelegt(zeilenWahl, spaltenWahl));
	updateGame(Vector2f(zeilenWahl, spaltenWahl));
	return;
}

bool Engine::wennGehtGewinnen()
{
	int k = 0;
	int i = 0;
	//Zeilen prüfen
	for (size_t i = 0; i < 3; i++)
	{
		if (raster[i][k] == 'X' && raster[i][k + 1] == 'X' && raster[i][k + 2] != 'O')
		{
			updateGame(Vector2f(i, k + 2));
			return true;
		}
		if (raster[i][k] == 'X' && raster[i][k + 2] == 'X' && raster[i][k + 1] != 'O')
		{
			updateGame(Vector2f(i, k + 1));
			return true;
		}
		if (raster[i][k + 1] == 'X' && raster[i][k + 2] == 'X' && raster[i][k] != 'O')
		{
			updateGame(Vector2f(i, k));
			return true;
		}
	}

	//Spalten prüfen
	for (size_t k = 0; k < 3; k++)
	{
		if (raster[i][k] == 'X' && raster[i + 1][k] == 'X' && raster[i + 2][k] != 'O')
		{
			updateGame(Vector2f(i + 2, k));
			return true;
		}
		if (raster[i][k] == 'X' && raster[i + 2][k] == 'X' && raster[i + 1][k] != 'O')
		{
			updateGame(Vector2f(i + 1, k));
			return true;
		}
		if (raster[i + 1][k] == 'X' && raster[i + 2][k] == 'X' && raster[i][k] != 'O')
		{
			updateGame(Vector2f(i, k));
			return true;
		}
	}

	//Erste HD prüfen
	{
		if (raster[i][k] == 'X' && raster[i + 1][k + 1] == 'X' && raster[i + 2][k + 2] != 'O')
		{
			updateGame(Vector2f(i + 2, k + 2));
			return true;
		}
		if (raster[i][k] == 'X' && raster[i + 2][k + 2] == 'X' && raster[i + 1][k + 1] != 'O')
		{
			updateGame(Vector2f(i + 1, k + 1));
			return true;
		}
		if (raster[i + 2][k + 2] == 'X' && raster[i + 1][k + 1] == 'X' && raster[i][k] != 'O')
		{
			updateGame(Vector2f(i, k));
			return true;
		}
	}

	//Zweite HD prüfen
	{

		if (raster[i + 2][k] == 'X' && raster[i + 1][k + 1] == 'X' && raster[i][k + 2] != 'O')
		{
			updateGame(Vector2f(i, k + 2));
			return true;
		}
		if (raster[i + 2][k] == 'X' && raster[i][k + 2] == 'X' && raster[i + 1][k + 1] != 'O')
		{
			updateGame(Vector2f(i + 1, k + 1));
			return true;
		}
		if (raster[i + 1][k + 1] == 'X' && raster[i][k + 2] == 'X' && raster[i + 2][k] != 'O')
		{
			updateGame(Vector2f(i + 2, k));
			return true;
		}
	}
	return false;
}

unsigned Engine::zufall()
{
	return static_cast<unsigned>(rand() % 3);
}
