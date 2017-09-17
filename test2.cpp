#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>

using namespace std;
using namespace sf;

Color color[6];
CircleShape circleArray[60];
RectangleShape rectangleArray[40];
bool pressUp, pressDown, pressLeft, pressRight, pressLeftMouse, notCollidedUp, notCollidedDown, notCollidedLeft, notCollidedRight;
RenderWindow window;

void borderCollisonChecker(){
	if((circleArray[0].getPosition().y < 0)) notCollidedUp = false; else notCollidedUp = true;
	if((circleArray[0].getPosition().y + 60 > 767)) notCollidedDown = false; else notCollidedDown = true;
	if((circleArray[0].getPosition().x < 0)) notCollidedLeft = false; else notCollidedLeft = true;
	if((circleArray[0].getPosition().x + 60 > 1023)) notCollidedRight = false; else notCollidedRight = true;
}

void moveIt(){
	if(pressUp && notCollidedUp){
		circleArray[0].move(0,-1.38889);
	}
	if(pressDown && notCollidedDown){
		circleArray[0].move(0,1.38889);
	}
	if(pressRight && notCollidedRight){
		circleArray[0].move(1.38889,0);
	}
	if(pressLeft && notCollidedLeft){
		circleArray[0].move(-1.38889,0);
	}
	if(pressLeftMouse){
		double x2 = Mouse::getPosition(window).x - rectangleArray[0].getPosition().x;
		double y2 = Mouse::getPosition(window).y - rectangleArray[0].getPosition().y;
		double distance = sqrt(pow(x2, 2) + pow(y2, 2));
		rectangleArray[0].move(x2/distance * 1.38889, y2/distance * 1.38889);
	}
	for(int i = 1; i < 60; i++) circleArray[i].move(0,0.13889);
	for(int i = 1; i < 40; i++) rectangleArray[i].move(0.13889,0);
}

void renderThread(RenderWindow* window){
	while(window->isOpen()){
		moveIt();
		window->clear(Color::Black);
		for(int i = 0; i < 60; i++) window->draw(circleArray[i]);
		for(int i = 0; i < 40; i++) window->draw(rectangleArray[i]);
		window->display();
	}
}

int main(){
	ContextSettings settings;
	settings.antialiasingLevel = 8;
	window.create(VideoMode(1024,768), "Vectors & Circles", Style::Default, settings);
	window.setFramerateLimit(144);
	window.setKeyRepeatEnabled(false);
	window.setActive(false);
	srand(time(NULL));
	int curr = 0;
	color[0] = Color::Magenta;
	color[1] = Color::Cyan;
	color[2] = Color::Blue;
	color[3] = Color::Yellow;
	color[4] = Color::Green;
	color[5] = Color::Red;
	for(int i = 0; i < 60; i++){
		circleArray[i].setPosition(rand() % 1023, rand() % 767);
		circleArray[i].setRadius(30.0f);
		circleArray[i].setFillColor(color[curr]);
		curr++;
		if(curr > 5) curr = 0;
	}
	for(int j = 0; j < 40; j++){
		rectangleArray[j].setPosition(rand() % 1023, rand() % 767);
		rectangleArray[j].setSize(Vector2f(50.0f, 50.0f));
		rectangleArray[j].setFillColor(color[curr]);
		curr++;
		if(curr > 5) curr = 0;
	}
	circleArray[0].setOutlineThickness(7);
	circleArray[0].setOutlineColor(Color::White);
	rectangleArray[0].setOutlineThickness(7);
	rectangleArray[0].setOutlineColor(Color::White);
	Thread thread(&renderThread, &window);
	thread.launch();
	while(window.isOpen()){
		borderCollisonChecker();
		Event event;
		while(window.pollEvent(event)){
			switch(event.type){
				case Event::Closed:
				window.close();
				break;
				case Event::KeyPressed:
				switch(event.key.code){
					case Keyboard::W:
					pressUp = true;
					break;
					case Keyboard::A:
					pressLeft = true;
					break;
					case Keyboard::S:
					pressDown = true;
					break;
					case Keyboard::D:
					pressRight = true;
					break;
					case Keyboard::Escape:
					window.close();
					break;
				}
				break;
				case Event::KeyReleased:
				switch(event.key.code){
					case Keyboard::W:
					pressUp = false;
					break;
					case Keyboard::A:
					pressLeft = false;
					break;
					case Keyboard::S:
					pressDown = false;
					break;
					case Keyboard::D:
					pressRight = false;
					break;
				}
				break;
				case Event::MouseButtonPressed:
				switch(event.mouseButton.button){
					case Mouse::Left:
					pressLeftMouse = true;
					break;
				}
				break;
				case Event::MouseButtonReleased:
				switch(event.mouseButton.button){
					case Mouse::Left:
					pressLeftMouse = false;
					break;
				}
				break;				
			}
		}
	}
	return 0;
}
