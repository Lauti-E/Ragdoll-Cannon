#include "juego.h"

//Constructor de la clase juego.
juego::juego(Vector2i resolucion, std::string titulo) {
	//Definiciones.
	fps = 60;
	tiempoFrame = 1 / 60.f;
	tiempo2 = 0;
	tiempoUltimoDisparo = 0;

	tiempoCajasFuera = -1;

	cajasFuera = false;
	juegoGanado = false;

	//Inicialización de la ventana y el mundo de Box2D.
	ventana = new RenderWindow(VideoMode(resolucion.x, resolucion.y), titulo);
	ventana->setFramerateLimit(fps);

	mundo = new b2World(b2Vec2(0.f, 9.81f));

	setCamara();

	iniciarFisica();
	iniciarImg();

	//Inicialización del tiempo y reloj.
	tiempo1 = new Time;
	reloj = new Clock;
	relojDisparo = new Clock;

	for (int i = 0;i < 6;i++) {
		actBody[i] = new actor(body[i], figBody[i]);
	}

	//Se inicializa el cañón.
	actSuelo = new actor(bodySuelo, figSuelo);
	can = new cannon(Vector2f(61.5, 105.5));
	can->setCanPos(Vector2f(61.5, 105.5));

	inicializarImagen();

	cargarMenu();

	mostrarMenu();

	gameLoop();
}

//Función para la configuración de la cámara.
void juego::setCamara() {

	camara = new View({ 12.5f, 12.5f }, { 45.f, 45.f });

	camara->move(49.f, 77.f);

	ventana->setView(*camara);
}

//Función para iniciar las físicas de los objetos. 
void juego::iniciarFisica() {
	//Llamada a las funciones.
	fisicaCuerpo();
	fisicaSuelo();
	fisicaPlataformas();
	fisicaCajas();
	fisicaPendulo();
}

//Función para la física y configuración del péndulo.
void juego::fisicaPendulo() {
	//Pivote

	bodyDefPivote.type = b2_staticBody; //Tipo de body.
	bodyDefPivote.position = b2Vec2(61.5f, 86.f); //Posición.

	bodyPivote = mundo->CreateBody(&bodyDefPivote); //Crear body.

	b2CircleShape shapePivote; //Shape.
	shapePivote.m_radius = 0.5f; //Radio.

	//Configuraciones del pivote.
	fixDefPivote.shape = &shapePivote;
	fixDefPivote.restitution = 0.5f;
	fixDefPivote.friction = 0.3f;
	fixDefPivote.density = 1.f;

	fixPivote = bodyPivote->CreateFixture(&fixDefPivote); //Crear fixture.

	//Péndulo

	bodyDefPen.type = b2_dynamicBody; //Tipo de body.
	bodyDefPen.position = b2Vec2(60.f, 0.f); //Posición.

	bodyPen = mundo->CreateBody(&bodyDefPen); //Crear body.

	b2PolygonShape shapePen; //Shape.
	b2Vec2 vertices[4];

	vertices[0].Set(-0.5f, 0.5f);
	vertices[1].Set(0.5f, 0.5f);
	vertices[2].Set(0.5f, 12.f);
	vertices[3].Set(-0.5f, 12.f);

	shapePen.Set(vertices, 4); //4 Vertices para formar un rectángulo.

	//Configuraciones del péndulo.
	fixDefPen.shape = &shapePen;
	fixDefPen.restitution = 0.3f;
	fixDefPen.friction = 0.3f;
	fixDefPen.density = 1.f;

	fixPen = bodyPen->CreateFixture(&fixDefPen); //Crear fixture.

	//Joint entre el pivote y el péndulo.
	jointDefPen.bodyA = bodyPen;
	jointDefPen.bodyB = bodyPivote;

	jointDefPen.localAnchorA.Set(0.f, -1.f); //Anclaje en la parte superior del péndulo.
	jointDefPen.localAnchorB.Set(0.f, 0.f); //Anclaje en el centro del pivote.
	jointDefPen.collideConnected = false; //No colisionar con el pivote.

	mundo->CreateJoint(&jointDefPen); //Crear joint.
}

