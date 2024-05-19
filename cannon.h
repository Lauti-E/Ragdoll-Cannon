#pragma once

#include "Box2D/Box2D.h"
#include "SFML/Graphics.hpp"

#include <cmath>

using namespace sf;

class cannon {
private:
	ConvexShape base; //Base del ca��n.
	RectangleShape can; // Ca��n.

	Vector2f posBase; //Posici�n de la base.
	Vector2f posCan; //Posici�n del ca��n.

	float ang; //�ngulo del ca��n.
public:
	//Constructor.
	cannon(Vector2f pos);

	void updateMov(Vector2f target); //M�todo para actualizar el movimiento del ca��n.
	void draw(RenderWindow& window); //Dibuja el ca��n en la ventana.

	//Obtener la posici�n de la base y del ca�on.
	Vector2f getPosBase() const {
		return posBase;
	}
	Vector2f getPosCan() const {
		return posCan;
	}

	//Posicionar las base y el ca��n.
	void setBasePos(Vector2f pos) {
		posBase = pos;
		base.setPosition(posBase);
	}
	void setCanPos(Vector2f pos) {
		posCan = pos;
		can.setPosition(posCan);
	}
};