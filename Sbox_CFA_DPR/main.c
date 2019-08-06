/*************************************************************************************
 *   This program is built based on SM4 S-box algorithm :                            *
 *   The ref. is from : https://blog.csdn.net/qq_36291381/article/details/80156315   *                                       *
 *                                                                                   *
 ************************************************************************************/

#include "sbox.h"

/*ref : DPR Based AES/SM4 Encryption Highly Efficient Implementation*/
/*take values in delta horizontally*/
uint8_t delta[8] = {
		0xd2,
		0xac,
		0xde,
		0x70,
		0x04,
		0xfc,
		0xe0,
		0xa1		};

/*take values in delta_inverse vertically*/
uint8_t delta_inv[8] = {
		0xd9, 0x63, 0xdb, 0xa2, 0x0c, 0xb0, 0x50, 0x01};

uint8_t T[8] = {
		0x7c,
		0x8e,
		0xac,
		0x2c,
		0xe2,
		0x74,
		0x66,
		0x63   };

/*take values in delta_inverse vertically*/
uint8_t T_inv[8] = {
		0x3a, 0x52, 0xa5, 0xc5, 0x77, 0x2a, 0x26, 0x01};

/*the matrix has to be in the form of 2 hexagonal numbers*/
int matrix88_mul_vector8(uint8_t* matrix, int X){
	int i, j, tmp1, tmp2, result;

	result = 0;
	for(i=0;i<8;i++){
		tmp1 = matrix[i] & X;
		tmp2 = 0;
		for(j=0;j<8;j++){
			tmp2 ^= (tmp1 & 1);
			tmp1 >>= 1;
		}
		//printf("%d", tmp2);
		//printf("\n");
		result |= (tmp2 << (7-i));
	}
	return result;
}

uint8_t * matrix88_mul_matrix88(int M, uint8_t * matrix){//(int M, uint8_t* matrix){

	int i, j, k, flag, tmp1, tmp2;
	uint8_t* result = malloc(8*sizeof(uint8_t));

	for(i=0; i<8; i++){
		//printf("result %d \n", i+1);
		flag=(M & 0x80)>>7;
		for(j=0; j<8; j++){
			tmp1 = M & matrix[j];
			//printf("0x%02x ", tmp1);
			tmp2 = 0;
			for(k=0; k<8; k++){
				tmp2 ^= (tmp1 & 1);
				tmp1 >>= 1;
			}
			//printf("%d ", tmp2);
			//printf("\n");
			result[i] |= tmp2 << (7-j);
		}
		M=(M<<1) | flag;
		//printf("\n");
	}
	return result;
}

int mul_CF(int a, int b){
	int a3 = (a & 0x08) >> 3;
	int a2 = (a & 0x04) >> 2;
	int a1 = (a & 0x02) >> 1;
	int a0 = a & 0x01;

	int b3 = (b & 0x08) >> 3;
	int b2 = (b & 0x04) >> 2;
	int b1 = (b & 0x02) >> 1;
	int b0 = b & 0x01;

	int c3, c2, c1, c0;
	c3 = ((a3&b1) ^ (a1&b3)) ^ ((a3&b0) ^ (a0&b3)) ^ ((a2&b1) ^ (a1&b2)) ^ (a2&b2);
	c2 = ((a3&b1) ^ (a1&b3)) ^ ((a2&b0) ^ (a0&b2)) ^ (a2&b2) ^ (a1&b1);
	c1 = ((a3&b1) ^ (a1&b3)) ^ ((a1&b0) ^ (a0&b1)) ^ (a3&b3) ^ (a2&b2);
	c0 = ((a3&b2) ^ (a2&b3)) ^ ((a3&b1) ^ (a1&b3)) ^ (a2&b2) ^ (a0&b0);

	int c = (c3 << 3) | (c2 << 2) | (c1 << 1) | c0;;
	return c;
}

