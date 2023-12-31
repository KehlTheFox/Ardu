/* 
Alarm Button Laurel v1.2

Реализация аварийной сигнализации (одновременное моргание всеми лампами поворотов) на плате ардуино.
Использвать в случае крайней необходимости или с целью продолжительного тестирования и изучения влияния агрессивных условий внешней 
среды на работу платы Ардуино. Рекомендовано обесточивать плату ардуино при перемещении ключа в замке зажигания в положение "0".

Концептуальная логика: физическая кнопка аварийной сигнализации, находящаяся на торпедо в автомобиле, замыкает 
сенсорный пин (А0 - INPUT_PULLUP) на GND. 
При нажатой (включеной) кнопке цепь постоянно замкнута и 2 пина (D12 и D11) начинают 
одновременно (*на сколько это возможно*) менять свое состояние с 1 на 0 и наоборот. 

При отпущеной (отключеной) кнопке и отсутствии других условий (задел на будущее) эти пины стремятся занять состояние 0. 
*/

/* Патч-лист:
v1.1
- Блок relayBreaker. Таймер. micros заменены на millis с целью упрощения подсчетов.
- Блок relayBreaker. Таймер. Уменьшен интервал с 700 до 500 мс для улучшени отклика и большей схожести с оригинальным алгоритмом.
- Блок relayBreaker. Модуль сброса. Добавлено присвоение значения 0 переменной flag0 при размыкании физической кнопки аварийной сигнализации
 с целью устранения бага избыточного времени ожидания при повторном включении аварийной сигнализации.

v1.2
- Описание и комментарии приведены к общему стандартному виду. Правок в логику и код нет.
*/


//Блок инициализации переменных
unsigned long T0; // Переменная времени таймера мигания
boolean flag0 = 0; // Флаг мигания


// Блок сетап
void setup() {

  pinMode(11, OUTPUT); // Пин поворотников левого борта
  pinMode(12, OUTPUT); // Пин поворотников правого борта
  pinMode(13, OUTPUT); // Контрольный пин для отладки
  
  pinMode(A0, INPUT_PULLUP); // Пин кнопки аварийной сигнализации
}

// Блок-цикл луп
void loop() {
  relayBreaker();  
}

// Блок Реле-прерывателя, отвечающего за поворотники и аварийную сигнализацию
void relayBreaker (){
  if (millis() - T0 > (500)) { //Раз в указанное количество ms происходит срабатывание программного Реле-прерывателя.
    T0 = millis();
  // Модуль аварийной сигнализации
      if (digitalRead(A0) == 0){ // Кнопка нажата, пин А0 замыкается на GND.
        digitalWrite(11, flag0 = !flag0); // Пин левого борта
        digitalWrite(12, flag0); // Пин правого борта
        digitalWrite(13, flag0); // Пин отладки
      }
  
  
  // ОЧЕНЬ АККУРАТНО С ЭТИМ МЕСТОМ КОДА. 
  // Модуль сброса состояния пинов на 0      
      if (digitalRead(A0) == 1) { // Добавлять условия при добавлении новых модулей
        digitalWrite(11, 0);
        digitalWrite(12, 0);
        digitalWrite(13, 0);
        flag0 = 0;
      }    
  }
}

