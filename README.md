# I2C Two Wire Interface

This repository implements I2c protocol on single arduino and 2 arduinos

## Timing diagram

![Alt Text](https://github.com/Basantloay/I2C_Demo/blob/main/2_arduinos/TimingDiagram.PNG)

## I2C on 2 Arduinos

Writes 200 bytes from master to slave

Slave Address is 24

## I2C on single Arduino

Write 200 bytes from master to slave

Slave Address is 24

Using Timer and ISR

OCRx - Output Compare Register

ICRx - Input Capture Register (only for 16bit timer)

TIMSKx - Timer/Counter Interrupt Mask Register. To enable/disable timer interrupts.

TIFRx - Timer/Counter Interrupt Flag Register. Indicates a pending timer interrupt.