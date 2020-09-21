#include <SoftwareSerial.h>


SoftwareSerial soft(12,13);
uint16_t easyyy = 46870; //~3 seconds
uint16_t easy = 31300; //~2 seconds
uint16_t medium = 15670; //~1 second
uint16_t hard = 7800; //~0.5 second
uint16_t impossible = 3900; //~0.25 seconds

//how many turns each player gets
uint8_t num_to_play = 10;
uint8_t played_so_far = 0;

//score and highscore
int score = 0;
int high_score = 0;

volatile uint8_t readed = 0; // keeps track of mole state
volatile uint8_t randomness; //choses which mole to activate
int servo;
int won; //has the player won
volatile int com = 1;


void setup()
{
  pinMode(3,OUTPUT);
  pinMode(4,OUTPUT);
  pinMode(5,OUTPUT);
  pinMode(6,OUTPUT);
  pinMode(2, OUTPUT);
  
  
    //these are the four buttons
  pinMode(8, INPUT_PULLUP);
  pinMode(9, INPUT_PULLUP);
  pinMode(10, INPUT_PULLUP);
  pinMode(11, INPUT_PULLUP);
  
  randomSeed(analogRead(A0)); //should be left floating
  
  Serial.begin(9600);
  soft.begin(3000);//for com with other arduino
  
  //clear the registers that are needed for later use
  TCCR1A = 0;// clear this register
  TCCR1B = 0;// same for this one
  TCNT1  = 0;//initialize counter value to 0
  
  
  // set compare match register for difficulty level
  OCR1A = impossible;// predefined compare values map roughly to
  					//multiples of 0.25 seconds (global ints)
  
  
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // CS10 = 1 and CS12= 1 for 1024 prescaler(biggest available)
  TCCR1B |= (1 << CS12) | (1 << CS10);  
  
  // enable timer compare interrupt for timer 1
  //TIMSK1 |= (1 << OCIE1A);
  setup_game();
  sei(); // Enable interrupts globally
}

void loop()
{
  if(played_so_far < num_to_play){  
  	play();
  }
  else{
    end_game();
    setup_game();
  }
  
  
}

void play()
{
  readed = 0;
  won = 0;
  com = 1;
  randomness = (int) random(3, 7); //which mole to turn on
  servo = 2*randomness -5;
  Serial.begin(9600);
  //enable timer interrupts for turning on/off specific mole
  TIMSK1 |= (1 << OCIE1A);
  
  while(readed < 3){
    while(readed == 0);
    while(readed  == 1){
      if(digitalRead(randomness+5) == LOW){
        won = 1;
      }
    }
    Serial.print(servo);
    
    while(readed < 3){
      if(digitalRead(randomness+5) == LOW){
        won = 1;
      }
    }
    Serial.print('n');
    Serial.print(servo+1);
  }
  
  //disable timer interrupts because it's already flashed
  /*
  *code between enabling the interrupt and disabling it should
  *be as optimized as possible because that is where the most
  *accuracy is required
  */
  TIMSK1 &= ~(1 << OCIE1A);
  delay(200);
  Serial.print('n');
  Serial.end();
  if(won){
    score++;
    soft.print("Got it! Score:");
    soft.print(score);
    digitalWrite(12, LOW);
    digitalWrite(2,HIGH);
    delay(100);
    digitalWrite(2,LOW);
  }
  played_so_far++;
  delay(1000);
}

void setup_game(){
  int chosen = 0;
  score = 0;
  delay(500);
  
  soft.print("High Score: ");
  soft.print(high_score);
  delay(500);
  soft.print("High Score: ");
  soft.print(high_score);
  delay(500);
  
  
  soft.print("Set Difficulty");
  delay(500);
  
  
  while(!chosen){
   	soft.print("1. Easy   2. Med");
  	soft.print("3. Hard  4.Impos");
    if(digitalRead(11) == LOW){
      delay(500);
      OCR1A = easy; //set time difference to easy mode
      soft.print("Mode: Easy");
      chosen++;
      delay(500);
      chosen++;
    }
    else if (digitalRead(10) == LOW){
      delay(500);
      OCR1A = medium;
      soft.print("Mode: Medium");
      chosen++;
      delay(500);
    }
    else if (digitalRead(9) == LOW){
      delay(500);
      OCR1A = hard;
      soft.print("Mode: Hard");
      chosen++;
      delay(500);
    }
    else if (digitalRead(8) == LOW){
      delay(500);
      OCR1A = impossible;
      soft.print("Mode: Impossible");
      chosen++;
      delay(500);
    }
  }
  
}

void end_game()
{
  played_so_far = 0;
  delay(500);
  soft.print("Congrats!       ");
  soft.print("You scored: ");
  soft.print(score);
  delay(500);
  soft.print("Congrats!       ");
  soft.print("You scored: ");
  soft.print(score);
  delay(500);
  soft.print("Congrats!       ");
  soft.print("You scored: ");
  soft.print(score);
  delay(500);
  
  if(score> high_score){
    high_score = score;
    soft.print("You set         ");
    soft.print("a high score!");
    delay(500);
    soft.print("You set         ");
    soft.print("a high score!");
    delay(500);
    soft.print("You set         ");
    soft.print("a high score!");
    delay(500);
  }
}

ISR(TIMER1_COMPA_vect)
{
    readed++;
    com = !com;
    digitalWrite(randomness, com);
 
}