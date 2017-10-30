#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>

#define FPS 60
#define TIMESTEP 1.0f / 60.0f
#define FORCE 160.0f
#define mass 1.0f
#define MASS 1.0f / mass
#define WIDTH 800
#define HEIGHT 600
#define RADIUS 30.0f
#define ELASTICITY 1.0f
#define FRICTION 0.2f * TIMESTEP

using namespace std;
using namespace sf;

RenderWindow window;
CircleShape circle;
Vector2f velocity, acceleration, position;
bool withFriction = false;

void bounceCheck(){
	float cx = circle.getPosition().x, cy = circle.getPosition().y;
	if(cy + RADIUS > HEIGHT - 1) {
		velocity.y = -ELASTICITY * velocity.y;
		circle.setPosition(cx, HEIGHT - RADIUS - 1);
	}
	if(cy - RADIUS < 0){
		velocity.y = -ELASTICITY * velocity.y;
		circle.setPosition(cx, RADIUS);
	}
	if(cx + RADIUS > WIDTH - 1){
		velocity.x = -ELASTICITY * velocity.x;
		circle.setPosition(WIDTH - RADIUS - 1, cy);
	}
	if(cx - RADIUS < 0){
		velocity.x = -ELASTICITY * velocity.x;
		circle.setPosition(RADIUS, cy);
	}
}

void updatePosition(){
	velocity = velocity + (acceleration * TIMESTEP);
	if(withFriction) velocity -= (FRICTION * velocity * MASS);
	position = circle.getPosition();
	position = position + (0.5f * acceleration * TIMESTEP * TIMESTEP) + (velocity * TIMESTEP);
	circle.setPosition(position);
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
		updatePosition();
		checkBounce();
		window.clear(Color::Black);
		window.draw(circle);
		window.display();
	}
}
