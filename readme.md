# Local Oscillator and User/Computer Interface (Subsystem C)
> **ECE295 Hardware Design and Communication — Flexible Radio Transceiver (FLRTRX)**

[![Hardware](https://img.shields.io/badge/Hardware-Altium%20Designer-brightgreen)](#altium-pcb-layout--routing)
[![Firmware](https://img.shields.io/badge/Firmware-Embedded%20C%20%7C%20ATmega-blue)](#microcontroller-firmware--cat-integration)
[![Protocol](https://img.shields.io/badge/Protocol-Yaesu%20CAT%20%7C%20UART-orange)](#microcontroller-firmware--cat-integration)

---

## Project Overview

The **Local Oscillator and User/Computer Interface (Subsystem C)** provides the frequency synthesis, state sequencing, and operational control for the Flexible Radio Transceiver (FLRTRX). The subsystem synthesizes two unipolar 3.3 $V_{pp}$ quadrature oscillator clocks ($0^\circ$ and $90^\circ$ phase-shifted at frequency $f_{LO}$) that drive both the receive (RX) and transmit (TX) quadrature mixers across the 8–16 MHz HF band. 

Additionally, Subsystem C coordinates operational state switching between Receive and Transmit modes through an electronic TX/RX RF switch, supporting both manual front-panel operation and remote PC-driven automation.

```
       +-------------------------------------------------------+
       |             Host PC (USB-Serial / CAT GUI)             |
       +---------------------------+---------------------------+
                                   | UART (TXD / RXD / DTR)
                                   v
+------------------+     +-------------------+     +------------------+
| Front-Panel UI   | --> | ATmega Controller | --> | PLL Synthesizer  |
| (LCD & Controls) |     | (Subsystem C.1)   |     | (I2C Control)    |
+------------------+     +---------+---------+     +--------+---------+
                                   |                        |
                                   | /TXEN                  | LO_F1_0 / LO_F1_90
                                   v                        v
                         +-------------------+     +------------------+
                         | TX/RX RF Switch   |     | Quadrature Mixers|
                         | (Subsystem C.2)   |     | (Subsystems A/D) |
                         +-------------------+     +------------------+
```

---

## Launching & Testing Phase

### Hardware Debugging & Waveform Verification
* **Quadrature Clock Generation:** Probed dual LO outputs on a high-speed digital oscilloscope to verify that `LO_F1_0` and `LO_F1_90` maintain a stable $90^\circ \pm 12.5^\circ$ phase relationship at matching amplitudes.
* **Frequency Tuning Accuracy:** Confirmed output clock synthesis across the 8–16 MHz HF spectrum with an accuracy within 1000 Hz.
* **Control Rail Verification:** Validated active-low transmit enable (`/TXEN`) logic transitions and verified power rail stability (+3.3V and +5V DC) under active switching.

> *(Insert oscilloscope PLL capture screenshot here: `docs/images/pll_quadrature_waveform.png`)*

### Microcontroller Firmware Bring-up & CAT Integration
* **Embedded Control Architecture:** Implemented C firmware on an 8-bit ATmega microcontroller to manage frequency selection, system state changes, and peripheral communication over I2C and UART.
* **CAT Protocol Engine:** Engineered a serial parser adhering to the Yaesu FT-991A Computer-Aided Transceiver (CAT) specification at 3.3V LVTTL levels via an FTDI FT232 USB-to-UART bridge.
* **Supported Command Set:**
  * `FA[9i];`: Reads/updates main local oscillator frequency $f_{LO}$ in Hz.
  * `FB[9i];`: Secondary VFO register support.
  * `TX[1b];` & `TX;`: Configures or queries the radio state (0 = RX, 1 = TX).
  * `IF;`: Returns structured transceiver status packets (`IF[3i][9i][13i];`).
  * Hardware `DTR` line monitoring for automated computer-controlled TX/RX toggling.

### Final Integration & RF Switching
* **C.1 & C.2 Mechanical Mating:** Verified daughtercard clearances and header alignments (standard 2.54 mm pitch, 4-pin male headers) against the FLRTRX mainboard with a 6 mm stack height.
* **Antenna Switching Performance:** Validated isolation and insertion loss for the C.2 TX/RX switch between the external BNC antenna port (`ANT`), receiver front-end (`RX_SIG`), and power amplifier output (`PA_OUT`).

---

## Design Phase

### Subsystem Architecture

The subsystem is implemented as two discrete daughtercards to isolate RF antenna signals from digital processor noise:

| Module | Board Identifier | Mechanical Dimensions | Functional Role |
| :--- | :--- | :--- | :--- |
| **C.1** | Oscillator & Microprocessor | $128.27\text{ mm} \times 50.80\text{ mm}$ | Microcontroller, JTAG header, PLL module header, FTDI UART, front-panel UI |
| **C.2** | TX/RX Switch | $38.10\text{ mm} \times 38.10\text{ mm}$ | RF antenna routing, TX isolation, `/TXEN` signal actuation |

### Altium PCB Layout & Routing
* **Subsystem C.1 Layout:** Routed high-speed digital traces for UART, I2C, and JTAG lines away from clock outputs. Incorporated mounting standoffs ($\varnothing 3.51\text{ mm}$) along the bottom perimeter to provide mechanical support on the mainboard.
* **Subsystem C.2 Layout:** Built with controlled-impedance RF traces between connectors `J11` (Antenna), `J12` (Receiver), and `J13` (Power Amplifier) to preserve signal fidelity and maximize isolation.

> *(Insert 3D PCB render or Altium layout screenshot here: `docs/images/subsystem_c_altium_render.png`)*

---

## Repository Structure

```text
├── docs/
│   ├── ECE295_FLRTRX_ICD_v3.0.pdf      # System Interface Control Document
│   └── Subsystem_C_Presentation.pdf     # Presentation and test analysis
├── hardware/
│   ├── C1_MCU_Oscillator/              # Altium schematics and layout files (C.1)
│   └── C2_TX_RX_Switch/                # Altium schematics and layout files (C.2)
├── firmware/
│   ├── src/                            # ATmega embedded C source code
│   │   ├── main.c
│   │   ├── cat_parser.c
│   │   ├── cat_parser.h
│   │   └── i2c_pll.c
│   └── Makefile
└── README.md
```

---

## Authors & Acknowledgments
* **Melani Keshani Gunettige** — Electrical & Computer Engineering, University of Toronto
* Course staff and laboratory instructors for **ECE295: Hardware Design and Communication**.