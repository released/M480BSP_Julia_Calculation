/*_____ I N C L U D E S ____________________________________________________*/
#include <stdio.h>
#include "NuMicro.h"

#include "misc_config.h"
#include "Julia_Calculation.h"

/*_____ D E C L A R A T I O N S ____________________________________________*/

/*_____ D E F I N I T I O N S ______________________________________________*/

volatile uint32_t t0_counter_tick = 0;

const signed short int animation_zoom[ANIMATION_LENGHT] =
{
	120	, 110	, 100	, 150	, 200	, 275	, 350	, 450	,	600	, 800, 
	1000, 1200	, 1500	, 2000	, 1500	,1200	, 1000	, 800	, 600	, 450, 
	350, 275, 200,150, 100, 110
};

unsigned char buffer[SCREEN_X_SIZE * SCREEN_Y_SIZE];

unsigned char   animation_pointer = 0;
/*_____ M A C R O S ________________________________________________________*/

/*_____ F U N C T I O N S __________________________________________________*/

uint32_t get_t0_tick(void)
{
	return (t0_counter_tick);
}

void set_t0_tick(uint32_t t)
{
	t0_counter_tick = t;
}

void t0_tick_counter(void)
{
	t0_counter_tick++;
}

void TMR0_IRQHandler(void)
{	
    if(TIMER_GetIntFlag(TIMER0) == 1)
    {
        TIMER_ClearIntFlag(TIMER0);
		t0_tick_counter();
		
    }
}

void InitTIMER(void)
{
    TIMER_Open(TIMER0, TIMER_PERIODIC_MODE, 1000);
    TIMER_EnableInt(TIMER0);
    NVIC_EnableIRQ(TMR0_IRQn);	
    TIMER_Start(TIMER0);
}

/**
  * @brief  Resets timer counter and start counting
  * @param  None
  * @retval None
  */
void StartTIMER(void)
{
    set_t0_tick(0);
}

/**
  * @brief  Stops timer count and returns counter value
  * @param  None
  * @retval Counter Value
  */
unsigned int StopTIMER(void)
{
    return get_t0_tick();
}


void GenerateJulia_fpu(unsigned short int size_x, 
							unsigned short int size_y, 
							unsigned short int offset_x, 
							unsigned short int offset_y, 
							unsigned short int zoom, 
							unsigned char * buffer)
{
	float tmp1, tmp2;
	float num_real, num_img;
	float rayon;

	unsigned char i;
	unsigned short int x, y;

	// unsigned short int quotient = 0;
	// static unsigned char increasenum = 0;	
	// static unsigned short int counter = 0;

	for (y = 0; y < size_y; y++)
	{
		for (x = 0; x < size_x; x++)
		{
			num_real = y - offset_y;
			num_real = num_real / zoom;
			num_img = x - offset_x;
			num_img = num_img / zoom;
			i = 0;
			rayon = 0;
			while ((i < ITERATION - 1) && (rayon < 4))
			{
				tmp1 = num_real * num_real;
				tmp2 = num_img * num_img;
				num_img = 2 * num_real * num_img + IMG_CONSTANT;
				num_real = tmp1 - tmp2 + REAL_CONSTANT;
				rayon = tmp1 + tmp2;
				i++;
			}
			/* Store the value in the buffer */
			buffer[x+y*size_x] = i;
		}

        #if 0	//debug
        //log the process percent

        quotient = size_y/100;
        if (counter++ == quotient)
        {
            counter = 0;
            printf("%s : percent : %2d)\r\n",__FUNCTION__,increasenum++);	
        }
        #endif

  	}		

}

void Julia_Calculation_Preparation(void)
{
	InitTIMER();

	/* Clear Screen */
	reset_buffer(buffer,0x00,sizeof(buffer));

	printf("%s\r\n",__FUNCTION__);
}

void Julia_Calculation_Start(void)
{
	/* Benchmark result */
	unsigned int  score_fpu = 0;

	static unsigned int Cnt = 0;
	static unsigned char times = 0;

	/* Start generating the fractal */
	if (!times)
	{
	    /* Start the timer */
	    StartTIMER();

	    /* Make the calculation */
	    GenerateJulia_fpu(SCREEN_X_SIZE,
		              SCREEN_Y_SIZE,
		              SCREEN_X_SIZE / 2,
		              SCREEN_Y_SIZE / 2,
		              animation_zoom[animation_pointer],
		              buffer);

		/* Get elapsed time */
		score_fpu = StopTIMER();
	  	printf("%s:%5dms (%3d)\r\n",__FUNCTION__,score_fpu >> 2,Cnt++);

        if (animation_pointer++ == ANIMATION_LENGHT)
        {
			animation_pointer = 0;
            times = 1;
		}
	}
}


