/**
  ******************************************************************************
  * @file    usb_desc.c
  * @author  MCD Application Team
  * @version V4.0.0
  * @date    21-January-2013
  * @brief   Descriptors for Msotptool Mouse Demo
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */


/* Includes ------------------------------------------------------------------*/
#include "usb_lib.h"
#include "usb_desc.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Extern variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/* USB Standard Device Descriptor */
const uint8_t Msotptool_DeviceDescriptor[Msotptool_SIZ_DEVICE_DESC] =
{
    0x12,                       /*bLength */
    USB_DEVICE_DESCRIPTOR_TYPE, /*bDescriptorType*/
    0x00,                       /*bcdUSB */
    0x02,
    0x00,                       /*bDeviceClass*/
    0x00,                       /*bDeviceSubClass*/
    0x00,                       /*bDeviceProtocol*/
    0x40,                       /*bMaxPacketSize 64*/
		
    0x57,                       /*idVendor (0x1357)*/
    0x13,
		
    0x68,                       /*idProduct = 0x2468*/
    0x24,
		
    0x01,                       /*bcdDevice rel. 2.00*/
    0x02,
		
    0x01,                       /*Index of string descriptor describing manufacturer */
    0x02,                       /*Index of string descriptor describing product*/
    0x03,                       /*Index of string descriptor describing the device serial number */
    0x01                        /*bNumConfigurations*/
}; /* Msotptool_DeviceDescriptor */



/* USB Configuration Descriptor */
/*   All Descriptors (Configuration, Interface, Endpoint, Class, Vendor */
const uint8_t Msotptool_ConfigDescriptor[Msotptool_SIZ_CONFIG_DESC] =
{
    0x09, /* bLength: Configuration Descriptor size */
    USB_CONFIGURATION_DESCRIPTOR_TYPE, /* bDescriptorType: Configuration */
    Msotptool_SIZ_CONFIG_DESC,
    /* wTotalLength: Bytes returned */
    0x00,
		
    0x01,         /*bNumInterfaces: 1 interface*/
    0x01,         /*bConfigurationValue: Configuration value*/
    0x00,         /*iConfiguration: Index of string descriptor describing the configuration*/
    0x80,         /*bmAttributes: Self powered */
    0xFA,         /*MaxPower 100 mA: this current is used for detecting Vbus*/

    /*******************�ӿ�������*********************/
    //bLength�ֶΡ��ӿ��������ĳ���Ϊ9�ֽڡ�
    0x09,
    
    //bDescriptorType�ֶΡ��ӿ��������ı��Ϊ0x04��
    0x04,
    
    //bInterfaceNumber�ֶΡ��ýӿڵı�ţ���һ���ӿڣ����Ϊ4��   ��5���ӿڣ�UVC&UAC�Ѿ�ռ��4���ӿ�
    0x00,      
    
    //bAlternateSetting�ֶΡ��ýӿڵı��ñ�ţ�Ϊ0��
    0x00,
    
    //bNumEndpoints�ֶΡ���0�˵����Ŀ����ʵ����Ҫ����
    //�ж϶˵㣨һ������һ�����������˸�ֵΪ2������Ҫ�ж϶˵㣬ֻʹ�ÿ��ƴ��䡣
    0x00,
    
    //bInterfaceClass�ֶΡ��ýӿ���ʹ�õ��ࡣ��ʵ����HID�࣬
    //HID��ı���Ϊ0x03��
    0x03,
    
    //bInterfaceSubClass�ֶΡ��ýӿ���ʹ�õ����ࡣ��HID1.1Э���У�
    //ֻ�涨��һ�����ࣺ֧��BIOS�������������ࡣ
    //USB���̡�������ڸ����࣬�������Ϊ0x01��
    //�������������Զ����HID�豸�����Բ�ʹ�����ࡣ
    0x00,
    
    //bInterfaceProtocol�ֶΡ��������Ϊ֧���������������࣬
    //��Э���ѡ�����ͼ��̡����̴���Ϊ0x01��������Ϊ0x02��
    //�Զ����HID�豸��Ҳ��ʹ��Э�顣
    0x00,
    
    //iConfiguration�ֶΡ��ýӿڵ��ַ�������ֵ������û�У�Ϊ0��
    0x00,

    /******************HID������************************/
    //bLength�ֶΡ���HID��������ֻ��һ���¼������������Գ���Ϊ9�ֽڡ�
    0x09,
    
    //bDescriptorType�ֶΡ�HID�������ı��Ϊ0x21��
    0x21,
    
    //bcdHID�ֶΡ���Э��ʹ�õ�HID1.1Э�顣ע����ֽ����ȡ�
    0x10,
    0x01,
    
    //bCountyCode�ֶΡ��豸���õĹ��Ҵ��룬����ѡ��Ϊ����������0x21��
    0x21,
    
    //bNumDescriptors�ֶΡ��¼�����������Ŀ������ֻ��һ��������������
    0x01,
    
    //bDescriptorType�ֶΡ��¼������������ͣ�Ϊ���������������Ϊ0x22��
    0x22,
    
    //bDescriptorLength�ֶΡ��¼��������ĳ��ȡ��¼�������Ϊ������������
    sizeof(Msotptool_ReportDescriptor)&0xFF,
    (sizeof(Msotptool_ReportDescriptor)>>8)&0xFF,     
}; /* MOUSE_ConfigDescriptor */
	
