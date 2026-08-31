/******************************************************************************
* File Name:   main.c
*
* Description: This is the source code for the PSOC Control C1 MCU: CAN Receiver example
*              for ModusToolbox. The CAN node 1 is configured to receive a CAN
*              message over the CAN bus. Successful message reception is
*              indicated by toggling the USER LED1. The USER LED2 is updated based
*              on the message received in the CAN frame.
*
* Related Document: See README.md
*
*******************************************************************************
* (c) 2026, Infineon Technologies AG, or an affiliate of Infineon
* Technologies AG. All rights reserved.
* This software, associated documentation and materials ("Software") is
* owned by Infineon Technologies AG or one of its affiliates ("Infineon")
* and is protected by and subject to worldwide patent protection, worldwide
* copyright laws, and international treaty provisions. Therefore, you may use
* this Software only as provided in the license agreement accompanying the
* software package from which you obtained this Software. If no license
* agreement applies, then any use, reproduction, modification, translation, or
* compilation of this Software is prohibited without the express written
* permission of Infineon.
*
* Disclaimer: UNLESS OTHERWISE EXPRESSLY AGREED WITH INFINEON, THIS SOFTWARE
* IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
* INCLUDING, BUT NOT LIMITED TO, ALL WARRANTIES OF NON-INFRINGEMENT OF
* THIRD-PARTY RIGHTS AND IMPLIED WARRANTIES SUCH AS WARRANTIES OF FITNESS FOR A
* SPECIFIC USE/PURPOSE OR MERCHANTABILITY.
* Infineon reserves the right to make changes to the Software without notice.
* You are responsible for properly designing, programming, and testing the
* functionality and safety of your intended application of the Software, as
* well as complying with any legal requirements related to its use. Infineon
* does not guarantee that the Software will be free from intrusion, data theft
* or loss, or other breaches ("Security Breaches"), and Infineon shall have
* no liability arising out of any Security Breaches. Unless otherwise
* explicitly approved by Infineon, the Software may not be used in any
* application where a failure of the Product or any consequences of the use
* thereof can reasonably be expected to result in personal injury.
*******************************************************************************/

#include "cybsp.h"
#include "cy_utils.h"

/*******************************************************************************
* Defines
*******************************************************************************/

#define IRQ_NUMBER                  CAN0_3_IRQn           /* Interrupt number */
#define CAN_IRQ_HANDLER             CAN0_3_IRQHandler     /* CAN Interrupt Handler */

/*******************************************************************************
* Variables
*******************************************************************************/
/* Variable to indicate that CAN frame is received */
volatile bool frame_received = false;
/* Runtime receive message object copy (generated object is const). */
static Cy_CAN_MO_t can_rx_msg;


/*******************************************************************************
* Function Name: CAN_IRQ_HANDLER
********************************************************************************
* Summary:
* This is the interrupt handler function for the CAN node
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void CAN_IRQ_HANDLER(void)
{
    /* Receive the message into mutable runtime object. */
    Cy_CAN_MO_Receive(&can_rx_msg);

    /* Toggle LED1 to indicate that the message is received */
    Cy_GPIO_ToggleOutput(CYBSP_USER_LED_PORT, CYBSP_USER_LED_PIN);

    /* Set the frame received flag to true */
    frame_received = true;
}

/*******************************************************************************
* Function Name: main
********************************************************************************
* Summary:
* This is the main function. This function performs
*  - initial setup of device
*  - initialize CAN Node and receive message object
*  - turns USER LED2 based on the command received.
*
* Parameters:
*  none
*
* Return:
*  int
*
*******************************************************************************/
int main(void)
{
    cy_rslt_t result;

    /* Initialize the device and board peripherals */
    result = cybsp_init();
    if (result != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);
    }

    /* Use a mutable runtime copy for receive data updates. */
    can_rx_msg = CAN_NODE_LMO_0;

    /* Enable NVIC node */
    NVIC_EnableIRQ(IRQ_NUMBER);

    for(;;)
    {
        /* CAN Frame is received */
        if(frame_received)
        {

            #ifdef CYBSP_USER_LED2_PIN
            /* Update USER LED2 based on command received */
            if(can_rx_msg.can_data_byte[0] == 0)
            {
                Cy_GPIO_SetOutputLow(CYBSP_USER_LED2_PORT, CYBSP_USER_LED2_PIN);
            }
            else
            {
                Cy_GPIO_SetOutputHigh(CYBSP_USER_LED2_PORT, CYBSP_USER_LED2_PIN);
            }
            #endif

            /* Reset flag */
            frame_received = false;
        }
    }
}

/* [] END OF FILE */