int inverse_CF(int X){
	int Ah, Al, rsl1, rsl2, rsl3, rsl4, rsl5, rsl6, rsl7;

	Ah = (X & 0xf0) >> 4;
	Al = X & 0x0f;
	//printf("Ah = %02x, Al = %02x", Ah, Al);

	rsl1 = Ah ^ Al;

	/*
	int q,r;
	division(0x1f,multiplication(rsl1, Al),&q,&r);
	rsl2 = r;
	*/
	rsl2 = mul_CF(rsl1,Al);

	rsl3 = (Ah & 0x02) << 2;	// 3rd bit of ()^2xvI
	rsl3 |= (Ah & 0x08) >> 1;	// 2nd
	rsl3 |= (Ah & 0x01) << 1;	// 1st
	rsl3 |= (Ah & 0x04) >> 2;	// 0th

	rsl4 = rsl2 ^ rsl3;

	int q0, q1, q2, q3;
	q3 = (rsl4 & 0x08) >> 3;
	q2 = (rsl4 & 0x04) >> 2;
	q1 = (rsl4 & 0x02) >> 1;
	q0 = rsl4 & 0x01;

	int rsl50, rsl51, rsl52, rsl53;
	rsl53 = (q3 & q2 & q0) ^ (q3 & q1 & q0) ^ (q3 & q1) ^ (q2 & q0) ^ q2 ^ q1;
	rsl52 = (q3 & q2 & q0) ^ (q2 & q1 & q0) ^ (q2 & q0) ^ (q1 & q0) ^ q3 ^ q1;
	rsl51 = (q3 & q2 & q1) ^ (q3 & q1 & q0) ^ (q2 & q1 & q0) ^ (q3 & q0) ^ (q2 & q1) ^ (q2 & q0) ^ q1;
	rsl50 = (q3 & q2 & q1) ^ (q3 & q2 & q0) ^ (q3 & q2) ^ (q2 & q0) ^ q1 ^ q0;

	rsl5 = (rsl53 << 3) | (rsl52 << 2) | (rsl51 << 1) | rsl50;

	/*
	division(0x1f, multiplication(Ah, rsl5), &q, &r);
	rsl6 = r;*/
	rsl6 = mul_CF(Ah, rsl5);

	/*division(0x1f, multiplication(rsl5, rsl1), &q, &r);
	rsl7 = r;*/
	rsl7 = mul_CF(rsl5, rsl1);

	return ((rsl6 << 4) | rsl7);
}

int main(int argc, char* argv[]){

	int i, j;

	printf("\n__________________________________SM4 S-box__________________________________\n\n");
	for(j=0;j<=0xf;j++)printf("\t%x",j);
	printf("\n");

	for(i=0;i<=0xf;i++){
		printf("%x",i);
		for(j=0;j<=0xf;j++){
			printf("\t%02x",affine(0xa7,inverse(0x1f5,affine(0xa7, (i<<4)|j, 0xd3)), 0xd3));
		}
		printf("\n");
	}

	printf("\n__________________________________AES S-box__________________________________\n\n");
	for(j=0;j<=0xf;j++)printf("\t%x",j);
	printf("\n");

	for(i=0;i<=0xf;i++){
		printf("%x",i);
		for(j=0;j<=0xf;j++){
			printf("\t%02x",affine(0xf1,inverse(0x11b,(i<<4)|j), 0x63));
		}
		printf("\n");
	}

	printf("\n__________________________________AES S-box through CFA__________________________________\n\n");
	uint8_t *Md;  // M : affine matrix, d : delta_inverse
	Md = matrix88_mul_matrix88(0xf1, delta_inv);
	//for(i=0; i<8; i++) printf("0x%02x ", Md[i]);

	for(j=0;j<=0xf;j++)printf("\t%x",j);
	printf("\n");

	for(i=0;i<=0xf;i++){
		printf("%x",i);
		for(j=0;j<=0xf;j++){
			printf("\t%02x", matrix88_mul_vector8(Md, inverse_CF(matrix88_mul_vector8(delta, (i<<4)|j))) ^0x63);
		}
		printf("\n");
	}

	printf("\n__________________________________SM4 S-box through CFA__________________________________\n\n");
	uint8_t *AT;
	AT = matrix88_mul_matrix88(0xa7, T_inv);
	for(j=0;j<=0xf;j++) printf("\t%x",j);
	printf("\n");

	for(i=0;i<=0xf;i++){
		printf("%x",i);
		for(j=0;j<=0xf;j++){
			int s1 = affine(0xa7, (i<<4)|j, 0xd3);
			int s2 = matrix88_mul_vector8(T, s1);
			printf("\t%02x", matrix88_mul_vector8(AT, inverse_CF(s2)) ^ 0xd3);
		}
		printf("\n");
	}

	//cal();

	return 0;
}
