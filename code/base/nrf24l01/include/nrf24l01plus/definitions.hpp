/*
 *
 * Copyright Niels Post 2019.
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * https://www.boost.org/LICENSE_1_0.txt)
 *
*/


#ifndef PROJECT_NRF24L01_DEFINITIONS_HPP
#define PROJECT_NRF24L01_DEFINITIONS_HPP


namespace nrf24l01 {
    /**
     * \addtogroup nrf24l01Plus
     * @{
     */

    /**
     * \brief Register addresses for the NRF24L01
     */
    struct NRF_REGISTER {
        //! Chip configuration register
        static constexpr const uint8_t CONFIG = 0x00;
        //! Enable 'Auto Acknowledgement' per RX Pipe
        static constexpr const uint8_t EN_AA = 0x01;
        //! Enabled RX Pipes
        static constexpr const uint8_t EN_RXADDR = 0x02;
        //! Setup of Address Widths
        static constexpr const uint8_t SETUP_AW = 0x03;
        //! Setup of Automatic Retransmission
        static constexpr const uint8_t SETUP_RETR = 0x04;
        //! RF Channel number
        static constexpr const uint8_t RF_CH = 0x05;
        //! RF Setup Register
        static constexpr const uint8_t RF_SETUP = 0x06;
        //! General status register
        static constexpr const uint8_t NRF_STATUS = 0x07;
        //! Transmit observe register
        static constexpr const uint8_t OBSERVE_TX = 0x08;
        //! Received Power Detector
        static constexpr const uint8_t RPD = 0x09;
        //! Address of RX Pipe 0
        static constexpr const uint8_t RX_ADDR_P0 = 0x0A;
        //! Address of RX Pipe 1
        static constexpr const uint8_t RX_ADDR_P1 = 0x0B;
        //! Address of RX Pipe 2
        static constexpr const uint8_t RX_ADDR_P2 = 0x0C;
        //! Address of RX Pipe 3
        static constexpr const uint8_t RX_ADDR_P3 = 0x0D;
        //! Address of RX Pipe 4
        static constexpr const uint8_t RX_ADDR_P4 = 0x0E;
        //! Address of RX Pipe 5
        static constexpr const uint8_t RX_ADDR_P5 = 0x0F;
        //! Address of TX pipe
        static constexpr const uint8_t TX_ADDR = 0x10;
        //! Payload width for RX pipe 0
        static constexpr const uint8_t RX_PW_P0 = 0x11;
        //! Payload width for RX pipe 1
        static constexpr const uint8_t RX_PW_P1 = 0x12;
        //! Payload width for RX pipe 2
        static constexpr const uint8_t RX_PW_P2 = 0x13;
        //! Payload width for RX pipe 3
        static constexpr const uint8_t RX_PW_P3 = 0x14;
        //! Payload width for RX pipe 4
        static constexpr const uint8_t RX_PW_P4 = 0x15;
        //! Payload width for RX pipe 5
        static constexpr const uint8_t RX_PW_P5 = 0x16;
        //! Status of FIFO registers
        static constexpr const uint8_t FIFO_STATUS = 0x17;
        //! Enable dynamic payload length per RX pipe
        static constexpr const uint8_t DYNPD = 0x1C;
        //! Enable features
        static constexpr const uint8_t FEATURE = 0x1D;
    };


    /**
     * \brief CONFIG register bit mappings
     */
    struct NRF_CONFIG {
        //! Mask interrupt caused by RX_DataReady event
        static constexpr const uint8_t MASK_RX = 0x40;
        //! Mask interrupt caused by TX_DataSent event
        static constexpr const uint8_t MASK_TX_DS = 0x20;
        //! Mask interrupt caused by TX_MaxRetransmission event
        static constexpr const uint8_t MASK_MAX_RT = 0x10;
        //! Enable CRC checksum
        static constexpr const uint8_t CONFIG_EN_CRC = 0x8;
        //! CRC encoding scheme (0: 1 byte, 1: 2 bytes)
        static constexpr const uint8_t CONFIG_CRCO = 0x04;
        //! Control Power up/down
        static constexpr const uint8_t CONFIG_PWR_UP = 0x02;
        //! RX/TX control
        static constexpr const uint8_t CONFIG_PRIM_RX = 0x01;
    };

