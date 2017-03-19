#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "dht/dht.h"
#include "dht/dht.c"

#define UART_BAUD_RATE 2400
#include "uart/uart.h"
#include <uart/uart.c>

#include "lcd/LCD.c"

int main(void)
{
    lcd_init();
    /*while(1){
    	lcd_setcursor(0,1);
    	lcd_string("Hi!");
    };*/

	char printbuff[100];

	//init uart
	uart_init( UART_BAUD_SELECT(UART_BAUD_RATE,F_CPU) );

	//init interrupt
	sei();

	#if DHT_FLOAT == 0
	int8_t temperature = 0;
	int8_t humidity = 0;
	#elif DHT_FLOAT == 1
	float temperature = 0;
	float humidity = 0;
	#endif

	for (;;) {
		if(dht_gettemperaturehumidity(&temperature, &humidity) != -1) {

			#if DHT_FLOAT == 0
			itoa(temperature, printbuff, 10);
			#elif DHT_FLOAT == 1
			dtostrf(temperature, 3, 3, printbuff);
			#endif
			uart_puts("temperature: "); uart_puts(printbuff); uart_puts("C");uart_puts("\r\n");
			lcd_setcursor(0,1);
			lcd_string(printbuff);

			#if DHT_FLOAT == 0
			itoa(humidity, printbuff, 10);
			#elif DHT_FLOAT == 1
			dtostrf(humidity, 3, 3, printbuff);
			#endif
			uart_puts("humidity: "); uart_puts(printbuff); uart_puts("%RH");uart_puts("\r\n");
			lcd_setcursor(0,2);
			lcd_string(printbuff);

		}

		uart_puts("\r\n");

		_delay_ms(1500);
	}
	return 0;
}
