#include "tm4c123gh6pm.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define M_PI 3.14159265358979323846
#define Red 0x02
#define Yellow 0x0A
#define Green 0x08
#include <stdio.h>
#include <stdbool.h>
#define uint8_t        unsigned char



void led_initializaiton(){
  SYSCTL_RCGCGPIO_R|=0x20;
  while((SYSCTL_PRGPIO_R & 0x20)==0);
  GPIO_PORTF_LOCK_R=0x4C4F434B;
  GPIO_PORTF_CR_R|=0x0E;
  GPIO_PORTF_AFSEL_R&=~0x0E;
  GPIO_PORTF_AMSEL_R&=~0x0E;
  GPIO_PORTF_DEN_R|=0x0E;
  GPIO_PORTF_PCTL_R&=~0x0000FFF0;
  GPIO_PORTF_DIR_R|=0x0E;
  GPIO_PORTF_DATA_R &=~0x0E;
}

void led_output(unsigned char data){
   GPIO_PORTF_DATA_R&=~0x0E;
   GPIO_PORTF_DATA_R|=data;
}


void UART2_initialization(){
  SYSCTL_RCGCUART_R|=0x04;                 //enable UART2
  SYSCTL_RCGCGPIO_R|=0x08;                //enable gpio clk
  while((SYSCTL_PRGPIO_R & 0x08)==0);     //wait 
  UART2_CTL_R&=~0x0001;                    //disable UART2 at beginning
  UART2_IBRD_R=104;
  UART2_FBRD_R=11;
  UART2_LCRH_R=0x0070;
  UART2_CTL_R=0x0301;
  GPIO_PORTD_LOCK_R=0x4C4F434B;
  GPIO_PORTD_CR_R|=0xC0;
  GPIO_PORTD_AFSEL_R|=0xC0;
  GPIO_PORTD_PCTL_R=0x11000000;
  GPIO_PORTD_DEN_R|=0xC0;
	GPIO_PORTD_DIR_R&=~0x40;
	GPIO_PORTD_DIR_R|=0x80;
  GPIO_PORTD_AMSEL_R&=~0xC0;
}

unsigned char UART2_in(){
  while( (UART2_FR_R & 0x10)!=0 );
  return (char)(UART2_DR_R & 0xFF);
}




		/*char *token;
      uint8_t token_count;
			uint8_t sentence_index;
			char sentence[80];
			char c;
			bool flag;
void get_lat_long(double *latitude, double *longitude) {
		
			flag=true;
			c= UART2_in();
			sentence_index = 0;
    if (c == '$') {
        sentence_index = 0;
    }
    while (flag) {
        sentence[sentence_index++] = c;
			c= UART2_in();
        if (c == '*') {
            sentence[sentence_index] = '\0';
						flag=false;
						
            if (strstr(sentence, "$GPRMC")) {
								token_count = 0;
                token = strtok(sentence, ",");
                while (token != NULL) {
                    token_count++;
                    //if (token_count == 3)        {if (strstr(token, "V")) break;}
                     if (token_count == 4)  {*latitude = atof(token);}
                    //else if (token_count == 5)  {if (strstr(token, "S")) *latitude=-*latitude;}
                    else if (token_count == 6)  {*longitude = atof(token);}
                    //else if (token_count == 7)  {if (strstr(token, "W")) *longitude=-*longitude;}
                    token = strtok(NULL, ",");
                }
            }
          }
        }
      }*/
			
			
			char lat_substring[12];
       char long_substring[12];
			uint8_t sentence_index;
			char sentence[80];
			char c;
			bool flag;
void get_lat_long(double *latitude, double *longitude) {

flag = true;
     c=UART2_in();
	       sentence_index = 0;
    if (c == '$') {
        sentence_index = 0;

    while (flag) {
			        sentence[sentence_index++] = c;
			     c=UART2_in();


        if (c == '*') {
            sentence[sentence_index] = '\0';
flag = false;
            if (strstr(sentence, "$GPRMC")) {


							strncpy(lat_substring,sentence+(19),10);
    strncpy(long_substring,sentence+(32),11);
							*latitude = atof(lat_substring);
							*longitude = atof(long_substring);
			


            }
          }
        }
		}
      }
			
			
			
			

double GPS_getdistance(double lat1, double lon1, double lat2, double lon2)
{
return  2 * asin(sqrt(pow(sin(((lat2 - lat1) *M_PI / 180.0)/2),2) + pow(sin(((lon2 - lon1) *M_PI / 180.0)/2),2) * cos((lat1) * M_PI / 180.0) * cos((lat2) * M_PI / 180.0))) *6371000;
}



double latitude1;
double longitude1;
double latitudeadj;
double longitudeadj;
double latitude2=30.0633040 ;
double longitude2=31.2791293;
double distance;

int main(){
	
  led_initializaiton();
  UART2_initialization();
	
  while(1){

    get_lat_long(&latitude1,&longitude1);
		latitudeadj = (int)(latitude1 / 100) + (latitude1 - (int)(latitude1 / 100) * 100) / 60.0;
		longitudeadj = (int)(longitude1 / 100) + (longitude1 - (int)(longitude1 / 100) * 100) / 60.0;
		distance=GPS_getdistance(latitudeadj,longitudeadj,latitude2,longitude2); 
		
		 if(5<=distance){ led_output(Red); } 
    else if(2<distance && distance<5) {led_output(Yellow);}
		else if(0<=distance && distance<2) {led_output(Green);}
  }
}
