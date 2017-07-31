/**
  ******************************************************************************
  * @file    usb_prop.c
  * @author  MCD Application Team
  * @version V4.0.0
  * @date    21-January-2013
  * @brief   All processing related to Msotptool Mouse Demo
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
#include "usb_conf.h"
#include "usb_prop.h"
#include "usb_desc.h"
#include "usb_pwr.h"
#include "hw_config.h"
#include "stm32f10x.h"
#include "ICP.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint32_t ProtocolValue;

HID_otprw_TypeDef ms8005otp;

/* -------------------------------------------------------------------------- */
/*  Structures initializations */
/* -------------------------------------------------------------------------- */

DEVICE Device_Table =
  {
    EP_NUM,
    1
  };

DEVICE_PROP Device_Property =
  {
    Msotptool_init,
    Msotptool_Reset,
    Msotptool_Status_In,
    Msotptool_Status_Out,
    Msotptool_Data_Setup,
    Msotptool_NoData_Setup,
    Msotptool_Get_Interface_Setting,
    Msotptool_GetDeviceDescriptor,
    Msotptool_GetConfigDescriptor,
    Msotptool_GetStringDescriptor,
    0,
    0x40 /*MAX PACKET SIZE*/
  };
USER_STANDARD_REQUESTS User_Standard_Requests =
  {
    Msotptool_GetConfiguration,
    Msotptool_SetConfiguration,
    Msotptool_GetInterface,
    Msotptool_SetInterface,
    Msotptool_GetStatus,
    Msotptool_ClearFeature,
    Msotptool_SetEndPointFeature,
    Msotptool_SetDeviceFeature,
    Msotptool_SetDeviceAddress
  };

ONE_DESCRIPTOR Device_Descriptor =
  {
    (uint8_t*)Msotptool_DeviceDescriptor,
    Msotptool_SIZ_DEVICE_DESC
  };

ONE_DESCRIPTOR Config_Descriptor =
  {
    (uint8_t*)Msotptool_ConfigDescriptor,
    Msotptool_SIZ_CONFIG_DESC
  };

ONE_DESCRIPTOR Msotptool_Report_Descriptor =
  {
    (uint8_t *)Msotptool_ReportDescriptor,
    Msotptool_SIZ_REPORT_DESC
  };

ONE_DESCRIPTOR Mouse_Hid_Descriptor =
  {
    (uint8_t*)Msotptool_ConfigDescriptor + Msotptool_OFF_HID_DESC,
    Msotptool_SIZ_HID_DESC
  };

ONE_DESCRIPTOR String_Descriptor[4] =
  {
    {(uint8_t*)Msotptool_StringLangID, Msotptool_SIZ_STRING_LANGID},
    {(uint8_t*)Msotptool_StringVendor, Msotptool_SIZ_STRING_VENDOR},
    {(uint8_t*)Msotptool_StringProduct, Msotptool_SIZ_STRING_PRODUCT},
    {(uint8_t*)Msotptool_StringSerial, Msotptool_SIZ_STRING_SERIAL}
  };

/* Extern variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Extern function prototypes ------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : Msotptool_init.
* Description    : Msotptool Mouse init routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Msotptool_init(void)
{

  /* Update the serial number string descriptor with the data from the unique
  ID*/
//  uint16_t i;
//  for(i=0;i<0x8000;i++)
//  {
//      otpcode_buffer[i] = 0xFF;
//  }
  Get_SerialNum();

  pInformation->Current_Configuration = 0;
  /* Connect the device */
  PowerOn();

  /* Perform basic device initialization operations */
  USB_SIL_Init();

  bDeviceState = UNCONNECTED;
    /* Perform basic device initialization operations */
}

