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



//#define Led PORTDbits.RD0
#define Sube PORTBbits.RB0
#define Enter PORTBbits.RB2

void  DelayMs(unsigned char ms);
void Delay10Ms(unsigned char ms);
void Delay10us(unsigned char us);
void Display (unsigned int aux1,unsigned int aux2, unsigned int aux3, unsigned int aux4);

void Display_mod (unsigned int digito1,unsigned int digito2, unsigned int digito3, unsigned int digito4);
void Resuelve_digito ( void);


unsigned int bit_mod_hora, puntero_mod_hora, cambio_de_digito;
unsigned int  aux1, aux2,aux3, aux4, primer_dig, seg_dig, tercer_dig, cuarto_dig;

const unsigned char DB[] = { 0x3f,0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7c, 0x07, 0x7f, 0x67};




unsigned int cont = 0;
unsigned int milesimas = 0;
unsigned int prende = 0;
unsigned int apaga = 0;
unsigned int variable = 0;
unsigned int bit_mod_hora = 0;
unsigned int puntero_mod_hora = 0;
unsigned int cambio_de_digito = 0;
unsigned int digito_a_modificar1 = 0;
unsigned int digito_a_modificar2 = 0;
unsigned int transferir =0;
unsigned int numero_de_pulsos = 0;
	unsigned int segundo = 0;
		unsigned int minutos = 0;
		unsigned int Hora =0;
		unsigned int marca = 0;
int M0_0 = 0;

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
					
							
                       				INTCON3bits.INT1IF =0;	// limpia el flag de interrupciones.

								bit_mod_hora=1;
								cambio_de_digito++;
								
								}
							if (INTCONbits.TMR0IF ==1) 
								{
							
								
								milesimas ++;
                                                 
							INTCONbits.TMR0IF =0;	// limpia el flag de interrupciones.
								}
						}