//Función para la física y configuración del cuerpo.
void juego::fisicaCuerpo() {
	//Cuerpo

	for (int i = 0;i < 6;i++) {
		bodyDef[i].type = b2_dynamicBody; //Tipo de body.
		body[i] = mundo->CreateBody(&bodyDef[i]); //Crear body.
	}

	b2PolygonShape shapeBody[6];

	//Dimensiones del cuerpo
	shapeBody[0].SetAsBox(0.25, 0.30); //Cabeza
	shapeBody[1].SetAsBox(0.62, 1.25); //Torso
	shapeBody[2].SetAsBox(0.150, 0.75); //Brazo derecho
	shapeBody[3].SetAsBox(0.150, 0.75); //Brazo izquierdo
	shapeBody[4].SetAsBox(0.210, 0.75); //Pierna derecha
	shapeBody[5].SetAsBox(0.210, 0.75); //Pierna izquierda

	//Configuraciones del ragdoll.
	for (int i = 0;i < 6;i++) {
		fixDefBody[i].shape = &shapeBody[i];
		fixDefBody[i].density = 1.0f;
		fixDefBody[i].friction = 0.3f;
		fixDefBody[i].restitution = 0.3f;
		fixBody[i] = body[i]->CreateFixture(&fixDefBody[i]); //Crear fixture.
	}

	//Ajustes de resortes a los diferentes ragdolls.

	//Resorte cabeza - torso.
	jointDefBody[0].Initialize(body[0], body[1], b2Vec2(body[0]->GetPosition().x, body[0]->GetPosition().y + 0.23),
		b2Vec2(body[1]->GetPosition().x, body[1]->GetPosition().y - 1.23));

	//Resorte torso - brazo derecho.
	jointDefBody[1].Initialize(body[1], body[2], b2Vec2(body[1]->GetPosition().x + 0.60, body[1]->GetPosition().y - 1.23),
		b2Vec2(body[2]->GetPosition().x - 0.148, body[2]->GetPosition().y - 0.73));

	//Resorte torso - brazo izquierdo.
	jointDefBody[2].Initialize(body[1], body[3], b2Vec2(body[1]->GetPosition().x - 0.60, body[1]->GetPosition().y - 1.23),
		b2Vec2(body[3]->GetPosition().x + 0.148, body[3]->GetPosition().y - 0.73));

	//Resorte torso - pierna derecha.
	jointDefBody[3].Initialize(body[1], body[4], b2Vec2(body[1]->GetPosition().x + 0.60, body[1]->GetPosition().y + 1.23),
		b2Vec2(body[4]->GetPosition().x - 0.208, body[4]->GetPosition().y - 0.73));

	//Resorte torso - pierna izquierda.
	jointDefBody[4].Initialize(body[1], body[5], b2Vec2(body[1]->GetPosition().x - 0.60, body[1]->GetPosition().y + 1.23),
		b2Vec2(body[5]->GetPosition().x + 0.208, body[5]->GetPosition().y - 0.73));

	//Configuración y creación de los resortes.
	for (int i = 0;i < 5;i++) {
		jointDefBody[i].dampingRatio = 0.3f;
		jointDefBody[i].frequencyHz = 4.0f;
		jointDefBody[i].length = 0.010f;
		jointDefBody[i].collideConnected = true;
		jointBody[i] = (b2DistanceJoint*)mundo->CreateJoint(&jointDefBody[i]);
	}
}

