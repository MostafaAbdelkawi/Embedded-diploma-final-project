
# Embedded-diploma-final-project

Door Locker Security System implemented using two ATMega32 Microcontrollers one acting as HMI unit reading user input via Keypad and displaying output via LCD
Communicates via UART 

to the
Second MCU which is acting as Control unit verify user password by comparing the input password with the EEPROM stored password

as well as openning the door incase of success or firing an alarm incase of failure


#### All drivers are implemented in C taking static and dynamic configurations, abstraction, portability and clean coding into considerations

  ## MCAL :

  UART,
  Timers ,
  DIO and
  I2C

 ## HAL :

  Keypad, LCD, DC Motor, Buzzer and External EEPROM

  
## Screenshots
#### System Design

![App Screenshot](https://github.com/MostafaAbdelkawi/Embedded-diploma-final-project/blob/main/system%20design.png?raw=true)

#### Flowchart

![App Screenshot](https://github.com/MostafaAbdelkawi/Embedded-diploma-final-project/blob/main/flowchart.png?raw=true)
