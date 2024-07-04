/*_____ I N C L U D E S ____________________________________________________*/
#include <stdio.h>
#include "NuMicro.h"

/*_____ D E C L A R A T I O N S ____________________________________________*/

/*_____ D E F I N I T I O N S ______________________________________________*/
#define ITERATION      						((unsigned int)128)
#define	REAL_CONSTANT						(0.285f)
#define	IMG_CONSTANT						(0.01f)

#define SCREEN_X_SIZE               		((unsigned short int)320)
#define SCREEN_Y_SIZE               		((unsigned char)240)
#define ANIMATION_LENGHT            		((unsigned int)26)


/*_____ M A C R O S ________________________________________________________*/

/*_____ F U N C T I O N S __________________________________________________*/
void Julia_Calculation_Preparation(void);
void Julia_Calculation_Start(void);