//Función para la física y configuración del suelo.
void juego::fisicaSuelo() {
	//Suelo

	bodyDefSuelo.type = b2_staticBody; //Tipo de body.
	bodyDefSuelo.position = b2Vec2(50.f, 110.f); //Posición.

	bodySuelo = mundo->CreateBody(&bodyDefSuelo); //Crear body.

	b2PolygonShape shapeSuelo; //Shape.
	shapeSuelo.SetAsBox(50.f, 2.5f); //Caja de colisión.

	//Configuraciones del suelo.
	fixDefSuelo.shape = &shapeSuelo;
	fixDefSuelo.restitution = 0.f;
	fixDefSuelo.friction = 0.3f;
	fixDefSuelo.density = 1.f;

	fixSuelo = bodySuelo->CreateFixture(&fixDefSuelo); //Crear fixture.
}

//Función para la física y configuración de las plataformas.
void juego::fisicaPlataformas() {
	//Plataformas

	//Plataforma 1.
	bodyDefPlataforma1.type = b2_staticBody; //Tipo de body.
	bodyDefPlataforma1.position = b2Vec2(48.f, 98.f); //Posición.

	bodyPlataforma1 = mundo->CreateBody(&bodyDefPlataforma1); //Crear body.

	b2PolygonShape shapePlataforma1; //Shape.
	shapePlataforma1.SetAsBox(2.5f, 2.5f); //Caja de colisión.

	//Configuraciones de la plataforma 1.
	fixDefPlataforma1.shape = &shapePlataforma1;
	fixDefPlataforma1.restitution = 0.f;
	fixDefPlataforma1.friction = 0.3f;
	fixDefPlataforma1.density = 1.f;

	fixPlataforma1 = bodyPlataforma1->CreateFixture(&fixDefPlataforma1); //Crear fixture.

	//Plataforma 2.
	bodyDefPlataforma2.type = b2_staticBody; //Tipo de body.
	bodyDefPlataforma2.position = b2Vec2(75.f, 98.f); //Posición.

	bodyPlataforma2 = mundo->CreateBody(&bodyDefPlataforma2); //Crear body.

	b2PolygonShape shapePlataforma2; //Shape.
	shapePlataforma2.SetAsBox(2.5f, 2.5f); //Caja de colisión.

	//Configuraciones de la plataforma 2.
	fixDefPlataforma2.shape = &shapePlataforma2;
	fixDefPlataforma2.restitution = 0.f;
	fixDefPlataforma2.friction = 0.3f;
	fixDefPlataforma2.density = 1.f;

	fixPlataforma2 = bodyPlataforma2->CreateFixture(&fixDefPlataforma2); //Crear fixture.
}

//Función para la física y configuración de las cajas.
void juego::fisicaCajas() {
	//Cajas

	//Caja 1.
	bodyDefCaja1.type = b2_dynamicBody; //Tipo de body.
	bodyDefCaja1.position = b2Vec2(48.f, 95.f); //Posición.

	bodyCaja1 = mundo->CreateBody(&bodyDefCaja1); //Crear body.

	b2PolygonShape shapeCaja1; //Shape.
	shapeCaja1.SetAsBox(1.75f, 1.75f); //Caja de colisión.

	//Configuraciones de la caja 1.
	fixDefCaja1.shape = &shapeCaja1;
	fixDefCaja1.restitution = 0.f;
	fixDefCaja1.friction = 0.3f;
	fixDefCaja1.density = 1.5f;

	fixCaja1 = bodyCaja1->CreateFixture(&fixDefCaja1); //Crear fixture.

	//Caja 2.
	bodyDefCaja2.type = b2_dynamicBody; //Tipo de body.
	bodyDefCaja2.position = b2Vec2(75.f, 95.f); //Posición.

	bodyCaja2 = mundo->CreateBody(&bodyDefCaja2); //Crear body.

	b2PolygonShape shapeCaja2; //Shape.
	shapeCaja2.SetAsBox(1.75f, 1.75f); //Caja de colisión.

	//Configuraciones de la caja 2.
	fixDefCaja2.shape = &shapeCaja2;
	fixDefCaja2.restitution = 0.f;
	fixDefCaja2.friction = 0.3f;
	fixDefCaja2.density = 2.f;

	fixCaja2 = bodyCaja2->CreateFixture(&fixDefCaja2); //Crear fixture.
}

