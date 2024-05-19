#pragma once

#include "Box2D/Box2D.h"
#include "SFML/Graphics.hpp"

#include <cmath>

using namespace sf;

class cannon {
private:
	ConvexShape base; //Base del cañón.
	RectangleShape can; // Cañón.

	Vector2f posBase; //Posición de la base.
	Vector2f posCan; //Posición del cañón.

	float ang; //Ángulo del cañón.
public:
	//Constructor.
	cannon(Vector2f pos);

	void updateMov(Vector2f target); //Método para actualizar el movimiento del cañón.
	void draw(RenderWindow& window); //Dibuja el cañón en la ventana.

	//Obtener la posición de la base y del cañon.
	Vector2f getPosBase() const {
		return posBase;
	}
	Vector2f getPosCan() const {
		return posCan;
	}

	//Posicionar las base y el cañón.
	void setBasePos(Vector2f pos) {
		posBase = pos;
		base.setPosition(posBase);
	}
	void setCanPos(Vector2f pos) {
		posCan = pos;
		can.setPosition(posCan);
	}
};