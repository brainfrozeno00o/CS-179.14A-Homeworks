#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>
#include <stdlib.h>

#define FPS 60
#define WIDTH 1080
#define HEIGHT 1080
#define TIMESTEP 1.0f / FPS
//#define LIMIT 15
#define CONSTANT 7

using namespace std;
using namespace sf;

RenderWindow window;
RectangleShape rArray[CONSTANT];
RectangleShape AABB[CONSTANT];
Vector2f minXMinY[CONSTANT];
Vector2f minXMaxY[CONSTANT];
Vector2f maxXMinY[CONSTANT];
Vector2f maxXMaxY[CONSTANT];
Vector2f colAABB[CONSTANT];

void drawEverything(){
	rArray[0].setSize(Vector2f(100, 315));
	rArray[1].setSize(Vector2f(100, 315));
	rArray[2].setSize(Vector2f(70, 65));
	rArray[3].setSize(Vector2f(250, 250));
	rArray[4].setSize(Vector2f(230, 65));
	rArray[5].setSize(Vector2f(350, 85));
	rArray[6].setSize(Vector2f(350, 85));
	for(int i = 0; i < CONSTANT; i++){//set random size based on the number of rectangles needed
		rArray[i].setOrigin(rArray[i].getSize().x / 2, rArray[i].getSize().y / 2);
	}
	rArray[0].setPosition(260, 350);
	rArray[1].setPosition(260, 700);
	rArray[2].setPosition(880, 670);
	rArray[3].setPosition(550, 550);
	rArray[4].setPosition(750, 753);
	rArray[5].setPosition(540, 890);
	rArray[6].setPosition(545, 270);
	for(int i = 0; i < CONSTANT; i++){ //getting the original positions of the points of the objects and setting up AABBs
		//setting up AABBs
		AABB[i].setFillColor(Color::Transparent);
		AABB[i].setOutlineColor(Color::Magenta);
		AABB[i].setOutlineThickness(1.5f);
		//getting original positions of points for AABB drawing
		minXMinY[i] = Vector2f((rArray[i].getPosition().x - (rArray[i].getSize().x / 2)), (rArray[i].getPosition().y - (rArray[i].getSize().y / 2)));
		minXMaxY[i] = Vector2f((rArray[i].getPosition().x - (rArray[i].getSize().x / 2)), (rArray[i].getPosition().y + (rArray[i].getSize().y / 2)));
		maxXMinY[i] = Vector2f((rArray[i].getPosition().x + (rArray[i].getSize().x / 2)), (rArray[i].getPosition().y - (rArray[i].getSize().y / 2)));
		maxXMaxY[i] = Vector2f((rArray[i].getPosition().x + (rArray[i].getSize().x / 2)), (rArray[i].getPosition().y + (rArray[i].getSize().y / 2)));
	}
}

void rotateRectangles(){
	for(int i = 0; i < CONSTANT; i++){ //rotating objects
		rArray[i].rotate(3 * (i+1) * TIMESTEP);
	} 
}

