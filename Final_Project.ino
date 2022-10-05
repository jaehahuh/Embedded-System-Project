#include <Adafruit_CircuitPlayground.h>

int start = 0;
int ex_type = -1; // Type of exercise
int total_ex = 0; // total number of exercise
int left_button;
int right_button;

double x[25];
double y[25];
double z[25];
double total_x;
double total_y;
double total_z;


bool on = false;

bool left_pressed;
bool right_pressed;

bool jumping_jacks = false;
bool jumping_up = false;

bool squats = false;
bool squat_down = false;
bool squat_steady = false;

bool push_ups = false;
bool push_down = false;
bool push_steady = false;

bool sit_ups = false;



 





void setup() {
  // put your setup code here, to run once:
  PORTD &= ~(1<<4);
  PORTF &= ~(1<<6);
  
  CircuitPlayground.begin();
  Serial.begin(9600);
}   


void loop() {
  // put your main code here, to run repeatedly:
  
  left_button = PIND & (1<<4);
  right_button = PINF & (1<<6);
  
  left_pressed = CircuitPlayground.leftButton();
  right_pressed = CircuitPlayground.rightButton();

  
  if (on == false) {
    start = 0;
    CircuitPlayground.clearPixels();
  }

  if (right_button != 0) { // if the right button is pressed, reset the exercise    
    ex_type = -1;
    total_ex = 0;
    CircuitPlayground.clearPixels();
  }
  
  if (((on == false) & (left_button != 0)) | (on == true)) {

    on = true;
    
    x[start] = CircuitPlayground.motionX();
    y[start] = CircuitPlayground.motionY();
    z[start] = CircuitPlayground.motionZ();
  
    if (start != 24) {
      start += 1;
      total_x += x[start];
      total_y += y[start];
      total_z += z[start];
    }
  
    else { 

      // sit_ups 
      if (ex_type == -1 | ex_type == 0) {
        if ((sit_ups == false) & (total_x < 100) & (total_y < 25) & (total_z > 200)) {
          CircuitPlayground.clearPixels();
          CircuitPlayground.setPixelColor(0, 0, 255, 0); // green
          sit_ups = true;      
        }
    
      if ((sit_ups == true) & ((total_x < -200) | (total_x > 200)) & (total_y < 25) & (total_z < 10)) { 

        if (ex_type == -1) {
          ex_type = 0;
        }

        sit_ups = false;
        total_ex += 1;
        
        CircuitPlayground.clearPixels();
        CircuitPlayground.setPixelColor(7, 255, 255, 0); // yellow
      }
     }

    
    //push_ups 
    if (ex_type == -1 | ex_type == 1) {
      if ((total_x < 100) & (total_y < 25) & (total_z < -200) & (push_ups == false) & (push_down == false)) {
        CircuitPlayground.clearPixels();
        CircuitPlayground.setPixelColor(1, 0, 0, 255); // blue
        push_ups = true;
      }
  
      if ((total_z < -200) & (push_down == true)) { // when 
        push_down = false;
        push_steady = true;
      }
  
      if ((push_down == false) & (push_steady == true) & (total_z > -150)) 
      {
        if (ex_type == -1) {
          ex_type = 1;
        }
        
        CircuitPlayground.clearPixels();
        CircuitPlayground.setPixelColor(7, 255, 255, 0); // yellow
        total_ex += 1;
        push_steady = false;
        push_ups = false;
      }
      
      if ((total_z < -300) & (push_ups == true) & (push_down == false)) { 
        push_down = true; 
      }
    }
    
       
      // jumping jacks 

      if (ex_type == -1 | ex_type == 2) { 
        if ((jumping_jacks == false) & (jumping_up == false)&(total_x < -200)) { // jump to upward
          jumping_jacks = true;
          CircuitPlayground.clearPixels();
          CircuitPlayground.setPixelColor(2, 255, 0, 0);  
        }
        
        if ((jumping_up == true) & (total_x < -200)) {
        
          if (ex_type == -1) { 
            ex_type = 2;
          }

          total_ex += 1;

          jumping_jacks = false;
          jumping_up = false;
        }
       
        if ((jumping_jacks == true) & (jumping_up == false) & (total_x < -350)) { 
          CircuitPlayground.clearPixels();
          CircuitPlayground.setPixelColor(7, 255, 255, 0); 
          jumping_up = true;
        }
      }
      

      
      // squats 

      if (ex_type == -1 | ex_type == 3) {
        if ((squats == false) & (squat_down == false) & (total_x < -200)) {
          CircuitPlayground.clearPixels();
          CircuitPlayground.setPixelColor(3, 0, 0, 255);
          squats = true;
        }
    
        if ( (squat_down == true) & (total_x < -200)) {
          squat_down = false;
          squat_steady = true;
        }
    
        if ((squat_down == false) & (squat_steady == true) & (total_x > -150)) 
        {
          
          if (ex_type == -1) {
            ex_type = 3;
          }
          
          CircuitPlayground.clearPixels();
          CircuitPlayground.setPixelColor(7, 255, 255, 0); // yellow
          total_ex += 1;
          squat_steady = false;
          squats = false;
        }
        
        if ((squats == true) & (squat_down == false) & (total_x < -300)) {
          squat_down = true;
        }
      }

    


  
     if (total_ex == 15) {
        CircuitPlayground.clearPixels();
        CircuitPlayground.setPixelColor(9, 255, 0, 0); // if exercise is over, red light on. 
        ex_type = 5; // set ex_type as none
     }
           
      start = 0;
      total_x = 0;
      total_y = 0;
      total_z = 0;
    }

    Serial.print(total_x);
    Serial.print(total_y);
    Serial.print(total_z);
  
  }

}
