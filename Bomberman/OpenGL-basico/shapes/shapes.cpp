#include "shapes.h"


//    v4----- v7
//   /|      /|
//  v3------v2|
//  | |     | |
//  | |v5---|-|v6
//  |/      |/
//  v0------v1
retangulo3dss createRetangulo3d(vertice vertices[8]) 
{
	return {
		{ /*Cordenadas*/
			vertices[0].cordenadas[0], vertices[0].cordenadas[1], vertices[0].cordenadas[2],
			vertices[1].cordenadas[0], vertices[1].cordenadas[1], vertices[1].cordenadas[2],
			vertices[2].cordenadas[0], vertices[2].cordenadas[1], vertices[2].cordenadas[2],
			vertices[3].cordenadas[0], vertices[3].cordenadas[1], vertices[3].cordenadas[2], //Cara sur



			vertices[3].cordenadas[0], vertices[3].cordenadas[1], vertices[3].cordenadas[2],
			vertices[4].cordenadas[0], vertices[4].cordenadas[1], vertices[4].cordenadas[2],
			vertices[5].cordenadas[0], vertices[5].cordenadas[1], vertices[5].cordenadas[2],
			vertices[0].cordenadas[0], vertices[0].cordenadas[1], vertices[0].cordenadas[2], //Cara oeste



			vertices[3].cordenadas[0], vertices[3].cordenadas[1], vertices[3].cordenadas[2],
			vertices[2].cordenadas[0], vertices[2].cordenadas[1], vertices[2].cordenadas[2],
			vertices[7].cordenadas[0], vertices[7].cordenadas[1], vertices[7].cordenadas[2],
			vertices[4].cordenadas[0], vertices[4].cordenadas[1], vertices[4].cordenadas[2], //Cara superior



			vertices[5].cordenadas[0], vertices[5].cordenadas[1], vertices[5].cordenadas[2],
			vertices[4].cordenadas[0], vertices[4].cordenadas[1], vertices[4].cordenadas[2],
			vertices[7].cordenadas[0], vertices[7].cordenadas[1], vertices[7].cordenadas[2],
			vertices[6].cordenadas[0], vertices[6].cordenadas[1], vertices[6].cordenadas[2], //Cara norte



			vertices[1].cordenadas[0], vertices[1].cordenadas[1], vertices[1].cordenadas[2],
			vertices[6].cordenadas[0], vertices[6].cordenadas[1], vertices[6].cordenadas[2],
			vertices[7].cordenadas[0], vertices[7].cordenadas[1], vertices[7].cordenadas[2],
			vertices[2].cordenadas[0], vertices[2].cordenadas[1], vertices[2].cordenadas[2], //Cara este



			vertices[0].cordenadas[0], vertices[0].cordenadas[1], vertices[0].cordenadas[2],
			vertices[5].cordenadas[0], vertices[5].cordenadas[1], vertices[5].cordenadas[2],
			vertices[6].cordenadas[0], vertices[6].cordenadas[1], vertices[6].cordenadas[2],
			vertices[1].cordenadas[0], vertices[1].cordenadas[1], vertices[1].cordenadas[2], //Cara inferior
		},
		{  /*Normales*/
			0	,-1	,0,
			-1	,0	,0,
			0	,0	,1,
			0	,1	,0,
			1	,0	,0,
			0	,0	,-1
		},
		{  /*Colores*/
			vertices[0].colores[0], vertices[0].colores[1], vertices[0].colores[2],
			vertices[1].colores[0], vertices[1].colores[1], vertices[1].colores[2],
			vertices[2].colores[0], vertices[2].colores[1], vertices[2].colores[2],
			vertices[3].colores[0], vertices[3].colores[1], vertices[3].colores[2], //Cara sur



			vertices[3].colores[0], vertices[3].colores[1], vertices[3].colores[2],
			vertices[4].colores[0], vertices[4].colores[1], vertices[4].colores[2],
			vertices[5].colores[0], vertices[5].colores[1], vertices[5].colores[2],
			vertices[0].colores[0], vertices[0].colores[1], vertices[0].colores[2], //Cara oeste



			vertices[3].colores[0], vertices[3].colores[1], vertices[3].colores[2],
			vertices[2].colores[0], vertices[2].colores[1], vertices[2].colores[2],
			vertices[7].colores[0], vertices[7].colores[1], vertices[7].colores[2],
			vertices[4].colores[0], vertices[4].colores[1], vertices[4].colores[2], //Cara superior



			vertices[5].colores[0], vertices[5].colores[1], vertices[5].colores[2],
			vertices[4].colores[0], vertices[4].colores[1], vertices[4].colores[2],
			vertices[7].colores[0], vertices[7].colores[1], vertices[7].colores[2],
			vertices[3].colores[0], vertices[3].colores[1], vertices[3].colores[2], //Cara norte



			vertices[1].colores[0], vertices[1].colores[1], vertices[1].colores[2],
			vertices[6].colores[0], vertices[6].colores[1], vertices[6].colores[2],
			vertices[7].colores[0], vertices[7].colores[1], vertices[7].colores[2],
			vertices[2].colores[0], vertices[2].colores[1], vertices[2].colores[2], //Cara este



			vertices[0].colores[0], vertices[0].colores[1], vertices[0].colores[2],
			vertices[5].colores[0], vertices[5].colores[1], vertices[5].colores[2],
			vertices[6].colores[0], vertices[6].colores[1], vertices[6].colores[2],
			vertices[1].colores[0], vertices[1].colores[1], vertices[1].colores[2], //Cara inferior
		},
		{	/*Indices*/
			0,1,2,3,
			4,5,6,7,
			8,9,10,11,
			12,13,14,15,
			16,17,18,19,
			20,21,22,23
		},
		 //texturas
		{
			0.0,0.0,
			0.0,1.0,
			1.0,1.0,
			1.0,0.0,

			0.0,0.0,
			0.0,1.0,
			1.0,1.0,
			1.0,0.0,

			0.0,0.0,
			0.0,1.0,
			1.0,1.0,
			1.0,0.0,

			0.0,0.0,
			0.0,1.0,
			1.0,1.0,
			1.0,0.0,

			0.0,0.0,
			0.0,1.0,
			1.0,1.0,
			1.0,0.0,

			0.0,0.0,
			0.0,1.0,
			1.0,1.0,
			1.0,0.0,

		}

	};
}