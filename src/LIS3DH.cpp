#include "LIS3DH.h"



LIS3DH::LIS3DH(SPI& spi,volatile unsigned char& port, int cs) :
m_spi(spi),m_port(port), m_cs(cs)
{
}

bool LIS3DH::init()
{
 
  /* Check connection */
  uint8_t deviceid = readRegister(LIS3DH_REG_WHOAMI);
  if (deviceid != 0x33) {
    /* No LIS3DH detected ... return false */
    // Serial.println(deviceid, HEX);
    return false;
  }

  // enable all axes, normal mode
  writeRegister( LIS3DH_REG_CTRL1, 0x07);
  // 400Hz rate
  setDataRate( LIS3DH_DATARATE_100_HZ);

  // High res & BDU enabled
  writeRegister( LIS3DH_REG_CTRL4, 0x88);

  // DRDY on INT1
  writeRegister( LIS3DH_REG_CTRL3, 0x10);

  // Turn on orientation config
  // writeRegister8(LIS3DH_REG_PL_CFG, 0x40);

  // enable adcs
  //writeRegister( LIS3DH_REG_TEMPCFG, 0x80);
	return true;
}


void LIS3DH::setDataRate(lis3dh_dataRate_t dataRate)
 {
  uint8_t ctl1 = readRegister( LIS3DH_REG_CTRL1);
  ctl1 &= ~(0xF0); // mask off bits
  ctl1 |= (dataRate << 4);
  writeRegister( LIS3DH_REG_CTRL1, ctl1);
}
void LIS3DH::setDataRange(lis3dh_range_t range)
{
	uint8_t r = readRegister(LIS3DH_REG_CTRL4);
	r &= ~(0x30);
	r |= range << 4;
	writeRegister(LIS3DH_REG_CTRL4, r);
}
lis3dh_range_t LIS3DH::getRange() {
  /* Read the data format register to preserve bits */
  return (lis3dh_range_t)((readRegister(LIS3DH_REG_CTRL4) >> 4) & 0x03);
}

uint8_t LIS3DH::readRegister( uint8_t reg)
{
	m_port &= ~(1<<m_cs);
    m_spi.transfer(reg | 0x80); // read, bit 7 high
    uint8_t value = m_spi.transfer();
	m_port |= (1<<m_cs);
	
	return value;
}
void LIS3DH::writeRegister(uint8_t reg, uint8_t data)
{
    m_port &= ~(1<<m_cs);
    m_spi.transfer(reg & ~0x80); // write, bit 7 low
    m_spi.transfer(data);
	m_port |= (1<<m_cs);
}

void LIS3DH::enableFreeFallInt1()
{
	
	writeRegister(0x22, 1<<6);
	
	//interupt config register (0x30)
	
	
	//writeRegister(0x30, 0b10010101);
	writeRegister(0x30, 0b10010101);
	
	//interrupt threshold (0x32)
	writeRegister(0x32,0x04); //depending on data range, this sets the threshold
	//approximately val*8*dataRange/1000
	
	//interupt duration(0x33) val number of clock cycles
	writeRegister(0x33,0x08); 
	
	//interrupt source register (0x31)-read to clear
	
}

void LIS3DH::clearInterrupts()
{
	readRegister(0x31);
}
void LIS3DH::read(float& x_g, float& y_g, float& z_g)
 {

    m_port &= ~(1<<m_cs);
    m_spi.transfer(LIS3DH_REG_OUT_X_L | 0x80 | 0x40); // read multiple, bit 7&6 high

    int16_t x = m_spi.transfer();
    x |= ((int16_t)m_spi.transfer()) << 8;
    int16_t y = m_spi.transfer();
    y |= ((int16_t)m_spi.transfer()) << 8;
    int16_t z = m_spi.transfer();
    z |= ((int16_t)m_spi.transfer()) << 8;

    m_port |= (1<<m_cs);

  uint8_t range = getRange();
  uint16_t divider = 1;
  if (range == LIS3DH_RANGE_16_G)
    divider = 1365; // different sensitivity at 16g
  if (range == LIS3DH_RANGE_8_G)
    divider = 4096;
  if (range == LIS3DH_RANGE_4_G)
    divider = 8190;
  if (range == LIS3DH_RANGE_2_G)
    divider = 16380;

  x_g = (float)x / divider;
  y_g = (float)y / divider;
  z_g = (float)z / divider;
 }