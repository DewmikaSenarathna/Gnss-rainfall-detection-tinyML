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
    if (input[3] <= 29.38116455078125) {
        if (input[4] <= 84.625) {
            if (input[4] <= 80.69499969482422) {
                if (input[0] <= 29.0) {
                    memcpy(var5, (double[]){0.016025641025641024, 0.8942307692307693, 0.08974358974358974}, 3 * sizeof(double));
                } else {
                    memcpy(var5, (double[]){0.013333333333333334, 0.8, 0.18666666666666668}, 3 * sizeof(double));
                }
            } else {
                if (input[4] <= 83.6150016784668) {
                    memcpy(var5, (double[]){0.0, 0.6773109243697479, 0.3226890756302521}, 3 * sizeof(double));
                } else {
                    memcpy(var5, (double[]){0.0, 0.5169811320754717, 0.4830188679245283}, 3 * sizeof(double));
                }
            }
        } else {
            if (input[5] <= 33.5) {
                if (input[3] <= 29.005000114440918) {
                    memcpy(var5, (double[]){0.0, 0.012033398821218075, 0.9879666011787819}, 3 * sizeof(double));
                } else {
                    memcpy(var5, (double[]){0.0, 0.9, 0.1}, 3 * sizeof(double));
                }
            } else {
                if (input[4] <= 93.25500106811523) {
                    memcpy(var5, (double[]){0.0, 0.8926974664679582, 0.10730253353204174}, 3 * sizeof(double));
                } else {
                    memcpy(var5, (double[]){0.0, 0.21935483870967742, 0.7806451612903226}, 3 * sizeof(double));
                }
            }
        }
    } else {
        if (input[0] <= 31.295000076293945) {
            if (input[4] <= 65.52999877929688) {
                if (input[4] <= 64.36999893188477) {
                    memcpy(var5, (double[]){0.9998394348105331, 0.00016056518946692356, 0.0}, 3 * sizeof(double));
                } else {
                    memcpy(var5, (double[]){0.75, 0.25, 0.0}, 3 * sizeof(double));
                }
            } else {
                if (input[5] <= 45.5) {
                    memcpy(var5, (double[]){0.03460837887067395, 0.9134790528233151, 0.05191256830601093}, 3 * sizeof(double));
                } else {
                    memcpy(var5, (double[]){0.8807947019867549, 0.11920529801324503, 0.0}, 3 * sizeof(double));
                }
            }
        } else {
            if (input[3] <= 31.594999313354492) {
                if (input[5] <= 44.5) {
                    memcpy(var5, (double[]){0.12328767123287672, 0.7945205479452055, 0.08219178082191782}, 3 * sizeof(double));
                } else {
                    memcpy(var5, (double[]){0.9310344827586207, 0.06896551724137931, 0.0}, 3 * sizeof(double));
                }
            } else {
                if (input[4] <= 58.454999923706055) {
                    memcpy(var5, (double[]){1.0, 0.0, 0.0}, 3 * sizeof(double));
                } else {
                    memcpy(var5, (double[]){0.7142857142857143, 0.2857142857142857, 0.0}, 3 * sizeof(double));
                }
            }
        }
    }
    double var6[3];
    if (input[5] <= 31.5) {
        if (input[4] <= 80.85499954223633) {
            if (input[4] <= 71.8072509765625) {
                if (input[4] <= 71.31725311279297) {
                    memcpy(var6, (double[]){0.0, 1.0, 0.0}, 3 * sizeof(double));
                } else {
                    memcpy(var6, (double[]){0.0, 0.008849557522123894, 0.9911504424778761}, 3 * sizeof(double));
                }
            } else {
                if (input[5] <= 27.5) {
                    memcpy(var6, (double[]){0.0, 0.0, 1.0}, 3 * sizeof(double));
                } else {
                    memcpy(var6, (double[]){0.0, 0.9454545454545454, 0.05454545454545454}, 3 * sizeof(double));
                }
            }
        } else {
            if (input[5] <= 29.5) {
                if (input[3] <= 28.440000534057617) {
                    memcpy(var6, (double[]){0.0, 0.0, 1.0}, 3 * sizeof(double));
                } else {
                    memcpy(var6, (double[]){0.0, 0.032467532467532464, 0.9675324675324676}, 3 * sizeof(double));
                }
            } else {
                if (input[3] <= 29.391164779663086) {
                    memcpy(var6, (double[]){0.0, 0.0763888888888889, 0.9236111111111112}, 3 * sizeof(double));
                } else {
                    memcpy(var6, (double[]){0.0, 1.0, 0.0}, 3 * sizeof(double));
                }
            }
        }
    } else {
        if (input[5] <= 42.5) {
            if (input[4] <= 63.60499954223633) {
                if (input[5] <= 36.68169403076172) {
                    memcpy(var6, (double[]){0.9844559585492227, 0.015544041450777202, 0.0}, 3 * sizeof(double));
                } else {
                    memcpy(var6, (double[]){1.0, 0.0, 0.0}, 3 * sizeof(double));
                }
            } else {
                if (input[5] <= 36.68169403076172) {
                    memcpy(var6, (double[]){0.0072254335260115606, 0.8328516377649325, 0.1599229287090559}, 3 * sizeof(double));
                } else {
                    memcpy(var6, (double[]){0.0106951871657754, 0.9858670741023682, 0.003437738731856379}, 3 * sizeof(double));
                }
            }
        } else {
            if (input[5] <= 44.5) {
                if (input[1] <= 56.829999923706055) {
                    memcpy(var6, (double[]){0.7819047619047619, 0.2180952380952381, 0.0}, 3 * sizeof(double));
                } else {
                    memcpy(var6, (double[]){0.5258064516129032, 0.47419354838709676, 0.0}, 3 * sizeof(double));
                }
            } else {
                if (input[3] <= 29.15499973297119) {
                    memcpy(var6, (double[]){0.09333333333333334, 0.9066666666666666, 0.0}, 3 * sizeof(double));
                } else {
                    memcpy(var6, (double[]){0.9876464323748669, 0.01235356762513312, 0.0}, 3 * sizeof(double));
                }
            }
        }
    }
    add_vectors(var5, var6, 3, var4);
    double var7[3];
    if (input[4] <= 64.78000259399414) {
        if (input[4] <= 63.71500015258789) {
            if (input[3] <= 28.94499969482422) {
                memcpy(var7, (double[]){0.7777777777777778, 0.2222222222222222, 0.0}, 3 * sizeof(double));
            } else {
                memcpy(var7, (double[]){1.0, 0.0, 0.0}, 3 * sizeof(double));
            }
        } else {
            if (input[5] <= 42.0) {
                memcpy(var7, (double[]){0.3333333333333333, 0.6666666666666666, 0.0}, 3 * sizeof(double));
            } else {
                memcpy(var7, (double[]){1.0, 0.0, 0.0}, 3 * sizeof(double));
            }
        }
    } else {
        if (input[5] <= 31.5) {
            if (input[3] <= 29.391164779663086) {
                if (input[4] <= 70.5822525024414) {
                    memcpy(var7, (double[]){0.0, 1.0, 0.0}, 3 * sizeof(double));
                } else {
                    memcpy(var7, (double[]){0.0, 0.013310058913375519, 0.9866899410866244}, 3 * sizeof(double));
                }
            } else {
                if (input[3] <= 29.670000076293945) {
                    memcpy(var7, (double[]){0.0, 0.7, 0.3}, 3 * sizeof(double));
                } else {
                    memcpy(var7, (double[]){0.0, 1.0, 0.0}, 3 * sizeof(double));
                }
            }
        } else {
            if (input[3] <= 25.079999923706055) {
                if (input[0] <= 30.49000072479248) {
                    memcpy(var7, (double[]){0.0, 0.009900990099009901, 0.9900990099009901}, 3 * sizeof(double));
                } else {
                    memcpy(var7, (double[]){0.0, 0.2, 0.8}, 3 * sizeof(double));
                }
            } else {
                if (input[5] <= 45.5) {
                    memcpy(var7, (double[]){0.015338042381432897, 0.9523713420787084, 0.032290615539858736}, 3 * sizeof(double));
                } else {
                    memcpy(var7, (double[]){0.8022598870056498, 0.1977401129943503, 0.0}, 3 * sizeof(double));
                }
            }
        }
    }
    add_vectors(var4, var7, 3, var3);
    double var8[3];
    if (input[5] <= 31.5) {
        if (input[3] <= 29.579999923706055) {
            if (input[4] <= 80.25) {
                if (input[4] <= 71.8072509765625) {
                    memcpy(var8, (double[]){0.0, 0.008064516129032258, 0.9919354838709677}, 3 * sizeof(double));
                } else {
                    memcpy(var8, (double[]){0.0, 0.5689655172413793, 0.43103448275862066}, 3 * sizeof(double));
                }
            } else {
                if (input[1] <= 19.449999809265137) {
                    memcpy(var8, (double[]){0.0, 0.016597510373443983, 0.983402489626556}, 3 * sizeof(double));
                } else {
                    memcpy(var8, (double[]){0.0, 0.0016185594820609657, 0.9983814405179391}, 3 * sizeof(double));
                }
            }
        } else {
            memcpy(var8, (double[]){0.0, 1.0, 0.0}, 3 * sizeof(double));
        }
    } else {
        if (input[5] <= 42.5) {
            if (input[4] <= 63.60499954223633) {
                if (input[3] <= 29.479999542236328) {
                    memcpy(var8, (double[]){0.9642857142857143, 0.03571428571428571, 0.0}, 3 * sizeof(double));
                } else {
                    memcpy(var8, (double[]){0.9988207547169812, 0.0011792452830188679, 0.0}, 3 * sizeof(double));
                }
            } else {
                if (input[3] <= 25.0649995803833) {
                    memcpy(var8, (double[]){0.0, 0.0, 1.0}, 3 * sizeof(double));
                } else {
                    memcpy(var8, (double[]){0.0076524870582939455, 0.9594868332207968, 0.032860679720909294}, 3 * sizeof(double));
                }
            }
        } else {
            if (input[3] <= 29.140000343322754) {
                if (input[3] <= 28.785000801086426) {
                    memcpy(var8, (double[]){0.01020408163265306, 0.9897959183673469, 0.0}, 3 * sizeof(double));
                } else {
                    memcpy(var8, (double[]){0.2830188679245283, 0.7169811320754716, 0.0}, 3 * sizeof(double));
                }
            } else {
                if (input[3] <= 31.3149995803833) {
                    memcpy(var8, (double[]){0.8898195876288659, 0.11018041237113402, 0.0}, 3 * sizeof(double));
                } else {
                    memcpy(var8, (double[]){0.992491787893008, 0.007508212106992022, 0.0}, 3 * sizeof(double));
                }
            }
        }
    }
    add_vectors(var3, var8, 3, var2);
    double var9[3];
    if (input[5] <= 31.5) {
        if (input[3] <= 29.391164779663086) {
            if (input[0] <= 1.0600000023841858) {
                memcpy(var9, (double[]){0.0, 0.2, 0.8}, 3 * sizeof(double));
            } else {
                if (input[5] <= 30.5) {
                    memcpy(var9, (double[]){0.0, 0.004624277456647399, 0.9953757225433526}, 3 * sizeof(double));
                } else {
                    memcpy(var9, (double[]){0.0, 0.26506024096385544, 0.7349397590361446}, 3 * sizeof(double));
                }
            }
        } else {
            if (input[3] <= 29.579999923706055) {
                memcpy(var9, (double[]){0.0, 0.9166666666666666, 0.08333333333333333}, 3 * sizeof(double));
            } else {
                memcpy(var9, (double[]){0.0, 1.0, 0.0}, 3 * sizeof(double));
            }
        }
    } else {
        if (input[4] <= 65.64999771118164) {
            if (input[2] <= 357.114990234375) {
                if (input[3] <= 28.765000343322754) {
                    memcpy(var9, (double[]){0.3, 0.7, 0.0}, 3 * sizeof(double));
                } else {
                    memcpy(var9, (double[]){0.9985897837668443, 0.0014102162331557505, 0.0}, 3 * sizeof(double));
                }
            } else {
                if (input[3] <= 30.210000038146973) {
                    memcpy(var9, (double[]){0.8571428571428571, 0.14285714285714285, 0.0}, 3 * sizeof(double));
                } else {
                    memcpy(var9, (double[]){1.0, 0.0, 0.0}, 3 * sizeof(double));
                }
            }
        } else {
            if (input[5] <= 46.5) {
                if (input[3] <= 25.02500057220459) {
                    memcpy(var9, (double[]){0.0, 0.004524886877828055, 0.995475113122172}, 3 * sizeof(double));
                } else {
                    memcpy(var9, (double[]){0.019580137262817925, 0.9461041582559547, 0.03431570448122729}, 3 * sizeof(double));
                }
            } else {
                if (input[4] <= 73.02725219726562) {
                    memcpy(var9, (double[]){0.9904761904761905, 0.009523809523809525, 0.0}, 3 * sizeof(double));
                } else {
                    memcpy(var9, (double[]){0.0, 1.0, 0.0}, 3 * sizeof(double));
                }
            }
        }
    }
    add_vectors(var2, var9, 3, var1);
    mul_vector_number(var1, 0.2, 3, var0);
    memcpy(output, var0, 3 * sizeof(double));
}
