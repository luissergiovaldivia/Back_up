#include <p18f4520.h>
#include <p18cxxx.h>
#include <delays.h>



#pragma config OSC = RCIO6 // configura oscilador interno.
#pragma config MCLRE = ON // desabilita pin de reset
#pragma config WDT = OFF
#pragma config PWRT = ON
#pragma config STVREN = ON
#pragma config LVP = OFF

#define Fosc  3276800 /*4000000*/

#include <delays.h>

#define Led PORTDbits.RD0
#define Sube PORTDbits.RB0
#define Enter PORTDbits.RB2

void  DelayMs(unsigned char ms);
void Delay10Ms(unsigned char ms);
void Delay10us(unsigned char us);
void Display (unsigned int aux1,unsigned int aux2, unsigned int aux3, unsigned int aux4);
//void Display_mod (unsigned int aux1,unsigned int aux2, unsigned int aux3, unsigned int aux4);
void Display_mod (unsigned int digito1,unsigned int digito2, unsigned int digito3, unsigned int digito4);

//void Display_mod(void);
unsigned int bit_mod_hora, puntero_mod_hora, cambio_de_digito;




unsigned int cont = 0;
unsigned int milesimas = 0;
unsigned int prende = 0;
unsigned int apaga = 0;
unsigned int variable = 0;
unsigned int bit_mod_hora = 0;
unsigned int puntero_mod_hora = 0;
unsigned int cambio_de_digito = 0;

/*****************************************************************************************
**********      				Interrupcion											**********
*****************************************************************************************/


void high_interrupt (void); //declara una funcion para tratar la interrupcion

#pragma code high_vector = 0x08 // direccion de vector de interrupcion de alta prioridad

void interrupt_at_high_vector (void)
								{
									_asm GOTO high_interrupt _endasm
								}

#pragma code // retorno para direccion donde fue guardado el contexto del programa principal.

#pragma interrupt high_interrupt // define que una interrupcion sera de alta prioridad.

void high_interrupt (void)
						{

							if (INTCON3bits.INT1IF ==1) 
								{
					
							//INTCONbits.TMR0IE = 0;
							//Display_mod;
                       				INTCON3bits.INT1IF =0;	// limpia el flag de interrupciones.

								bit_mod_hora=1;
								cambio_de_digito++;
								
								}
							if (INTCONbits.TMR0IF ==1) 
								{
								//cont ++;
								
								milesimas ++;
                                                  //TMR0L = 255; // carga del timer inicial con un valor de 6
							INTCONbits.TMR0IF =0;	// limpia el flag de interrupciones.
								}
						}