/*******************************************************************************
* Function Name  : Msotptool_Reset.
* Description    : Msotptool Mouse reset routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Msotptool_Reset(void)
{
  /* Set Msotptool_DEVICE as not configured */
  pInformation->Current_Configuration = 0;
  pInformation->Current_Interface = 0;/*the default Interface*/

  /* Current Feature initialization */
  pInformation->Current_Feature = Msotptool_ConfigDescriptor[7];
  SetBTABLE(BTABLE_ADDRESS);
  /* Initialize Endpoint 0 */
  SetEPType(ENDP0, EP_CONTROL);
  SetEPTxStatus(ENDP0, EP_TX_STALL);
  SetEPRxAddr(ENDP0, ENDP0_RXADDR);
  SetEPTxAddr(ENDP0, ENDP0_TXADDR);
  Clear_Status_Out(ENDP0);
  SetEPRxCount(ENDP0, Device_Property.MaxPacketSize);
  SetEPRxValid(ENDP0);
    

  /* Initialize Endpoint 1 */
  SetEPType(ENDP1, EP_INTERRUPT);
  SetEPTxAddr(ENDP1, ENDP1_TXADDR);
  SetEPTxCount(ENDP1, 4);
  SetEPRxStatus(ENDP1, EP_RX_DIS);
  SetEPTxStatus(ENDP1, EP_TX_NAK);

  /* Set this device to response on default address */
  SetDeviceAddress(0);
  bDeviceState = ATTACHED;
}
/*******************************************************************************
* Function Name  : Msotptool_SetConfiguration.
* Description    : Update the device state to configured.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Msotptool_SetConfiguration(void)
{
  DEVICE_INFO *pInfo = &Device_Info;

  if (pInfo->Current_Configuration != 0)
  {
    /* Device configured */
    bDeviceState = CONFIGURED;
  }
}
/*******************************************************************************
* Function Name  : Msotptool_SetConfiguration.
* Description    : Update the device state to addressed.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Msotptool_SetDeviceAddress (void)
{
  bDeviceState = ADDRESSED;
}


void set_report_deocder()
{
    switch(ms8005otp.usb_hid_rx_buffer[0])  
    {
        case 0x22:      //enter icp mode
            ms8005otp.icp_status  =  ICP_init();
            break;
        case 0x33:      //exit icp mode
            ICP_deinit();
            break;
        case MS_HID_CMD_SET_CONFIG:
//            Set_ICP_mode(ms8005otp.usb_hid_rx_buffer[1]);
            break;
        case 0xbb:      //读编程器数据标志
            ms8005otp.otp_read_addr = ms8005otp.usb_hid_rx_buffer[1] + (uint16_t)ms8005otp.usb_hid_rx_buffer[2]*256;
            ms8005otp.otp_read_length = ms8005otp.usb_hid_rx_buffer[3] + (uint16_t)ms8005otp.usb_hid_rx_buffer[4]*256;
            break;
        case 0xcc:      //读芯片数据标志
            ms8005otp.otp_read_addr = ms8005otp.usb_hid_rx_buffer[1] + (uint16_t)ms8005otp.usb_hid_rx_buffer[2]*256;
            ms8005otp.otp_read_length = ms8005otp.usb_hid_rx_buffer[3] + (uint16_t)ms8005otp.usb_hid_rx_buffer[4]*256;
            break;
        case 0xdd:      //写数据标志，编程芯片
            ms8005otp.otp_write_addr = ms8005otp.usb_hid_rx_buffer[1] + (uint16_t)ms8005otp.usb_hid_rx_buffer[2]*256;
            ms8005otp.otp_write_length = ms8005otp.usb_hid_rx_buffer[3];
            ICP_write(ms8005otp.otp_write_addr,ms8005otp.otp_write_length, &ms8005otp.usb_hid_rx_buffer[4]);
            break;
        case 0xee:      //将数据下载到STM32编程器，待处理
            ms8005otp.otp_write_addr = ms8005otp.usb_hid_rx_buffer[1] + (uint16_t)ms8005otp.usb_hid_rx_buffer[2]*256;
            ms8005otp.otp_write_length = ms8005otp.usb_hid_rx_buffer[3];
            if(ms8005otp.otp_write_addr == 0)
            {
//                clear_memory();   // 地址为0 ，擦除MEMORY;
            }
//            STMFLASH_Write(ms8005otp.otp_write_addr  + STM32_FLASH_DATA_BASE,(u16 *)(&ms8005otp.usb_hid_rx_buffer[4]),(ms8005otp.otp_write_length/2 + ms8005otp.otp_write_length%2));
            break;
        default:
            break;
    }
}

/*******************************************************************************
* Function Name  : Msotptool_Status_In.
* Description    : Msotptool status IN routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Msotptool_Status_In(void)
{
    if(ms8005otp.set_report_flag == 0x5a)   // 指示已经完成Set_report数据接收 关闭USB 待完成数据处理后才重新打开
    {
        ms8005otp.set_report_flag = 0xcc;
        set_report_deocder();
    }
}

/*******************************************************************************
* Function Name  : Msotptool_Status_Out
* Description    : Msotptool status OUT routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Msotptool_Status_Out (void)
{
}



uint8_t *get_report(uint16_t Length)
{
    uint8_t i;
    uint16_t j;
    if(Length == 0)
    {    
            pInformation->Ctrl_Info.Usb_wLength = 64;       
            return 0; 
    }
    switch(ms8005otp.usb_hid_rx_buffer[0])  
    {
        case MS_HID_CMD_GET_VERSION:
            ms8005otp.usb_hid_tx_buffer[0] = MS_HID_CMD_GET_VERSION;
            ms8005otp.usb_hid_tx_buffer[1] = MS_LINK_VERSION_L;
            ms8005otp.usb_hid_tx_buffer[2] = MS_LINK_VERSION_H;
            break;
        case MS_HID_CMD_GET_CONFIG:
            ms8005otp.usb_hid_tx_buffer[0] = MS_HID_CMD_GET_CONFIG;
//            ms8005otp.usb_hid_tx_buffer[1] = Get_ICP_mode();
            break;
        case 0x22:      //icp status.
            ms8005otp.usb_hid_tx_buffer[0] = 0x22;
            ms8005otp.usb_hid_tx_buffer[1] = ms8005otp.icp_status;
            break;
        case 0xcc:      // 读芯片
            ms8005otp.usb_hid_tx_buffer[0] = 0xcc;
            ms8005otp.usb_hid_tx_buffer[1] = ms8005otp.otp_read_addr;   //低位
            ms8005otp.usb_hid_tx_buffer[2] = ms8005otp.otp_read_addr>>8;//高位
            if(ms8005otp.otp_read_length>=60)
            {
                 ms8005otp.usb_hid_tx_buffer[3] = 60;
                 ms8005otp.otp_read_length = ms8005otp.otp_read_length  - 60;   //剩下的数据长度
            }
            else
            {
                ms8005otp.usb_hid_tx_buffer[3] = ms8005otp.otp_read_length;
                ms8005otp.otp_read_length = 0;   //已经发送全部数据
            }
            ICP_read(ms8005otp.otp_read_addr,ms8005otp.usb_hid_tx_buffer[3],&ms8005otp.usb_hid_tx_buffer[4]);
            ms8005otp.otp_read_addr = ms8005otp.otp_read_addr+ ms8005otp.usb_hid_tx_buffer[3];  //下次发送数据起始地址
            break;
        case 0xbb:  // 读编程器
            ms8005otp.usb_hid_tx_buffer[0] = 0xbb;
            ms8005otp.usb_hid_tx_buffer[1] = ms8005otp.otp_read_addr;   //低位
            ms8005otp.usb_hid_tx_buffer[2] = ms8005otp.otp_read_addr>>8;//高位
            if(ms8005otp.otp_read_length>=60)
            {
                 ms8005otp.usb_hid_tx_buffer[3] = 60;
                 ms8005otp.otp_read_length = ms8005otp.otp_read_length  - 60;   //剩下的数据长度
            }
            else
            {
                ms8005otp.usb_hid_tx_buffer[3] = ms8005otp.otp_read_length;
                ms8005otp.otp_read_length = 0;   //已经发送全部数据
            }
            for(i = 0;i<ms8005otp.usb_hid_tx_buffer[3]/2 + ms8005otp.usb_hid_tx_buffer[3]%2 ;i++)
            {
//                j = STMFLASH_ReadHalfWord(STM32_FLASH_DATA_BASE + ms8005otp.otp_read_addr+2*i);
                ms8005otp.usb_hid_tx_buffer[2*i+4] = j;
                ms8005otp.usb_hid_tx_buffer[2*i+1+4] = j>>8;
            }
            ms8005otp.otp_read_addr = ms8005otp.otp_read_addr+ ms8005otp.usb_hid_tx_buffer[3];  //下次发送数据起始地址
            break;
        default:
            break;
    }
    return  (uint8_t *)&ms8005otp.usb_hid_tx_buffer;
}



uint8_t *set_report(uint16_t Length)
{   
    if(Length == 0)
    {    
            pInformation->Ctrl_Info.Usb_rLength = 64;
            return 0; 
    }
    ms8005otp.set_report_flag = 0x5a;
    return  (uint8_t *)&ms8005otp.usb_hid_rx_buffer;    
}

uint8_t IDLE_TIME = 0   ;
uint8_t *get_idle(uint16_t Length)
{
    if(Length == 0)
    {    
            pInformation->Ctrl_Info.Usb_wLength = 1;
            return 0; 
    }
    return  (uint8_t *)&IDLE_TIME;
}

/*******************************************************************************
* Function Name  : Msotptool_Data_Setup
* Description    : Handle the data class specific requests.
* Input          : Request Nb.
* Output         : None.
* Return         : USB_UNSUPPORT or USB_SUCCESS.
*******************************************************************************/
RESULT Msotptool_Data_Setup(uint8_t RequestNo)
{
    uint8_t *(*CopyRoutine)(uint16_t);

    CopyRoutine = NULL;
    if ((RequestNo == GET_DESCRIPTOR)
      && (Type_Recipient == (STANDARD_REQUEST | INTERFACE_RECIPIENT))
      && (pInformation->USBwIndex0 == 0))
    {
        if (pInformation->USBwValue1 == REPORT_DESCRIPTOR)
        {
            CopyRoutine = Msotptool_GetReportDescriptor;
        }
        else if (pInformation->USBwValue1 == HID_DESCRIPTOR_TYPE)
        {
            CopyRoutine = Msotptool_GetHIDDescriptor;
        }

    } /* End of GET_DESCRIPTOR */

  /*** GET_PROTOCOL ***/
    else if ((Type_Recipient == (CLASS_REQUEST | INTERFACE_RECIPIENT))
              && RequestNo == GET_PROTOCOL)
    {
        CopyRoutine = Msotptool_GetProtocolValue;
    }
    
    else if((pInformation->USBbmRequestType & 0x20) && (pInformation->USBbRequest == GET_REPORT)) // GET_REPORT
    {
        CopyRoutine = get_report;
    }
    else if((pInformation->USBbmRequestType & 0x20) && (pInformation->USBbRequest == GET_IDLE)) //GET_IDLE
    {
        CopyRoutine = get_idle;
    }
    else if((pInformation->USBbmRequestType & 0x20) && (pInformation->USBbRequest == SET_REPORT))       //SET_REPORT
    {
        CopyRoutine =set_report;
    }
    
    
    if (CopyRoutine == NULL)
    {
        return USB_UNSUPPORT;
    }
    pInformation->Ctrl_Info.CopyData = CopyRoutine;
    pInformation->Ctrl_Info.Usb_wOffset = 0;
    (*CopyRoutine)(0);
    return USB_SUCCESS;
}

