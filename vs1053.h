#if ARDUINO < 100
#include <WProgram.h>
#else
#include <Arduino.h>
#endif

#include <SPI.h>

#define ROTARY_ANGLE_SENSOR A4
#define BUTTON              7
////////////////////////for MMC////////////////////////////////////
#define SD_CS_PIN 10  //SD card select pin linked to pin10 of MCU

///////////////////////for vs10xx///////////////////////////////////
#define VS_RESET A0
#define VS_DREQ  A1
#define VS_DCS   A2
#define VS_CS    A3

/////////////////////for keys//////////////////////////////////////
#define KEY_VD   7
#define KEY_BK   6
#define KEY_PS   5
#define KEY_NT   4
#define KEY_VU   3

//////////////////for Green LED///////////////////////////////////////
#define GREEN_LED 	8
#define GREEN_LED_ON()	digitalWrite(GREEN_LED, HIGH)
#define GREEN_LED_OFF()	digitalWrite(GREEN_LED, LOW)


/** VS10xx SCI Write Command byte is 0x02 */
#define VS_WRITE_COMMAND 0x02
/** VS10xx SCI Read Command byte is 0x03 */
#define VS_READ_COMMAND 0x03


#define SPI_MODE	0x0   /**< VS10xx register */
#define SPI_STATUS	0x1   /**< VS10xx register */
#define SPI_BASS	0x2   /**< VS10xx register */
#define SPI_CLOCKF	0x3   /**< VS10xx register */
#define SPI_DECODE_TIME	0x4   /**< VS10xx register */
#define SPI_AUDATA	0x5   /**< VS10xx register */
#define SPI_WRAM	0x6   /**< VS10xx register */
#define SPI_WRAMADDR	0x7   /**< VS10xx register */
#define SPI_HDAT0	0x8   /**< VS10xx register */
#define SPI_HDAT1	0x9   /**< VS10xx register */
#define SPI_AIADDR	0xa   /**< VS10xx register */
#define SPI_VOL		0xb   /**< VS10xx register */
#define SPI_AICTRL0	0xc   /**< VS10xx register */
#define SPI_AICTRL1	0xd   /**< VS10xx register */
#define SPI_AICTRL2	0xe   /**< VS10xx register */
#define SPI_AICTRL3	0xf   /**< VS10xx register */

#define SM_DIFF		0x01   /**< VS10xx register */
#define SM_JUMP		0x02   /**< VS10xx register */
#define SM_RESET	0x04   /**< VS10xx register */
#define SM_OUTOFWAV	0x08   /**< VS10xx register */
#define SM_PDOWN	0x10   /**< VS10xx register */
#define SM_TESTS	0x20   /**< VS10xx register */
#define SM_STREAM	0x40   /**< VS10xx register */
#define SM_PLUSV	0x80   /**< VS10xx register */
#define SM_DACT		0x100   /**< VS10xx register */
#define SM_SDIORD	0x200   /**< VS10xx register */
#define SM_SDISHARE	0x400   /**< VS10xx register */
#define SM_SDINEW	0x800   /**< VS10xx register */
#define SM_ADPCM        0x1000   /**< VS10xx register */
#define SM_ADPCM_HP     0x2000   /**< VS10xx register */

class VS1053
{
public:

  inline void selectControlBus()   const { 
    //Serial.println("digitalWrite(VS_CS,LOW);");
    digitalWrite(VS_CS,LOW); 
  }
  inline void deselectControlBus() const { 
    //Serial.println("digitalWrite(VS_CS,HIGH);");
    digitalWrite(VS_CS,HIGH); 
  }
  inline void selectDataBus()      const { 
    //Serial.println("digitalWrite(VS_DCS,LOW);");
    digitalWrite(VS_DCS,LOW);
  }
  inline void deselectDataBus()    const { 
    //Serial.println("digitalWrite(VS_DCS,HIGH);");
    digitalWrite(VS_DCS,HIGH);
  }
  inline void putInReset()         const { 
    //Serial.println("digitalWrite(VS_RESET,LOW);");
    digitalWrite(VS_RESET,LOW);
  }
  inline void releaseFromReset()   const { 
    //Serial.println("digitalWrite(VS_RESET,HIGH); ");
    digitalWrite(VS_RESET,HIGH); 
  }
  inline void await_data_request() const { 
    //Serial.println("digitalRead(VS_DREQ);");
    while ( !digitalRead(VS_DREQ) ); 
  }

  inline byte transfer(const byte data) const
  {
    //Serial.print("transfer "); 
    //Serial.println(data, HEX);
    return SPI.transfer(data);
  }

  inline byte receive()  const {
    SPDR = 0xff;
    while (!(SPSR & _BV(SPIF)));
    return SPDR;
  }

public:
  void begin(void) const;
  void softReset() const;
  void setVolume(byte vol) const;
  void write_register(byte _reg, unsigned int _value) const;
  unsigned int read_register(byte _reg) const;
  void write_data(const uint8_t* buf, unsigned char len) const;
  void send_zeroes(size_t len) const;
  void printDetails(void) const;
};