void main (void)
				{

/**************************************************************************/
/**             Definir variables                                                                                **/
/***************************************************************************/

		unsigned int contador = 0;
		
		unsigned int segundo = 0;
		unsigned int minutos = 0;
		unsigned int Hora =0;
		unsigned int marca = 0;
		
	

		unsigned char a =0xFF;

		unsigned char cero =0b00111111;
		unsigned char uno =0b00000110;
		unsigned char dos =0b01011011;
		unsigned char tres =0b01001111;
		unsigned char cuatro =0b01100110;
		unsigned char cinco =0b01101101;
		unsigned char seis =0b11111100;
		unsigned char siete =0b00000111;
		unsigned char ocho =0b01111111;
		unsigned char nueve =0b01100111;

		const unsigned char DB[] = { 0x3f,0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7c, 0x07, 0x7f, 0x67};
		unsigned int j,i;
		unsigned int  aux1, aux2,aux3, aux4, primer_dig, seg_dig, tercer_dig, cuarto_dig;
		unsigned int digito1, digito2, digito3, digito4;

		unsigned char b =0;

	/***************************************************************
		CONFIGURACION DE LOS REGISTROS DEL MICROPROCESADOR
	****************************************************************/

OSCCONbits.IRCF1 = 1;  //configura oscilador interno de 4 Mhz.

INTCONbits.GIE = 1; // habilita interrupcion global
INTCONbits.PEIE = 0; // habilita interrupcion de perifericos
INTCONbits.TMR0IE = 1; // habilia interrupcion TMR0
INTCONbits.TMR0IF = 0; // limpia el flag de interrupciones
INTCONbits.INT0IE = 0; // habilita interrupci¨®n INT0 RB0
INTCON3bits.INT1IE = 1;  // habilita interrupci¨®n INT0 RB1
INTCON3bits.INT2IE = 0;  // habilita interrupci¨®n INT0 RB2
//ADCON0bits.ADON = 0;
//ANSELH= 0x00; 
ADCON0 = 0x00;
ADCON1 = 0x07;

/*
bit7 ;	1 = liga un timer
bit6 ;	1 = timer de 8 bit
bit5 ;	0 = oscilador interno
bit4 ;	0 = flanco de subida
bit3 ;	0 = habilita prescaler
bit2 = 0;       bit1 = 0 ;	  bit0 = 1 ; prescaler 1:4
*/


T0CON = 0b11000001;

TRISD = 0X00; // PORTD como salida
PORTD = 0X00; // limpia el contenido del PORTD
TRISC = 0X00;
TRISB = 0x07;
//bit_mod_hora=0;


/*************************************************************************
*************************************************************************/

TMR0L = 255; //  carga al timer inicia com valor 6



	
			while(1)
					{
						if ( milesimas >= 1000 ) 
										{
											INTCONbits.TMR0IF = 1;
										
											segundo ++;
											milesimas = 30;
											INTCONbits.TMR0IF = 0;
										}

						if (segundo >= 60 ) 
										{
											
											minutos ++;
										segundo = 0;
											
										}
							if (minutos>=60)
											{
												Hora ++;
												minutos=0;

											}
							if (Hora==24)
										{
											Hora = 0;
											
										}

					/*if ( PORTBbits.RB0 == 1)
									{
									bit_mod_hora=1;	
									}*/	



/******************************************************************************************************
********                          Resuelve digitos a visualizar                                                                             ********
******************************************************************************************************/

						primer_dig= minutos%10; //resuelve primer digito a visualizar
						aux1=DB[primer_dig];
						//aux1=nueve;
						seg_dig = minutos / 10; // resuelve segundo digito a visualizar
						aux2 = DB[seg_dig];
						tercer_dig= Hora%10; //resuelve primer digito a visualizar
						aux3=DB[tercer_dig];
						cuarto_dig = Hora / 10; // resuelve segundo digito a visualizar
						aux4 = DB[cuarto_dig];


             

											
/******************************************************************************************************
*******                              visualizacion                                                                                                      ****
*******************************************************************************************************/       
								//INTCONbits.TMR0IF = 0;
								
								Display (aux1, aux2, aux3,aux4); // llama la visualizacion, le entrega variable segundo y cero

								//INTCONbits.TMR0IF = 1;
						
/*******************************************************************************************************								
******************																				******* 
*******************************************************************************************************/                                                                                     
				
			//	if (bit_mod_hora ==1)
			prende = 1;
			while ( bit_mod_hora ==1)
									{
										//aux4 = uno;
									if ( cambio_de_digito ==1) 
									{

									if (prende == 1) 
													{
													digito1 = aux1;
													digito2 = aux2;
													digito3 = cero;
													digito4 = cero;
													

													}

									if (apaga ==1)
													{
													digito1 = 0x00;
													digito2 = 0x00;
													digito3 = cero;
													digito4 = cero;


													}
									}

									if ( cambio_de_digito ==2) 
									{

									if (prende == 1) 
													{
													digito2 = cero;
													digito1 = cero;
													digito3 = seis;
													digito4 = seis;
													

													}

									if (apaga ==1)
													{
													digito2 = cero;
													digito1 = cero;
													digito3 = 0x00;
													digito4 = 0x00;
													}
									}		
						/*	if ( cambio_de_digito ==3) 
									{

									if (prende == 1) 
													{
													digito3 = seis;
													digito2 = cero;
													digito1 = cero;
													digito4 = cero;
													

													}

									if (apaga ==1)
													{
													digito3 = 0x00;
													digito2 = cero;
													digito1 = cero;
													digito4 = cero;
													}
									}		
						if ( cambio_de_digito ==4) 
									{

									if (prende == 1) 
													{
													digito4 = seis;
													digito2 = cero;
													digito3 = cero;
													digito1 = cero;
													

													}

									if (apaga ==1)
													{
													digito4 = 0x00;
													digito2 = cero;
													digito3 = cero;
													digito1 = cero;
													}
									//cambio_de_digito = 0;
									}*/
								if (cambio_de_digito == 3)
													{
												cambio_de_digito = 1;		
													}
										
										
										
										Display_mod  (digito1, digito2, digito3,digito4);
										variable ++;	

							//	if (aux1 != 0x00)
								//		{
								//		prende = 0;
								if (variable == 10)
										{
									if ((prende == 1) && (apaga == 0) )
										{
										apaga = 1;
										prende = 0;
										variable = 0;
										}
									else {

									if ((prende == 0) && (apaga == 1) )
										{
										apaga = 0;
										prende = 1;
										variable = 0;
										} 
									}	
										}	
								//		}	

									}
								


					}
		}


	void  DelayMs(unsigned char ms)
							{
							//	Delay10KTCYx((((Fosc/3276,8)*ms)/1000)); 
							Delay10KTCYx((((Fosc/4000)*ms)/1000));
								//Delay10KTCYx(ms);
								}
								
								void Delay10Ms(unsigned char ms)
								{
								/*Delay1KTCYx((((Fosc/)/1000));*/
								
								Delay1KTCYx((((Fosc/4000)*ms)/1000));
								}

