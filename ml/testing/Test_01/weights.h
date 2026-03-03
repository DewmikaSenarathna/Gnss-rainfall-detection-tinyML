#ifndef WEIGHTS_H
#define WEIGHTS_H

// Layer 1 
extern float W1[6][12];
extern float b1[12];

// Layer 2 
extern float W2[12][8];
extern float b2[8];

// Layer 3 
extern float W3[8][3];
extern float b3[3];

// Normalization 
extern float mean_vals[6];
extern float std_vals[6];

#endif
