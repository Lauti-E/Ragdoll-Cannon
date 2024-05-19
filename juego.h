#pragma once
#include "actor.h"
#include "cannon.h"

#include <vector>
#include <iostream>

using namespace std;
using namespace sf;

class juego {
private:
	//Variable ventana y cámara.
	RenderWindow* ventana;
	View* camara;

	//Variables tiempo y reloj.
	Time* tiempo1;
	Clock* reloj;
	Clock* relojDisparo;

	//Variables generales.
	float fps, tiempoFrame, tiempo2, tiempoUltimoDisparo, tiempoCajasFuera;
	bool cajasFuera;
	bool juegoGanado;

	//Mundo de Box2D.
	b2World* mundo;

	//Cuerpo.

	b2Body* body[6];
	b2BodyDef bodyDef[6];
	b2Fixture* fixBody[6];
	b2FixtureDef fixDefBody[6];

	//Suelo.

	b2Body* bodySuelo;
	b2BodyDef bodyDefSuelo;
	b2Fixture* fixSuelo;
	b2FixtureDef fixDefSuelo;

	//Pivote.
	b2Body* bodyPivote;
	b2BodyDef bodyDefPivote;
	b2Fixture* fixPivote;
	b2FixtureDef fixDefPivote;

	//Péndulo.
	b2Body* bodyPen;
	b2BodyDef bodyDefPen;
	b2Fixture* fixPen;
	b2FixtureDef fixDefPen;

	//Plataformas.

	//Bodys.
	b2Body* bodyPlataforma1;
	b2Body* bodyPlataforma2;

	b2BodyDef bodyDefPlataforma1;
	b2BodyDef bodyDefPlataforma2;

	//Fixtures.
	b2Fixture* fixPlataforma1;
	b2Fixture* fixPlataforma2;

	b2FixtureDef fixDefPlataforma1;
	b2FixtureDef fixDefPlataforma2;

	//Cajas.

	//Bodys.
	b2Body* bodyCaja1;
	b2Body* bodyCaja2;

	b2BodyDef bodyDefCaja1;
	b2BodyDef bodyDefCaja2;

	//Fixtures.
	b2Fixture* fixCaja1;
	b2Fixture* fixCaja2;

	b2FixtureDef fixDefCaja1;
	b2FixtureDef fixDefCaja2;

	//Objeto para el ragdoll.
	actor* actBody[6];
	actor* actSuelo;

	//Objeto para el cañón.
	cannon* can;

	//Formas.
	RectangleShape* figBody[6];
	RectangleShape* figSuelo;

	RectangleShape* figPlataforma1;
	RectangleShape* figPlataforma2;

	RectangleShape* figCaja1;
	RectangleShape* figCaja2;

	//Resortes.

	b2DistanceJoint* jointBody[5];
	b2DistanceJointDef jointDefBody[5];
	b2RevoluteJointDef jointDefPen;

	//Imagenes.
	Texture texVict; //Textura de la imagen.
	Sprite sprVict; //Sprite de la imagen.
	
	Texture texPlay; //Textura de la imagen.
	Sprite sprPlay; //Sprite de la imagen.

	Texture texSalir; //Textura de la imagen.
	Sprite sprSalir;//Sprite de la imagen.

public:
	//Constructor.
	juego(Vector2i resolucion, std::string titulo);

	//Función para la configuración de la cámara.
	void setCamara();

	//Funciones de físicas generales.
	void iniciarFisica();

	//Funciones generales.
	void iniciarImg();
	void actualizarFisica();
	void procesarEventos();
	void gameLoop();
	void dibujar();

	//Funciones para las físicas de cada objeto.
	void fisicaPendulo();
	void fisicaCuerpo();
	void fisicaSuelo();
	void fisicaPlataformas();
	void fisicaCajas();

	//Funciones para dibujar los objetos.
	void dibujarPendulo();
	void dibujarPlataformas();
	void dibujarCajas();

	//Función para mostrar la imagen de victoria.
	void inicializarImagen();

	//Funciones para cargar y configurar el menú.
	void cargarMenu();
	void mostrarMenu();

	void cambiarPos();
};
