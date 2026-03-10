#include "gnss_filter.h"

int valid_satellite(Satellite sat)
{
  if(sat.prn < 1 || sat.prn > 32)
    return 0;

  if(sat.elevation < 10 || sat.elevation > 70)
    return 0;

  if(sat.azimuth < 0 || sat.azimuth > 360)
    return 0;

  if(sat.snr > 60)
    return 0;

  return 1;
}

float get_max_snr(Satellite sats[], int count)
{
  float max_snr = 0;

  for(int i=0;i<count;i++)
  {
    if(valid_satellite(sats[i]))
    {
      if(sats[i].snr > max_snr)
        max_snr = sats[i].snr;
    }
  }

  return max_snr;
}