#include "actor.h"

//Constructor de la clase actor.
actor::actor(b2Body* cuerpo, RectangleShape* figura) {

	//Inicializaci�n de miembros.
	bodyActor = cuerpo;
	figActor = figura;

	//Obtener posici�n y dimensiones.
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

//M�todo para dibujar la posici�n y la rotaci�n de la figura en funci�n de la posici�n y �ngulo del cuerpo de Box2D y se dibuja.
void actor::dibujar(RenderWindow& ventana) {

	posicion = bodyActor->GetPosition();

	figActor->setPosition(posicion.x, posicion.y);

	figActor->setRotation(rad2deg(bodyActor->GetAngle()));

	ventana.draw(*figActor);
}

//M�todo para convertir radianes a grados mediante la f�rmula "grados = radianes * 180 / pi".
float actor::rad2deg(float radianes) {
	return radianes * 180 / b2_pi;
}