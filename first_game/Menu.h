#include <iostream>
#include <SFML/Graphics.hpp>

#pragma once
using namespace std;
using namespace sf;
class Menu
{
private:
	int selected;
	Font font;
public:
	Text mainmenu[3];
	Menu(float width, float height);
	void draw(RenderWindow& window);
	void Move_up();
	void Move_down();
	void set_selected(int n);
	int pressed();
	~Menu();
};
