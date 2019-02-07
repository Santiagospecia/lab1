#include <Stepper.h>

// Pines utilizados
#define LEDVERDE 2
#define LEDAMARILLO 3
#define LEDROJO 4
#define TRIGGER 7
#define ECHO 8
#define BUZZER 9

// Constantes
const float sonido = 34300.0; // Velocidad del sonido en cm/s
const float limite1 = 115.0;
const float limite2 = 70.0;
const float limite3 = 20.0;

const int stepsPerRevolution = 1500; //cambie esto para que se ajuste al número de pasos por revolución para su motor


float distancia;


// // inicialice la biblioteca de pasos en los pines 10 a 14:
Stepper myStepper(stepsPerRevolution, 10, 12, 11, 14);




void setup() {

 

  // Modo entrada/salida de los pines
  pinMode(LEDVERDE, OUTPUT);
  pinMode(LEDAMARILLO, OUTPUT);
  pinMode(LEDROJO, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(TRIGGER, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  

  myStepper.setSpeed(20); // Asignamos la velocidad en RPM 
  
  Serial.begin(9600); // Iniciamos el monitor serie, nos muestra los valores

  
  encenderLEDs(); //encendemos todos los leds
  delay(2000);     //esperamos un segundo
  apagarLEDs(); // Apagamos todos los leds

}

void loop() {
 
  iniciarTrigger(); // Preparamos el sensor de ultrasonidos

  distancia = calcularDistancia();  // Obtenemos la distancia

  apagarLEDs();  // Apagamos todos los LEDs

  Serial.println("Gira sentido horario");
  myStepper.step(stepsPerRevolution);
  
  //delay(500);

  // Lanzamos alerta si estamos dentro del rango de peligro
  if (distancia < limite1)
  {
    // Lanzamos alertas
    alertas(distancia);
  }

}

// Apaga todos los LEDs
void apagarLEDs()
{
  // Apagamos todos los LEDs
  digitalWrite(LEDVERDE, LOW);
  digitalWrite(LEDAMARILLO, LOW);
  digitalWrite(LEDROJO, LOW);
}

//prender todos los leds
void encenderLEDs()
{
  // Prendemos todos los LEDs
  digitalWrite(LEDVERDE, HIGH);
  digitalWrite(LEDAMARILLO, HIGH);
  digitalWrite(LEDROJO, HIGH);
}


// Función que comprueba si hay que lanzar alguna alerta visual o sonora
void alertas(float distancia)
{
  if (distancia < limite1 && distancia >= limite2)
  {
    // Encendemos el LED verde
    digitalWrite(LEDVERDE, HIGH);
    tone(BUZZER, 2000, 200); //TONO DEL BUZZER
    //Stepper.setSpeed(NUMPASOS); //MODIFICAMOS LA VELOCIDAD
  }
  else if (distancia < limite2 && distancia > limite3)
  {
    // Encendemos el LED amarillo
    digitalWrite(LEDAMARILLO, HIGH);
    tone(BUZZER, 3300, 200);  //1° argumento -> n° pin
                  //2° argumento -> frecuencia 
                  //3° argumento -> duracion 
         //Stepper.setSpeed(NUMPASOS);
  }
  else if (distancia <= limite3)
  {
    // Encendemos el LED rojo
    digitalWrite(LEDROJO, HIGH);
    tone(BUZZER, 6000, 200);
    Serial.println("Sentido antihorario");
    myStepper.step(-stepsPerRevolution);
   
    
  }
}

// Método que calcula la distancia a la que se encuentra un objeto.
// Devuelve una variable tipo float que contiene la distancia
float calcularDistancia()
{
  // La función pulseIn obtiene el tiempo que tarda en cambiar entre estados, en este caso a HIGH
  unsigned long tiempo = pulseIn(ECHO, HIGH);

  // Obtenemos la distancia en cm, hay que convertir el tiempo en segudos ya que está en microsegundos
  // por eso se multiplica por 0.000001
  float distancia = tiempo * 0.000001 * sonido / 2.0;
  Serial.print(distancia);
  Serial.print("cm");
  Serial.println();
  delay(500);

  return distancia;
}

// Método que inicia la secuencia del Trigger para comenzar a medir
void iniciarTrigger()
{
  // Ponemos el Triiger en estado bajo y esperamos 2 ms
  digitalWrite(TRIGGER, LOW);
  delayMicroseconds(2);

  // Ponemos el pin Trigger a estado alto y esperamos 10 ms
  digitalWrite(TRIGGER, HIGH);
  delayMicroseconds(10);

  // Comenzamos poniendo el pin Trigger en estado bajo
  digitalWrite(TRIGGER, LOW);
}
