#include <math.h>
#include "weights.h"

float relu(float x)
{
    return (x > 0) ? x : 0;
}

void softmax(float *input, float *output)
{
    float sum = 0.0;

    for(int i=0;i<3;i++)
    {
        output[i] = exp(input[i]);
        sum += output[i];
    }

    for(int i=0;i<3;i++)
        output[i] /= sum;
}

int predict_rain(float input[6])
{
    float h1[12];
    float h2[8];
    float out_raw[3];
    float out[3];

    // Normalize
    for(int i=0;i<6;i++)
        input[i] = (input[i] - mean_vals[i]) / std_vals[i];

    // Layer 1
    for(int j=0;j<12;j++)
    {
        h1[j] = b1[j];
        for(int i=0;i<6;i++)
            h1[j] += input[i] * W1[i][j];

        h1[j] = relu(h1[j]);
    }

    // Layer 2
    for(int j=0;j<8;j++)
    {
        h2[j] = b2[j];
        for(int i=0;i<12;i++)
            h2[j] += h1[i] * W2[i][j];

        h2[j] = relu(h2[j]);
    }

    // Output layer
    for(int j=0;j<3;j++)
    {
        out_raw[j] = b3[j];
        for(int i=0;i<8;i++)
            out_raw[j] += h2[i] * W3[i][j];
    }

    softmax(out_raw, out);

    int result = 0;
    float max = out[0];

    for(int i=1;i<3;i++)
    {
        if(out[i] > max)
        {
            max = out[i];
            result = i;
        }
    }

    return result;
}
