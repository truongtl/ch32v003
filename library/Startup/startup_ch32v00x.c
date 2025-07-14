;/********************************** (C) COPYRIGHT *******************************
;* File Name          : startup_ch32v00x.c
;* Author             : WCH
;* Version            : V1.0.1
;* Date               : 2023/12/11
;* Description        : vector table for eclipse toolchain.
;*********************************************************************************
;* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
;* Attention: This software (modified or not) and binary are used for 
;* microcontroller manufactured by Nanjing Qinheng Microelectronics.
;*******************************************************************************/

#include <stdio.h>
#include <stdint.h>

extern uint32_t _eusrstack;

extern uint32_t _data_lma;
extern uint32_t _data_vma;
extern uint32_t _edata;

extern uint32_t _sbss;
extern uint32_t _ebss;

extern uint32_t _highcode_lma;
extern uint32_t _highcode_vma_start;
extern uint32_t _highcode_vma_end;

extern void main(void);
extern void SystemInit(void);

void Dummy_Handler(void) {};

void Reset_Handler(void) __attribute__((weak));
void NMI_Handler(void) __attribute__((weak, alias("Dummy_Handler")));
void HardFault_Handler(void) __attribute__((weak, alias("Dummy_Handler")));
void SysTick_Handler(void) __attribute__((weak, alias("Dummy_Handler")));
void SW_Handler(void) __attribute__((weak, alias("Dummy_Handler")));
void WWDG_Handler(void) __attribute__((weak, alias("Dummy_Handler")));
void PVD_Handler(void) __attribute__((weak, alias("Dummy_Handler")));
void FLASH_Handler(void) __attribute__((weak, alias("Dummy_Handler")));
void RCC_Handler(void) __attribute__((weak, alias("Dummy_Handler")));
void EXTI7_0_Handler(void) __attribute__((weak, alias("Dummy_Handler")));
void AWU_Handler(void) __attribute__((weak, alias("Dummy_Handler")));
void DMA1_CH1_Handler(void) __attribute__((weak, alias("Dummy_Handler")));
void DMA1_CH2_Handler(void) __attribute__((weak, alias("Dummy_Handler")));
void DMA1_CH3_Handler(void) __attribute__((weak, alias("Dummy_Handler")));
void DMA1_CH4_Handler(void) __attribute__((weak, alias("Dummy_Handler")));
void DMA1_CH5_Handler(void) __attribute__((weak, alias("Dummy_Handler")));
void DMA1_CH6_Handler(void) __attribute__((weak, alias("Dummy_Handler")));
void DMA1_CH7_Handler(void) __attribute__((weak, alias("Dummy_Handler")));
void ADC_Handler(void) __attribute__((weak, alias("Dummy_Handler")));
void I2C1_EV_Handler(void) __attribute__((weak, alias("Dummy_Handler")));
void I2C1_ER_Handler(void) __attribute__((weak, alias("Dummy_Handler")));
void USART1_Handler(void) __attribute__((weak, alias("Dummy_Handler")));
void SPI1_Handler(void) __attribute__((weak, alias("Dummy_Handler")));
void TIM1BRK_Handler(void) __attribute__((weak, alias("Dummy_Handler")));
void TIM1UP_Handler(void) __attribute__((weak, alias("Dummy_Handler")));
void TIM1TRG_Handler(void) __attribute__((weak, alias("Dummy_Handler")));
void TIM1CC_Handler(void) __attribute__((weak, alias("Dummy_Handler")));
void TIM2_Handler(void) __attribute__((weak, alias("Dummy_Handler")));

__attribute__ ((section(".vector"))) void (*const vector_table[])(void) =
{
    Dummy_Handler,
    NMI_Handler,
    HardFault_Handler,
    Dummy_Handler,
    Dummy_Handler,
    Dummy_Handler,
    Dummy_Handler,
    Dummy_Handler,
    Dummy_Handler,
    Dummy_Handler,
    Dummy_Handler,
    SysTick_Handler,
    Dummy_Handler,
    SW_Handler,
    Dummy_Handler,
    WWDG_Handler,
    PVD_Handler,
    FLASH_Handler,
    RCC_Handler,
    EXTI7_0_Handler,
    AWU_Handler,
    DMA1_CH1_Handler,
    DMA1_CH2_Handler,
    DMA1_CH3_Handler,
    DMA1_CH4_Handler,
    DMA1_CH5_Handler,
    DMA1_CH6_Handler,
    DMA1_CH7_Handler,
    ADC_Handler,
    I2C1_EV_Handler,
    I2C1_ER_Handler,
    USART1_Handler,
    SPI1_Handler,
    TIM1BRK_Handler,
    TIM1UP_Handler,
    TIM1TRG_Handler,
    TIM1CC_Handler,
    TIM2_Handler
};

__attribute__((section(".init"))) void Reset_Handler(void)
{
    __asm__ volatile (
        // Set global pointer
        "la gp, __global_pointer$\n"
        // Set stack pointer
        "la sp, _eusrstack\n"
    );

    // Copy highcode section
    uint32_t *src = &_highcode_lma;
    uint32_t *dst = &_highcode_vma_start;
    while (dst < &_highcode_vma_end)
        *dst++ = *src++;

    // Copy data section
    src = &_data_lma;
    dst = &_data_vma;
    while (dst < &_edata)
        *dst++ = *src++;

    // Clear BSS section
    dst = &_sbss;
    while (dst < &_ebss)
        *dst++ = 0;

    // Setup CSRs
    __asm__ volatile (
        // Enable global interrupt (MIE)
        "li t0, 0x1880\n"
        "csrw mstatus, t0\n"
        // Enable nested interrupt & hardware stack
        "li t0, 0x3\n"
        "csrw 0x804, t0\n"
        // Setup mtvec
        "la t0, vector_table\n"
        "ori t0, t0, 3\n"
        "csrw mtvec, t0\n"
    );

    SystemInit();

    __asm__ volatile (
        "la t0, main\n"
        "csrw mepc, t0\n"
        "mret\n"
    );
}