void Display (unsigned int aux1, unsigned int aux2,unsigned int aux3, unsigned int aux4)
       		{
					unsigned int j,i;
					//unsigned int  aux1, aux2, primer_dig, seg_dig;
					//const unsigned char DB[] = { 0x3f,0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7c, 0x07, 0x7f, 0x67};
						//for( j=1; j <= 20;j++)
								//{ 
						//	primer_dig= segundo%10; //resuelve primer digito a visualizar
						//	aux1=DB[primer_dig];

								//PORTDbits.RD1=0;
								//PORTD=0b00000000;
								PORTDbits.RD3=0;
								PORTC=aux1;            	//visualiza primer digito
								//PORTC=primer_dig;
								PORTDbits.RD0=1;

								/*PORTDbits.RD7=0;*/
								Delay10Ms(5);

								//seg_dig = segundo / 10; // resuelve segundo digito a visualizar
								//aux2 = DB[seg_dig];
								PORTDbits.RD0=0;
								//PORTD=0b00000000;
								PORTC=aux2;				// visualiza segundo digito
								PORTDbits.RD1=1;
								Delay10Ms(5);
					
								//PORTD=0b00000000;
								PORTDbits.RD1=0;
								PORTC=aux3;				// visualiza segundo digito
								PORTDbits.RD2=1;
								Delay10Ms(5);

								//PORTD=0b00000000;
								PORTDbits.RD2=0;
								PORTC=aux4;				// visualiza segundo digito
								PORTDbits.RD3=1;
								Delay10Ms(5);
							//	}

				}

void Display_mod(unsigned int aux1, unsigned int aux2,unsigned int aux3, unsigned int aux4)
			{
			
			unsigned int j,i;

			PORTDbits.RD3=0;
			PORTC=aux1;            	//visualiza primer digito
			PORTDbits.RD0=1;
			Delay10Ms(5);
			PORTDbits.RD0=0;
			PORTC=aux2;			//visualiza segundo digito

			PORTDbits.RD1=1;
			Delay10Ms(5);

			PORTDbits.RD1=0;
			PORTC=aux3;			// visualiza tercer digito
			PORTDbits.RD2=1;
			Delay10Ms(5);
				
			PORTDbits.RD2=0;
			PORTC=aux4;			// visualiza cuarto digito
			//PORTC=uno;
			PORTDbits.RD3=1;
			Delay10Ms(5);

			}