    /**
     * \brief EN_AA register bit mappings
     */
    struct NRF_EN_AA {
        //! Enable Auto Acknowledgement for RX Pipe 5
        static constexpr const uint8_t ENAA_P5 = 0x20;
        //! Enable Auto Acknowledgement for RX pipe 4
        static constexpr const uint8_t ENAA_P4 = 0x10;
        //! Enable Auto Acknowledgement for RX pipe 3
        static constexpr const uint8_t ENAA_P3 = 0x08;
        //! Enable Auto Acknowledgement for RX pipe 2
        static constexpr const uint8_t ENAA_P2 = 0x04;
        //! Enable Auto Acknowledgement for RX pipe 1
        static constexpr const uint8_t ENAA_P1 = 0x02;
        //! Enable Auto Acknowledgement for RX pipe 0
        static constexpr const uint8_t ENAA_P0 = 0x01;
    };


    /**
     * \brief EN_RXADDR register bit mappings
     */
    struct NRF_EN_RXADDR {
        //! Enable Pipe 5
        static constexpr const uint8_t ERX_P5 = 0x20;
        //! Enable Pipe 4
        static constexpr const uint8_t ERX_P4 = 0x10;
        //! Enable Pipe 3
        static constexpr const uint8_t ERX_P3 = 0x08;
        //! Enable Pipe 2
        static constexpr const uint8_t ERX_P2 = 0x04;
        //! Enable Pipe 1
        static constexpr const uint8_t ERX_P1 = 0x02;
        //! Enable Pipe 0
        static constexpr const uint8_t ERX_P0 = 0x01;
    };


    /**
     * \brief RF_SETUP register bit mappings
     */
    struct NRF_RF_SETUP {
        //! Force PLL Lock signal
        static constexpr const uint8_t PLL_LOCK = 0x10;
        //! Select between high speed data ates
        static constexpr const uint8_t RF_DR_HIGH = 0x08;
        //! Set Data rate to 250 kbps
        static constexpr const uint8_t RF_DR_LOW = 0x20;
        //! Set RF Output power (in TX mode)
        //! output power (in dBm)  = -18 + RF_PWR*6
        static constexpr const uint8_t RF_PWR = 6;
    };

    /**
     * \brief STATUS register bit mappings
     */
    struct NRF_STATUS {
        //! Flag: Data Ready in RX FIFO (write 1 to clear)
        static constexpr const uint8_t RX_DR = 0x40;
        //! Flag: Data was transmitted (write 1 to clear)
        static constexpr const uint8_t TX_DS = 0x20;
        //! Flag: Maximum number of retransmissions reached (write 1 to clear)
        static constexpr const uint8_t MAX_RT = 0x10;
        //! Pipe number of data in first slot of fifo (111 if no data is available)
        static constexpr const uint8_t RX_P_NO = 0x0D;
        //! Flag: Transmit register is full
        static constexpr const uint8_t TX_FULL = 0x01;
    };

    /**
     * \brief OBSERVE_TX register bit mappings
     */
    struct NRF_OBSERVE_TX {
        //! Counter for amount of lost packets
        static constexpr const uint8_t TX_PLOS_CNT = 0xF0;
        //! Counter for amount of retransmissions, reset when a new Transmission starts
        static constexpr const uint8_t TX_ARC_CNT = 0x0F;
    };

