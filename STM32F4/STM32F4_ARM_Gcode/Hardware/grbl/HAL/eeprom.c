#include "eeprom.h"
#include <string.h>


static uint8_t EepromData[EEPROM_SIZE];


void EE_Init(void)
{
	memcpy(EepromData, (uint8_t*)EEPROM_START_ADDRESS, EEPROM_SIZE);
}

uint8_t EE_ReadByte(uint16_t VirtAddress)
{
	return EepromData[VirtAddress];
}

void EE_WriteByte(uint16_t VirtAddress, uint8_t Data)
{
	EepromData[VirtAddress] = Data;
}

uint8_t EE_ReadByteArray(uint8_t *DataOut, uint16_t VirtAddress, uint16_t size)
{
	uint8_t data, checksum = 0;

	for(; size > 0; size--) {
		data = EE_ReadByte(VirtAddress++);
		checksum = (checksum << 1) || (checksum >> 7);
		checksum += data;
		*(DataOut++) = data;
	}

	data = EE_ReadByte(VirtAddress);
	if(data == checksum) {
		return 1;
	}

	return 0;
}

void EE_WriteByteArray(uint16_t VirtAddress, uint8_t *DataIn, uint16_t size)
{
	unsigned char checksum = 0;

	for(; size > 0; size--) {
		checksum = (checksum << 1) || (checksum >> 7);
		checksum += *DataIn;
		EE_WriteByte(VirtAddress++, *(DataIn++));
	}

	EE_WriteByte(VirtAddress, checksum);
}

void EE_Program(void)
{
	uint16_t data = 0;
	EE_Erase();

//	FLASH_Unlock();
	HAL_FLASH_Unlock();

//	for(uint16_t i = 0; i < EEPROM_SIZE; ++i) {
////		FLASH_ProgramByte(EEPROM_START_ADDRESS + i, EepromData[i]);
//		HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD,EEPROM_START_ADDRESS + i, EepromData[i]);
//	}
	
		for(uint16_t i = 0; i < EEPROM_SIZE/2; ++i) {
//		FLASH_ProgramByte(EEPROM_START_ADDRESS + i, EepromData[i]);
			data =  (EepromData[2*i]) | (EepromData[2*i+1]<<8); //HAL库最小是半字(16位)
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD,EEPROM_START_ADDRESS + i*2, data);//地址如果是奇数会有问题，addr_offset必须是偶数
	}

//	FLASH_Lock();
		HAL_FLASH_Lock();
}


#ifdef STM32F407xx

/* Base address of the Flash sectors */
#define ADDR_FLASH_SECTOR_0     ((uint32_t)0x08000000) /* Base @ of Sector 0, 16 Kbytes */
#define ADDR_FLASH_SECTOR_1     ((uint32_t)0x08004000) /* Base @ of Sector 1, 16 Kbytes */
#define ADDR_FLASH_SECTOR_2     ((uint32_t)0x08008000) /* Base @ of Sector 2, 16 Kbytes */
#define ADDR_FLASH_SECTOR_3     ((uint32_t)0x0800C000) /* Base @ of Sector 3, 16 Kbytes */
#define ADDR_FLASH_SECTOR_4     ((uint32_t)0x08010000) /* Base @ of Sector 4, 64 Kbytes */
#define ADDR_FLASH_SECTOR_5     ((uint32_t)0x08020000) /* Base @ of Sector 5, 128 Kbytes */
#define ADDR_FLASH_SECTOR_6     ((uint32_t)0x08040000) /* Base @ of Sector 6, 128 Kbytes */
#define ADDR_FLASH_SECTOR_7     ((uint32_t)0x08060000) /* Base @ of Sector 7, 128 Kbytes */
#define ADDR_FLASH_SECTOR_8     ((uint32_t)0x08080000) /* Base @ of Sector 8, 128 Kbytes */
#define ADDR_FLASH_SECTOR_9     ((uint32_t)0x080A0000) /* Base @ of Sector 9, 128 Kbytes */
#define ADDR_FLASH_SECTOR_10    ((uint32_t)0x080C0000) /* Base @ of Sector 10, 128 Kbytes */
#define ADDR_FLASH_SECTOR_11    ((uint32_t)0x080E0000) /* Base @ of Sector 11, 128 Kbytes */

