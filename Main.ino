//sensores ultrassônicos
#define trigPin1 24
#define echoPin1 25
#define trigPin2 26
#define echoPin2 27
#define trigPin3 28
#define echoPin3 29
#define trigPinPoco 30
#define echoPinPoco 31
#define trigPinBebedouro 32
#define echoPinBebedouro 33

//relés
#define valvulaCaixa1 34
#define valvulaCaixa2 35
#define valvulaCaixa3 36
#define motor 37
#define valvulaBebedouro 38
#define valvulaIrrigacao 39

//controle
#define ledLiga 42
#define botao 22
#define sensorUmidade A0
#define sensorVelocidade 42

//bombeamento de água ou geração de energia
#define dinamo 40



long duration, distance, Sensor1,Sensor2,Sensor3, SensorPoco, SensorBebedouro;
int x1, x2, x3;
int veloMin;
int m; 

void setup(){
  
  //lista de entradas e saídas
  
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
  pinMode(trigPin3, OUTPUT);
  pinMode(echoPin3, INPUT);
  pinMode(trigPinPoco, OUTPUT);
  pinMode(echoPinPoco, INPUT);
  pinMode(trigPinBebedouro, OUTPUT);
  pinMode(echoPinBebedouro, INPUT);

  pinMode(botao, INPUT);
  pinMode(sensorUmidade, INPUT);
  pinMode(sensorVelocidade, INPUT);

  pinMode(valvulaCaixa1, OUTPUT);
  pinMode(valvulaCaixa2, OUTPUT);
  pinMode(valvulaCaixa3, OUTPUT);
  pinMode(valvulaIrrigacao, OUTPUT);  
  pinMode(valvulaBebedouro, OUTPUT);
  
  pinMode(ledLiga,OUTPUT);
  pinMode(dinamo, OUTPUT);
  pinMode(motor, OUTPUT);
  

  //variáveis para controlar a prioridade  
  x1 = 0;  
  x2 = 0;   
  x3 = 0;  
  m = 1; 
}


void loop() {       
  
  if(digitalRead(botao) == HIGH){   //lógica do botão de liga
    digitalWrite(ledLiga, HIGH);
    
    //só ligar a bomba elétrica caso o catavento esteja com pouca velocidade
    if (analogRead(sensorVelocidade) < 400 && m == 1){    
      SonarSensor(trigPinPoco, echoPinPoco);
      SensorPoco = distance;
    
      //E se o poço estiver com água, para evitar eventuais danos, de ligar com ele vazio
     if(distance < 100){                                 
        digitalWrite(motor,HIGH);
      }else{
        digitalWrite(motor,LOW);
        }
    }else{          
        digitalWrite(motor,LOW);
    }
  
 
  //lógica da caixa d'água da casa
  SonarSensor(trigPin1, echoPin1);
  Sensor1 = distance; 
    
    //caso a caixa esteja vazia
    if(Sensor1 > 200) { 
        m = 1;
        x1 = 1; 
        x2 = 0; 
        x3 = 0;
        LigarBombeamento();
        digitalWrite(valvulaCaixa1, HIGH); 
    
    //caso ela esteja cheia
    }else if(Sensor1 < 15){  
        x1 = 0;      
        digitalWrite(valvulaCaixa1, LOW);
  }else{
        delay(1000);    
    }               
 
  
  //lógica da caixa d'água dos animais
  SonarSensor(trigPin2, echoPin2);
  Sensor2 = distance;    
  //ver se é necessário encher a caixa d'água dos animais ou não
  VerificarBebedouro();   
  
  //caso a caixa esteja vazia e a primeira não precise encher
  if(Sensor2 > 200 && x1 == 0) { 
        m = 1;
        x2 = 1;   
        x3 = 0;   
        LigarBombeamento();
        digitalWrite(valvulaCaixa2, HIGH); 
    
    //caso ela esteja cheia
    }else if(Sensor2 < 15){  
        x2 = 0;         
        digitalWrite(valvulaCaixa2, LOW);
            
    //caso ela esteja vazia, mas a mais importante esteja enchendo
    }else if(x1 == 1 && (Sensor2 > 200 || (Sensor2 < 200 && Sensor2 > 15))){                
        if(Sensor2 < 200){
          }
        delay(1000);
        digitalWrite(valvulaCaixa2, LOW);  
  }else{             
        delay(1000);
      
    } 
 
  
  //lógica da caixa d'água da plantação
  SonarSensor(trigPin3, echoPin3);
  Sensor3 = distance; 
  
  //verifica quão seco está o solo
  VerificarUmidade();  
  
    //caso a caixa d'água esteja vazia e mais nenhuma precise encher
    if(Sensor3 > 200 && x1 == 0 && x2 == 0) { 
        m = 1;
        x3 = 1; 
        LigarBombeamento();
        digitalWrite(valvulaCaixa3, HIGH);   
        delay(1000);
    
    //caso ela esteja cheia
    }else if(Sensor3 < 15){ 
        x3 = 0; 
        digitalWrite(valvulaCaixa3, LOW); 
        delay(100);
    
    //caso ela esteja vazia mas uma das outras esteja enchendo
    }else if((x1 == 1 || x2 == 1) && (Sensor3 > 200 || (Sensor3 < 200 && Sensor3 > 15))){
        x3 = 0;
        digitalWrite(valvulaCaixa3, LOW); 
        delay(1000);
  }else{ 
        delay(1000);

      
    }
    //lógica para ligar o dínamo caso todas as caixas estejam cheias
    if(x1 == 0 && x2 == 0 && x3 == 0){ 
      m = 0;
      digitalWrite(motor,LOW);
      LigarDinamo();
      
     //caso o botão esteja desativado
     }}else {
        desligarTudo();  
  }}




