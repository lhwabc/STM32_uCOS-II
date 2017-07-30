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

    /*******************接口描述符*********************/
    //bLength字段。接口描述符的长度为9字节。
    0x09,
    
    //bDescriptorType字段。接口描述符的编号为0x04。
    0x04,
    
    //bInterfaceNumber字段。该接口的编号，第一个接口，编号为4。   第5个接口，UVC&UAC已经占用4个接口
    0x00,      
    
    //bAlternateSetting字段。该接口的备用编号，为0。
    0x00,
    
    //bNumEndpoints字段。非0端点的数目。本实例需要二个
    //中断端点（一个输入一个输出），因此该值为2，不需要中断端点，只使用控制传输。
    0x00,
    
    //bInterfaceClass字段。该接口所使用的类。本实例是HID类，
    //HID类的编码为0x03。
    0x03,
    
    //bInterfaceSubClass字段。该接口所使用的子类。在HID1.1协议中，
    //只规定了一种子类：支持BIOS引导启动的子类。
    //USB键盘、鼠标属于该子类，子类代码为0x01。
    //但这里我们是自定义的HID设备，所以不使用子类。
    0x00,
    
    //bInterfaceProtocol字段。如果子类为支持引导启动的子类，
    //则协议可选择鼠标和键盘。键盘代码为0x01，鼠标代码为0x02。
    //自定义的HID设备，也不使用协议。
    0x00,
    
    //iConfiguration字段。该接口的字符串索引值。这里没有，为0。
    0x00,

    /******************HID描述符************************/
    //bLength字段。本HID描述符下只有一个下级描述符。所以长度为9字节。
    0x09,
    
    //bDescriptorType字段。HID描述符的编号为0x21。
    0x21,
    
    //bcdHID字段。本协议使用的HID1.1协议。注意低字节在先。
    0x10,
    0x01,
    
    //bCountyCode字段。设备适用的国家代码，这里选择为美国，代码0x21。
    0x21,
    
    //bNumDescriptors字段。下级描述符的数目。我们只有一个报告描述符。
    0x01,
    
    //bDescriptorType字段。下级描述符的类型，为报告描述符，编号为0x22。
    0x22,
    
    //bDescriptorLength字段。下级描述符的长度。下级描述符为报告描述符。
    sizeof(Msotptool_ReportDescriptor)&0xFF,
    (sizeof(Msotptool_ReportDescriptor)>>8)&0xFF,     
}; /* MOUSE_ConfigDescriptor */
	
const uint8_t Msotptool_ReportDescriptor[Msotptool_SIZ_REPORT_DESC] =
{
    0x06, 0x00, 0xFF,                   // usage page(﹚竡)
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

//使用UNICODE编码
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

