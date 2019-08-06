/*************************************************************************************
 *   This program is built based on SM4 S-box algorithm :                            *
 *   Ref. Boolean Matrix Masking :   *                                       *
 *                                                                                   *
 ************************************************************************************/
#include "SM4sbox.h"

uint8_t M_A1_A2(uint8_t mask){
	int i, j, k;
	uint8_t A1 = 0xa7;
	uint8_t A2 = 0xcb;
	uint8_t flag1, flag2;

	int r = 0;
	uint8_t result[64];

	/*calculate (A1*A2)*/
	for(i = 0; i < 8; i++){
		for(j = 0; j < 8; j++){
			uint8_t mul = 0;
			uint8_t tmp = A1 & A2;

			for(k = 0; k < 8; k++){
				mul ^= (tmp & 1);
				tmp >>= 1;
			}
			result[r] = mul;
			//printf("%d ", result[r]);
			r++;

			flag2 = (A2 & 0x80) >> 7;
			A2 <<= 1;
			A2 |= flag2;

		}
		flag1 = (A1 & 0x80) >> 7;
		A1 <<= 1;
		A1 |= flag1;
		//printf("\n");
	}

	/*calculate (M*A1*A2)*/
	int M = mask;
	uint8_t MAA = 0;

	for(i=0; i<8; i++){
		uint8_t  tmpAA[8]={0}; /*make AA from horizontal to vertical*/
		uint8_t  tmpMAA, tmp;
		for(j=0; j<8; j++) tmpAA[i] |= (result[i + j*8] << j);

		tmpMAA = M & tmpAA[i];

		tmp = 0;
		for(j=0; j<8; j++){
			tmp ^= (tmpMAA & 1);
			tmpMAA >>= 1;
		}
		MAA |= tmp << i;
	}
	return MAA;
}

void mask(FILE *op){

	int i, j;

	fprintf(op, "\n__________________________________SM4 S-box with masking__________________________________\n\n");
  fprintf(op, "\t(sbox, maske_sbox, ref_out)\n");
	for(i=0;i<=0xf;i++)fprintf(op, "\t%x\t",i);
	fprintf(op, "\n");

	for(i=0;i<=0xf;i++){
		fprintf(op, "%x",i);
		for(j=0;j<=0xf;j++){

			int sbox = affine(0xa7,inverse(0x1f5,affine(0xa7, (i<<4)|j, 0xd3)), 0xd3);
			uint8_t mask = 1;

			int mask_input = ((i<<4)|j) ^ mask;
			int masked_out = affine(0xa7,inverse(0x1f5,affine(0xa7, mask_input, 0xd3)), 0xd3);
			int ref_out = sbox ^ M_A1_A2(mask);

			fprintf(op, "\t(%02x, %02x, %02x)", sbox, masked_out, ref_out);
		}
		fprintf(op, "\n");
	}
}

int main(void){

	int i, j;
	FILE *op;

	op = fopen("output", "w");
	if (op == NULL){
		printf("Cannot open output file\n");
		exit(0);
	}

	fprintf(op, "\n__________________________________SM4 S-box__________________________________\n\n");
	for(i=0;i<=0xf;i++)fprintf(op, "\t%x",i);
	fprintf(op, "\n");

	for(i=0;i<=0xf;i++){
		fprintf(op, "%x",i);
		for(j=0;j<=0xf;j++){
			fprintf(op, "\t%02x",affine(0xa7,inverse(0x1f5,affine(0xa7, (i<<4)|j, 0xd3)), 0xd3));
		}
		fprintf(op, "\n");
	}

	mask(op);

	fclose(op);

	return 0;
}
