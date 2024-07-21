/*
 ***********************ETAPA DE COMUNICACIÓN-MONITOREO**************************
 * Descripción: AULA 1 que tiene por código identificador el # 101. 
 * 
 * ::::SALÓN:::
 * RC0 = Entrada de personas
 * RC1 = Salida de personas
 *
 */

#include <xc.h>
#include "Config.h"
#include "lib_USART.h"

#define codigo 101
#define Total_Personas 5

char i=0;                                          //i= número de personas 
char codigo_aula=0;
void Tx_i (void);
    
    
void InitPort(void)
{
    ADCON1bits.PCFG = 0xFF;                         //Pines en digital
    TRISCbits.RC0 = 1;                             
    TRISCbits.RC1 = 1;                              
    TRISCbits.RC2=0;                                

}

void main(void) {
  InitPort();
  USART_Init (9600);                             // USART a 9600 baudios
    
  while (1)
  {     
    codigo_aula = USART_RX();
    __delay_ms (50);
    Tx_i();

    if((PORTCbits.RC0 == 0) && (i<Total_Personas))   //se ejecuta cuando detecta el button 1 (entrada de persona)
        {                              
            __delay_ms (50);                         //efecto rebote
            while(PORTCbits.RC0 == 0);                //el bit sigue siendo 0, seguirá en este bucle                      
            i++;                                      //aumento en 1                                                
        }

    else if ((PORTCbits.RC1 == 0) && (i>0) )          //se ejecuta cuando detecta el button 2 (salida de persona)
        {
            __delay_ms (50);                         //efecto rebote
            while(PORTCbits.RC1 == 0);               // el bit sigue siendo 0, seguirá en este bucle
            i--;                                     //disminución en 1  
        }                
  } 
}

void Tx_i (void)
{
    if(codigo_aula==codigo)
    {
        LATCbits.LATC2=1;
        USART_TX(i);
        __delay_ms (150); 
        LATD=i;      
    }

    else 
    {
        LATCbits.LATC2=0;
    }
 
}   

