#include <string.h>
void add_vectors(double *v1, double *v2, int size, double *result) {
    for(int i = 0; i < size; ++i)
        result[i] = v1[i] + v2[i];
}
void mul_vector_number(double *v1, double num, int size, double *result) {
    for(int i = 0; i < size; ++i)
        result[i] = v1[i] * num;
}
void score(double * input, double * output) {
    double var0[3];
    double var1[3];
    double var2[3];
    double var3[3];
    double var4[3];
    double var5[3];
    if (input[4] <= 65.36000061035156) {
        if (input[4] <= 64.36999893188477) {
            if (input[5] <= 143.49942016601562) {
                if (input[6] <= 40.5) {
                    memcpy(var5, (double[]){0.9633507853403142, 0.03664921465968586, 0.0}, 3 * sizeof(double));
                } else {
                    memcpy(var5, (double[]){1.0, 0.0, 0.0}, 3 * sizeof(double));
                }
            } else {
                memcpy(var5, (double[]){1.0, 0.0, 0.0}, 3 * sizeof(double));
            }
        } else {
            if (input[4] <= 64.94499969482422) {
                if (input[2] <= 193.13672637939453) {
                    memcpy(var5, (double[]){0.75, 0.25, 0.0}, 3 * sizeof(double));
                } else {
                    memcpy(var5, (double[]){0.2222222222222222, 0.7777777777777778, 0.0}, 3 * sizeof(double));
                }
            } else {
                memcpy(var5, (double[]){0.8461538461538461, 0.15384615384615385, 0.0}, 3 * sizeof(double));
            }
        }
    } else {
        if (input[3] <= 26.414999961853027) {
            if (input[5] <= 99.67000198364258) {
                if (input[4] <= 86.9749984741211) {
                    memcpy(var5, (double[]){0.0, 0.9705882352941176, 0.029411764705882353}, 3 * sizeof(double));
                } else {
                    memcpy(var5, (double[]){0.0, 0.6379310344827587, 0.3620689655172414}, 3 * sizeof(double));
                }
            } else {
                if (input[5] <= 115.29000091552734) {
                    memcpy(var5, (double[]){0.0, 0.2222222222222222, 0.7777777777777778}, 3 * sizeof(double));
                } else {
                    memcpy(var5, (double[]){0.0, 0.007210845111047015, 0.992789154888953}, 3 * sizeof(double));
                }
            }
        } else {
            if (input[5] <= 191.034423828125) {
                if (input[3] <= 33.11000061035156) {
                    memcpy(var5, (double[]){0.023809523809523808, 0.935672514619883, 0.04051796157059315}, 3 * sizeof(double));
                } else {
                    memcpy(var5, (double[]){0.9801980198019802, 0.019801980198019802, 0.0}, 3 * sizeof(double));
                }
            } else {
                memcpy(var5, (double[]){0.0, 0.0, 1.0}, 3 * sizeof(double));
            }
        }
    }
    double var6[3];
    if (input[5] <= 190.07442474365234) {
        if (input[6] <= 42.5) {
            if (input[3] <= 32.375) {
                if (input[5] <= 114.79000091552734) {
                    memcpy(var6, (double[]){0.04230857668429524, 0.9317084387215452, 0.025982984594159576}, 3 * sizeof(double));
                } else {
                    memcpy(var6, (double[]){0.15145813734713076, 0.30950141110065854, 0.5390404515522107}, 3 * sizeof(double));
                }
            } else {
                if (input[0] <= 27.649999618530273) {
                    memcpy(var6, (double[]){0.9117082533589251, 0.08829174664107485, 0.0}, 3 * sizeof(double));
                } else {
                    memcpy(var6, (double[]){1.0, 0.0, 0.0}, 3 * sizeof(double));
                }
            }
        } else {
            if (input[6] <= 44.5) {
                if (input[0] <= 8.394999980926514) {
                    memcpy(var6, (double[]){0.6, 0.4, 0.0}, 3 * sizeof(double));
                } else {
                    memcpy(var6, (double[]){0.7631067961165049, 0.23689320388349513, 0.0}, 3 * sizeof(double));
                }
            } else {
                if (input[4] <= 71.84725189208984) {
                    memcpy(var6, (double[]){0.9963527140098691, 0.0036472859901308732, 0.0}, 3 * sizeof(double));
                } else {
                    memcpy(var6, (double[]){0.0, 1.0, 0.0}, 3 * sizeof(double));
                }
            }
        }
    } else {
        memcpy(var6, (double[]){0.0, 0.0, 1.0}, 3 * sizeof(double));
    }
    add_vectors(var5, var6, 3, var4);
    double var7[3];
    if (input[4] <= 64.78000259399414) {
        if (input[3] <= 28.780000686645508) {
            memcpy(var7, (double[]){0.375, 0.625, 0.0}, 3 * sizeof(double));
        } else {
            if (input[2] <= 164.01499938964844) {
                if (input[4] <= 64.04500198364258) {
                    memcpy(var7, (double[]){1.0, 0.0, 0.0}, 3 * sizeof(double));
                } else {
                    memcpy(var7, (double[]){0.8, 0.2, 0.0}, 3 * sizeof(double));
                }
            } else {
                memcpy(var7, (double[]){1.0, 0.0, 0.0}, 3 * sizeof(double));
            }
        }
    } else {
        if (input[1] <= 40.89000129699707) {
            if (input[5] <= 119.5150032043457) {
                if (input[3] <= 25.03499984741211) {
                    memcpy(var7, (double[]){0.0, 0.04878048780487805, 0.9512195121951219}, 3 * sizeof(double));
                } else {
                    memcpy(var7, (double[]){0.025, 0.9603174603174603, 0.014682539682539682}, 3 * sizeof(double));
                }
            } else {
                if (input[2] <= 101.16500091552734) {
                    memcpy(var7, (double[]){0.007194244604316547, 0.04748201438848921, 0.9453237410071943}, 3 * sizeof(double));
                } else {
                    memcpy(var7, (double[]){0.019442984760903836, 0.08670520231213873, 0.8938518129269575}, 3 * sizeof(double));
                }
            }
        } else {
            if (input[3] <= 26.335000038146973) {
                if (input[6] <= 36.68169403076172) {
                    memcpy(var7, (double[]){0.0, 0.022564667033571822, 0.9774353329664281}, 3 * sizeof(double));
                } else {
                    memcpy(var7, (double[]){0.0, 0.9754098360655737, 0.02459016393442623}, 3 * sizeof(double));
                }
            } else {
                if (input[4] <= 90.17500305175781) {
                    memcpy(var7, (double[]){0.04862306368330465, 0.8493975903614458, 0.10197934595524957}, 3 * sizeof(double));
                } else {
                    memcpy(var7, (double[]){0.0, 0.2188612099644128, 0.7811387900355872}, 3 * sizeof(double));
                }
            }
        }
    }
    add_vectors(var4, var7, 3, var3);
    double var8[3];
    if (input[6] <= 31.5) {
        if (input[3] <= 29.579999923706055) {
            if (input[3] <= 27.454999923706055) {
                if (input[5] <= 77.375) {
                    memcpy(var8, (double[]){0.0, 0.7647058823529411, 0.23529411764705882}, 3 * sizeof(double));
                } else {
                    memcpy(var8, (double[]){0.0, 0.000503651473180559, 0.9994963485268195}, 3 * sizeof(double));
                }
            } else {
                if (input[4] <= 79.96000289916992) {
                    memcpy(var8, (double[]){0.0, 0.5952380952380952, 0.40476190476190477}, 3 * sizeof(double));
                } else {
                    memcpy(var8, (double[]){0.0, 0.020618556701030927, 0.979381443298969}, 3 * sizeof(double));
                }
            }
        } else {
            memcpy(var8, (double[]){0.0, 1.0, 0.0}, 3 * sizeof(double));
        }
    } else {
        if (input[0] <= 1.1950000524520874) {
            if (input[4] <= 64.79000091552734) {
                memcpy(var8, (double[]){1.0, 0.0, 0.0}, 3 * sizeof(double));
            } else {
                if (input[5] <= 89.63999938964844) {
                    memcpy(var8, (double[]){0.0, 1.0, 0.0}, 3 * sizeof(double));
                } else {
                    memcpy(var8, (double[]){0.0, 0.7777777777777778, 0.2222222222222222}, 3 * sizeof(double));
                }
            }
        } else {
            if (input[6] <= 42.5) {
                if (input[5] <= 134.35499572753906) {
                    memcpy(var8, (double[]){0.10649580203269995, 0.8809102960671675, 0.012593901900132567}, 3 * sizeof(double));
                } else {
                    memcpy(var8, (double[]){0.43491735537190085, 0.25413223140495866, 0.3109504132231405}, 3 * sizeof(double));
                }
            } else {
                if (input[4] <= 71.80224990844727) {
                    memcpy(var8, (double[]){0.9901286797109113, 0.009871320289088666, 0.0}, 3 * sizeof(double));
                } else {
                    memcpy(var8, (double[]){0.0, 1.0, 0.0}, 3 * sizeof(double));
                }
            }
        }
    }
    add_vectors(var3, var8, 3, var2);
    double var9[3];
    if (input[6] <= 31.5) {
        if (input[1] <= 35.55500030517578) {
            if (input[1] <= 35.42999839782715) {
                if (input[6] <= 29.5) {
                    memcpy(var9, (double[]){0.0, 0.0044444444444444444, 0.9955555555555555}, 3 * sizeof(double));
                } else {
                    memcpy(var9, (double[]){0.0, 0.48333333333333334, 0.5166666666666667}, 3 * sizeof(double));
                }
            } else {
                memcpy(var9, (double[]){0.0, 0.3333333333333333, 0.6666666666666666}, 3 * sizeof(double));
            }
        } else {
            if (input[3] <= 29.391164779663086) {
                if (input[4] <= 79.88999938964844) {
                    memcpy(var9, (double[]){0.0, 0.07777777777777778, 0.9222222222222223}, 3 * sizeof(double));
                } else {
                    memcpy(var9, (double[]){0.0, 0.002028397565922921, 0.9979716024340771}, 3 * sizeof(double));
                }
            } else {
                memcpy(var9, (double[]){0.0, 1.0, 0.0}, 3 * sizeof(double));
            }
        }
    } else {
        if (input[0] <= 6.275000095367432) {
            if (input[4] <= 65.62999725341797) {
                if (input[6] <= 37.18169403076172) {
                    memcpy(var9, (double[]){0.9782608695652174, 0.021739130434782608, 0.0}, 3 * sizeof(double));
                } else {
                    memcpy(var9, (double[]){1.0, 0.0, 0.0}, 3 * sizeof(double));
                }
            } else {
                if (input[4] <= 93.04999923706055) {
                    memcpy(var9, (double[]){0.036523929471032744, 0.9307304785894207, 0.0327455919395466}, 3 * sizeof(double));
                } else {
                    memcpy(var9, (double[]){0.0, 0.13793103448275862, 0.8620689655172413}, 3 * sizeof(double));
                }
            }
        } else {
            if (input[0] <= 6.434999942779541) {
                if (input[2] <= 133.0199966430664) {
                    memcpy(var9, (double[]){0.45454545454545453, 0.45454545454545453, 0.09090909090909091}, 3 * sizeof(double));
                } else {
                    memcpy(var9, (double[]){0.2571428571428571, 0.7142857142857143, 0.02857142857142857}, 3 * sizeof(double));
                }
            } else {
                if (input[6] <= 42.5) {
                    memcpy(var9, (double[]){0.1660951564509216, 0.7621088726960995, 0.07179597085297901}, 3 * sizeof(double));
                } else {
                    memcpy(var9, (double[]){0.9242303078768492, 0.07576969212315074, 0.0}, 3 * sizeof(double));
                }
            }
        }
    }
    add_vectors(var2, var9, 3, var1);
    mul_vector_number(var1, 0.2, 3, var0);
    memcpy(output, var0, 3 * sizeof(double));
}