void main (void)
				{
					

/**************************************************************************/
/**             Definir variables                                                                                **/
/***************************************************************************/

		unsigned int contador = 0;
		
	/*	unsigned int segundo = 0;
		unsigned int minutos = 0;
		unsigned int Hora =0;
		unsigned int marca = 0; */
		
	

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

	//	const unsigned char DB[] = { 0x3f,0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7c, 0x07, 0x7f, 0x67};
		unsigned int j,i;
		//unsigned int  aux1, aux2,aux3, aux4, primer_dig, seg_dig, tercer_dig, cuarto_dig;
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
/*INTCON3bits.INT2IE = 0;  // habilita interrupci¨®n INT0 RB2*/

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




/*************************************************************************
*************************************************************************/

TMR0L = 255; //  carga al timer inicia com valor 6
PORTBbits.RB7 = 1;


	
			while(1)
					{
							//PORTBbits.RB7 = 1;
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
	



/******************************************************************************************************
********                          Resuelve digitos a visualizar                                                                             ********
******************************************************************************************************/
						
						Resuelve_digito () ;
					/*	primer_dig= minutos%10; //resuelve primer digito a visualizar
						aux1=DB[primer_dig];
						//aux1=nueve;
						seg_dig = minutos / 10; // resuelve segundo digito a visualizar
						aux2 = DB[seg_dig];
						tercer_dig= Hora%10; //resuelve primer digito a visualizar
						aux3=DB[tercer_dig];
						cuarto_dig = Hora / 10; // resuelve segundo digito a visualizar
						aux4 = DB[cuarto_dig];*/


             

											
/******************************************************************************************************
*******                              visualizacion                                                                                                      ****
*******************************************************************************************************/       
							
								
								Display (aux1, aux2, aux3,aux4); // llama la visualizacion, le entrega variable segundo y cero

								
						
/*******************************************************************************************************								
******************																				******* 
*******************************************************************************************************/                                                                                     
				
			
			prende = 1;
			while ( bit_mod_hora ==1)
									{
										
									if ( cambio_de_digito ==1) 
															{
															/*	if ( transferir == 0)
																				{
																					digito_a_modificar1 =aux1 ;
																					digito_a_modificar2 =  ;
																					transferir = 1;
																				}*/

															if (prende == 1) 
																			{
																				/*digito1 = digito_a_modificar1;
																				digito2 = digito_a_modificar2; */
																				digito1 = aux1;
																				digito2 = aux2;
																				digito3 = aux3;
																				digito4 = aux3;
																			}

															if (apaga ==1)
																			{
																			digito1 = 0x00;
																			digito2 = 0x00;
																			digito3 = aux3;
																			digito4 = aux4;
																			}
																}

									if ( cambio_de_digito ==2) 
															{

																if (prende == 1) 
																				{
																				digito2 = aux2;
																				digito1 = aux1;
																				digito3 = aux3;
																				digito4 = aux4;
																				}
							
																if (apaga ==1)
																				{
																				digito2 = aux2;
																				digito1 = aux1;
																				digito3 = 0x00;
																				digito4 = 0x00;
																				}
															}		
					
									if (cambio_de_digito == 3)
															{
																cambio_de_digito = 1;		
															}
										
										
										
									Display_mod  (digito1, digito2, digito3,digito4);
										variable ++;	

						
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

							if  (( Sube == 1) && ( numero_de_pulsos == 0) && ( cambio_de_digito ==1) ) 
																	{
																		numero_de_pulsos = 1;

												//	if ( numero_de_pulsos <= 10)
																			//	{
																				//primer_dig++;
																					minutos ++;
																			//	}
												/*	else
														{
														seg_dig ++;
														}*/
												
																		Resuelve_digito () ;
																	}
							if ((Sube == 0) && ( numero_de_pulsos == 1))
													{
														numero_de_pulsos = 0;

													}

							if  (( Sube == 1) && ( numero_de_pulsos == 0) && ( cambio_de_digito ==2) ) 
																	{
																		numero_de_pulsos = 1;
																		Hora ++;
											
																		Resuelve_digito () ;
																	}	
								



							
									if (Enter == 1)
												{
													bit_mod_hora = 0;
												}
	
									

									}   // fin del while
								


					}
		}


	void  DelayMs(unsigned char ms)
							{
							
							Delay10KTCYx((((Fosc/4000)*ms)/1000));
							
								}
								
								void Delay10Ms(unsigned char ms)
								{
								
								
								Delay1KTCYx((((Fosc/4000)*ms)/1000));
								}

void Display (unsigned int aux1, unsigned int aux2,unsigned int aux3, unsigned int aux4)
       		{
					unsigned int j,i;
					
								PORTDbits.RD3=0;
								PORTC=aux1;            	//visualiza primer digito
								
								PORTDbits.RD0=1;

							
								Delay10Ms(5);

								
								PORTDbits.RD0=0;
							
								PORTC=aux2;				// visualiza segundo digito
								PORTDbits.RD1=1;
								Delay10Ms(5);
					
							
								PORTDbits.RD1=0;
								PORTC=aux3;				// visualiza segundo digito
								PORTDbits.RD2=1;
								Delay10Ms(5);

							
								PORTDbits.RD2=0;
								PORTC=aux4;				// visualiza segundo digito
								PORTDbits.RD3=1;
								Delay10Ms(5);
							

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


void Resuelve_digito (void)
						{
						primer_dig= minutos%10; //resuelve primer digito a visualizar
						aux1=DB[primer_dig];
						//aux1=nueve;
						seg_dig = minutos / 10; // resuelve segundo digito a visualizar
						aux2 = DB[seg_dig];
						tercer_dig= Hora%10; //resuelve primer digito a visualizar
						aux3=DB[tercer_dig];
						cuarto_dig = Hora / 10; // resuelve segundo digito a visualizar
						aux4 = DB[cuarto_dig];
							
						}