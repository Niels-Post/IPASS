.syntax	unified
.arch	armv7-m
.thumb

.global vtable
.global default_interrupt_handler

.type vtable, %object
.section .vectors,"a",%progbits
__vectors:
    .long    __stack_end           /* Top of Stack */
    .long    __startup             /* Reset Handler */
    .long    NMI_Handler           /* NMI Handler */
    .long    HardFault_Handler     /* Hard Fault Handler */
    .long    MemManage_Handler     /* MPU Fault Handler */
    .long    BusFault_Handler      /* Bus Fault Handler */
    .long    UsageFault_Handler    /* Usage Fault Handler */
    .long    0                     /* Reserved */
    .long    0                     /* Reser ved */
    .long    0                     /* Reserved */
    .long    0                     /* Reserved */
    .long    SVC_Handler           /* SVCall Handler */
    .long    DebugMon_Handler      /* Debug Monitor Handler */
    .long    0                     /* Reserved */
    .long    PendSV_Handler        /* PendSV Handler */
    .long    SysTick_Handler       /* SysTick Handler */

                // External Interrupts
    .long    WWDG_IRQHandler            // Window Watchdog
    .long    PVD_IRQHandler             // PVD through EXTI Line detect
    .long    TAMPER_IRQHandler          // Tamper
    .long    RTC_IRQHandler             // RTC
    .long    FLASH_IRQHandler           // Flash
    .long    RCC_IRQHandler             // RCC
    .long    EXTI0_IRQHandler           // EXTI Line 0
    .long    EXTI1_IRQHandler           // EXTI Line 1
    .long    EXTI2_IRQHandler           // EXTI Line 2
    .long    EXTI3_IRQHandler           // EXTI Line 3
    .long    EXTI4_IRQHandler           // EXTI Line 4
    .long    DMA1_Channel1_IRQHandler   // DMA1 Channel 1
    .long    DMA1_Channel2_IRQHandler   // DMA1 Channel 2
    .long    DMA1_Channel3_IRQHandler   // DMA1 Channel 3
    .long    DMA1_Channel4_IRQHandler   // DMA1 Channel 4
    .long    DMA1_Channel5_IRQHandler   // DMA1 Channel 5
    .long    DMA1_Channel6_IRQHandler   // DMA1 Channel 6
    .long    DMA1_Channel7_IRQHandler   // DMA1 Channel 7
    .long    ADC1_2_IRQHandler          // ADC1_2
    .long    USB_HP_CAN1_TX_IRQHandler  // USB High Priority or CAN1 TX
    .long    USB_LP_CAN1_RX0_IRQHandler // USB Low  Priority or CAN1 RX0
    .long    CAN1_RX1_IRQHandler        // CAN1 RX1
    .long    CAN1_SCE_IRQHandler        // CAN1 SCE
    .long    EXTI9_5_IRQHandler         // EXTI Line 9..5
    .long    TIM1_BRK_IRQHandler        // TIM1 Break
    .long    TIM1_UP_IRQHandler         // TIM1 Update
    .long    TIM1_TRG_COM_IRQHandler    // TIM1 Trigger and Commutation
    .long    TIM1_CC_IRQHandler         // TIM1 Capture Compare
    .long    TIM2_IRQHandler            // TIM2
    .long    TIM3_IRQHandler            // TIM3
    .long    TIM4_IRQHandler            // TIM4
    .long    I2C1_EV_IRQHandler         // I2C1 Event
    .long    I2C1_ER_IRQHandler         // I2C1 Error
    .long    I2C2_EV_IRQHandler         // I2C2 Event
    .long    I2C2_ER_IRQHandler         // I2C2 Error
    .long    SPI1_IRQHandler            // SPI1
    .long    SPI2_IRQHandler            // SPI2
    .long    USART1_IRQHandler          // USART1
    .long    USART2_IRQHandler          // USART2
    .long    USART3_IRQHandler          // USART3
    .long    EXTI15_10_IRQHandler       // EXTI Line 15..10
    .long    RTCAlarm_IRQHandler        // RTC Alarm through EXTI Line
    .long    USBWakeUp_IRQHandler       // USB Wakeup from suspend

.weak NMI_Handler
.thumb_set NMI_Handler,default_interrupt_handler
.weak HardFault_Handler
.thumb_set HardFault_Handler,default_interrupt_handler
.weak MemManage_Handler
.thumb_set MemManage_Handler,default_interrupt_handler
.weak BusFault_Handler
.thumb_set BusFault_Handler,default_interrupt_handler
.weak UsageFault_Handler
.thumb_set UsageFault_Handler,default_interrupt_handler

