#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>
#include <stdlib.h>


#define FPS 144
#define WIDTH 1024
#define HEIGHT 768
#define TIMESTEP 1.0f/FPS
#define LIMIT 10000

using namespace std;
using namespace sf;

RenderWindow window;
int shapes, points, xPoint, yPoint;
ConvexShape poly[LIMIT];
RectangleShape outline[LIMIT];
FloatRect AABBSet[LIMIT];
FloatRect AABBUpdate[LIMIT];
bool rot;

void setObjects(){
	cin >> shapes;
	for(int i = 0; i < shapes; i++){
		cin >> points;
		poly[i].setPointCount(points);
		for(int j = 0; j < points + 1; j++){
			cin >> xPoint;
			cin >> yPoint;
			Vector2f point = Vector2f(xPoint,yPoint);
			if(j == points) poly[i].setPosition(xPoint,yPoint);
			else poly[i].setPoint(j, point);
		}
		AABBSet[i] = poly[i].getLocalBounds();
		poly[i].setOrigin(AABBSet[i].left + AABBSet[i].width/2, AABBSet[i].top + AABBSet[i].height/2);
		poly[i].setFillColor(Color::Blue);
	}
}

void getAABB(){
	for(int i = 0; i < shapes; i++){
		Vector2f minimum = poly[i].getTransform().transformPoint(poly[i].getPoint(0));
		Vector2f maximum = minimum;
		for(int j = 1; j < poly[i].getPointCount(); ++j){
			Vector2f point = poly[i].getTransform().transformPoint(poly[i].getPoint(j));
			minimum.x = min(minimum.x, point.x);
			minimum.y = min(minimum.y, point.y);
			maximum.x = max(maximum.x, point.x);
			maximum.y = max(maximum.y, point.y);
		}
		AABBUpdate[i] = FloatRect(minimum, maximum-minimum);
		outline[i].setSize(Vector2f(AABBUpdate[i].width, AABBUpdate[i].height));
		outline[i].setPosition(AABBUpdate[i].left, AABBUpdate[i].top);
		//outline[i].setPosition(poly[i].getPosition().x - AABBUpdate[i].width/2, poly[i].getPosition().y - AABBUpdate[i].height/2);
		outline[i].setFillColor(Color::Transparent);
		outline[i].setOutlineColor(Color::Magenta);
		outline[i].setOutlineThickness(1);			
	}
}

void rotateShapes(bool r){
	if(r){
		for(int i = 0; i < shapes; i++){
			poly[i].rotate(6 * (i+1) * TIMESTEP);
		}
	}else{
		for(int i = 0; i < shapes; i++){
			poly[i].rotate(0);
		}
	}
}

int main(){
	setObjects(); //draw for manual input, not for text files yet
	ContextSettings settings;
	settings.antialiasingLevel = 8;
	window.create(VideoMode(WIDTH, HEIGHT), "Poly Shit", Style::Default, settings);
	window.setFramerateLimit(FPS);
	while(window.isOpen()){
		Event event;
		while(window.pollEvent(event)){
			switch(event.type){
				case Event::Closed:
				window.close();
				break;
				case Event::KeyPressed:
				if(Keyboard::isKeyPressed(Keyboard::Escape)) window.close();
				if(Keyboard::isKeyPressed(Keyboard::W)) poly[0].move(0, -1.38889);
				if(Keyboard::isKeyPressed(Keyboard::A)) poly[0].move(-1.38889, 0);
				if(Keyboard::isKeyPressed(Keyboard::S)) poly[0].move(0, 1.38889);
				if(Keyboard::isKeyPressed(Keyboard::D)) poly[0].move(1.38889, 0);
				if(Keyboard::isKeyPressed(Keyboard::R)) rot = !rot;
				break;
			}
		}
		//method of getting AABB
		rotateShapes(rot);
		getAABB();
		window.clear(Color::Black);
		for(int i = 0; i < shapes; i++) window.draw(poly[i]);
		for(int i = 0; i < shapes; i++) window.draw(outline[i]);
		window.display();
	}
}
