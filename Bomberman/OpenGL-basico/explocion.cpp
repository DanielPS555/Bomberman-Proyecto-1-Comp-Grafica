#include "explocion.h"
#include "mapa.h"
#include "util.h"

explocion::explocion(float life, float** rango)
{
	this->life = life;	
	this->rango = rango;
	this->textura = inicializarTextura("assets/explosion.jpg");
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
	if (this->rango != nullptr) {
		int cuadrante_bomba_x = this->rango[0][0];
		int cuadrante_bomba_y = this->rango[0][1];
		
		for (int i = 0; i <= cant; i++) {
			int y = this->rango[i][0];
			int x = this->rango[i][1];
			
			for (int j = 0; j < LARGO_UNIDAD; j=j+5) {
				for (int k = 0; k < LARGO_UNIDAD; k=k+5) {
					for (int m = 0; m < LARGO_UNIDAD / 2; m=m+5) {
						this->expPart.Position.x = x * LARGO_UNIDAD + j;
						this->expPart.Position.y = y * LARGO_UNIDAD + k;
						this->expPart.Position.z = m;
						int distancia_centro_x = this->expPart.Position.x - (cuadrante_bomba_x * LARGO_UNIDAD + LARGO_UNIDAD / 2);
						int distancia_centro_y = this->expPart.Position.y - (cuadrante_bomba_x * LARGO_UNIDAD + LARGO_UNIDAD / 2);
						this->expPart.distancia_centro = sqrt(distancia_centro_x * distancia_centro_x + distancia_centro_y * distancia_centro_y);
						this->expPart.LifeTime = life;
						this->expPart.SizeBegin = LARGO_UNIDAD;
						this->expPart.SizeEnd = LARGO_UNIDAD;
						this->expPart.SizeVariation = 0;//Random::Float() * 2;
						this->expPart.Velocity.x = (Random::Float() - 0.5) * 0.1;
						this->expPart.Velocity.y = (Random::Float() - 0.5) * 0.1;
						this->expPart.Velocity.z = (Random::Float() - 0.5) * 0.1;
						this->expPart.VelocityVariation.x = 0;//Random::Float() * 2;
						this->expPart.VelocityVariation.y = 0;//Random::Float() * 2;
						this->expPart.VelocityVariation.z = 0;//Random::Float() * 2;
						this->expPart.weight = -4;
						this->expPart.textura = this->textura;
						pSistem->Emit(this->expPart);
					}
				}
			}
			

			


		}
	}
}