//Función para detalles de los bodys.
void juego::iniciarImg() {
	//Suelo.
	figSuelo = new RectangleShape;
	figSuelo->setFillColor(Color::Blue);

	//Ragdoll.
	for (int i = 0;i < 6;i++) {
		figBody[i] = new RectangleShape;
	}

	//Color cabeza.
	figBody[0]->setFillColor(Color::Cyan);
	//Color torso.
	figBody[1]->setFillColor(Color::Red);
	//Color brazo derecho e izquierdo.
	figBody[2]->setFillColor(Color::White);
	figBody[3]->setFillColor(Color::White);
	//Color pierna derecha e izquierda.
	figBody[4]->setFillColor(Color::Magenta);
	figBody[5]->setFillColor(Color::Magenta);
}

//Función para la actualización y borrado.
void juego::actualizarFisica() {

	mundo->Step(tiempoFrame, 8, 8);

	mundo->ClearForces();
}

//Función para procesar eventos del mouse y teclado.
void juego::procesarEventos() {

	Event evento;

	while (ventana->pollEvent(evento)) {

		switch (evento.type) {
		case Event::Closed:
			ventana->close();
			break;

		case Event::MouseButtonPressed:
			if (evento.mouseButton.button == Mouse::Left) {
				if (!actBody[0]->estaDisparado()) {

					//Obtener la posición del cañón.
					Vector2f canPos = can->getPosCan();

					//Crear un nuevo ragdoll en la posición del cañón.
					for (int i = 0; i < 6; i++) {
						body[i]->SetTransform(b2Vec2(canPos.x, canPos.y), 0.f);
						body[i]->SetLinearVelocity(b2Vec2_zero);
						body[i]->SetAngularVelocity(0.f);
					}

					//Marcar como disparado.
					for (int i = 0; i < 6; i++) {
						actBody[i]->setDisparado(true);
					}
					
					//Actualiza el tiempo del último disparo.
					tiempoUltimoDisparo = relojDisparo->getElapsedTime().asSeconds();

					//Calcular el vector de dirección desde el centro del cañón hacia el objetivo.
					Vector2f mousePos = ventana->mapPixelToCoords(Mouse::getPosition(*ventana));
					Vector2f dir = mousePos - canPos;

					//Normalizar la dirección para obtener un vector de magnitud 1.
					float length = sqrt(dir.x * dir.x + dir.y * dir.y);
					dir /= length;

					//Multiplicar la dirección normalizada por la magnitud del impulso.
					b2Vec2 impulse = b2Vec2(dir.x * 23.5f, dir.y * 23.5f);

					//Aplicar un impulso al nuevo ragdoll.
					for (int i = 0; i < 6; i++) {
						body[i]->ApplyLinearImpulse(impulse, body[i]->GetWorldCenter(), true);
					}
				}
				else {
					//Verifica si pasaron 3 segundos desde el último disparo.
					float tiempoActual = relojDisparo->getElapsedTime().asSeconds();

					//Verifica si está fuera de los límites de la cámara.
					Vector2f ragdollPos = actBody[0]->getPos();
					
					if (ragdollPos.x < camara->getCenter().x - camara->getSize().x / 2 ||
						ragdollPos.x > camara->getCenter().x + camara->getSize().x / 2 ||
						ragdollPos.y < camara->getCenter().y - camara->getSize().y / 2 ||
						ragdollPos.y > camara->getCenter().y + camara->getSize().y / 2 || 
						tiempoActual - tiempoUltimoDisparo >= 3) {
						//Restablece el estado del ragdoll para permitir que sea disparado nuevamente.
						for (int i = 0; i < 6; i++) {
							actBody[i]->setDisparado(false);
						}
					}
				}
			}
			break;
		}
	}
}