    /**
     * \brief FIFO_STATUS register bit mappings
     */
    struct NRF_FIFO_STATUS {
        //! Last TX payload is being reused
        static constexpr const uint8_t TX_REUSE = 0x40;
        //! Flag: TX FIFO is full
        static constexpr const uint8_t TX_FULL = 0x20;
        //! Flag: TX FIFO is empty
        static constexpr const uint8_t TX_EMPTY = 0x10;
        //! Flag: RX FIFO is full
        static constexpr const uint8_t RX_FULL = 0x02;
        //! Flag: RX FIFO is empty
        static constexpr const uint8_t RX_EMPTY = 0x01;
    };

    /**
     * \brief DYNPD register bit mappings
     */
    struct NRF_DYNPD {
        //! Enable Dynamic Payload Length for Pipe 5
        static constexpr const uint8_t DPL_P5 = 0x20;
        //! Enable Dynamic Payload Length for Pipe 4
        static constexpr const uint8_t DPL_P4 = 0x10;
        //! Enable Dynamic Payload Length for Pipe 3
        static constexpr const uint8_t DPL_P3 = 0x08;
        //! Enable Dynamic Payload Length for Pipe 2
        static constexpr const uint8_t DPL_P2 = 0x04;
        //! Enable Dynamic Payload Length for Pipe 1
        static constexpr const uint8_t DPL_P1 = 0x02;
        //! Enable Dynamic Payload Length for Pipe 0
        static constexpr const uint8_t DPL_P0 = 0x01;
    };

    /**
     * \brief FEATURE register bit mappings
     */
    struct NRF_FEATURE {
        //! Dynamic Payload Length feature is enabled
        static constexpr const uint8_t EN_DPL = 0x04;
        //! Acknowledgement Payload feature is enabled
        static constexpr const uint8_t EN_ACK_PAY = 0x02;
        //! W_TX_PAYLOAD_NOACK command is enabled
        static constexpr const uint8_t EN_DYN_ACK = 0x01;
    };

    /**
     * \brief NRF24L01 SPI command mnemonics
     */
    struct NRF_INSTRUCTION {
        //! Read a memory Register
        static constexpr const uint8_t R_REGISTER = 0x00;
        //! Write to a memory register
        static constexpr const uint8_t W_REGISTER = 0x20;
        //! Read Payload Width for first available RX payload
        static constexpr const uint8_t R_RX_PL_WID = 0x60;
        //! Read first available RX payload
        static constexpr const uint8_t R_RX_PAYLOAD = 0x61;
        //! Write TX payload to TX FIFO
        static constexpr const uint8_t W_TX_PAYLOAD = 0xA0;
        //! Write TX Packet with auto acknowledgement disabled (EN_DYN_ACK needs to be set in FEATURE register)
        static constexpr const uint8_t W_TX_PAYLOAD_NO_ACK = 0xB0;
        //! Write Acknowledgement Payload (EN_ACK_PAY needs to be set in FEATURE register)
        static constexpr const uint8_t W_ACK_PAYLOAD = 0xA8;
        //! Empty TX FIFO register
        static constexpr const uint8_t FLUSH_TX = 0xE1;
        //! Empty RX FIFO register
        static constexpr const uint8_t FLUSH_RX = 0xE2;
        //! Reuse last TX payload in future transmission
        static constexpr const uint8_t REUSE_TX_PL = 0xE3;
        //! No Operation, can be used to retrieve status register
        static constexpr const uint8_t RF24_NOP = 0xFF;
    };

    /**
     * @}
     */

#ifdef NIKS
    //
    ///* Non-P omissions */
    //#define LNA_HCURR   0
    //
    ///* P model memory Map */
    //#define RPD         0x09
    //#define W_TX_PAYLOAD_NO_ACK  0xB0
    //
    ///* P model bit Mnemonics */
    //#define RF_DR_LOW   5
    //#define RF_DR_HIGH  3
    //#define RF_PWR_LOW  1
    //#define RF_PWR_HIGH 2
    //
#endif
}
#endif //PROJECT_NRF24L01_DEFINITIONS_HPP