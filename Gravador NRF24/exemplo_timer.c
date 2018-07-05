#include<Nordic\reg24le1.h>
#include<stdint.h>
#include<nRF-SPICommands.h>
#include<timer0.h>

#define LED P03

void main(){
	P0DIR = 0xF7; //1111 0111
	//LED = 0;
	//setup_T0_ticks(0xFFFF, 1);
	setup_T0_elapsed_ms(10, 25);
	start_T0();
	while(1){
		if(timer_elapsed){
			timer_elapsed = 0;
			LED = !LED;
		}
	}
}