//Función para mantener abierta la ventana y llamar funciones.
void juego::gameLoop() {

	while (ventana->isOpen()) {

		//Obtener el tamaño y centro de la ventana.
		Vector2f ventanaSize = ventana->getView().getSize();
		Vector2f ventanaCentro = ventana->getView().getCenter();

		//Obtener las coordenadas del mouse en la ventana.
		Vector2f mousePos = ventana->mapPixelToCoords(Mouse::getPosition(*ventana));
		can->updateMov(mousePos);

		*tiempo1 = reloj->getElapsedTime();

		if (tiempo2 + tiempoFrame < tiempo1->asSeconds()) {

			tiempo2 = tiempo1->asSeconds();

			ventana->clear();

			procesarEventos();

			actualizarFisica();

			dibujar();

			if (!juegoGanado) {
				//Obtener las posiciones de las cajas.
				b2Vec2 posCaja1 = bodyCaja1->GetPosition();
				b2Vec2 posCaja2 = bodyCaja2->GetPosition();

				//Verificar si las cajas salieron de los límites de la ventana.
				bool caja1Fuera = posCaja1.x < ventanaCentro.x - ventanaSize.x / 2 || posCaja1.x > ventanaCentro.x + ventanaSize.x / 2 ||
								  posCaja1.y < ventanaCentro.y - ventanaSize.y / 2 || posCaja1.y > ventanaCentro.y + ventanaSize.y / 2;

				bool caja2Fuera = posCaja2.x < ventanaCentro.x - ventanaSize.x / 2 || posCaja2.x > ventanaCentro.x + ventanaSize.x / 2 ||
								  posCaja2.y < ventanaCentro.y - ventanaSize.y / 2 || posCaja2.y > ventanaCentro.y + ventanaSize.y / 2;

				//Si la caja 1 y la caja 2 salieron de la ventana.
				if (caja1Fuera && caja2Fuera) {
					//Se completa el nivel.
					juegoGanado = true;
					
					//Se obtiene el tiempo.
					tiempoCajasFuera = reloj->getElapsedTime().asSeconds();
					cajasFuera = true; //Se marca que las cajas salieron de la ventana.
				}
			}

			//Si las cajas salieron de la ventana.
			if (cajasFuera) {
				//Verifica si pasaron 2 segundos desde que las cajas salieron de la ventana.
				if (reloj->getElapsedTime().asSeconds() - tiempoCajasFuera >= 2) {

					//Se llama a la función para cambiar las posiciones de los objetos.
					cambiarPos();
					sprVict.setColor(Color(255, 255, 255, 0)); //Volver transparente la imagen.

					cajasFuera = false; //Se desmarca que las cajas salieron de la ventana.
				}
			}

			//Si se realiza la condición de victoria.
			if (juegoGanado) {
				//Se muestra la imagen de victoria.
				sprVict.setPosition(ventanaCentro);
				ventana->draw(sprVict);
			}

			ventana->display();
		}
	}
}

//Función para dibujar.
void juego::dibujar() {

	actSuelo->dibujar(*ventana);

	//Llamada a las funciones.
	dibujarPlataformas();
	dibujarCajas();
	dibujarPendulo();

	can->draw(*ventana);

	for (int i = 0;i < 6;i++) {
		actBody[i]->dibujar(*ventana);
	}
}

//Función para dibujar el péndulo.
void juego::dibujarPendulo() {
	//Pivote.
	CircleShape figPivote(1.f);
	figPivote.setFillColor(Color::Blue); //Color.
	figPivote.setOrigin(1.f, 1.f); //Origen en el centro.

	b2Vec2 posPivote = bodyPivote->GetPosition();
	figPivote.setPosition(posPivote.x, posPivote.y);

	ventana->draw(figPivote);

	//Péndulo.
	RectangleShape figPen(Vector2f(1.5f, 12.f)); //Tamaño.
	figPen.setFillColor(Color::Yellow); //Color.
	figPen.setOrigin(0.75f, 0.f); //Origen en el centro superior.

	b2Vec2 posPen = bodyPen->GetPosition();
	figPen.setPosition(posPen.x, posPen.y);

	//Rotar el rectángulo según la rotación del péndulo.
	float ang = bodyPen->GetAngle() * 180 / b2_pi;
	figPen.setRotation(ang);

	ventana->draw(figPen);
}

