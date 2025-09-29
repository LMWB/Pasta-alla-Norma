# Pasta_alla_Norma
Circular shaped RGB-LED PCB with integrated Microcontroller and EEPROM

# cubeIDE Setting for evaluate used flash size
```arm-none-eabi-nm --size-sort --print-size "RGB_Ring_LimbusZero.elf" > "RGB_Ring_LimbusZero.sizes"```  

- T / t → Flash (code)  
- R / r → Flash (read-only const)  
- D / d → RAM (with init in Flash)  
- B / b → RAM only (zero-init)  
- W / w → Weak (depends where resolved, often Flash functions)  

# Fassung
E27 bedeutet Edison Fassung 27 mm (siehe Zeichnung).  
Ein zylindrischer Stempel der in die Fassung passt ohne Gewinde, sollte 30mm Lange haben und 24mm Durchmesser.  