void set_idle()
{
    IDLE_TIME =  pInformation->USBwValues.bw.bb0;   
}

/*******************************************************************************
* Function Name  : Msotptool_NoData_Setup
* Description    : handle the no data class specific requests
* Input          : Request Nb.
* Output         : None.
* Return         : USB_UNSUPPORT or USB_SUCCESS.
*******************************************************************************/
RESULT Msotptool_NoData_Setup(uint8_t RequestNo)
{
    if ((Type_Recipient == (CLASS_REQUEST | INTERFACE_RECIPIENT))
         && (RequestNo == SET_PROTOCOL))
    {
        return Msotptool_SetProtocol();
    }
    else if((pInformation->USBbmRequestType & 0x20) && (pInformation->USBbRequest == SET_IDLE)) // //SET_IDLE
    {
        set_idle();
        return USB_SUCCESS;
    }
    else
    {
        return USB_UNSUPPORT;
    }
}

/*******************************************************************************
* Function Name  : Msotptool_GetDeviceDescriptor.
* Description    : Gets the device descriptor.
* Input          : Length
* Output         : None.
* Return         : The address of the device descriptor.
*******************************************************************************/
uint8_t *Msotptool_GetDeviceDescriptor(uint16_t Length)
{
    return Standard_GetDescriptorData(Length, &Device_Descriptor);
}

