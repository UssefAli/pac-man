#include "Menu.h"
#include <iostream>
#include <SFML/Graphics.hpp>
using namespace std;
using namespace sf;
Menu::Menu(float width, float height) {
	font.loadFromFile("Gatrich-BF63dc6fcf996b7.otf");
	selected = 0;
	for (int i = 0; i < 3; i++)
	{
		mainmenu[i].setPosition(Vector2f(25 , i * 150 + 50));
		mainmenu[i].setCharacterSize(60);
		mainmenu[i].setFont(font);
		if (i == 0 ) {
			mainmenu[i].setString("Play");
			if (selected == 0) {
				mainmenu[i].setFillColor(Color{255,204,0});
			}
			else {
				mainmenu[i].setFillColor(Color :: White);
			}
		}
		else if (i == 1) {
			mainmenu[i].setString("Instructions");
			if (selected == 1) {
				mainmenu[i].setFillColor(Color{255,204,0});
			}
			else {
				mainmenu[i].setFillColor(Color :: White);
			}
		}
		else {
			mainmenu[i].setString("Exit");
			if (selected == 2) {
				mainmenu[i].setFillColor(Color{255,204,0});
			}
			else {
				mainmenu[i].setFillColor(Color :: White);
			}
		}
	}
	
}
Menu::~Menu() {};
void Menu::set_selected(int n) {
	selected = n;
}
int Menu::pressed() {
	return selected;
}
void Menu::draw(RenderWindow& window) {
	for (int i = 0; i < 3; i++) {
		window.draw(mainmenu[i]);
	}
}
void Menu::Move_down() {
	if (selected <= 2) {
		mainmenu[selected].setFillColor(Color::White);
		selected++;
		if (selected > 2) {
			selected = 0;		
		}
		mainmenu[selected].setFillColor(Color{ 255,204,0 });
	}
}
void Menu::Move_up() {
	if (selected >= 0) {
		mainmenu[selected].setFillColor(Color::White);
		selected--;
		if (selected < 0)
			selected = 2;
		mainmenu[selected].setFillColor(Color{ 255,204,0 });
	}
}