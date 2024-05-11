#include "explocion.h"
#include "mapa.h"
#include "util.h"

explocion::explocion(GLuint texturaf, GLuint texturaH, float** rango){
	this->rango = rango;

	texturaFuego = texturaf;
	texturaHumo = texturaH;


}

explocion::~explocion() {
	
	
}

bool explocion::timer(float deltaT)
{
	if (deltaT >= this->life) {
		return true;
	}
	else {
		this->life = this->life - deltaT;
		return false;
	}
}

void explocion::generateExplocion(int cant, particleGenerator* pSistem)
{

	std::cout << cant << "\n";

	if (this->rango != nullptr) {
		int cuadrante_bomba_x = this->rango[4][1];
		int cuadrante_bomba_y = this->rango[4][0];
		

		
		//Fuego
		for (int cont = 0; cont < 140; cont++) {

			this->expPart.position.x = cuadrante_bomba_x * LARGO_UNIDAD + LARGO_UNIDAD / 2;
			this->expPart.position.y = cuadrante_bomba_y * LARGO_UNIDAD + LARGO_UNIDAD / 2;
			this->expPart.position.z = ALTURA_CAMARA_PRIMERA_PERSONA;

			this->expPart.origen = this->expPart.position;

			this->expPart.t_inicio = 0.0f;

			this->expPart.lifeTime = 3000;

			this->expPart.sizeBegin = 6;
			this->expPart.sizeEnd = Random::Float() * 30;

			this->expPart.velocity.x = (0.5 - Random::Float()) * 70;
			this->expPart.velocity.y = (0.5 - Random::Float()) * 70;
			this->expPart.velocity.z = (0.5 - Random::Float()) * 70;

			this->expPart.velocityVariation.x = (Random::Float()) * 0.0001;
			this->expPart.velocityVariation.y = (Random::Float()) * 0.0001;
			this->expPart.velocityVariation.z = (Random::Float()) * 0.0001;

			GLfloat coloresInicio[4] = { 243.f / 256.f, 236.f / 256.f, 5.f / 256.f, 0.6f };
			for (int i = 0; i < 4; i++) {
				this->expPart.colorBegin[i] = coloresInicio[i];
			}

			if (cont == 0) {
				this->expPart.useLight7 = true;
			}

			GLfloat coloresFinal[4] = { 239.f / 256.f,	 162.f / 256.f,		37.f / 256.f, 0.0f };
			for (int i = 0; i < 4; i++) {
				this->expPart.colorEnd[i] = coloresFinal[i];
			}

			this->expPart.textura = this->texturaFuego;
			pSistem->Emit(this->expPart);
		}



		bool isNoche = configuraciones::getInstancia()->getModoIluminacion() != MODOS_ILUMINACION_ATARDESER;
		
		for (int i = 0; i <= cant*4; i++) {
			int y = this->rango[i][0];
			int x = this->rango[i][1];

			//Humo
			for (int cont = 0; cont < 40; cont++) {

				this->expPart.position.x = x * LARGO_UNIDAD + LARGO_UNIDAD / 2;
				this->expPart.position.y = y * LARGO_UNIDAD + LARGO_UNIDAD / 2;
				this->expPart.position.z = ALTURA_CAMARA_PRIMERA_PERSONA;

				this->expPart.origen = this->expPart.position;

				this->expPart.t_inicio = 4500.f;

				this->expPart.lifeTime = 8000.f;

				this->expPart.sizeBegin = 20;
				this->expPart.sizeEnd = Random::Float() * 40;

				this->expPart.velocity.x = (0.5 - Random::Float()) * 25;
				this->expPart.velocity.y = (0.5 - Random::Float()) * 25;
				this->expPart.velocity.z = (0.5 - Random::Float()) * 25;

				this->expPart.velocityVariation.x = ( Random::Float()) * 0.40;
				this->expPart.velocityVariation.y = ( Random::Float()) * 0.40;
				this->expPart.velocityVariation.z = ( Random::Float()) * 0.40;

				if (isNoche) {
					GLfloat coloresInicio[4] = { 255.f / 256.f,	 239.f / 256.f,		186.f / 256.f, 0.5f };
					for (int i = 0; i < 4; i++) {
						this->expPart.colorBegin[i] = coloresInicio[i];
					}
				}
				else {
					GLfloat coloresInicio[4] = { 1.f, 1.f, 1.f, 0.5f };
					for (int i = 0; i < 4; i++) {
						this->expPart.colorBegin[i] = coloresInicio[i];
					}
				}
				
				if (isNoche) {
					GLfloat coloresFinal[4] = { 45.f / 256.f,	 37.f / 256.f,		111.f / 256.f, 0.0f };
					for (int i = 0; i < 4; i++) {
						this->expPart.colorEnd[i] = coloresFinal[i];
					}
				}
				else {
					GLfloat coloresFinal[4] = { 102.f / 256.f,	 102.f / 256.f,		102.f / 256.f, 0.0f };
					for (int i = 0; i < 4; i++) {
						this->expPart.colorEnd[i] = coloresFinal[i];
					}
				}

				
				
				this->expPart.textura = this->texturaHumo;
				pSistem->Emit(this->expPart);
			}

		}
	}
}




