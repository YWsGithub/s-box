#include "sbox.h"

uint8_t A_t1[8][8]= {
	1, 1, 1, 0, 0, 1, 0, 1,
	1, 1, 1, 1, 0, 0, 1, 0,
	0, 1, 1, 1, 1, 0, 0, 1,
	1, 0, 1, 1, 1, 1, 0, 0,
	0, 1, 0, 1, 1, 1, 1, 0,
    0, 0, 1, 0, 0, 1, 1, 1,
	1, 0, 0, 1, 0, 1, 1, 1,
	1, 1, 0, 0, 1, 0, 1, 1	  };

uint8_t A_t2[8][8]= {
	1, 0, 1, 0, 0, 1, 1, 1,
	1, 1, 0, 1, 0, 0, 1, 1,
	1, 1, 1, 0, 1, 0, 0, 1,
	1, 1, 1, 1, 0, 1, 0, 0,
	0, 1, 1, 1, 1, 0, 1, 0,
    0, 0, 1, 1, 1, 1, 0, 1,
	1, 0, 0, 1, 1, 1, 1, 0,
	0, 1, 0, 0, 1, 1, 1, 1	  };

uint8_t X_inv[8][8]= {
	1, 1, 1, 0, 0, 1, 1, 1,
	0, 1, 1, 1, 0, 0, 0, 1,
	0, 1, 1, 0, 0, 0, 1, 1,
	1, 1, 1, 0, 0, 0, 0, 1,
	1, 0, 0, 1, 1, 0, 1, 1,
	0, 0, 0, 0, 0, 0, 0, 1,
	0, 1, 1, 0, 0, 0, 0, 1,
	0, 1, 0, 0, 1, 1, 1, 1,
	  };

uint8_t X[8][8]= {
	0, 0, 0, 1, 0, 0, 1, 0,
	1, 1, 1, 0, 1, 0, 1, 1,
	1, 1, 1, 0, 1, 1, 0, 1,
	0, 1, 0, 0, 0, 0, 1, 0,
	0, 1, 1, 1, 1, 1, 1, 0,
    1, 0, 1, 1, 0, 0, 1, 0,
	0, 0, 1, 0, 0, 0, 1, 0,
	0, 0, 0, 0, 0, 1, 0, 0	  };

uint8_t D[8][8]= {
	1, 1, 0, 1, 0, 0, 1, 0,
	1, 0, 1, 0, 1, 1, 0, 0,
	1, 1, 0, 1, 1, 1, 1, 0,
	0, 1, 1, 1, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 1, 0, 0,
    1, 1, 1, 1, 1, 1, 0, 0,
	1, 1, 1, 0, 0, 0, 0, 0,
	1, 0, 1, 0, 0, 0, 0, 1	  };

uint8_t D_inv[8][8] = {
	1, 0, 1, 1, 0, 1, 0, 0,
	1, 1, 1, 0, 0, 0, 1, 0,
	0, 1, 0, 1, 0, 1, 0, 0,
	1, 0, 1, 0, 0, 1, 1, 0,
	1, 0, 1, 0, 1, 0, 0, 0,
	0, 0, 0, 0, 1, 0, 0, 0,
	0, 1, 1, 1, 0, 0, 0, 0,
	1, 1, 1, 0, 0, 0, 0, 1		   };

uint8_t M_affine[8][8] = {
	1, 1, 1, 1, 1, 0, 0, 0,
	0, 1, 1, 1, 1, 1, 0, 0,
	0, 0, 1, 1, 1, 1, 1, 0,
	0, 0, 0, 1, 1, 1, 1, 1,
	1, 0, 0, 0, 1, 1, 1, 1,
	1, 1, 0, 0, 0, 1, 1, 1,
	1, 1, 1, 0, 0, 0, 1, 1,
	1, 1, 1, 1, 0, 0, 0, 1		   	};

uint8_t M_D_inv[8][8] = {
	0, 0, 0, 0, 1, 1, 0, 0,
	1, 0, 1, 1, 0, 0, 0, 0,
	0, 0, 1, 0, 0, 0, 1, 0,
	1, 0, 0, 1, 0, 1, 1, 1,
	1, 0, 0, 0, 0, 1, 0, 1,
	1, 1, 0, 0, 1, 1, 1, 1,
	1, 0, 0, 1, 0, 0, 1, 1,
	0, 1, 0, 0, 0, 1, 0, 1		   	};

void m88_mul_m88(int i, int j, uint8_t matrix1[i][j], uint8_t matrix2[i][j]){

	int k, tmp;

	for(i = 0; i<8; i++){
		for(j = 0; j<8; j++){
			tmp = 0;
			for(k = 0; k<8; k++){
				tmp ^= matrix1[i][k] & matrix2[k][j];
			}
			printf("%d", tmp);
		}
		printf("\n");
	}
}

void cal(void){
	//m88_mul_m88(8, 8, D, D_inv);

	//m88_mul_m88(8, 8, M_affine, X);
	printf("\n");
	//m88_mul_m88(8, 8, M_D_inv, D);

	printf("A_t1 mul X :\n");
	m88_mul_m88(8, 8, A_t1, X);
	printf("\n");

	printf("A_t2 mul X :\n");
	m88_mul_m88(8, 8, A_t2, X);
	printf("\n");

}
