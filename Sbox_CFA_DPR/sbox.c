#include "sbox.h"

int multiplication(int a,int b){
	 int tem=0;
	 int i=0;
	 while(b){
		 if(b&1){
			 tem^=a<<i;
		 }
		 i++;
		 b>>=1;
	 }
	 return tem;
}

int length(int x){
	 int i=0;
	 int comp=1;
	 while(1){
		 if(comp>=x) return i;

		 comp=(comp<<1)+1;
		 i++;
	 }
}

// do the polynomial division of a and b (a/b), then store Quotient q to *round, Remainder p to *left
void division(int a,int b,int* round,int* left){
	*round=0;
	*left=0;
	int distance;
	while(1){
		distance=length(a)-length(b);
		if(distance>=0 && a){
			a=a ^ (b<<distance);
			*round=(*round) | (1<<distance);
		}else{
			*left=a;
			break;
		}
	}
}

/* a is the irreducible polynomial, b is the input
 * SM4 : a = 0x1f5, AES : a = 0x11b				 */
int inverse(int a,int b){

	int w0 = 0;
	int w1 = 1;
	int q,r;

	while(b){
		 int w2;
		 division(a,b,&q,&r);

		 w2 = w0 ^ multiplication(q, w1);

		 a = b;
		 b = r;

		 w0 = w1;
		 w1 = w2;
	 }
	 return w0;
}

/* SM4 : A1 = 0xa7, C1 = 0xd3, AES : A1 = 0xf1, C1 = 0x63*/
int affine(int A1, int x, int C1){
	 int flag;
	 int result=0;
	 int tem;
	 int flag2;

	 for(int i=0;i<8;i++){
		 flag=(A1 & 0x80)>>7;
		 tem=x & A1;
		 flag2=0;
		 for(int j=0;j<8;j++){
			 flag2^=(tem & 1);
			 tem>>=1;
		 }
		 result=result | (flag2<<i);
		 A1=(A1<<1) | flag;
	 }
	 result ^= C1;

	 return result;
}
