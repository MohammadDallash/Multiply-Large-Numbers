#include "stdio.h"
#include "tm4c123gh6pm.h"
#define BIC  &=~
#define RGB_R 0x2 
#define RGB_G 0x8 
#define RGB_B 0x4 



void SWOneIntialize()                                   //port F4
{
    SYSCTL_RCGCGPIO_R |= 0x20;                          //Activate the clock RCGCGPIO for pot F
    while ( (SYSCTL_PRGPIO_R & 0x20) == 0) {}              // wait for its status bit in PRGPIO
    GPIO_PORTF_LOCK_R = 0x4C4F434B;                     //unlocking port F
    GPIO_PORTF_CR_R |= 0x10;                             //enable change register
    GPIO_PORTF_AMSEL_R BIC 0x10;                         //Disable the analog function AMSEL_R
    GPIO_PORTF_AFSEL_R BIC 0x10;                         //Disable alternate function AFSEL_R
    GPIO_PORTF_PCTL_R BIC 0xF0000;                          //Disable function PCT and set to GPIO
    GPIO_PORTF_DEN_R |= 0x10;                            //Enable digital port DEN_R
    GPIO_PORTF_DIR_R BIC 0x10;                            //Set direction register DIR_R (0 for In)  
    GPIO_PORTF_PUR_R |= 0x10;                            // Enable pull up register because switches are negative logic by defult
}

void SWTwoIntialize()                                   //port F0
{
    SYSCTL_RCGCGPIO_R |= 0x20;                          //Activate the clock RCGCGPIO
    while ( (SYSCTL_PRGPIO_R & 0x20) == 0) {}             // wait for its status bit in PRGPIO
    GPIO_PORTF_LOCK_R = 0x4C4F434B;                     //unlocking port F
    GPIO_PORTF_CR_R |= 0x1;                             //enable change register
    GPIO_PORTF_AMSEL_R BIC 0x1;                         //Disable the analog function AMSEL_R
    GPIO_PORTF_AFSEL_R BIC 0x1;                         //Disable alternate function AFSEL_R
    GPIO_PORTF_PCTL_R BIC 0xF;                         //Disable function PCT and set to GPIO
    GPIO_PORTF_DEN_R |= 0x1;                            //Enable digital port DEN_R
    GPIO_PORTF_DIR_R BIC 0x1;                            //Set direction register DIR_R (0 for In)  
    GPIO_PORTF_PUR_R |= 0x1;                            // Enable pull up register because switches are negative logic by defult
}

void RGBIntialize()
{
    SYSCTL_RCGCGPIO_R |= 0x20;                         //Activate the clock RCGCGPIO
    while ( (SYSCTL_PRGPIO_R & 0x20) == 0) {}             // wait for its status bit in PRGPIO
    GPIO_PORTF_LOCK_R = 0x4C4F434B;                    //unlocking port F
    GPIO_PORTF_CR_R |= 0xE;                            //enable change register
    GPIO_PORTF_AMSEL_R BIC 0xE;                        //Disable the analog function AMSEL_R
    GPIO_PORTF_AFSEL_R BIC 0xE;                        //Disable alternate function AFSEL_R
    GPIO_PORTF_PCTL_R BIC 0xFFF0;                      //Disable function PCT and set to GPIO
    GPIO_PORTF_DEN_R |= 0xE;                           //Enable digital port DEN_R
    GPIO_PORTF_DIR_R |= 0xE;                           //Set direction register DIR_R (1 for out)
}

unsigned char SWOneIn()
{
    return !(GPIO_PORTF_DATA_R & 0x10);
}

unsigned char SWTwoIn()
{
    return !(GPIO_PORTF_DATA_R & 0x01);
}

void RGBOut(unsigned char data)
{    
    GPIO_PORTF_DATA_R BIC 0xE;
    GPIO_PORTF_DATA_R |= data;   
}

int labThree(void)
{
	while(1){	
			SWOneIntialize();
			SWTwoIntialize();
			RGBIntialize();

			if (SWOneIn() &&  SWTwoIn()) RGBOut(RGB_R);
			if (SWOneIn() &&  !(SWTwoIn())) RGBOut(RGB_B);
			if (!(SWOneIn()) &&  SWTwoIn()) RGBOut(RGB_G);
	}

}


void systick_initialize(void)
{
	NVIC_ST_CTRL_R = 0;
	NVIC_ST_RELOAD_R = 0XFFFFFF;
	NVIC_ST_CURRENT_R = 0;
	NVIC_ST_CTRL_R = 0x5;
}

void wait10ms(void)
{
	NVIC_ST_RELOAD_R = 160000-1;
	NVIC_ST_CURRENT_R = 0;
	while ( (NVIC_ST_CTRL_R & 0x10000) == 0){};
}



void sheet6 ()/*Write a C program to flash the RGB LED of TivaC for each color in order
red, blue then green, with delay 1 sec between each color, where red
color is represented by 0x02, blue color is represented by 0x04, and
green color is represented by 0x08. Assume the SysTick timer operates
on 80 MHZ.*/
{
	int color = RGB_B; int i;
	systick_initialize();
	RGBIntialize();
	while(1)
	{
		for (i=0; i<100; i++) wait10ms();
		RGBOut(((color*=2) % 7) *2);
	}
	
}



void lab4()
{
	int i;  int j;
	systick_initialize();
	RGBIntialize();
    

    RGBOut(RGB_B + RGB_G + RGB_R);
		
	for (i=0; i<6000; i++) wait10ms();
     
    for (i=0; i<3;i++)
    {
        RGBOut(0);			  
        for (j=0; j<100;j++) wait10ms();//wating a second
        RGBOut(RGB_B + RGB_G + RGB_R);
        for (j=0; j<100;j++) wait10ms();//wating a second
    }
    
}




