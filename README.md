# ROBOT-AUTOMATIZAT-PENTRU-ELIMINAREA-OBSTACOLELOR

## Descriere
Această lucrare descrie un robot care folosește metode și componente complexe pentru a naviga și elimina obiecte dintr-un spațiu predefinit marcat cu o linie albă. Pentru a realiza acest lucru, proiectul este bazat pe un Arduino Uno, un senzor ultrasonic pentru a detecta obstacolele și un senzor infraroșu pentru a identifica linia albă.

Robotul are capacitatea de a-și modifica traiectoria în funcție de obiectivele sale și de mediul înconjurător. Odată ce un obiect este detectat, robotul se orientează către el și începe să se deplaseze direct spre acesta pentru a-l scoate din spațiul delimitat.

Componentele hardware principale ale sistemului includ un Arduino Uno ca microcontroler, 2 motoare pentru roți, un shield prototype v5 pentru Arduino, o punte H dublă L298N pentru controlul motoarelor, un senzor infraroșu pentru detectarea liniei albe și o baterie de 9V pentru alimentare.
	
 De asemenea, robotul are posibilitatea de a fi controlat și manual prin intermediul unei telecomenzi RadioMaster TX16S.


## Adresa Repository-ului
Puteți accesa întregul cod sursă al proiectului la următoarea adresă:
https://github.com/AlbertMircea/ROBOT-AUTOMATIZAT-PENTRU-ELIMINAREA-OBSTACOLELOR.git

## Pașii de Compilare
1. Asigură-te că ai instalat Arduino IDE pe computerul tău. Poți descărca Arduino IDE de la [aici](https://www.arduino.cc/en/software).
2. Clonează repository-ul:
   ```sh
   git clone https://github.com/AlbertMircea/ROBOT-AUTOMATIZAT-PENTRU-ELIMINAREA-OBSTACOLELOR.git
   ```
3. Deschide Arduino IDE și navighează la File -> Open, apoi selectează fișierul Aleznauer_Albert-Mircea_Aplicatie_Practica_AIA_Licenta.ino din folderul clonat.

## Pașii de Instalare și Lansare
1. Conectează placa Arduino la computer folosind un cablu USB.
2. În Arduino IDE, selectează placa corespunzătoare și portul USB:
- Navighează la Tools -> Board și selectează Arduino UNO.
- Navighează la Tools -> Port și selectează portul corespunzător (de obicei ceva de genul COM3 sau /dev/ttyUSB0).
3. Apasă butonul Upload (săgeata spre dreapta) din Arduino IDE pentru a încărca codul pe placa Arduino.
4. După ce încărcarea este completă, deconectează placa Arduino de la computer și conectează bateria de 9V la robot.
5. Robotul ar trebui să înceapă să se rotească și să detecteze obiectele și linia albă conform descrierii proiectului.
  
## Contact
Mail student: albert-aleznauer@student.upt.ro sau aaleznauer@gmail.com