//Función para dibujar las plataformas.
void juego::dibujarPlataformas() {
	//Plataformas.

	//Plataforma 1.
	figPlataforma1 = new RectangleShape(Vector2f(5.f, 5.f)); //Tamaño.
	figPlataforma1->setFillColor(Color::White); //Color.
	figPlataforma1->setOrigin(figPlataforma1->getSize().x / 2.f, figPlataforma1->getSize().y / 2.f); //Origen en el centro.

	b2Vec2 posPlataforma1 = bodyPlataforma1->GetPosition();
	figPlataforma1->setPosition(posPlataforma1.x, posPlataforma1.y);

	ventana->draw(*figPlataforma1);

	//Plataforma 2.
	figPlataforma2 = new RectangleShape(Vector2f(5.f, 5.f)); //Tamaño.
	figPlataforma2->setFillColor(Color::White); //Color.
	figPlataforma2->setOrigin(figPlataforma2->getSize().x / 2.f, figPlataforma2->getSize().y / 2.f); //Origen en el centro.

	b2Vec2 posPlataforma2 = bodyPlataforma2->GetPosition();
	figPlataforma2->setPosition(posPlataforma2.x, posPlataforma2.y);

	ventana->draw(*figPlataforma2);
}

//Función para dibujar las cajas.
void juego::dibujarCajas() {
	//Cajas.

	//Caja 1.
	figCaja1 = new RectangleShape(Vector2f(3.5f, 3.5f)); //Tamaño.
	figCaja1->setFillColor(Color::Cyan); //Color.
	figCaja1->setOrigin(figCaja1->getSize().x / 2.f, figCaja1->getSize().y / 2.f); //Origen en el centro.

	b2Vec2 posCaja1 = bodyCaja1->GetPosition();
	figCaja1->setPosition(posCaja1.x, posCaja1.y);

	ventana->draw(*figCaja1);

	//Caja 2.
	figCaja2 = new RectangleShape(Vector2f(3.5f, 3.5f)); //Tamaño.
	figCaja2->setFillColor(Color::Cyan); //Color.
	figCaja2->setOrigin(figCaja2->getSize().x / 2.f, figCaja2->getSize().y / 2.f); //Origen en el centro.

	b2Vec2 posCaja2 = bodyCaja2->GetPosition();
	figCaja2->setPosition(posCaja2.x, posCaja2.y);

	ventana->draw(*figCaja2);
}

//Función para inicializar la imagen de victoria.
void juego::inicializarImagen() {

	//Cargar imagen.
	if (!texVict.loadFromFile("Imagen//Check.png")) {
		cout << "Error al cargar la imagen." << endl;
	}

	sprVict.setTexture(texVict);
	sprVict.setScale(0.02f, 0.02f);

	//Centrar la imagen.
	FloatRect spriteRect = sprVict.getLocalBounds();
	sprVict.setOrigin(spriteRect.left + spriteRect.width / 2.f, spriteRect.top + spriteRect.height / 2.f);
}

//Función para configurar el menú.
void juego::cargarMenu() {
	//Cargar imagenes.
	if (!texPlay.loadFromFile("Imagen//play.png")) {
		cout << "Error al cargar la imagen." << endl;
	}

	if (!texSalir.loadFromFile("Imagen//home.png")) {
		cout << "Error al cargar la imagen." << endl;
	}

	//Configurar las imagenes.
	sprPlay.setTexture(texPlay);
	sprPlay.setScale(0.010f, 0.010f);

	sprSalir.setTexture(texSalir);
	sprSalir.setScale(0.010f, 0.010f);

	//Centrar las imágenes.
	FloatRect spriteRectPlay = sprPlay.getLocalBounds();
	sprPlay.setOrigin(spriteRectPlay.left + spriteRectPlay.width / 2.f, spriteRectPlay.top + spriteRectPlay.height / 2.f);

	FloatRect spriteRectSalir = sprSalir.getLocalBounds();
	sprSalir.setOrigin(spriteRectSalir.left + spriteRectSalir.width / 2.f, spriteRectSalir.top + spriteRectSalir.height / 2.f);
}