const uint8_t Msotptool_ReportDescriptor[Msotptool_SIZ_REPORT_DESC] =
{
    0x06, 0x00, 0xFF,                   // usage page(�۩w�q)
    0x09, 0x01,                         // usage(0) undefined
    0xA1, 0x01,                         // Collection(Application)
 //   0x85, 0x01,                         // REPORT_ID (1)
    0x15, 0x00,                         // Logical Minimum (0)
    0x26, 0xFF, 0x00,                   // Logical Maximum (255)
    0x19, 0x01,                         // Usage Minimum(0x01)
    0x29, 0x02,                         // Usage Maximum(0x05)
    0x75, 0x08,                         // REPORT SIZE (8)
    0x95, 0x40,                         // REPORT COUNT(64)
    0xB1, 0x02,                         // FEATURE(DATA,VARIABLE,ABSOLUTE)
    0xC0 ,                              // END COLLECTION
}; /* Msotptool_ReportDescriptor */

/* USB String Descriptors (optional) */
const uint8_t Msotptool_StringLangID[Msotptool_SIZ_STRING_LANGID] =
{
    Msotptool_SIZ_STRING_LANGID,
    USB_STRING_DESCRIPTOR_TYPE,
    0x09,
    0x04
}; /* LangID = 0x0409: U.S. English */

//ʹ��UNICODE����
const u8 Msotptool_StringVendor[Msotptool_SIZ_STRING_VENDOR] =	    
{
	Msotptool_SIZ_STRING_VENDOR,    /* Size of manufaturer string */
	USB_STRING_DESCRIPTOR_TYPE,     /* bDescriptorType = String descriptor */
	/* Manufacturer: "MACROSILICON" */ 
	'H',0x00, 'e',0x00, 'f',0x00, 'e',0x00, 'i',0x00, ' ',0x00,
	'M',0x00, 'a',0x00, 'c',0x00, 'r',0x00, 'o',0x00,
    'S',0x00, 'i',0x00, 'l',0x00, 'i',0x00, 'c',0x00, 'o',0x00, 'n',0x00, ' ',0x00,
};

const u8 Msotptool_StringProduct[Msotptool_SIZ_STRING_PRODUCT] =
{
	Msotptool_SIZ_STRING_PRODUCT,          /* bLength */
	USB_STRING_DESCRIPTOR_TYPE,        /* bDescriptorType */
    'M',0x00, 'S',0x00, '8',0x00, '0',0x00, '0',0x00, '5',0x00, ' ',0x00,
    'M',0x00, 'S',0x00, 'L',0x00, 'i',0x00, 'n',0x00, 'k',0x00, ' ',0x00, 
//    'd',0x00, 'o',0x00, 'n',0x00, 'g',0x00, 'l',0x00, 'e',0x00, ' ',0x00, 
//    'V',0x00, '2',0x00, '.',0x00, '0',0x00, '1',0x00, ' ',0x00,
};
uint8_t Msotptool_StringSerial[Msotptool_SIZ_STRING_SERIAL] =
  {
    Msotptool_SIZ_STRING_SERIAL,           /* bLength */
    USB_STRING_DESCRIPTOR_TYPE,        /* bDescriptorType */
    'S', 0, 'T', 0, 'M', 0, '3', 0, '2', 0
  };

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

