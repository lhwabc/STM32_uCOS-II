/**
  ******************************************************************************
  * @file    usb_prop.h
  * @author  MCD Application Team
  * @version V4.0.0
  * @date    21-January-2013
  * @brief   All processing related to Msotptool Mouse demo
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


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USB_PROP_H
#define __USB_PROP_H

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
typedef enum _HID_REQUESTS
{
  GET_REPORT = 1,
  GET_IDLE,
  GET_PROTOCOL,

  SET_REPORT = 9,
  SET_IDLE,
  SET_PROTOCOL
} HID_REQUESTS;

typedef struct HID_otprw
{
    uint8_t usb_hid_tx_buffer[64];
    uint8_t usb_hid_rx_buffer[64];
    uint8_t set_report_flag;
    uint16_t otp_read_addr ;
    uint16_t otp_write_addr;
    uint16_t otp_read_length ;
    uint8_t otp_write_length ;
    uint8_t icp_status ;
}
HID_otprw_TypeDef;

extern HID_otprw_TypeDef ms8005otp;

/* Exported constants --------------------------------------------------------*/
/* Definition of MS Link Version */
#define MS_LINK_VERSION_L               0x02  //次版本号  v2.02
#define MS_LINK_VERSION_H               0x02  //主版本号  v2.02
/* Definition of MS HID command */
#define MS_HID_CMD_GET_VERSION          0x30    //get mslink firmware version
#define MS_HID_CMD_GET_CONFIG           0x60    //get mslink config information
#define MS_HID_CMD_SET_CONFIG           0x80    //Set mslink config information
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void set_report_deocder(void);
void Msotptool_init(void);
void Msotptool_Reset(void);
void Msotptool_SetConfiguration(void);
void Msotptool_SetDeviceAddress (void);
void Msotptool_Status_In (void);
void Msotptool_Status_Out (void);
RESULT Msotptool_Data_Setup(uint8_t);
RESULT Msotptool_NoData_Setup(uint8_t);
RESULT Msotptool_Get_Interface_Setting(uint8_t Interface, uint8_t AlternateSetting);
uint8_t *Msotptool_GetDeviceDescriptor(uint16_t );
uint8_t *Msotptool_GetConfigDescriptor(uint16_t);
uint8_t *Msotptool_GetStringDescriptor(uint16_t);
RESULT Msotptool_SetProtocol(void);
uint8_t *Msotptool_GetProtocolValue(uint16_t Length);
RESULT Msotptool_SetProtocol(void);
uint8_t *Msotptool_GetReportDescriptor(uint16_t Length);
uint8_t *Msotptool_GetHIDDescriptor(uint16_t Length);

/* Exported define -----------------------------------------------------------*/
#define Msotptool_GetConfiguration          NOP_Process
//#define Msotptool_SetConfiguration          NOP_Process
#define Msotptool_GetInterface              NOP_Process
#define Msotptool_SetInterface              NOP_Process
#define Msotptool_GetStatus                 NOP_Process
#define Msotptool_ClearFeature              NOP_Process
#define Msotptool_SetEndPointFeature        NOP_Process
#define Msotptool_SetDeviceFeature          NOP_Process
//#define Msotptool_SetDeviceAddress          NOP_Process

#define REPORT_DESCRIPTOR                  0x22

#endif /* __USB_PROP_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
