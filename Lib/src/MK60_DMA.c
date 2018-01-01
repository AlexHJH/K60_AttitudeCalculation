/***************************************/
/********����ԭ������ɿƼ�**************/
/***************************************/
/*������2017.11.8  By Alex*/

#include "MK60_DMA.h"



//-------------------------------------------------------------------------------------------------------------------
//  @brief      DMA��ʼ������IO������������˿ڵ����ݵ��ڴ�
//  @param      DMA_CHn         ͨ���ţ�DMA_CH0 ~ DMA_CH15��
//  @param      SADDR           Դ��ַ( (void * )&X_IN_DATA
//  @param      DADDR           Ŀ�ĵ�ַ
//  @param      PTxn            �����˿�
//  @param      DMA_BYTEn       ÿ��DMA�����ֽ���
//  @param      count           һ����ѭ�������ֽ���
//  @param      cfg             DMA�������ã���DMA_cfg��ѡ��
//  @return     void
//  @since      v1.0
//  Sample usage:   uint8 dat[10];
//                  dma_portx2buff_init(DMA_CH0, (void *)&C_IN_DATA(0), dat, A7, DMA_BYTE1, 10, DADDR_RECOVER);
//                  												//DMA��ʼ����Դ��ַ��C_IN_DATA(��C0-C7)��Ŀ�ĵ�ַ��dat,A7����(Ĭ��������)��ÿ�δ���1�ֽڣ������� 10�� �����������ָ���ַ
//                  port_init(A7,ALT1 | DMA_FALLING);               //Ĭ�ϴ���Դ�������أ��˴���Ϊ �½��ش���
//                  DMA_EN(DMA_CH0);                				//��Ҫʹ�� DMA ����ܴ�������
//-------------------------------------------------------------------------------------------------------------------
void DMA_DDR_init(DMA_CHn CHn, void *SADDR, void *DADDR, PTX_n ptxn, DMA_BYTEn byten, uint32 count, uint32 cfg)
{
    uint8 BYTEs = 1;
    uint8 i = DMA_BYTE1;
    
    while(i)//���㴫���ֽ���
    {
        BYTEs *= 2;
        i--;
    }
    //DMA �Ĵ��� ����

    // ����ʱ�� 
    SIM_SCGC7 |= SIM_SCGC7_DMA_MASK;                        //��DMAģ��ʱ��
    
#if (defined(MK60D10) || defined(MK60DZ10) || defined(MK66F18))
    SIM_SCGC6 |= SIM_SCGC6_DMAMUX_MASK;                     //��DMA��·������ʱ��
#elif defined(MK60F15)
    SIM_SCGC6 |= SIM_SCGC6_DMAMUX0_MASK;                    //��DMA��·������ʱ��
#endif

    
    
    // ���� DMA ͨ�� �� ������ƿ� TCD ( Transfer Control Descriptor ) 
    DMA_BASE_PTR->TCD[CHn].SADDR = 0;
    DMA_BASE_PTR->TCD[CHn].SADDR = (uint32)SADDR;                   // ����  Դ��ַ
    DMA_BASE_PTR->TCD[CHn].DADDR = 0;                               
    DMA_BASE_PTR->TCD[CHn].DADDR = (uint32)DADDR;                   // ����Ŀ�ĵ�ַ
    DMA_BASE_PTR->TCD[CHn].SOFF = 0;                                
    DMA_BASE_PTR->TCD[CHn].SOFF = 0x00u;                            // ����Դ��ַƫ�� = 0x0, ������
    DMA_BASE_PTR->TCD[CHn].DOFF = 0;                                
    DMA_BASE_PTR->TCD[CHn].DOFF = BYTEs;                            // ÿ�δ����Ŀ�ĵ�ַ�� BYTEs
    DMA_BASE_PTR->TCD[CHn].ATTR = 0;
    DMA_BASE_PTR->TCD[CHn].ATTR = (0
                         | DMA_ATTR_SMOD(0x0)               // Դ��ַģ����ֹ  Source address modulo feature is disabled
                         | DMA_ATTR_SSIZE(byten)            // Դ����λ�� ��DMA_BYTEn  ��    SSIZE = 0 -> 8-bit ��SSIZE = 1 -> 16-bit ��SSIZE = 2 -> 32-bit ��SSIZE = 4 -> 16-byte
                         | DMA_ATTR_DMOD(0x0)               // Ŀ���ַģ����ֹ
                         | DMA_ATTR_DSIZE(byten)            // Ŀ������λ�� ��DMA_BYTEn  ��  ���òο�  SSIZE
                        );
    DMA_BASE_PTR->TCD[CHn].CITER_ELINKNO = DMA_CITER_ELINKNO_CITER(count);   //��ǰ��ѭ������ ��ǰѭ�������ľ����ٴμ�����ʼ��ѭ������
    DMA_BASE_PTR->TCD[CHn].BITER_ELINKNO = DMA_BITER_ELINKNO_BITER(count);   //��ʼ��ѭ������
    DMA_BASE_PTR->CR &= ~DMA_CR_EMLM_MASK;                                // CR[EMLM] = 0
    DMA_BASE_PTR->TCD[CHn].NBYTES_MLNO = DMA_NBYTES_MLNO_NBYTES(BYTEs); // ͨ��ÿ�δ����ֽ�������������ΪBYTEs���ֽڡ�ע��ֵΪ0��ʾ����4GB 
    
    
    // ���� DMA ���������Ĳ��� 
    DMA_BASE_PTR->TCD[CHn].SLAST = 0;//����  Դ��ַ�ĸ���ֵ,��ѭ��������ָ�  Դ��ַ
    DMA_BASE_PTR->TCD[CHn].DLAST_SGA = (uint32)( (cfg & DADDR_KEEPON ) == 0 ? (-count)  : 0 ); //����Ŀ�ĵ�ַ�ĸ���ֵ,��ѭ��������ָ�Ŀ�ĵ�ַ���߱��ֵ�ַ;
    DMA_BASE_PTR->TCD[CHn].CSR = 0;
    DMA_BASE_PTR->TCD[CHn].CSR = (0
                             | DMA_CSR_BWC(3)               //�������,ÿ��һ�Σ�eDMA ����ֹͣ 8 �����ڣ�0��ֹͣ��1������2ֹͣ4���ڣ�3ֹͣ8���ڣ�
                             | DMA_CSR_DREQ_MASK            //��ѭ��������ֹͣӲ������
                             | DMA_CSR_INTMAJOR_MASK        //��ѭ������������ж�
                            );
    DMA_BASE_PTR->TCD[CHn].SLAST = 0;
    
    
    // ���� DMA ����Դ 

#if (defined(MK60D10) || defined(MK60DZ10) || defined(MK66F18))
    DMAMUX_BASE_PTR->CHCFG[CHn] = (0
#elif (defined(MK60F15))
    DMAMUX0_BASE_PTR->CHCFG[CHn] = (0
#endif
                            | DMAMUX_CHCFG_ENBL_MASK                        // Enable routing of DMA request 
                            //| DMAMUX_CHCFG_TRIG_MASK                      // Trigger Mode: Periodic   PIT���ڴ�������ģʽ   ͨ��1��ӦPIT1������ʹ��PIT1����������Ӧ��PIT��ʱ���� 
                            | DMAMUX_CHCFG_SOURCE(PTX(ptxn) + DMA_PORTA)  // ͨ����������Դ:    
                          );
    
    DMA_DIS(CHn);                                    //ʹ��ͨ��CHn Ӳ������
    DMA_IRQ_CLEAN(CHn);
}