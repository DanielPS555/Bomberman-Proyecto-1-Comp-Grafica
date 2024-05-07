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
				this->expPart.Position.x = x * LARGO_UNIDAD + (LARGO_UNIDAD/2);
				this->expPart.Position.y = y * LARGO_UNIDAD + (LARGO_UNIDAD/2);
				this->expPart.Position.z = (ALTURA_PARED / 5);
				this->expPart.LifeTime = 2000;
				this->expPart.SizeBegin = LARGO_UNIDAD/3;
				this->expPart.SizeEnd = LARGO_UNIDAD/2;
				this->expPart.SizeVariation = 0;//Random::Float() * 2;
				this->expPart.Velocity.x = (Random::Float() - 0.5) * 0.1;
				this->expPart.Velocity.y = (Random::Float() - 0.5) * 0.1;
				this->expPart.Velocity.z = (Random::Float() - 0.5) * 0.1;
				this->expPart.VelocityVariation.x = 0;//Random::Float() * 2;
				this->expPart.VelocityVariation.y = 0;//Random::Float() * 2;
				this->expPart.VelocityVariation.z = 0;//Random::Float() * 2;
				this->expPart.weight = -4;
				this->expPart.ColorBegin[0] = 12;//Random::Float() * 256;
				this->expPart.ColorBegin[1] = 200;//Random::Float() * 256;
				this->expPart.ColorBegin[2] = 2;//Random::Float() * 256;
				this->expPart.ColorBegin[3] = 2;//Random::Float() * 256;
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




