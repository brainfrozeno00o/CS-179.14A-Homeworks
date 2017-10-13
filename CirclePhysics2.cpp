#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>
#include <stdlib.h>

#define FPS 60
#define TIMESTEP 1.0f / 60.0f
#define FORCE 10000.0f
#define MAIN_MASS 50.0f
#define OTHER_MASS 1.0f
#define WIDTH 800
#define HEIGHT 600
#define RADIUS 30.0f
#define ELASTICITY 1.0f
#define FRICTION 0.2f * TIMESTEP

using namespace std;
using namespace sf;

RenderWindow window;
CircleShape cArray[36];
Vector2f cVel[36];
Vector2f cAccel[36];
float cMass[36];
Color colorArray[3];
bool withFriction = false;

int main(int argc, char *argv[]){
	int x = atoi(argv[1]) + 1;
	ContextSettings settings;
	settings.antialiasingLevel = 8;
	window.create(VideoMode(WIDTH,HEIGHT), "Circle Physics 2", Style::Default, settings);
	window.setFramerateLimit(FPS);
	window.setKeyRepeatEnabled(false);
	colorArray[0] = Color::Red;
	colorArray[1] = Color::Blue;
	colorArray[2] = Color::Yellow;
	srand(time(NULL));
	cArray[0].setPosition(120, 75);
	cArray[0].setRadius(RADIUS);
	cArray[0].setOrigin(RADIUS, RADIUS);
	cArray[0].setFillColor(Color::Magenta);
	cMass[0] = MAIN_MASS;
	for(int i = 1; i < x; i++){
		if(i < 6){
			cArray[i].setPosition((i * 110) + 120, 75);
		}	
		if(i >= 6 && i < 12){
			cArray[i].setPosition((i % 6) * 110 + 120, 165);
		}
		if(i >= 12 && i < 18){
			cArray[i].setPosition((i % 6) * 110 + 120, 255);
		}
		if(i >= 18 && i < 24){
			cArray[i].setPosition((i % 6) * 110 + 120, 345);
		}
		if(i >= 24 && i < 30){
			cArray[i].setPosition((i % 6) * 110 + 120, 435);
		}
		if(i >= 30 && i < 36){
			cArray[i].setPosition((i % 6) * 110 + 120, 525);
		}
		cArray[i].setRadius(RADIUS);
		cArray[i].setOrigin(RADIUS, RADIUS);
		cArray[i].setFillColor(colorArray[rand() % 3]);
		cMass[i] = OTHER_MASS;
	}
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
					cAccel[0].y += -(FORCE / cMass[0]);
					break;
					case Keyboard::A:
					cAccel[0].x += -(FORCE / cMass[0]);
					break;
					case Keyboard::S:
					cAccel[0].y += (FORCE / cMass[0]);
					break;
					case Keyboard::D:
					cAccel[0].x += (FORCE / cMass[0]);
					break;
					case Keyboard::F:
					withFriction = !withFriction;
					if(withFriction) cArray[0].setFillColor(Color::Green);
					else cArray[0].setFillColor(Color::Magenta);
					break;
					case Keyboard::Escape:
					window.close();
					break;
				}
				break;
				case Event::KeyReleased:
				switch(event.key.code){
					case Keyboard::W:
					cAccel[0].y = 0;
					break;
					case Keyboard::A:
					cAccel[0].x = 0;
					break;
					case Keyboard::S:
					cAccel[0].y = 0;
					break;
					case Keyboard::D:
					cAccel[0].x = 0;
					break;
				}
				break;
			}
		}
		//move the circles
		for(int i = 0; i < x; i++){
			Vector2f thisPos = cArray[i].getPosition();
			cVel[i] = cVel[i] + (cAccel[i] * TIMESTEP);
			if(withFriction) cVel[i] = cVel[i] - (FRICTION * cVel[i] / cMass[i]);
			thisPos = thisPos + (0.5f * cAccel[i] * TIMESTEP * TIMESTEP) + (cVel[i] * TIMESTEP);
			cArray[i].setPosition(thisPos);
		}
		//border check
		for(int i = 0; i < x; i++){
			float cx = cArray[i].getPosition().x, cy = cArray[i].getPosition().y;
			if(cy + RADIUS > HEIGHT - 1){
				cVel[i].y = -cVel[i].y;
				cArray[i].setPosition(cx, HEIGHT - RADIUS - 1);
			}
			if(cy - RADIUS < 0){
				cVel[i].y = -cVel[i].y;
				cArray[i].setPosition(cx, RADIUS);
			}
			if(cx + RADIUS > WIDTH - 1){
				cVel[i].x = -cVel[i].x;
				cArray[i].setPosition(WIDTH - RADIUS - 1, cy);
			}
			if(cx - RADIUS < 0){
				cVel[i].x = -cVel[i].x;
				cArray[i].setPosition(RADIUS, cy);
			}
		}
		//collision check
		for(int i = 0; i < x; i++){
			Vector2f aPos = cArray[i].getPosition();
			for(int j = 0; j < x; j++){
				if(i != j){
					Vector2f bPos = cArray[j].getPosition();	
					float impulse;
					float distancePos = ((aPos.x - bPos.x) * (aPos.x - bPos.x)) + ((aPos.y - bPos.y) * (aPos.y - bPos.y));
					float radiiDis = (cArray[i].getRadius() + cArray[j].getRadius()) * (cArray[i].getRadius() + cArray[j].getRadius());
					if(distancePos < radiiDis){
						Vector2f norm_vector = aPos - bPos;
						Vector2f rvel_vector = cVel[i] - cVel[j];
						float rel_norm_vel = (norm_vector.x * rvel_vector.x) + (norm_vector.y * rvel_vector.y);
						//cout << norm_vector.x << " " << norm_vector.y << "\n" << rvel_vector.x << " " << rvel_vector.y << "\n" << rel_norm_vel << endl;
						if(rel_norm_vel < 0){
							impulse = -((1.0f + ELASTICITY) * (rel_norm_vel) / (distancePos * ((1.0f / cMass[i]) + 1.0f / cMass[j])));
							cVel[i] = cVel[i] + ((impulse / cMass[i]) * norm_vector);
							cVel[j] = cVel[j] - ((impulse / cMass[j]) * norm_vector);
						}
					}
				}
			}
		}
		window.clear(Color::Black);
		for(int i = 0; i < x; i++){
			window.draw(cArray[i]);
		}
		window.display();
	}
}