//Función para mostrar el menú.
void juego::mostrarMenu() {

	const float despVertical = 10.f; //Desplazamiento vertical.

	while (ventana->isOpen()) {

		Event evento;

		Vector2f ventanaCentro = ventana->getView().getCenter();

		while (ventana->pollEvent(evento)) {
			if (evento.type == Event::Closed) {
				ventana->close();
			}
			else if (evento.type == Event::MouseButtonPressed && evento.mouseButton.button == Mouse::Left) {

				Vector2f mousePos = ventana->mapPixelToCoords(Mouse::getPosition(*ventana));

				if (sprPlay.getGlobalBounds().contains(mousePos)) {
					return; //Iniciar juego.
				}
				else if (sprSalir.getGlobalBounds().contains(mousePos)) {
					ventana->close(); //Cerrar la ventana.
				}
			}
		}

		//Configurar las posiciones de las imágenes.
		sprPlay.setPosition(ventanaCentro.x, ventanaCentro.y -despVertical);
		sprSalir.setPosition(ventanaCentro.x, ventanaCentro.y + despVertical);

		ventana->clear();

		ventana->draw(sprPlay);
		ventana->draw(sprSalir);

		ventana->display();
	}
}

//Función para cambiar la posición de los objetos.
void juego::cambiarPos() {

	bodyPlataforma1->DestroyFixture(fixPlataforma1);
	bodyPlataforma2->DestroyFixture(fixPlataforma2);
	bodyCaja1->DestroyFixture(fixCaja1);
	bodyCaja2->DestroyFixture(fixCaja2);
	bodyPivote->DestroyFixture(fixPivote);
	bodyPen->DestroyFixture(fixPen);

	//Plataforma 1.
	bodyDefPlataforma1.type = b2_staticBody; //Tipo de body.
	bodyDefPlataforma1.position = b2Vec2(65.f, 84.f); //Posición.

	bodyPlataforma1 = mundo->CreateBody(&bodyDefPlataforma1); //Crear body.

	b2PolygonShape shapePlataforma1; //Shape.
	shapePlataforma1.SetAsBox(2.5f, 2.5f); //Caja de colisión.

	//Configuraciones de la plataforma 1.
	fixDefPlataforma1.shape = &shapePlataforma1;
	fixDefPlataforma1.restitution = 0.f;
	fixDefPlataforma1.friction = 0.3f;
	fixDefPlataforma1.density = 1.f;

	fixPlataforma1 = bodyPlataforma1->CreateFixture(&fixDefPlataforma1); //Crear fixture.
	
	//Plataforma 2.
	bodyDefPlataforma2.type = b2_staticBody; //Tipo de body.
	bodyDefPlataforma2.position = b2Vec2(75.f, 83.f); //Posición.

	bodyPlataforma2 = mundo->CreateBody(&bodyDefPlataforma2); //Crear body.

	b2PolygonShape shapePlataforma2; //Shape.
	shapePlataforma2.SetAsBox(2.5f, 2.5f); //Caja de colisión.

	//Configuraciones de la plataforma 2.
	fixDefPlataforma2.shape = &shapePlataforma2;
	fixDefPlataforma2.restitution = 0.f;
	fixDefPlataforma2.friction = 0.3f;
	fixDefPlataforma2.density = 1.f;

	fixPlataforma2 = bodyPlataforma2->CreateFixture(&fixDefPlataforma2); //Crear fixture.

	//Caja 1.
	bodyDefCaja1.type = b2_dynamicBody; //Tipo de body.
	bodyDefCaja1.position = b2Vec2(65.f, 81.f); //Posición.

	bodyCaja1 = mundo->CreateBody(&bodyDefCaja1); //Crear body.

	b2PolygonShape shapeCaja1; //Shape.
	shapeCaja1.SetAsBox(1.75f, 1.75f); //Caja de colisión.

	//Configuraciones de la caja 1.
	fixDefCaja1.shape = &shapeCaja1;
	fixDefCaja1.restitution = 0.f;
	fixDefCaja1.friction = 0.3f;
	fixDefCaja1.density = 1.5f;

	fixCaja1 = bodyCaja1->CreateFixture(&fixDefCaja1); //Crear fixture.

	//Caja 2.
	bodyDefCaja2.type = b2_dynamicBody; //Tipo de body.
	bodyDefCaja2.position = b2Vec2(75.f, 80.f); //Posición.

	bodyCaja2 = mundo->CreateBody(&bodyDefCaja2); //Crear body.

	b2PolygonShape shapeCaja2; //Shape.
	shapeCaja2.SetAsBox(1.75f, 1.75f); //Caja de colisión.

	//Configuraciones de la caja 2.
	fixDefCaja2.shape = &shapeCaja2;
	fixDefCaja2.restitution = 0.f;
	fixDefCaja2.friction = 0.3f;
	fixDefCaja2.density = 2.f;

	fixCaja2 = bodyCaja2->CreateFixture(&fixDefCaja2); //Crear fixture.

	//Pivote

	bodyDefPivote.type = b2_staticBody; //Tipo de body.
	bodyDefPivote.position = b2Vec2(52.f, 84.f); //Posición.

	bodyPivote = mundo->CreateBody(&bodyDefPivote); //Crear body.

	b2CircleShape shapePivote; //Shape.
	shapePivote.m_radius = 0.5f; //Radio.

	//Configuraciones del pivote.
	fixDefPivote.shape = &shapePivote;
	fixDefPivote.restitution = 0.5f;
	fixDefPivote.friction = 0.3f;
	fixDefPivote.density = 1.f;

	fixPivote = bodyPivote->CreateFixture(&fixDefPivote); //Crear fixture.

	//Péndulo

	bodyDefPen.type = b2_dynamicBody; //Tipo de body.
	bodyDefPen.position = b2Vec2(60.f, 0.f); //Posición.

	bodyPen = mundo->CreateBody(&bodyDefPen); //Crear body.

	b2PolygonShape shapePen; //Shape.
	b2Vec2 vertices[4];

	vertices[0].Set(-0.5f, 0.5f);
	vertices[1].Set(0.5f, 0.5f);
	vertices[2].Set(0.5f, 12.f);
	vertices[3].Set(-0.5f, 12.f);

	shapePen.Set(vertices, 4); //4 Vertices para formar un rectángulo.

	//Configuraciones del péndulo.
	fixDefPen.shape = &shapePen;
	fixDefPen.restitution = 0.3f;
	fixDefPen.friction = 0.3f;
	fixDefPen.density = 1.f;

	fixPen = bodyPen->CreateFixture(&fixDefPen); //Crear fixture.

	//Joint entre el pivote y el péndulo.
	jointDefPen.bodyA = bodyPen;
	jointDefPen.bodyB = bodyPivote;

	jointDefPen.localAnchorA.Set(0.f, -1.f); //Anclaje en la parte superior del péndulo.
	jointDefPen.localAnchorB.Set(0.f, 0.f); //Anclaje en el centro del pivote.
	jointDefPen.collideConnected = false; //No colisionar con el pivote.

	mundo->CreateJoint(&jointDefPen); //Crear joint.

	can = new cannon(Vector2f(45, 105.5));
	can->setCanPos(Vector2f(45, 105.5));
}