void Uart0_int()
{
	SYSCTL_RCGCGPIO_R |= 0x01;                      //Activate the clock RCGCGPIO for pot A
	SYSCTL_RCGCUART_R |= 0X01; 											//Activate the clock RCGCGPIO for UART
	UART0_CTL_R  BIC  0x0001;                       // disable UART temporarily
	
	UART0_IBRD_R = 0x68;                             //int
	UART0_FBRD_R = 0x0B; 			                       //float
	UART0_LCRH_R = 0x0070;                           /* 8 bit word length (11 in bit 5 - 6) - enabel fifo (bit 4) all the remainig bits are zero and that means number of stop
                                                    bits is 1 and parity enable is flase */
	UART0_CTL_R  = 0x0301;                           // enable UART again and enable RXE and TXA(flags for FIFO)
	GPIO_PORTA_AFSEL_R |= 0x03;                      //enable alt functio for A0 and A1               // enable 
	GPIO_PORTA_PCTL_R = (GPIO_PORTA_PCTL_R & 0xFFFFFF00)+0x00000011; //Setup Uart for port A0 and port A1
	GPIO_PORTA_DEN_R |= 0x03; 											//enable digital function in A0 and A1
	GPIO_PORTA_AMSEL_R &= ~0x03; 										//disable analog function in A0 and A1
	
}


int can_we_send_data_UART0()
{
	return ((UART0_FR_R&0x20) == 0);
}


int there_is_data_available_to_be_received_UART0()
{
	return ((UART0_FR_R & 0x10) == 0)? 1:0; //return there is no data available when RXFE is 1
}

void transmit_one_byte_UART0(unsigned char byte) //sa7
{
	while (!can_we_send_data_UART0()) {}//while the transmiter FIFO is full just stay here
	UART0_DR_R = byte;
}


unsigned char receive_one_byte_UART0()
{
	while (!there_is_data_available_to_be_received_UART0()){}
	return ((char) UART0_DR_R & 0xFF);
	
}


unsigned char getCharCommand(){
	unsigned char c,d;

	c = receive_one_byte_UART0();
	transmit_one_byte_UART0(c);
	d = receive_one_byte_UART0();
	if(d == 0x0D) 
	{
		transmit_one_byte_UART0('\n');
		return c;
	};
	
}
void wait1s(int n)
{
	int i;
	for (i = 0; i < n *100; i++) wait10ms();

}
  

void int_interrupt_Portf(unsigned char portFn, int priority)
{

	
	GPIO_PORTF_IS_R BIC (1 << portFn); 														// Sensivity: Edge
	GPIO_PORTF_IBE_R BIC (1 << portFn);                          	// both edges: false
	GPIO_PORTF_IEV_R BIC (1 << portFn);                       			// Falling edge
	GPIO_PORTF_ICR_R |=(1 << portFn); //clear any prior interrupt 
			
			
	GPIO_PORTF_IM_R |= (1 << portFn);			// STEP 1: interrupt unmask (y3ne ab3at el interrupt request mn el port ll NVIC)	
	NVIC_EN0_R |= (1<<30);								// STEP 2: NVIC enable 	(y3ne ab3at el interrupt request mn el NVIC ll CPU)
	__enable_irq();												// STEP 3: enable the interrupts for the system
	
	NVIC_PRI7_R = (NVIC_PRI7_R & 0xFF00FFFF) | (priority<<21);    // PRI 7(30/4), segment 2(30%4)
}









void int_interrupt_sysTick()
{
	NVIC_ST_CTRL_R = 0;
	NVIC_ST_RELOAD_R = 0XFFFFFF;
	NVIC_ST_CURRENT_R = 0;
	NVIC_PRI13_R = (NVIC_PRI13_R & 0x00FFFFFF) | (1<29);    // PRI 7(30/4), segment 2(30%4)	
	NVIC_ST_CTRL_R = 0x7;   //the old value(101) was 5 and now we enable the bit 1 so the new value is (111 )
	__enable_irq();
}

unsigned char color = 0;
int time  = 0;


void SysTick_Handler( void )
{
	RGBOut(color);
	color ^= RGB_G;
	time++;
}


void GPIOF_Handler( void )
{
	if (GPIO_PORTF_MIS_R & 0x01)
	{
		GPIO_PORTF_ICR_R |= 0x01;
		RGBOut(0);
		NVIC_ST_CTRL_R=0x7;
	}

	if (GPIO_PORTF_MIS_R & 0x10)
	{
		GPIO_PORTF_ICR_R |= 0x10;	
		NVIC_ST_CTRL_R=0x00;
		RGBOut(RGB_R);
	}    
	
}



void main ()
{
		SWOneIntialize();   		
		RGBIntialize();
    int_interrupt_sysTick();
		int_interrupt_Portf( 4, 2);
		int_interrupt_Portf( 0, 2);
		while(1) {};	

}







void lab5 (void)
{
	unsigned char x;
	systick_initialize();
	Uart0_int();
	RGBIntialize();

	while (1)
	{
		x = getCharCommand();
		
		switch (x)
		{
		case 'A':
		wait1s(5);
		RGBOut(RGB_R);
			break;



		case 'B':
		wait1s(2);
		RGBOut(RGB_B);
			break;



		case 'C':
		wait1s(7);
		RGBOut(RGB_G);
			break;
		
		default:
			break;
		}
	}	
}






