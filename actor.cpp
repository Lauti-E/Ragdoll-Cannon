#include "actor.h"

//Constructor de la clase actor.
actor::actor(b2Body* cuerpo, RectangleShape* figura) {

	//Inicialización de miembros.
	bodyActor = cuerpo;
	figActor = figura;

	//Obtener posición y dimensiones.
	posicion = bodyActor->GetPosition();

	dimensiones.lowerBound = b2Vec2(FLT_MAX, FLT_MAX);
	dimensiones.upperBound = b2Vec2(-FLT_MAX, -FLT_MAX);

	for (b2Fixture* i = bodyActor->GetFixtureList();i;i = i->GetNext()) {
		dimensiones = i->GetAABB(0);
	}

	figActor->setSize({ dimensiones.GetExtents().x * 2, dimensiones.GetExtents().y * 2 });

	figActor->setOrigin(figActor->getSize().x / 2.f, figActor->getSize().y / 2.f);

	figActor->setPosition(posicion.x, posicion.y);
}

//Método para dibujar la posición y la rotación de la figura en función de la posición y ángulo del cuerpo de Box2D y se dibuja.
void actor::dibujar(RenderWindow& ventana) {

	posicion = bodyActor->GetPosition();

	figActor->setPosition(posicion.x, posicion.y);

	figActor->setRotation(rad2deg(bodyActor->GetAngle()));

	ventana.draw(*figActor);
}

//Método para convertir radianes a grados mediante la fórmula "grados = radianes * 180 / pi".
float actor::rad2deg(float radianes) {
	return radianes * 180 / b2_pi;
}