/*******************************************************************************
* Function Name  : Msotptool_GetConfigDescriptor.
* Description    : Gets the configuration descriptor.
* Input          : Length
* Output         : None.
* Return         : The address of the configuration descriptor.
*******************************************************************************/
uint8_t *Msotptool_GetConfigDescriptor(uint16_t Length)
{
    return Standard_GetDescriptorData(Length, &Config_Descriptor);
}

/*******************************************************************************
* Function Name  : Msotptool_GetStringDescriptor
* Description    : Gets the string descriptors according to the needed index
* Input          : Length
* Output         : None.
* Return         : The address of the string descriptors.
*******************************************************************************/
uint8_t *Msotptool_GetStringDescriptor(uint16_t Length)
{
  uint8_t wValue0 = pInformation->USBwValue0;
  if (wValue0 > 4)
  {
    return NULL;
  }
  else
  {
    return Standard_GetDescriptorData(Length, &String_Descriptor[wValue0]);
  }
}

/*******************************************************************************
* Function Name  : Msotptool_GetReportDescriptor.
* Description    : Gets the HID report descriptor.
* Input          : Length
* Output         : None.
* Return         : The address of the configuration descriptor.
*******************************************************************************/
uint8_t *Msotptool_GetReportDescriptor(uint16_t Length)
{
  return Standard_GetDescriptorData(Length, &Msotptool_Report_Descriptor);
}