.weak SVC_Handler
.thumb_set SVC_Handler,default_interrupt_handler
.weak DebugMon_Handler
.thumb_set DebugMon_Handler,default_interrupt_handler
.weak PendSV_Handler
.thumb_set PendSV_Handler,default_interrupt_handler
.weak SysTick_Handler
.thumb_set SysTick_Handler,default_interrupt_handler
.weak WWDG_IRQHandler
.thumb_set WWDG_IRQHandler,default_interrupt_handler
.weak PVD_IRQHandler
.thumb_set PVD_IRQHandler,default_interrupt_handler
.weak TAMPER_IRQHandler
.thumb_set TAMPER_IRQHandler,default_interrupt_handler
.weak RTC_IRQHandler
.thumb_set RTC_IRQHandler,default_interrupt_handler
.weak FLASH_IRQHandler
.thumb_set FLASH_IRQHandler,default_interrupt_handler
.weak RCC_IRQHandler
.thumb_set RCC_IRQHandler,default_interrupt_handler
.weak EXTI0_IRQHandler
.thumb_set EXTI0_IRQHandler,default_interrupt_handler
.weak EXTI1_IRQHandler
.thumb_set EXTI1_IRQHandler,default_interrupt_handler
.weak EXTI2_IRQHandler
.thumb_set EXTI2_IRQHandler,default_interrupt_handler
.weak EXTI3_IRQHandler
.thumb_set EXTI3_IRQHandler,default_interrupt_handler
.weak EXTI4_IRQHandler
.thumb_set EXTI4_IRQHandler,default_interrupt_handler
.weak DMA1_Channel1_IRQHandler
.thumb_set DMA1_Channel1_IRQHandler,default_interrupt_handler
.weak DMA1_Channel2_IRQHandler
.thumb_set DMA1_Channel2_IRQHandler,default_interrupt_handler
.weak DMA1_Channel3_IRQHandler
.thumb_set DMA1_Channel3_IRQHandler,default_interrupt_handler
.weak DMA1_Channel4_IRQHandler
.thumb_set DMA1_Channel4_IRQHandler,default_interrupt_handler
.weak DMA1_Channel5_IRQHandler
.thumb_set DMA1_Channel5_IRQHandler,default_interrupt_handler
.weak DMA1_Channel6_IRQHandler
.thumb_set DMA1_Channel6_IRQHandler,default_interrupt_handler
.weak DMA1_Channel7_IRQHandler
.thumb_set DMA1_Channel7_IRQHandler,default_interrupt_handler
.weak ADC1_2_IRQHandler
.thumb_set ADC1_2_IRQHandler,default_interrupt_handler
.weak USB_HP_CAN1_TX_IRQHandler
.thumb_set USB_HP_CAN1_TX_IRQHandler,default_interrupt_handler
.weak USB_LP_CAN1_RX0_IRQHandler
.thumb_set USB_LP_CAN1_RX0_IRQHandler,default_interrupt_handler
.weak CAN1_RX1_IRQHandler
.thumb_set CAN1_RX1_IRQHandler,default_interrupt_handler
.weak CAN1_SCE_IRQHandler
.thumb_set CAN1_SCE_IRQHandler,default_interrupt_handler
.weak EXTI9_5_IRQHandler
.thumb_set EXTI9_5_IRQHandler,default_interrupt_handler
.weak TIM1_BRK_IRQHandler
.thumb_set TIM1_BRK_IRQHandler,default_interrupt_handler
.weak TIM1_UP_IRQHandler
.thumb_set TIM1_UP_IRQHandler,default_interrupt_handler
.weak TIM1_TRG_COM_IRQHandler
.thumb_set TIM1_TRG_COM_IRQHandler,default_interrupt_handler
.weak TIM1_CC_IRQHandler
.thumb_set TIM1_CC_IRQHandler,default_interrupt_handler
.weak TIM2_IRQHandler
.thumb_set TIM2_IRQHandler,default_interrupt_handler
.weak TIM3_IRQHandler
.thumb_set TIM3_IRQHandler,default_interrupt_handler
.weak TIM4_IRQHandler
.thumb_set TIM4_IRQHandler,default_interrupt_handler
.weak I2C1_EV_IRQHandler
.thumb_set I2C1_EV_IRQHandler,default_interrupt_handler
.weak I2C1_ER_IRQHandler
.thumb_set I2C1_ER_IRQHandler,default_interrupt_handler
.weak I2C2_EV_IRQHandler
.thumb_set I2C2_EV_IRQHandler,default_interrupt_handler
.weak I2C2_ER_IRQHandler
.thumb_set I2C2_ER_IRQHandler,default_interrupt_handler
.weak SPI1_IRQHandler
.thumb_set SPI1_IRQHandler,default_interrupt_handler
.weak SPI2_IRQHandler
.thumb_set SPI2_IRQHandler,default_interrupt_handler
.weak USART1_IRQHandler
.thumb_set USART1_IRQHandler,default_interrupt_handler
.weak USART2_IRQHandler
.thumb_set USART2_IRQHandler,default_interrupt_handler
.weak USART3_IRQHandler
.thumb_set USART3_IRQHandler,default_interrupt_handler
.weak EXTI15_10_IRQHandler
.thumb_set EXTI15_10_IRQHandler,default_interrupt_handler
.weak RTCAlarm_IRQHandler
.thumb_set RTCAlarm_IRQHandler,default_interrupt_handler
.weak USBWakeUp_IRQHandler
.thumb_set USBWakeUp_IRQHandler,default_interrupt_handler

.size    __vectors, . - __vectors

.section .text.default_interrupt_handler,"ax",%progbits
default_interrupt_handler:
  default_interrupt_loop:
    B default_interrupt_loop
.size default_interrupt_handler, .-default_interrupt_handler

