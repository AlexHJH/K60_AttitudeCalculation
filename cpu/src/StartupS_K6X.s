/*****************************************************/      
/*Auther: ���ϴ�ѧ�ƾ���                             */       
/*DATE  �� 2016/12/01                                */      
/*DISCRIBE:Kinetis MK6Xϵ����������ļ�(����Ӳ������)*/     
/*****************************************************/    
 
       SECTION .noinit : CODE        
       EXPORT  Reset_Handler    ;����Reset��λ����
Reset_Handler
       MOV r1, #0
       MOV r2, #0
       MOV r3, #0
       MOV r4, #0
       MOV r5, #0
       MOV r6, #0
       MOV r7, #0
       MOV r8, #0
       MOV r9, #0
       MOV r10, #0
       MOV r11, #0
       MOV r12, #0                ;���ͨ�üĴ���
       CPSIE   i   
       import start                   ;���ļ��е���start����C����
       BL     start                    ;ת��start
__done
       B       __done       
       END