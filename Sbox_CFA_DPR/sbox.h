#ifndef _SBOX_TRIAL_H_
#define _SBOX_TRIAL_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

int affine(int A1, int x, int C1); /* SM4 : A1 = 0xa7, C1 = 0Xd3, AES : A1 = 0xf1, C1 = 0x63*/
int inverse(int a,int b); /* SM4 : a = 0x1f5, AES : a = 0x11b*/

int multiplication(int a,int b);
void division(int a,int b,int* round,int* left);

void cal(void);

#endif
