# MTRX2700 C Lab Project – Group 6

## Group Information


**Semester:** S1 2025  
**Unit:** MTRX2700 – Mechatronics 2  

### Group Members & Roles

| Name        | Role                         | Responsibilities                                                                                     |
|-------------|------------------------------|------------------------------------------------------------------------------------------------------|
| Marco       | Digital I/O Lead             | Completed all Digital I/O module tasks, implemented function pointer callback, state encapsulation   |
| Zhenyu Dai  | Timer Systems Lead           | Completed full timer module including periodic and one-shot callbacks                                |
| Oscar       | Serial Communications Lead   | Developed UART module with interrupts, callback handling, and basic RX/TX functionality              |

---

## Project Description

This project demonstrates modular embedded system development in **C** on the **STM32F3 Discovery Board**, completed during Weeks 6–8. The project builds on the earlier ARM assembly work by transitioning to interrupt-driven, structured C code modules that reflect good software design practices.

Key focus areas:
- Modular `.c`/`.h` separation
- Callback functions via function pointers
- Interrupt-based UART communication and timers
- Timer-based delay and one-shot tasks
- Final integration through command parsing

---

## Project Structure

Organized by **exercise number** (1–4), each directory includes:
- Module source (`.c`) and header (`.h`)
- Test or demo code (`main.c`)
- Comments and documentation
- Integration logic

---

## Program Overview

### Functionality:
A user sends commands over **serial**, and the board executes the appropriate action:
1. **LED Pattern:** `"led 10001010"` — Displays the binary pattern on GPIOE LEDs
2. **Serial Echo:** `"serial Hello!"` — Echoes the message back over UART
3. **One-Shot Timer:** `"oneshot 1000"` — Triggers a one-time callback after 1000 ms
4. **Periodic Timer:** `"timer 500"` — Calls a callback every 500 ms (e.g., toggle an LED)

### Modules:
| Module           | Description                                                                         |
|------------------|-------------------------------------------------------------------------------------|
| `digital_io`     | Encapsulated LED/button control with header-only access and callback support       |
| `serial_comm`    | Interrupt-based UART receiver with buffer and callback on message complete         |
| `timer_module`   | Timer interface with both periodic and one-shot events using TIM2                  |
| `command_parser` | Parses and dispatches commands based on serial input format                        |
| `main.c`         | Orchestrates modules and demonstrates integration                                  |

---

## Instructions for Use

### Requirements:
- STM32CubeIDE
- STM32F3 Discovery Board
- Micro-USB cable
- Serial terminal (e.g., Cutecom/ PuTTY)

### Steps:
1. Clone the group repository
2. Import project into STM32CubeIDE (`File > Import > Existing Projects`)
3. Build and flash to STM32 board
4. Open a serial terminal (115200 baud) and test command strings

---

## Testing Plan

| Module         | Test Description                                                  | Method                                               | Expected Outcome                                   | Status |
|----------------|-------------------------------------------------------------------|------------------------------------------------------|---------------------------------------------------|--------|
| Digital I/O    | LED set/get, button callback                                      | Press button and observe LED state toggling         | Callback fires and LED state changes              | ✅     |
| Serial Comm    | Receive until terminator, echo string                             | Send string via serial and observe echo             | Full string echoed back                           | ✅     |
| One-Shot Timer | Trigger single event after delay                                  | Observe one-time action after command               | Action occurs after delay once                    | ✅     |
| Periodic Timer | Repeatedly trigger event at defined interval                      | Observe repeating LED toggle or print               | Toggling repeats at interval                      | ✅     |
| Parser         | Parse command structure `"command operand"`                       | Send all 4 command types and check behavior         | Commands parsed and executed correctly            | ✅     |
| Final Demo     | Complete system test                                              | Use terminal to test all features end-to-end        | Fully integrated functionality                    | ✅     |

---

## Assessment Readiness Checklist

- ✅ All core modules completed
- ✅ Callback-driven design for modularity
- ✅ Interrupt-based UART and timers
- ✅ README updated with accurate responsibilities and testing
- ✅ Repository tagged with `assessment demonstration`
- ✅ Meeting minutes saved in `minutes/` folder
