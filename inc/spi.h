#ifndef _spi_h_
#define _spi_h_
#include "general.h"
#define SPI_SCK GPIO_PIN_3  //SCLK
#define SPI_SDI GPIO_PIN_4  //MOSI
#define SPI_NSS GPIO_PIN_5  //NSS
#define SPI_PORT GPIOC //SPI port
void _spi_start(void);
void _spi_stop(void);
void _spi_send(unsigned char d);
void _AD8400_set(unsigned char voltage_step);
#endif