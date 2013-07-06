#if ARDUINO < 100
#include <WProgram.h>
#else
#include <Arduino.h>
#endif

#include "vs1053.h"

void VS1053::begin(void) const
{
    SPI.setClockDivider(SPI_CLOCK_DIV8);
    pinMode(VS_RESET, OUTPUT);
    pinMode(VS_DREQ, INPUT);
    pinMode(VS_DCS, OUTPUT);
    pinMode(VS_CS, OUTPUT);
    digitalWrite(VS_DCS,HIGH);
    digitalWrite(VS_CS,HIGH);
    putInReset();
    delay(100);//it is a must
    deselectControlBus();
    deselectDataBus();
    releaseFromReset();
    await_data_request();
    /* Set clock register, doubler etc. */
    write_register(SPI_CLOCKF, 0xC000); 
    Serial.print("ClockF:");
    Serial.println(read_register(SPI_CLOCKF),HEX);
    await_data_request();
    softReset();//comment this, as it will be executed everytime playing a music file.
    write_register(SPI_WRAMADDR, 0xc013);
    setVolume(40);
    SPI.setClockDivider(SPI_CLOCK_DIV4);
} 

void VS1053::softReset() const
{
    /* Soft Reset of VS10xx */
    write_register (SPI_MODE, 0x0804); /* Newmode, Reset, No L1-2 */
    await_data_request();
    //setup I2S (see page77 of the datasheet of vs1053 )
    //set GPIO0 as output
    write_register(SPI_WRAMADDR, 0xc017);
    write_register(SPI_WRAM, 0x00f0);
    //enable I2S (MCLK enabled, 48kHz sample rate)
    write_register(SPI_WRAMADDR, 0xc040);
    write_register(SPI_WRAM, 0x000C);
    await_data_request();
}

void VS1053::setVolume(byte vol) const
{
    unsigned int value = vol;
    value <<= 8;
    value |= vol;
    write_register(SPI_VOL,value);
}

void VS1053::write_register(byte _reg, unsigned int _value) const
{
    deselectDataBus();
    selectControlBus();
    delayMicroseconds(1); // tXCSS
    transfer(B10); // Write operation
    transfer(_reg); // Which register
    transfer(_value >> 8); // Send hi byte
    transfer(_value & 0xff); // Send lo byte
    delayMicroseconds(1); // tXCSH
    deselectControlBus();
}

unsigned int VS1053::read_register(byte _reg) const
{
    unsigned int result;
    deselectDataBus();
    selectControlBus();
    delayMicroseconds(1); // tXCSS
    transfer(B11); // Read operation
    transfer(_reg); // Which register
    result = receive() << 8; // read high byte
    result |= receive(); // read low byte
    delayMicroseconds(1); // tXCSH
    deselectControlBus();
    return result;
}

void VS1053::write_data(const uint8_t* buf, unsigned char len) const
{
    selectDataBus();
    await_data_request();
    while(len --)
    {
        transfer(*buf ++);
    }
    deselectDataBus();
}

void VS1053::send_zeroes(size_t len) const
{
    unsigned int i;
    selectDataBus();
    for (i=0;i < len;i ++)
    {
        await_data_request();
        transfer(0);
    }
    deselectDataBus();
}  

void VS1053::printDetails(void) const
{
    Serial.println("VS1053 Configuration");
    Serial.print("MODE ");          
    Serial.println(read_register(0));
    Serial.print("STATUS ");        
    Serial.println(read_register(1));
    Serial.print("BASS ");          
    Serial.println(read_register(2));
    Serial.print("CLOCKF ");        
    Serial.println(read_register(3));
    Serial.print("DECODE_TIME ");   
    Serial.println(read_register(4));
    Serial.print("AUDATA ");        
    Serial.println(read_register(5));
    Serial.print("WRAM ");          
    Serial.println(read_register(6));
    Serial.print("WRAMADDR ");      
    Serial.println(read_register(7));
    Serial.print("HDAT0 ");         
    Serial.println(read_register(8));
    Serial.print("HDAT1 ");         
    Serial.println(read_register(9));
    Serial.print("AIADDR ");        
    Serial.println(read_register(10));
    Serial.print("VOL ");           
    Serial.println(read_register(11));
    Serial.print("AICTRL0 ");       
    Serial.println(read_register(12));
    Serial.print("AICTRL1 ");       
    Serial.println(read_register(13));
    Serial.print("AICTRL2 ");       
    Serial.println(read_register(14));
    Serial.print("AICTRL3 ");       
    Serial.println(read_register(15));
}