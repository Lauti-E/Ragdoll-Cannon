#include "cannon.h"

const float PI = 3.14; //Valor de PI.

cannon::cannon(Vector2f pos) {
	ang = 0;

	//Tamaño y posición de la base.
	base.setPointCount(3); //Número de vértices del triángulo.
	base.setPoint(0, Vector2f(0.f, 0.f));
	base.setPoint(1, Vector2f(4.f, 0.f));
	base.setPoint(2, Vector2f(2.f, -2.f));
	base.setOrigin(2.f, -2.f); //Origen en el punto medio inferior.
	posBase = pos;
	setBasePos(posBase);

	//Tamaño y posición del cañón.
	can.setSize(Vector2f(-4.f, -2.f));
	can.setOrigin(-4.f, -1.f); //El origen se establece en el extremo derecho del cañón.
	posCan = pos;
	setCanPos(posCan);

	//Color de la base y el cañón.
	base.setFillColor(Color::Red);
	can.setFillColor(Color::Yellow);
}

void cannon::updateMov(Vector2f target) {
	//Calcula el ángulo entre la posición del cañón y la posición del mouse.
	Vector2f dir = target - posCan;
	ang = atan2(dir.y, dir.x) * 180 / PI;

	//Actualizar la posición del cañón.
	can.setRotation(ang);
}

void cannon::draw(RenderWindow& window) {
	//Dibujar la base del cañón y el cañón.
	window.draw(base);
	window.draw(can);
}