//função para medir quão cheia está a caixa
void SonarSensor(int trigPin,int echoPin){ //função para medir a distância
    digitalWrite(trigPin, LOW); //desliga o trigger para se certificar da precisão
    delayMicroseconds(2); //esperar 2 microssegundos 
    digitalWrite(trigPin, HIGH); //mandar um pulso
    delayMicroseconds(10); //deixar o pulso por 10microssegundos
    digitalWrite(trigPin, LOW); //desligar o pulso
    duration = pulseIn(echoPin, HIGH); //medir a duração do pulso
    distance = (duration/2) / 29.1; //divir a duração por 29.1 que é a velocidade do som em 
                                    //cm/microssegundo e por 2 para ser só a ida do som
}

//função para decidir encher ou não o bebedouro
void VerificarBebedouro(){
    SonarSensor(trigPinBebedouro, echoPinBebedouro);
    SensorBebedouro = distance;
    if(distance < 15){
      digitalWrite(valvulaBebedouro, HIGH);
      } 
    else if(distance > 200){
      digitalWrite(valvulaBebedouro, LOW);
      }
  }

//função para medir a umidade do solo e decidir se vai irrigar ou não
void VerificarUmidade(){
  int umidade;
  umidade = analogRead(sensorUmidade);
  if(umidade > 800){
    digitalWrite(valvulaIrrigacao, HIGH);
    }
 else if(umidade < 100){
    digitalWrite(valvulaIrrigacao, LOW);
  }
  }

void LigarBombeamento(){
  digitalWrite(dinamo, LOW);
  }

void LigarDinamo(){
  digitalWrite(dinamo, HIGH);
  }

void desligarTudo(){
   digitalWrite(valvulaCaixa1,LOW);
   digitalWrite (valvulaCaixa2, LOW);
   digitalWrite (valvulaCaixa3,LOW);
   digitalWrite (motor, LOW);
   digitalWrite (valvulaBebedouro, LOW);
   digitalWrite (valvulaIrrigacao, LOW);
   digitalWrite(dinamo, LOW);
  }
