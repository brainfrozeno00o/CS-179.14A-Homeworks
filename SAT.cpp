#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <vector>


#define FPS 60
#define WIDTH 1024
#define HEIGHT 768
#define TIMESTEP 1.0f/FPS
#define LIMIT 10000

using namespace std;
using namespace sf;

RenderWindow window;
int shapes, points, xPoint, yPoint; //for initialization
ConvexShape poly[LIMIT]; //set convex shapes
RectangleShape outline[LIMIT]; //set rectangle shapes for outline
FloatRect AABBSet[LIMIT]; //for AABB in setting origin
FloatRect AABBUpdate[LIMIT]; //for getting AABB during rotation
Vector2f colAABB[LIMIT]; //for AABB collision
Color color1 = Color::Red; //color if polygons collided
Color color2 = Color::Yellow; //color if bounding boxes collided
Color color3 = Color::Blue; //color if both bounding boxes and polygons are not colliding
bool rot, goLeft, goRight, goUp, goDown;
//put input of the object
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
		poly[i].setFillColor(color3);
	}
}
//move the first polygon
void moveShape(){
	if(goLeft) poly[0].move(-(100 * TIMESTEP), 0);
	if(goRight) poly[0].move((100 * TIMESTEP), 0);
	if(goUp) poly[0].move(0, -(100 * TIMESTEP));
	if(goDown) poly[0].move(0, (100 * TIMESTEP));
}
//rotate all the shapes based on r
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
//get bounding bounds of each object
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
		outline[i].setFillColor(Color::Transparent);
		outline[i].setOutlineColor(Color::Magenta);
		outline[i].setOutlineThickness(1);	
		colAABB[i] = Vector2f(AABBUpdate[i].left, AABBUpdate[i].top);
		poly[i].setFillColor(color3);		
	}
}
//AABB collision detection using min-width method
void checkAABBCollision(){
	for(int i = 0; i < shapes; i++){
		Vector2f difference;
		for(int j = 0; j < shapes; j++){
			if(i != j){
				difference = colAABB[i] - colAABB[j];
				if(difference.x > outline[j].getSize().x || -difference.x > outline[i].getSize().x || difference.y > outline[j].getSize().y || -difference.y > outline[i].getSize().y){
					continue;
				}else{
					poly[i].setFillColor(color2);
					poly[j].setFillColor(color2);
				}
			}
		}
	}	
}
//return perpendicular vector
Vector2f unitPerp(Vector2f vec){
	Vector2f perp = Vector2f(-vec.y, vec.x);
	return perp;
}
//return a vector that contains all axes for checking
vector<Vector2f> getAxes(ConvexShape poly1, ConvexShape poly2){
	vector<Vector2f> axesToCheck;
	for(int i = 0; i < poly1.getPointCount() - 1; i++){
		axesToCheck.push_back(unitPerp(poly1.getTransform().transformPoint(poly1.getPoint(i)) - poly1.getTransform().transformPoint(poly1.getPoint(i+1))));
	}
	axesToCheck.push_back(unitPerp(poly1.getTransform().transformPoint(poly1.getPoint(poly1.getPointCount() - 1)) - poly1.getTransform().transformPoint(poly1.getPoint(0))));
	for(int j = 0; j < poly2.getPointCount() - 1; j++){
		axesToCheck.push_back(unitPerp(poly2.getTransform().transformPoint(poly2.getPoint(j)) - poly2.getTransform().transformPoint(poly2.getPoint(j+1))));
	}
	axesToCheck.push_back(unitPerp(poly1.getTransform().transformPoint(poly2.getPoint(poly2.getPointCount() - 1)) - poly2.getTransform().transformPoint(poly2.getPoint(0))));
	return axesToCheck;
}
//return dot product
float getDotProduct(Vector2f a, Vector2f b){
	return ((a.x * b.x) + (a.y * b.y));
}
//return scalar minimum value
float returnMin(ConvexShape poly1, Vector2f u){
	float polyMin = getDotProduct(poly1.getTransform().transformPoint(poly1.getPoint(0)), u);
	for(int i = 1; i < poly1.getPointCount(); i++){
		float checkMin = getDotProduct(poly1.getTransform().transformPoint(poly1.getPoint(i)), u);
		if(checkMin < polyMin) polyMin = checkMin;
	}
	return polyMin;
}
//return scalar maximum value
float returnMax(ConvexShape poly1, Vector2f u){
	float polyMax = getDotProduct(poly1.getTransform().transformPoint(poly1.getPoint(0)), u);
	for(int i = 1; i < poly1.getPointCount(); i++){
		float checkMax = getDotProduct(poly1.getTransform().transformPoint(poly1.getPoint(i)), u);
		if(checkMax > polyMax) polyMax = checkMax;
	}
	return polyMax;	
}
//check SAT collision for two convex shapes
bool checkSATCollisionForTwo(ConvexShape poly1, ConvexShape poly2){
	vector<Vector2f> check = getAxes(poly1, poly2);
	int overlapCounter = 0;
	for(int i = 0; i < check.size(); i++){
		float poly1Min = returnMin(poly1, check.at(i));
		float poly2Min = returnMin(poly2, check.at(i));
		float poly1Max = returnMax(poly1, check.at(i));
		float poly2Max = returnMax(poly2, check.at(i));
		if(poly2Min > poly1Max || poly2Max < poly1Min) break;
		else overlapCounter++;
	}
	if(overlapCounter == check.size()) return true; else return false;
}
//check SAT Collision for everything
void checkSATCollision(){
	for(int i = 0; i < shapes; i++){
		for(int j = 0; j < shapes; j++){
			if(i != j){
				if(checkSATCollisionForTwo(poly[i], poly[j])){
					poly[i].setFillColor(color1);
					poly[j].setFillColor(color1);
				}
			}
		}
	}
}

int main(){
	setObjects(); 
	ContextSettings settings;
	settings.antialiasingLevel = 8;
	window.create(VideoMode(WIDTH, HEIGHT), "Poly Stuff", Style::Default, settings);
	window.setKeyRepeatEnabled(false);
	window.setFramerateLimit(FPS);
	while(window.isOpen()){
		Event event;
		while(window.pollEvent(event)){
			switch(event.type){
				case Event::Closed:
				window.close();
				break;
				case Event::KeyPressed:
				switch(event.key.code){
					case sf::Keyboard::W:
					goUp = true;
					break;
					case sf::Keyboard::A:
					goLeft = true;
					break;
					case sf::Keyboard::S:
					goDown = true;
					break;
					case sf::Keyboard::D:
					goRight = true;
					break;
					case sf::Keyboard::R:
					rot = !rot;
					break;
					case sf::Keyboard::Escape:
					window.close();
					break;
				}
				break;
				case sf::Event::KeyReleased:
				switch(event.key.code){
					case sf::Keyboard::W:
					goUp = false;
					break;
					case sf::Keyboard::A:
					goLeft = false;
					break;
					case sf::Keyboard::S:
					goDown = false;
					break;
					case sf::Keyboard::D:
					goRight = false;
					break;
				}
				break;				
			}
		}
		rotateShapes(rot);
		moveShape();
		getAABB();
		checkAABBCollision();
		checkSATCollision();
		window.clear(Color::Black);
		for(int i = 0; i < shapes; i++) window.draw(poly[i]);
		for(int i = 0; i < shapes; i++) window.draw(outline[i]);
		window.display();
	}
}

/*own test values
2
4
-50 -50
50 -50
50 50
-50 50
200 70
4
-100 -100
100 -100
100 100
-100 100
500 250
*/