/**
  * @brief  Gets the sector of a given address
  * @param  None
  * @retval The sector of a given address
  */
static uint32_t GetSector(uint32_t Address)
{
  uint32_t sector = 0;
  
  if((Address < ADDR_FLASH_SECTOR_1) && (Address >= ADDR_FLASH_SECTOR_0))
  {
    sector = FLASH_SECTOR_0;  
  }
  else if((Address < ADDR_FLASH_SECTOR_2) && (Address >= ADDR_FLASH_SECTOR_1))
  {
    sector = FLASH_SECTOR_1;  
  }
  else if((Address < ADDR_FLASH_SECTOR_3) && (Address >= ADDR_FLASH_SECTOR_2))
  {
    sector = FLASH_SECTOR_2;  
  }
  else if((Address < ADDR_FLASH_SECTOR_4) && (Address >= ADDR_FLASH_SECTOR_3))
  {
    sector = FLASH_SECTOR_3;  
  }
  else if((Address < ADDR_FLASH_SECTOR_5) && (Address >= ADDR_FLASH_SECTOR_4))
  {
    sector = FLASH_SECTOR_4;  
  }
  else if((Address < ADDR_FLASH_SECTOR_6) && (Address >= ADDR_FLASH_SECTOR_5))
  {
    sector = FLASH_SECTOR_5;  
  }
  else if((Address < ADDR_FLASH_SECTOR_7) && (Address >= ADDR_FLASH_SECTOR_6))
  {
    sector = FLASH_SECTOR_6;  
  }
  else if((Address < ADDR_FLASH_SECTOR_8) && (Address >= ADDR_FLASH_SECTOR_7))
  {
    sector = FLASH_SECTOR_7;  
  }
  else if((Address < ADDR_FLASH_SECTOR_9) && (Address >= ADDR_FLASH_SECTOR_8))
  {
    sector = FLASH_SECTOR_8;  
  }
  else if((Address < ADDR_FLASH_SECTOR_10) && (Address >= ADDR_FLASH_SECTOR_9))
  {
    sector = FLASH_SECTOR_9;  
  }
  else if((Address < ADDR_FLASH_SECTOR_11) && (Address >= ADDR_FLASH_SECTOR_10))
  {
    sector = FLASH_SECTOR_10;  
  }
  else /* (Address < FLASH_END_ADDR) && (Address >= ADDR_FLASH_SECTOR_11) */
  {
    sector = FLASH_SECTOR_11;
  }

  return sector;
}

/**
  * @brief  Gets sector Size
  * @param  None
  * @retval The size of a given sector
  */
static uint32_t GetSectorSize(uint32_t Sector)
{
  uint32_t sectorsize = 0x00;

  if((Sector == FLASH_SECTOR_0) || (Sector == FLASH_SECTOR_1) || (Sector == FLASH_SECTOR_2) || (Sector == FLASH_SECTOR_3))
  {
    sectorsize = 16 * 1024;
  }
  else if(Sector == FLASH_SECTOR_4)
  {
    sectorsize = 64 * 1024;
  }
  else
  {
    sectorsize = 128 * 1024;
  }  
  return sectorsize;
}

