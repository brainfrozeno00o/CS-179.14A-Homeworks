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
Vector2f colAABB[LIMIT];
bool rot, goLeft, goRight, goUp, goDown;
//put input of the object, text file not yet done
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
//move the first polygon
void moveShape(){
	if(goLeft) poly[0].move(-0.5, 0);
	if(goRight) poly[0].move(0.5, 0);
	if(goUp) poly[0].move(0, -0.5);
	if(goDown) poly[0].move(0, 0.5);
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
		poly[i].setFillColor(Color::Blue);		
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
					poly[i].setFillColor(Color::Yellow);
					poly[j].setFillColor(Color::Yellow);
				}
			}
		}
	}	
}
//get perpendicular vector
Vector2f getPerpendicularNormal(ConvexShape poly, int i){
	int pointIndex1 = i;
	int pointIndex2 = i + 1;
	if(pointIndex2 == poly.getPointCount()) pointIndex2 = 0;
	Vector2f firstEdgePoint = poly.getTransform().transformPoint(poly.getPoint(pointIndex1));
	Vector2f secondEdgePoint = poly.getTransform().transformPoint(poly.getPoint(pointIndex2));
	Vector2f edgeVector = Vector2f(firstEdgePoint - secondEdgePoint);
	Vector2f axisVector = Vector2f(-edgeVector.y, edgeVector.x);
	Vector2f perpVector = Vector2f((axisVector.x) / sqrt((axisVector.x * axisVector.x) + (axisVector.y * axisVector.y)), (axisVector.y) / sqrt((axisVector.x * axisVector.x) + (axisVector.y * axisVector.y)));
	return perpVector;
}
//get minimum point
Vector2f getMinForLineSegment(ConvexShape poly, Vector2f axis){
	Vector2f minpoint = (((poly.getTransform().transformPoint(poly.getPoint(0)).x * axis.x) + (poly.getTransform().transformPoint(poly.getPoint(0)).y * axis.y))) * axis;
	for(int i = 1; i < poly.getPointCount(); i++){
		Vector2f projectPoint = (((poly.getTransform().transformPoint(poly.getPoint(i)).x * axis.x) + (poly.getTransform().transformPoint(poly.getPoint(i)).y * axis.y))) * axis;
		minpoint.x = min(minpoint.x, projectPoint.x);
		minpoint.y = min(minpoint.y, projectPoint.y);
	}
	return minpoint;
}
//get maximum projected point
Vector2f getMaxForLineSegment(ConvexShape poly, Vector2f axis){
	Vector2f maxpoint = (((poly.getTransform().transformPoint(poly.getPoint(0)).x * axis.x) + (poly.getTransform().transformPoint(poly.getPoint(0)).y * axis.y))) * axis;
	for(int i = 1; i < poly.getPointCount(); i++){
		Vector2f projectPoint = (((poly.getTransform().transformPoint(poly.getPoint(i)).x * axis.x) + (poly.getTransform().transformPoint(poly.getPoint(i)).y * axis.y))) * axis;
		maxpoint.x = max(maxpoint.x, projectPoint.x);
		maxpoint.y = max(maxpoint.y, projectPoint.y);
	}
	return maxpoint;
}

bool checkOverlap(ConvexShape poly1, ConvexShape poly2){
	bool poly1Collide = true, poly2Collide = true;
	for(int i = 0; i < poly1.getPointCount(); i++){
		Vector2f perpNorm1 = getPerpendicularNormal(poly1, i);
		Vector2f minPoint1 = getMinForLineSegment(poly1, perpNorm1);
		Vector2f maxPoint1 = getMaxForLineSegment(poly1, perpNorm1);
		Vector2f minPoint2 = getMinForLineSegment(poly2, perpNorm1);
		Vector2f maxPoint2 = getMaxForLineSegment(poly2, perpNorm1);
		if(minPoint2.x > maxPoint1.x || minPoint2.y > maxPoint1.y || maxPoint2.x > minPoint1.x || maxPoint2.y > minPoint1.y){
			continue;
		}else{
			poly1Collide = false;
			break;
		}
	}
	for(int i = 0; i < poly2.getPointCount(); i++){
		Vector2f perpNorm2 = getPerpendicularNormal(poly2, i);
		Vector2f minPoint3 = getMinForLineSegment(poly1, perpNorm2);
		Vector2f maxPoint3 = getMaxForLineSegment(poly1, perpNorm2);
		Vector2f minPoint4 = getMinForLineSegment(poly2, perpNorm2);
		Vector2f maxPoint4 = getMaxForLineSegment(poly2, perpNorm2);
		if(minPoint4.x > maxPoint3.x || minPoint4.y > maxPoint3.y || maxPoint4.x > minPoint3.x || maxPoint4.y > minPoint3.y){
			continue;
		}else{
			poly2Collide = false;
			break;
		}
	}
	return (poly1Collide && poly2Collide);
}

//void checkSATCollision()

int main(){
	setObjects(); //draw for manual input, not for text files yet
	ContextSettings settings;
	settings.antialiasingLevel = 8;
	window.create(VideoMode(WIDTH, HEIGHT), "Poly Shit", Style::Default, settings);
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
		//checkSATCollision();
		window.clear(Color::Black);
		for(int i = 0; i < shapes; i++) window.draw(poly[i]);
		for(int i = 0; i < shapes; i++) window.draw(outline[i]);
		window.display();
	}
}
