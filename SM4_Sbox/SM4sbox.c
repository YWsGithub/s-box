#include "SM4sbox.h"

/*q, r, w are used to record the result of inverse for doing inverse_inv*/
int k;
int q_inv[16]={0};
int r_inv[16]={0};
int w_inv[16]={0};

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

int inverse(int a,int b){

	int wtmp0 = 0;
	int wtmp1 = 1;
	int qtmp,rtmp;

	k = 0;
	while(b){
		 int wtmp2;
		 division(a,b,&qtmp,&rtmp);

		 q_inv[k+1] = qtmp;
		 r_inv[k+1] = rtmp;

		 wtmp2 = wtmp0 ^ multiplication(qtmp, wtmp1);
		 w_inv[k+1] = wtmp2;

		 a = b;
		 b = rtmp;

		 wtmp0 = wtmp1;
		 wtmp1 = wtmp2;
		 k++;
	 }
	 return wtmp0;
}

/* SM4 : A1 = 0xa7, C1 = 0Xd3
 * AES : A1 = 0xf1, C1 = 0x63*/
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

int inverse_inv(int invb){

	int n;
	int b;
	int wq,wr;

	if(k == 0) b = 0;
	else if(k == 1) b = 1;
	else if(k == 2){
		n=k;
		division((0x11b^r_inv[1]), w_inv[1], &wq, &wr);
		b = wq;
	}
	else {
		n=k;
		division((w_inv[n-3]^invb), w_inv[n-2], &wq, &wr);
		b = multiplication(r_inv[n-2], wq) ^ r_inv[n-1];

		for(; n > 3 ; n--){
			b = multiplication(b, q_inv[n-2]) ^ r_inv[n-2];
		}
	}

	return b;
}

int affine_inv_AES(int x){

	/*0xa4 is found out by exhaustive method, of course it was PC who is exhausted*/
	 int A1 = 0xa4;

	 int flag;
	 int result=0;
	 int tem;
	 int flag2;

	 x ^= 0x63;

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

	 return result;
 }
