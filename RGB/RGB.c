#include <stdio.h>
#include <stdlib.h>
#ifdef opt
#include <arm_neon.h>
#endif

#define linhas 320
#define colunas 240


int main(){
	int i, j, tmp=0;
	int r[linhas][colunas];
	int g[linhas][colunas];
	int b[linhas][colunas];
	int grayscale[linhas][colunas];
	#ifdef opt
	int32x4_t green, blue, red, aux;
	const int32x4_t const_red = vdupq_n_s32(77);
	const int32x4_t const_green = vdupq_n_s32(151);
	const int32x4_t const_blue = vdupq_n_s32(28);
	#endif 
	//gera numeros aleatórios para cada campo da matriz rgb
	for(i=0; i<linhas; i++){
		for(j=0; j<colunas; j++){
			r[i][j] = rand() % 256;		//aleatório entre 0 e 255
			g[i][j] = rand() % 256;		//aleatório entre 0 e 255
			b[i][j] = rand() % 256;		//aleatório entre 0 e 255
		}
	}

	//compara e pega o maior valor de cada indice entre matriz 1 e 2
	#ifdef opt
	for(i=0; i<linhas; i++){
		for(j=0; j<colunas - (colunas%4); j+=4){
			red = vld1q_s32(&r[i][j]);
			green = vld1q_s32(&g[i][j]);
			blue = vld1q_s32(&b[i][j]);
			aux = vmulq_s32(red, const_red);
			aux = vmlaq_s32(aux, green, const_green);
			aux = vmlaq_s32(aux, blue, const_blue);
			aux = vshrq_n_s32(aux, 8);
			vst1q_s32(&grayscale[i][j], aux);
		}
	}
	
	for(i=0; i<linhas - (linhas%4); i+=4){
		for(j= colunas - (colunas%4); j<colunas; j++){
			for(int c=0; c<4; c++){ //loads lines
				red[c] = r[i+c][j];
				green[c] = r[i+c][j];
				blue[c] = r[i+c][j];
			}
			aux = vmulq_s32(red, const_red);
			aux = vmlaq_s32(aux, green, const_green);
			aux = vmlaq_s32(aux, blue, const_blue);
			aux = vshrq_n_s32(aux, 8);
			for(int c=0; c<4; c++)
				grayscale[i+c][j] = aux[c];
		}
	}
	#else 
	for(i=0; i<linhas; i++){
		for(j=0; j<colunas; j++){
			tmp = (r[i][j] * 77) + (g[i][j] * 151) + (b[i][j] * 28);
			grayscale[i][j] = tmp >> 8;
		}
	}
	#endif

	for(i=0; i<linhas; i++){
		for(j= colunas - (colunas%4); j<colunas; j++){
			tmp = (r[i][j] * 77) + (g[i][j] * 151) + (b[i][j] * 28);
			grayscale[i][j] = tmp >> 8;
		}
	}
	
	//printa a grayscale
	for(i=0; i<linhas; i++){
		for(j=0; j<colunas; j++){
			printf("%d\t", grayscale[i][j]);
		}
		printf("\n");
	}
	printf("\n\n");


	return 0;
}
