/*************************************************************************************
 *   This program is built based on SM4 S-box algorithm :                            *
 *   The ref. is from : https://blog.csdn.net/qq_36291381/article/details/80156315   *                                       *
 *                                                                                   *
 ************************************************************************************/

#include "SM4sbox.h"

int AES_Sbox(int a,int b){
	return affine(0xf1,inverse(a,b),0x63);
}

int SM4ofAES_Sbox(int x){
	return affine(0xa7,inverse(0x1f5,affine(0xa7,inverse_inv(affine_inv_AES(x)),0xd3)), 0xd3);
}

int main(void){

	printf("\n__________________________________SM4 S-box__________________________________\n\n");
	for(int i=0;i<=0xf;i++)printf("\t%x",i);
	printf("\n");

	for(int i=0;i<=0xf;i++){
		printf("%x",i);
		for(int j=0;j<=0xf;j++){
			/*MI polynomial ref.(0x1f5) : Algebraic Cryptanalysis of SMS4: Gröbner Basis Attack and SAT Attack Compared (p.2)
			 * x^8+x^7+x^6+x^5+x^4+x^2+1 */
			printf("\t%02x",affine(0xa7,inverse(0x1f5,affine(0xa7, (i<<4)|j, 0xd3)), 0xd3));
		}
		printf("\n");
	}

	printf("\n__________________________________AES S-box__________________________________\n\n");
	for(int i=0;i<=0xf;i++)printf("\t%x",i);
	printf("\n");

	for(int i=0;i<=0xf;i++){
		printf("%x",i);
		for(int j=0;j<=0xf;j++){
			/*MI polynomial 0x11b : x^8+x^4+x+1 */
			printf("\t%02x",affine(0xf1,inverse(0x11b,(i<<4)|j), 0x63));
		}
		printf("\n");
	}

	printf("\n____________________Build SM4 Sbox finction of AES S-box____________________\n\n");
	for(int i=0;i<=0xf;i++)printf("\t%x",i);
	printf("\n");

	for(int i=0;i<=0xf;i++){
		printf("%x",i);
		for(int j=0;j<=0xf;j++){
			printf("\t%02x",SM4ofAES_Sbox(AES_Sbox(0x11B,(i<<4)|j)));
		}
		printf("\n");
	}

	return 0;
}
