#include<Nordic\reg24le1.h>
#include<hal_delay.h>
#include<stdint.h>
#include<nRF-SPICommands.h>

void setup()
{
P0DIR = 0x00; // ( todas as portas recebem 0 como entradas)
P1DIR = 0xFF; //( todas as portas recebem 1 como entradas)

P03 = 0; //( porta P03 recebe 0 e por isso é saida ) 
P02 = 1;	//( porta P02 recebe 1 e por isso é entrada )
}
void loop()
{
	if(P02 == 1)
	{
	  P03=1;
		delay_ms(500);
		P03=0;
		delay_ms(500);
	}
	if(P02 == 0)
	{
		P03=0;
	}
}

void main()
{ 	
	setup();
	while(1)
	{
	loop();
	}
}