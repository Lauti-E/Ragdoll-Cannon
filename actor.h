#pragma once
#include "Box2D/Box2D.h"
#include "SFML/Graphics.hpp"

using namespace sf;

class actor {
private:
	//Variables cuerpo y figura.
	b2Body* bodyActor;
	RectangleShape* figActor;

	b2AABB dimensiones;
	b2Vec2 posicion;

	bool disparado;
public:
	//Constructor.
	actor(b2Body* cuerpo, RectangleShape* figura);

	void dibujar(RenderWindow& ventana);

	//Método para convertir radianes a grados.
	float rad2deg(float radianes);

	//Método para saber si se disparó el ragdoll.
	bool estaDisparado() const { return disparado; }

	//Definir el valor del disparo.
	void setDisparado(bool value) { disparado = value; }

	//Obtener la posición.
	Vector2f getPos() const {
		b2Vec2 pos = bodyActor->GetPosition();

		return Vector2f(pos.x, pos.y);
	}
};