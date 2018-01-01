/***************************************/
/********����ԭ������ɿƼ�**************/
/***************************************/
/*������2017.11.8  By Alex*/


#include "MK60_uart.h"



UART_MemMapPtr UARTN[5] = {UART0_BASE_PTR, UART1_BASE_PTR, UART2_BASE_PTR, UART3_BASE_PTR, UART4_BASE_PTR}; //�������ָ�����鱣�� UARTN �ĵ�ַ

void uart_init(UARTn uartn, uint32 baud)
{
    register uint16 sbr, brfa;
    uint32 uartsclk;     //ʱ��
    //���йܽŸ�������
    switch(uartn)
    {
        case uart0:
        {
            SIM_SCGC4 |= SIM_SCGC4_UART0_MASK;      //ʹ�� UART0 ʱ��
            if(UART0_RX_PIN == A1)                                                             
            {
              PORT_init( UART0_RX_PIN, ALT2);
            }
            else if((UART0_RX_PIN == A15) || (UART0_RX_PIN == B16) || (UART0_RX_PIN == D6) )    
            {
              PORT_init( UART0_RX_PIN, ALT3);
            }

            if(UART0_TX_PIN == A2)    
            {
              PORT_init( UART0_TX_PIN, ALT2);
            }
            else if((UART0_TX_PIN == A14) || (UART0_TX_PIN == B17) || (UART0_TX_PIN == D7) )   
            {
              PORT_init( UART0_TX_PIN, ALT3);
            }
        }break;
        
        case uart1:
        {
            SIM_SCGC4 |= SIM_SCGC4_UART1_MASK;      //ʹ�� UART1 ʱ��
            if((UART1_RX_PIN == C3) || (UART1_RX_PIN == E1))       
            {
              PORT_init( UART1_RX_PIN, ALT3);
            }
            if((UART1_TX_PIN == C4) || (UART1_TX_PIN == E0))           
            {
              PORT_init( UART1_TX_PIN, ALT3);
            }
        }break;
        
        case uart2:
        {
            SIM_SCGC4 |= SIM_SCGC4_UART2_MASK;      //ʹ�� UART2 ʱ��
            if(UART2_TX_PIN == D3)           
            {
              PORT_init( UART2_TX_PIN, ALT3);
            }
            if(UART2_RX_PIN == D2)      
            {
              PORT_init( UART2_RX_PIN, ALT3);
            }
        }break;
        
        case uart3:
        {
            SIM_SCGC4 |= SIM_SCGC4_UART3_MASK;      //ʹ�� UART3 ʱ��
            if((UART3_RX_PIN == B10) || (UART3_RX_PIN == C16) || (UART3_RX_PIN == E5) )   
            {
              PORT_init( UART3_RX_PIN, ALT3);
            }
            if((UART3_TX_PIN == B11) || (UART3_TX_PIN == C17) || (UART3_TX_PIN == E4) ) 
            {
              PORT_init( UART3_TX_PIN, ALT3);
            }
        }break;
        
        case uart4:
        {
            SIM_SCGC1 |= SIM_SCGC1_UART4_MASK;      //ʹ�� UART4 ʱ��
            if((UART4_RX_PIN == C14) || (UART4_RX_PIN == E25)  )        
            {
              PORT_init( UART4_RX_PIN, ALT3);
            }
            if((UART4_TX_PIN == C15) || (UART4_TX_PIN == E24)  )       
            {
              PORT_init( UART4_TX_PIN, ALT3);
            }
        }break;
        default:
        return;
    }
    
    //���õ�ʱ��Ӧ�ý�ֹ���ͺͽ���
    UARTN[uartn]->C2 &= ~(0
                         | UART_C2_TE_MASK
                         | UART_C2_RE_MASK);
    
    //���ó�8λ��У��ģʽ
    //���� UART ���ݸ�ʽ��У�鷽ʽ��ֹͣλλ����ͨ������ UART ģ����ƼĴ��� C1 ʵ�֣�
    UARTN[uartn]->C1 |= (0
                        //| UART_C1_M_MASK                      //9 λ�� 8 λģʽѡ�� : 0 Ϊ 8λ ��1 Ϊ 9λ��ע���˱�ʾ0����8λ�� �������9λ��λ8��UARTx_C3�
                        //| UART_C1_PE_MASK                     //��żУ��ʹ�ܣ�ע���˱�ʾ���ã�
                        //| UART_C1_PT_MASK                     //У��λ���� : 0 Ϊ żУ�� ��1 Ϊ ��У��
                       );
    
    //���㲨���ʣ�����0��1ʹ���ں�ʱ�ӣ���������ʹ��busʱ��
    if ((uartn == uart0) || (uartn == uart1)) uartsclk = SysClock.core_khz * 1000;  //�ں�ʱ��
    else                                            uartsclk = SysClock.bus_khz * 1000;   //busʱ��

    //UART ������ = UART ģ��ʱ�� / (16 �� (SBR[12:0] + BRFA/32))
    //������ BRFA ������£� SBR = UART ģ��ʱ�� / (16 * UART ������)
    sbr = (uint16)(uartsclk / (baud * 16));
    if(sbr > 0x1FFF)sbr = 0x1FFF;                                       //SBR �� 13bit�����Ϊ 0x1FFF

    //��֪ SBR ���� BRFA = UART ģ��ʱ�� * 2/ UART ������ - 32 ��SBR[12:0]
    brfa = (uartsclk * 2 / baud)  - (sbr * 32);
    if(brfa>0x1F)   brfa = 0x1F;
    
    //д SBR
    UARTN[uartn]->BDH &= ~UART_BDH_SBR_MASK;                     //���ԭ��������
    UARTN[uartn]->BDH |= UART_BDH_SBR(sbr>>8);                   //��д��SBR��λ
    UARTN[uartn]->BDL  = UART_BDL_SBR((uint8)sbr);               //��д��SBR��λ

    //д BRFD
    UARTN[uartn]->C4 &= ~UART_C4_BRFA_MASK;
    UARTN[uartn]->C4 |= UART_C4_BRFA(brfa);
    
    //����FIFO(FIFO���������Ӳ�������ģ������������)
    UARTN[uartn]->PFIFO |= (0
                           //| UART_PFIFO_TXFE_MASK               //ʹ��TX FIFO(ע�ͱ�ʾ��ֹ)
                           //| UART_PFIFO_TXFIFOSIZE(0)         //��ֻ����TX FIFO ��С��0Ϊ1�ֽڣ�1~6Ϊ 2^(n+1)�ֽ�
                           //| UART_PFIFO_RXFE_MASK               //ʹ��RX FIFO(ע�ͱ�ʾ��ֹ)
                           //| UART_PFIFO_RXFIFOSIZE(0)         //��ֻ����RX FIFO ��С��0Ϊ1�ֽڣ�1~6Ϊ 2^(n+1)�ֽ�
                           );
    
    UARTN[uartn]->C2 |= (0
                        | UART_C2_TE_MASK                       //����ʹ��
                        | UART_C2_RE_MASK                       //����ʹ��
                        //| UART_C2_TIE_MASK                    //�����жϻ�DMA��������ʹ�ܣ�ע���˱�ʾ���ã�
                        //| UART_C2_TCIE_MASK                   //��������ж�ʹ�ܣ�ע���˱�ʾ���ã�
                        //| UART_C2_RIE_MASK                    //�������жϻ�DMA��������ʹ�ܣ�ע���˱�ʾ���ã�
                        );
}

