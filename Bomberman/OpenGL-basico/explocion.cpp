#include "explocion.h"
#include "mapa.h"
#include "util.h"

explocion::explocion(float life, float** rango)
{
	this->life = life;	
	this->rango = rango;
	this->textura = inicializarTextura("assets/nube.jpg");;
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
		for (int i = 0; i <= cant * 4; i++) {
			int y = this->rango[i][0];
			int x = this->rango[i][1];
			for (int j = 0; j < 10; j++) {
				this->expPart.Position.x = x * LARGO_UNIDAD + (Random::Float() * LARGO_UNIDAD);
				this->expPart.Position.y = y * LARGO_UNIDAD + (Random::Float() * LARGO_UNIDAD);
				this->expPart.Position.z = (LARGO_UNIDAD / 5);
				this->expPart.LifeTime = 1.0;
				this->expPart.SizeBegin = LARGO_UNIDAD/5;
				this->expPart.SizeEnd = LARGO_UNIDAD/3;
				this->expPart.SizeVariation = Random::Float() * 2;
				this->expPart.Velocity.x = Random::Float() * 3;
				this->expPart.Velocity.y = Random::Float() * 3;
				this->expPart.Velocity.z = Random::Float() * 3;
				this->expPart.VelocityVariation.x = Random::Float() * 2;
				this->expPart.VelocityVariation.y = Random::Float() * 2;
				this->expPart.VelocityVariation.z = Random::Float() * 2;
				this->expPart.weight = -4;
				this->expPart.ColorBegin[0] = Random::Float() * 256;
				this->expPart.ColorBegin[1] = Random::Float() * 256;
				this->expPart.ColorBegin[2] = Random::Float() * 256;
				this->expPart.ColorBegin[3] = Random::Float() * 256;
				this->expPart.ColorEnd[0] = this->expPart.ColorBegin[0] + 30;
				this->expPart.ColorEnd[1] = this->expPart.ColorBegin[0] + 30;
				this->expPart.ColorEnd[2] = this->expPart.ColorBegin[0] + 30;
				this->expPart.ColorEnd[3] = this->expPart.ColorBegin[0] + 30;
				this->expPart.textura = this->textura;
				pSistem->Emit(this->expPart);
			}
		}
	}
}




