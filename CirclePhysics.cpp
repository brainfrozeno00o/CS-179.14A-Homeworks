#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>

#define FPS 60
#define TIMESTEP 1.0f / 60.0f
#define FORCE 160.0f
#define MASS 1.0f 
#define WIDTH 800
#define HEIGHT 600
#define RADIUS 30.0f
#define ELASTICITY 1.0f
#define FRICTION 0.2f * TIMESTEP
#define VELOCITY

using namespace std;
using namespace sf;

RenderWindow window;
CircleShape circle;
Vector2f velocity, acceleration, position;
bool withFriction = false;

Vector2f normalize(Vector2f vector){
	float length = sqrt((vector.x * vector.x) + (vector.y * vector.y));
	if(length != 0){
		return Vector2f(vector.x / length, vector.y / length);
	}else{
		return vector;
	}
}

void bounceCheck(){
	float cx = circle.getPosition().x, cy = circle.getPosition().y;
	if(cy + RADIUS > HEIGHT - 1) {
		velocity.y = -ELASTICITY * velocity.y;
		circle.setPosition(cx, HEIGHT - RADIUS);
	}
	if(cy - RADIUS < 0){
		velocity.y = -ELASTICITY * velocity.y;
		circle.setPosition(cx, RADIUS);
	}
	if(cx + RADIUS > WIDTH - 1){
		velocity.x = -ELASTICITY * velocity.x;
		circle.setPosition(WIDTH - RADIUS, cy);
	}
	if(cx - RADIUS < 0){
		velocity.x = -ELASTICITY * velocity.x;
		circle.setPosition(RADIUS, cy);
	}
}

void updateVelocity(){
	position = position + (0.5f * acceleration * TIMESTEP * TIMESTEP) + (velocity * TIMESTEP) + position;
	velocity = velocity + (acceleration * TIMESTEP);
	if(withFriction) velocity -= (FRICTION * velocity * MASS);
}

void moveCircle(){
	circle.move(velocity);
}

void renderThread(RenderWindow* window){
	while(window->isOpen()){
		normalize(position);
		normalize(velocity);
		normalize(acceleration);
		bounceCheck();
		updateVelocity();
		moveCircle();
		window->clear(Color::Black);
		window->draw(circle);
		window->display();
	}
}


int main(){
	ContextSettings settings;
	settings.antialiasingLevel = 8;
	window.create(VideoMode(WIDTH, HEIGHT), "Circle Physics", Style::Default, settings);
	window.setFramerateLimit(FPS);
	window.setKeyRepeatEnabled(false);
	window.setActive(false);
	circle.setRadius(RADIUS);
	circle.setOrigin(RADIUS, RADIUS);
	circle.setFillColor(Color::Magenta);
	circle.setPosition(WIDTH/2, HEIGHT/2);
	Thread thread(&renderThread, &window);
	thread.launch();
	while(window.isOpen()){
		Event event;
		while(window.pollEvent(event)){
			switch(event.type){
				case Event::Closed:
				window.close();
				break;
				case Event::KeyPressed:
				switch(event.key.code){
					case Keyboard::W:
					acceleration.y += -(FORCE * MASS);
					break;
					case Keyboard::A:
					acceleration.x += -(FORCE * MASS);
					break;
					case Keyboard::S:
					acceleration.y += FORCE * MASS;
					break;
					case Keyboard::D:
					acceleration.x += FORCE * MASS;
					break;
					case Keyboard::F:
					withFriction = !withFriction;
					if(withFriction) circle.setFillColor(Color::Blue);
					else circle.setFillColor(Color::Magenta);
					break;
					case Keyboard::Escape:
					window.close();
					break;
				}
				break;
				case Event::KeyReleased:
				switch(event.key.code){
					case Keyboard::W:
					acceleration.y = 0;
					break;
					case Keyboard::A:
					acceleration.x = 0;
					break;
					case Keyboard::S:
					acceleration.y = 0;	
					break;
					case Keyboard::D:
					acceleration.x = 0;
					break;
				}
				break;
			}
		}
	}
}