//  @ fuction:  
//  @ description: 擦除flash 
//  @ input:
//  @ output:
//  @ note: STM32F4的flash较大，擦除花费时间较长
void EE_Erase(void)
{
//	FLASH_Unlock();
  HAL_FLASH_Unlock();
//初始化FLASH_EraseInitTypeDef     
	FLASH_EraseInitTypeDef f;     
	f.TypeErase = FLASH_TYPEERASE_SECTORS;//STM32f4是以sector为单位 
	f.Sector = GetSector(EEPROM_START_ADDRESS); 
	f.NbSectors = 1;     //设置PageError 擦除一个扇区  
  f.VoltageRange = FLASH_VOLTAGE_RANGE_3;	  
	uint32_t PageError = 0;     //调用擦除函数     
	HAL_FLASHEx_Erase(&f, &PageError);
//	FLASH_EraseSector(FLASH_SECTOR, VOLTAGE_RANGE);

//	FLASH_Lock();
	HAL_FLASH_Lock(); 
}
#else
//STM32F103C8 flash擦除
void EE_Erase(void)
{
//	FLASH_Unlock();
  HAL_FLASH_Unlock();
//初始化FLASH_EraseInitTypeDef     
	FLASH_EraseInitTypeDef f;     
	f.TypeErase = FLASH_TYPEERASE_PAGES;     
	f.PageAddress = EEPROM_START_ADDRESS;     
	f.NbPages = 1;     //设置PageError 擦除一个扇区    
	uint32_t PageError = 0;     //调用擦除函数     
	HAL_FLASHEx_Erase(&f, &PageError);
//	FLASH_EraseSector(FLASH_SECTOR, VOLTAGE_RANGE);

//	FLASH_Lock();
	HAL_FLASH_Lock();
}
#endif

/* 测试 */
#if 0
uint32_t writeFlashData = 0x55555555;
uint32_t addr = 0x08007000;
uint32_t addr_offset = 0;
uint8_t writeFlashData1 = 0x55;

//FLASH写入数据测试
void writeFlashTest(void)
{
		uint8_t temp1 = *(__IO uint8_t*)(addr);//没有锁定，读取的内容均是0xff
	uint16_t temp2 = *(__IO uint16_t*)(addr);
	uint32_t temp3 = *(__IO uint32_t*)(addr);
	
//	uint16_t xx = writeFlashData & 0xff;
//	writeFlashData = xx | temp;
	printf("data1:0x%x\r\n",temp1);
	printf("data2:0x%x\r\n",temp2);
	printf("data3:0x%x\r\n",temp3);
	
    //1、解锁FLASH
  HAL_FLASH_Unlock();

    //2、擦除FLASH
    //初始化FLASH_EraseInitTypeDef
    FLASH_EraseInitTypeDef f;
    f.TypeErase = FLASH_TYPEERASE_PAGES;
    f.PageAddress = addr;
    f.NbPages = 1;
    //设置PageError
    uint32_t PageError = 0;
    //调用擦除函数
    HAL_FLASHEx_Erase(&f, &PageError);
	
//	uint8_t temp1 = *(__IO uint8_t*)(addr);//没有锁定，读取的内容均是0xff
//	uint16_t temp2 = *(__IO uint16_t*)(addr);
//	uint32_t temp3 = *(__IO uint32_t*)(addr); 
temp2 &= 0xff00;
   uint16_t writeFlashData2 =  writeFlashData1 | temp2;
//	
////	uint16_t xx = writeFlashData & 0xff;
////	writeFlashData = xx | temp;
//	printf("data1:0x%x\r\n",temp1);
//	printf("data2:0x%x\r\n",temp2);
//	printf("data3:0x%x\r\n",temp3);

    //3、对FLASH烧写
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, addr+addr_offset, writeFlashData2);//地址如果是奇数会有问题，addr_offset必须是偶数

    //4、锁住FLASH
  HAL_FLASH_Lock();
}

//FLASH读取数据测试
void printFlashTest(void)
{
	uint32_t addr_a = addr+addr_offset;
  uint32_t temp = *(__IO uint32_t*)(addr_a);
	uint32_t temp1 = *(__IO uint32_t*)(addr_a+1);

    printf("addr:0x%x, data:0x%x\r\n", addr_a, temp);
	printf("addr+1:0x%x, data:0x%x\r\n", addr_a+1, temp1);
}
#endif