void uart_putchar(UARTn uartn, uint8 ch)
{
  while(!((UARTN[uartn]->S1) & UART_S1_TDRE_MASK) );  //�ȴ����ͻ�������
  UARTN[uartn]->D = (uint8)ch;                         //������
}

void uart_putbuff (UARTn uartn, uint8 *buff, uint32 len)
{
    while(len--)
    {
        uart_putchar(uartn, *buff);
        buff++;
    }
}

void uart_putstr (UARTn uartn, const uint8 *str)
{
    while(*str)
    {
        uart_putchar(uartn, *str++);
    }
}

void uart_getchar (UARTn uartn, uint8 *ch) 
{
    while(!(UARTN[uartn]->S1 & UART_S1_RDRF_MASK));  //�ȴ���������   
    *ch =  UARTN[uartn]->D;                          // ��ȡ���յ���8λ����
}

char uart_querychar (UARTn uartn, uint8 *ch)
{
    if( UARTN[uartn]->RCFIFO )         //��ѯ�Ƿ���ܵ�����
    {
        *ch  =   UARTN[uartn]->D;      //���ܵ�8λ������
        return  1;                    //���� 1 ��ʾ���ճɹ�
    }
    return 0;                         //����0��ʾ����ʧ��
}

int fputc(int ch, FILE *f)            //printf�ض���
{
  uart_putchar(uart0, (uint8)ch);
  return ch;
}