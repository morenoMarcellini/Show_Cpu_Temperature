//#include <stdlib.h>
#include <stdio.h>
#include <sys/param.h>

int read_temperature(char *text_string)
{
    FILE *fi;

    fi = fopen("/sys/class/thermal/thermal_zone0/temp", "r");

    if (fi == (FILE *)NULL)
    {
	fprintf(stderr, "I cannot open the thermal zone\n");
	return -1;
    }
    
    int temp;

    fscanf(fi, "%d", &temp);

    float temperature = ((float)temp) / 1000.f;

    snprintf(text_string, 5, "%.1f", MIN(temperature, 99.9));
    fclose(fi);
    return 0;
}
#if 0
int read_temperature(char *text_string, float *celsius)
{
    FILE *fi;

    fi = fopen("/sys/class/thermal/thermal_zone0/temp", "r");

    if (fi == (FILE *)NULL)
    {
	fprintf(stderr, "I cannot open the thermal zone\n");
	return 1;
    }
    int temp;

    fscanf(fi, "%d", &temp);

    float temperature = ((float)temp) / 1000.f;
	*celsius = MIN(temperature, 99.9);
    snprintf(text_string, 5, "%.1f", MIN(temperature, 99.9));
    fclose(fi);
    return 0;
}
#endif
