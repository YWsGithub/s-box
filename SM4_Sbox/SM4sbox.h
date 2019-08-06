#ifndef _SBOX_TRIAL_H_
#define _SBOX_TRIAL_H_

#include <stdio.h>
#include <stdint.h>
#include <string.h>

int affine(int A1, int x, int C1);
int inverse(int a,int b);

int inverse_inv(int invb);
int affine_inv_AES(int x);

#endif
