#include "cannon.h"

const float PI = 3.14; //Valor de PI.

cannon::cannon(Vector2f pos) {
	ang = 0;

	//Tama�o y posici�n de la base.
	base.setPointCount(3); //N�mero de v�rtices del tri�ngulo.
	base.setPoint(0, Vector2f(0.f, 0.f));
	base.setPoint(1, Vector2f(4.f, 0.f));
	base.setPoint(2, Vector2f(2.f, -2.f));
	base.setOrigin(2.f, -2.f); //Origen en el punto medio inferior.
	posBase = pos;
	setBasePos(posBase);

	//Tama�o y posici�n del ca��n.
	can.setSize(Vector2f(-4.f, -2.f));
	can.setOrigin(-4.f, -1.f); //El origen se establece en el extremo derecho del ca��n.
	posCan = pos;
	setCanPos(posCan);

	//Color de la base y el ca��n.
	base.setFillColor(Color::Red);
	can.setFillColor(Color::Yellow);
}

void cannon::updateMov(Vector2f target) {
	//Calcula el �ngulo entre la posici�n del ca��n y la posici�n del mouse.
	Vector2f dir = target - posCan;
	ang = atan2(dir.y, dir.x) * 180 / PI;

	//Actualizar la posici�n del ca��n.
	can.setRotation(ang);
}

void cannon::draw(RenderWindow& window) {
	//Dibujar la base del ca��n y el ca��n.
	window.draw(base);
	window.draw(can);
}