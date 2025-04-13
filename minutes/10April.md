# MTRX2700 Week 7 Lab Meeting Minutes  
Date: 10/04/2025  
Location: Mechatronics Dry Lab  
Group Number: 6    
Minute Taker: Marco 

---

## 1. Attendance

| Name           | Present | 
|----------------|---------|
| Oscar       | ❌       | 
| Marco       | ✅       | 
| Zhenyu Dai  | ✅       | 

---

## 2. Objectives for Week 7

- Finish exercise 1,2,3 by each member
- Helping each others of exercise 1,2,3
- Thinking about how to putting all the modules together to make the integration part
- Scheduling next meeting to do the integration

---

## 3. Technical Progress

### **Exercise 1: Digital I/O**
| Task (2.2.2)                              | Status | Owner     | Notes                                                                 |
|------------------------------------------|--------|-----------|-----------------------------------------------------------------------|
| a) Encapsulation digital I/O interface    | ✅   | Marco  | Module can control the Digital I/O interface  |
| b) Function pointer                       | ✅   | Marco  | Pointer correctly point to the main function  |
| c) Access through header file             | ✅   | Marco  | Function successfully get the bitmask state   |
| d) Advanced function                      | ✅   | Marco  |   |


---

### **Exercise 2: Serial interface**
| Task (2.3.2)                                | Status | Owner     | Notes                                                             |
|--------------------------------------------|--------|-----------|-------------------------------------------------------------------|
| a) UART reading in function                | ✅     | Oscar  | Function completed to receive message and then retransmit to show functionality |
| b) UART Receiving function                 | ✅     | Oscar  | Function can receive the string and call a callback function to retransmit |
| c) Applied with Interrupt                  | ✅     | Oscar  | Module uses interrupts instead of polling to receive |
| d) Advanced function                       | ❌     | Oscar  |  |


### **Exercise 3: Timer Interface**
| Task (2.4.2)                                | Status | Owner     | Notes                                                             |
|--------------------------------------------|--------|-----------|-------------------------------------------------------------------|
| a) Timer triggering the callback function  | ✅     | Zhenyu  |      |
| b) Reset the timer                         | ✅     | Zhenyu  |      |
| c) Advanced function                       | ✅     | Zhenyu  |      |


---

## 4. Repository Updates

| Folder     | New Files Added                                      | Description                                    |
|------------|------------------------------------------------------|------------------------------------------------|
| `Exercise 1` | `Digital_IO_2a`, `Digital_IO_2b` , `Digital_IO_2c` , `Digital_IO_2d`                                             | Digital I/O interface                          |
| `Exercise 2` | `serial.c`, `Task_a.c`                                            | Serial interface                               |
| `Exercise 3` | `C_LAB_EX3`                                            | Timer Interface                                |
| `Integration` | `main.c`                                            | Putting it all together                        |
| `minutes/`   | `10April.md`                                   | This meeting minutes file                      |

---


**Minutes saved in repo as:** `minutes/3April.md`
