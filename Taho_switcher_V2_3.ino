/* 
Taho Switcher V2.3 READY
 Свичер (переключатель) режима работы ЭБУ "Хонда Бит" с режима низкой мощности на режим высокой мощности.
 Предназначен для частного случая управления режимами работы двс автомобиля, замещая штатную систему, найти
 которую или восстановить не представляется возможным.

 Концептуальная логика: 
 Режим IDLE. Если количество rpm двс автомобиля ниже определенного прогового значения, 13 пин платы отправляет логический 0.
 В этом режиме у авто стабильный холостой ход и довольно малый угол опережения зажигая. 
 
 Режим POWER.Если количество rpm двс автомобиля выше другого определенного значения, 13 пин платы отправляет логическую 1. 
 В этом режиме двс автомобиля работает в мощностном режиме, обеспечивая лучшую (по сравнению с режимом IDLE)
 динамику и расход топлива. Однако, на холостых оборотах, работа двс нестабильна, в следствии чего данный режим
 применим к работе лишь при достижении двс определенных оборотов. 

 Между двумя граничными значениями rpm присутствует гистерезис, с целью придания получившейся системе наиболее
 возможной стабильности в работе и исключения "дребезга" при работе в пограничных состояниях.
*/


// Глобальные переменные
unsigned long T0; // Переменная времени таймера сброса счетчика
unsigned long T1; // Переменная времени сбрасывания таймера
unsigned long T2; // Переменная времени таймера фильтраии и сглаживания.
volatile int TIC = 1; // Переменная пойманых импульсов . "volatile" маст хев
float TIC_F; // Переменная отфильтрованных импульсов
float K = 0.06; // Коэфициент фильтрации для пойманых импульсов. 0.06 Наиболее корректно.
// - - - - - - - - - - - - - - - - - - - - - - 

// Сетап
void setup() {
//	Serial.begin(9600); // Инициализация порта
	attachInterrupt(0, tic, RISING); // Инициализация прерывания . Мб FALLING, мб CHANGE, по дефолту RISING
	pinMode(2, INPUT); // Инициализация пина прерывания
  pinMode(13, OUTPUT); // Инициализация пина управления
//  Serial.println("Инициализация завершена"); // Отчет в порт о успешном завершении инициализации.
}
// - - - - - - - - - - - - - - - - - - - - - - 


// Прерывание с улавливанием испульса TIC
void tic() {
  TIC = TIC + 1;  
}
// - - - - - - - - - - - - - - - - - - - - - - 


// Луп из 7 ...
void loop() {

  // Блок сброса счетчика
	if (micros() - T0 > (1000000)) { // Сброс счетчика раз в секунду
    T0 = micros();
		TIC = 0;
	}
  
  // Блок фильтрации и сглаживания значений
  if (micros() - T1 > (30000)) {
    T1 = micros();
    TIC_F += (TIC - TIC_F) * K; // Фильтрация значения TIC.
//    Serial.println(TIC_F); // Вывод в порт. ВКЛ для отладки, ВЫКЛ в рабочей прошивке.
  }

  // Блок выбора режима работы
  if (micros() - T2 > (500000)) { // 500к мик.сек = 0.5 с, скорректировать при необходимоси
    T2 = micros();
    // Оставить гистерезис ~10 TIC_F (мб больше, мб меньше), что бы исключить разнос в пограничных состояниях
    if (TIC_F < 25){ // Нижняя граница оборотов, режим IDLE 
      digitalWrite(13, 0);
    }
    if (TIC_F > 35){ // Верхняя граница оборотов, режим POWER
      digitalWrite(13, 1);
    }
  }

}