void calculateAABB(){
	for(int i = 0; i < CONSTANT; i++){//drawing AABBs and setting up for collision
		//getting the angle functions
		float cFunc = abs(cos(rArray[i].getRotation() * M_PI/180));
		float sFunc = abs(sin(rArray[i].getRotation() * M_PI/180));
		//getting AABB corners using the angle functions and original position of the points of the objects
		Vector2f upperLeft = Vector2f((((minXMinY[i].x - rArray[i].getPosition().x) * cFunc) - ((minXMinY[i].y - rArray[i].getPosition().y) * sFunc)) + rArray[i].getPosition().x, 
			(((minXMinY[i].x - rArray[i].getPosition().x) * sFunc) + ((minXMinY[i].y - rArray[i].getPosition().y) * cFunc)) + rArray[i].getPosition().y);
		Vector2f upperRight = Vector2f((((maxXMinY[i].x - rArray[i].getPosition().x) * cFunc) - ((maxXMinY[i].y - rArray[i].getPosition().y) * sFunc)) + rArray[i].getPosition().x, 
			(((maxXMinY[i].x - rArray[i].getPosition().x) * sFunc) + ((maxXMinY[i].y - rArray[i].getPosition().y) * cFunc)) + rArray[i].getPosition().y);
		Vector2f lowerLeft = Vector2f((((minXMaxY[i].x - rArray[i].getPosition().x) * cFunc) - ((minXMaxY[i].y - rArray[i].getPosition().y) * sFunc)) + rArray[i].getPosition().x, 
			(((minXMaxY[i].x - rArray[i].getPosition().x) * sFunc) + ((minXMaxY[i].y - rArray[i].getPosition().y) * cFunc)) + rArray[i].getPosition().y);
		Vector2f lowerRight = Vector2f((((maxXMaxY[i].x - rArray[i].getPosition().x) * cFunc) - ((maxXMaxY[i].y - rArray[i].getPosition().y) * sFunc)) + rArray[i].getPosition().x, 
			(((maxXMaxY[i].x - rArray[i].getPosition().x) * sFunc) + ((maxXMaxY[i].y - rArray[i].getPosition().y) * cFunc)) + rArray[i].getPosition().y);
		//determining the minimum and maximum x and y points of the AABB
		float aabbMinX = min(min(abs(upperLeft.x), abs(upperRight.x)), min(abs(lowerLeft.x), abs(lowerRight.x)));
		float aabbMinY = min(min(abs(upperLeft.y), abs(upperRight.y)), min(abs(lowerLeft.y), abs(lowerRight.y)));
		float aabbMaxX = max(max(abs(upperLeft.x), abs(upperRight.x)), max(abs(lowerLeft.x), abs(lowerRight.x)));
		float aabbMaxY = max(max(abs(upperLeft.y), abs(upperRight.y)), max(abs(lowerLeft.y), abs(lowerRight.y)));
		//setting the size of AABB, setting origin to center based on size, and setting position based on the position of the rectangle object
		float getWidth = aabbMaxX - aabbMinX;
		float getHeight = aabbMaxY - aabbMinY;
		AABB[i].setSize(Vector2f(getWidth, getHeight));
		AABB[i].setOrigin(AABB[i].getSize().x / 2, AABB[i].getSize().y / 2);
		AABB[i].setPosition(rArray[i].getPosition());
		//setting up for AABB collision using min-width method
		colAABB[i] = Vector2f(aabbMinX, aabbMinY);
		rArray[i].setFillColor(Color::Blue);
	}
}

void checkAABBCollision(){
	for(int i = 0; i < CONSTANT; i++){//collision detection using min-width method
		Vector2f difference;
		for(int j = 0; j < CONSTANT; j++){
			if(i != j){
				difference = colAABB[i] - colAABB[j];
				if(difference.x > AABB[j].getSize().x || -difference.x > AABB[i].getSize().x || difference.y > AABB[j].getSize().y || -difference.y > AABB[i].getSize().y){
					continue;
				}else{
					rArray[i].setFillColor(Color::Yellow);
					rArray[j].setFillColor(Color::Yellow);
				}
			}
		}
	}
}

int main(){
	ContextSettings settings;
	settings.antialiasingLevel = 8;
	window.create(VideoMode(WIDTH, HEIGHT), "Bounding Boxes", Style::Default, settings);
	window.setFramerateLimit(FPS);
	drawEverything();
	while(window.isOpen()){
		Event event;
		while(window.pollEvent(event)){
			switch(event.type){
				case Event::Closed:
				window.close();
				break;
				case Event::KeyPressed:
				switch(event.key.code){
					case Keyboard::Escape:
					window.close();
					break;
				}
				break;
			}
		}
		rotateRectangles();
		calculateAABB();
		checkAABBCollision();
		window.clear(Color::Black);//clear to black
		for(int i = 0; i < CONSTANT; i++){//draw the rectangles
			window.draw(rArray[i]);
		}
		for(int i = 0; i < CONSTANT; i++){//draw its respective AABBs
			window.draw(AABB[i]);
		}
		window.display();//display everything
	}
}
