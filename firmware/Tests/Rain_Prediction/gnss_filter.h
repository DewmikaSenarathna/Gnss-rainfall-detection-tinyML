#ifndef GNSS_FILTER_H
#define GNSS_FILTER_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
  int prn;
  float elevation;
  float azimuth;
  float snr;
} Satellite;

int valid_satellite(Satellite sat);
float get_max_snr(Satellite sats[], int count);

#ifdef __cplusplus
}
#endif

#endif