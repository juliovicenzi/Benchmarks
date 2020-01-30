#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#ifdef opt
#include <arm_neon.h>
#endif


#define linha1 64
#define linha2 64
#define coluna1 64
#define coluna2 64
//compile with:
//arm-linux-gnueabi-gcc -static MM.c -O3 -mfloat-abi=softfp -mfpu=neon -march=armv7-a -mtune=cortex-a9 -o mm_opt

//for optimization to work, the resulting matrix must have at leat 4 lines and 4 columns
//best case: linha1 is multiple of 4

int main(){
	float matriz1[linha1][coluna1];
	float matriz2[linha2][coluna2];
	float matriz3[linha1][coluna2];
	int i,j,x,c;
#ifdef opt
	int32x4_t mat_a, temp_b, acc;
 
	int linhas_neon = linha1  - (linha1 % 4);	//número de linhas optimizadas
#endif
	//gera numeros aleatórios para cada campo da matriz1
	for(i=0; i<linha1; i++){
		for(j=0; j<coluna1; j++){
			matriz1[i][j] = rand() % linha1;		//aleatório entre 0 e ALTURA
		}
	}

	//gera numeros aleatórios para cada campo da matriz2
	for(i=0; i<linha2; i++){
		for(j=0; j<coluna2; j++){
			matriz2[i][j] = linha2 - (rand() % linha2);		//aleatório entre 0 e ALTURA
		}
	}


	//multiplicacao das matrizes
#ifndef opt
    int aux = 0;
	for(i=0; i<linha1; i++){
		for(j=0; j<coluna2; j++){
			matriz3[i][j]=0;
			for(x=0; x<linha2; x++){
				aux += matriz1[i][x] * matriz2[x][j];
			}
			matriz3[i][j] = aux;
			aux=0;
		}
	}
#endif

#ifdef opt
	for (i = 0; i < linhas_neon; i+= 4) {
		for (j = 0; j < coluna2; j++) {
			acc = vdupq_n_s32(0);	//sets acc to 0

			for(x = 0; x < linha2; x++){
				//loads 4 lines values to mat_a
				//this is due to the fact that we cant use vld1q to load 4 lines
				//of the same columm (they are not contiguos positions on memory)
				//also, I was going to use vsetq_lane_s32 but for some reason in
				//testing it kept setting everything to 0, so I chose this option instead
				for(c = 0; c < 4; c++)
					mat_a[c] = matriz1[i + c][x];
				temp_b = vdupq_n_s32(matriz2[x][j]);
				acc = vmlaq_s32(acc, mat_a, temp_b);	//multiplies and accumulates
			}

			for(c = 0; c < 4; c++){
				matriz3[i + c][j] = acc[c];		//stores the results
				//the same considerations for loading 4 lines are true here as well
				//when storing the 4 lines
			}
		}
	}

	//calculate the remaining lines that could not be pararlled
	//worst case: 3 lines
	if(linha1%4 != 0){
		int aux = 0;
		for(i=linhas_neon; i < linha1; i++){
			for(j =  0; j<coluna2; j++){
					for(x = 0; x<linha2; x++){
						aux += matriz1[i][x] * matriz2[x][j];
					}
					matriz3[i][j] = aux;
					aux = 0;
			}
		}
	}
#endif
	//printa a matriz 3
		for(i=0; i<linha1; i++){
			for(j=0; j<coluna2; j++){
				printf("%g\t", matriz3[i][j]);
			}
			printf("\n");
		}
		printf("\n\n");


	return 0;
}