/*******************************************************************************
* Function Name  : Msotptool_GetHIDDescriptor.
* Description    : Gets the HID descriptor.
* Input          : Length
* Output         : None.
* Return         : The address of the configuration descriptor.
*******************************************************************************/
uint8_t *Msotptool_GetHIDDescriptor(uint16_t Length)
{
  return Standard_GetDescriptorData(Length, &Mouse_Hid_Descriptor);
}

/*******************************************************************************
* Function Name  : Msotptool_Get_Interface_Setting.
* Description    : tests the interface and the alternate setting according to the
*                  supported one.
* Input          : - Interface : interface number.
*                  - AlternateSetting : Alternate Setting number.
* Output         : None.
* Return         : USB_SUCCESS or USB_UNSUPPORT.
*******************************************************************************/
RESULT Msotptool_Get_Interface_Setting(uint8_t Interface, uint8_t AlternateSetting)
{
  if (AlternateSetting > 0)
  {
    return USB_UNSUPPORT;
  }
  else if (Interface > 0)
  {
    return USB_UNSUPPORT;
  }
  return USB_SUCCESS;
}

/*******************************************************************************
* Function Name  : Msotptool_SetProtocol
* Description    : Msotptool Set Protocol request routine.
* Input          : None.
* Output         : None.
* Return         : USB SUCCESS.
*******************************************************************************/
RESULT Msotptool_SetProtocol(void)
{
  uint8_t wValue0 = pInformation->USBwValue0;
  ProtocolValue = wValue0;
  return USB_SUCCESS;
}

/*******************************************************************************
* Function Name  : Msotptool_GetProtocolValue
* Description    : get the protocol value
* Input          : Length.
* Output         : None.
* Return         : address of the protocol value.
*******************************************************************************/
uint8_t *Msotptool_GetProtocolValue(uint16_t Length)
{
  if (Length == 0)
  {
    pInformation->Ctrl_Info.Usb_wLength = 1;
    return NULL;
  }
  else
  {
    return (uint8_t *)(&ProtocolValue);
  }
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
