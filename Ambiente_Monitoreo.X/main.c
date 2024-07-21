/*
 ***********************ETAPA DE COMUNICACIÓN-MONITOREO**************************
 * Descripción: Se tiene 1 ambiente de monitoreo y 2 salones. Estos salones 
 * se comunican con la sala de monitoreo mediante el estándar RS-485, la entrada
 * de personas a sus respectivos salones es representado por medio de un botón en 
 * el pin RC0 y la salida mediante un botón en el pin RC1. En el ambiente de monitoreo 
 * se podrá visualizar en un display 7 segmentos la cantidad de alumnos que hay por aula, 
 * para poder cambiar el aula que se mostratá en pantalla se tiene un botón en el pin 
 * RB0, en el ambiente de monitoreo.
 * 
 * ::::SALONES:::
 * RC0 = Entrada de personas
 * RC1 = Salida de personas
 * 
 * ::::AMBIENTE DE MONITOREO:::
 * RB0 = Cambio de salón que se desea mostrar en el LCD
 */

#include <xc.h>
#include "Config.h"
#include "lib_USART.h"
#include "stdio.h"
#include "string.h"

#define aula1 101
#define aula2 102
#define Total_Personas 5

char codigo_aula;
char aux;
char i;                                //i= número de personas 


void InitPort(void){
 ADCON1bits.PCFG = 0xFF;                         //Pines en digital
 TRISBbits.RB0=1;
 TRISA=0;
 TRISE=0;
 TRISB=0X0f;
}
void Init_Int (void)
{     
    INTCONbits.INT0IE = 1;                      // Habilitar INT0 
    INTCONbits.INT0IF = 0;                      //Bandera 0
    INTCON2bits.INTEDG0 = 0;                    //Flanco descendete
    INTCONbits.GIE = 1;
   
}

void main(void) {
    char temp;
    
    InitPort();
    Init_Int();
    USART_Init (9600);                     // a 9600 baudios
    
    codigo_aula=101;
    aux=1;
    temp=90;
    
    while(1)
    {   
        if (aux==1)
        {
             USART_TX(temp);
             __delay_ms (50);             
             aux=0;
             USART_TX(codigo_aula);
             __delay_ms (50); 
        }

        i = USART_RX();
        __delay_ms (50);
        
        if (i<Total_Personas) 
        {   
             if(codigo_aula==101)     
             {  
                 LATA=i;
             }    
             else
             {
                 i<<=4; 
                 LATB=(i);
             }
        }

        else
        {  
             if(codigo_aula==101)     
            {
                LATA=i;;
            }     
             else
             {
                 i<<=4; 
                 LATB=(i);
             }
        }    
    }    
}



void __interrupt() INT_isr(void)
{         
    __delay_ms(50);
    while(PORTBbits.RB0 == 1);               // el bit sigue siendo 1 seguirá en este bucle                      
    aux = 1;
    i=0;    
    if (codigo_aula == aula1)
    {codigo_aula = aula2;}
    else {codigo_aula=aula1;}
    INTCONbits.INT0IF = 0;
}