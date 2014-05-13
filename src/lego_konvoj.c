/**
	@brief This is the main file for the robot. It contains the general information about the movement of the robot.

	@author GRUPP1 (florevald@gmail.com)
	@license See the license document (./../license)
	
	Remember to change the robot wheel speeds, depending on how you placed the wheels
*/
#include <stdlib.h> 
#include <stdint.h>
#include <string.h>
#include "general.c"
#include "kernel.h"
#include "kernel_id.h"
#include "ecrobot_interface.h"

//define some values to handle the aliazing
#define REGULATOR_LIST_LENGTH 12
#define REGULATOR_DT 25

/**
	OSEK Declaration of task 1
*/
DeclareTask(Task1);

/**
	For the init of the robot
*/
void ecrobot_device_initialize(void)
{
	ecrobot_init_sonar_sensor(NXT_PORT_S1);
	ecrobot_init_sonar_sensor(NXT_PORT_S4);	
}

/**
	Function to handle the termination of the robot
*/
void ecrobot_device_terminate(void)
{
	ecrobot_term_sonar_sensor(NXT_PORT_S1);
	ecrobot_term_sonar_sensor(NXT_PORT_S4);	
}

/**
	The timing should focus around 1 ms
*/
void user_1ms_isr_type2(void){}

/*****************REGLERING LISTA**************************************/
	
/**
	Function to handle the aliazing of the input.
	
	@param list
		a allocated list, or vector.
	@param len
		length of the list
	@param value
		value to add into the list
*/
void regulator_list_add(uint8_t *list,size_t len,uint8_t value)
{
	uint8_t *ttlist=malloc(sizeof(uint8_t)*len);
	memcpy(ttlist,list,len);
	memcpy(list,ttlist+1,len-1);
	free(ttlist);
	/*
	uint32_t *ttlist=(void*)list;
	
	(*ttlist)<<=8;
	(*(ttlist+3))<<=8;
	(*(ttlist+6))<<=8;
	*/
	list[len-1]=value;
}

/**
	Extracts the value of the 'aliazing'

	@param list
		a allocated list, or vector.
	@param len
		length of the list
*/
float regulator_list_mean(uint8_t *list,size_t len)
{
	int total=0;
	for(int i=0;i<len;i++)
	{
		total+=list[i];
	}
	return (((float)total)/((float)len));
}
/**************************************************************************/
	
/** 
	Task to handle the main function, works 'kind of' the same as [int main()]

	this function will handle the main instructions and regulations.
*/
TASK(Task1)
{
	//int ultrasonics;
	int sonar1;
	int sonar2;

	//to handle the information string
	char *printstr3=NULL;
	uint8_t printstr3clr=0;
	
	//init the aliazing lists
	uint8_t reglist1[REGULATOR_LIST_LENGTH];
	uint8_t reglist2[REGULATOR_LIST_LENGTH];
	
	//make sure that they have the value 0.
	memset(reglist1,0,REGULATOR_LIST_LENGTH*sizeof(char));
	memset(reglist2,0,REGULATOR_LIST_LENGTH*sizeof(char));
	
	//the loop of death, to handle everything. This will close if you press the exit button
	while(1)
	{
		//add the sensor value to the list
		regulator_list_add(reglist1,REGULATOR_LIST_LENGTH,(uint8_t)ecrobot_get_sonar_sensor(NXT_PORT_S1));
		regulator_list_add(reglist2,REGULATOR_LIST_LENGTH,(uint8_t)ecrobot_get_sonar_sensor(NXT_PORT_S4));
		
		//extract the current value
		sonar1=(int)regulator_list_mean(reglist1,REGULATOR_LIST_LENGTH);
		sonar2=(int)regulator_list_mean(reglist2,REGULATOR_LIST_LENGTH);
		
		//constants to handle the sight-range of the robot. top motor is the range that will give inacurrate values, like if the object is placed very far away
		#define bottom_motor 15
		#define top_motor 55
		#define error_len 5
		
		//if the robot sees the object, or forward robot.
		if(sonar2<=top_motor || sonar1<=top_motor)
		{
			//if the robot is behind in the confortable area
			if(sonar1>(bottom_motor+error_len) && sonar2>(bottom_motor+error_len))
			{
				nxt_motor_set_speed(NXT_PORT_A, -((sonar2+40)*1.052), 1);
				nxt_motor_set_speed(NXT_PORT_C, -((sonar1+40)*1.052), 1);
				printstr3="IM FEELING LUCKY";
			}
			else
			{
				//if the robot sees in one of the sensors.
				if(sonar1>(bottom_motor+error_len))
				{
					nxt_motor_set_speed(NXT_PORT_A, -((sonar1+40)*1.052), 1);
				}
				//if the robot is too close
				else if(sonar1<bottom_motor)
				{
					nxt_motor_set_speed(NXT_PORT_A, 50,1);
				}
				//if the sensor is satisfied
				else
				{
					nxt_motor_set_speed(NXT_PORT_A, 0, 1);
				}
				
				//same as above
				if(sonar2>(bottom_motor+error_len))
				{
					nxt_motor_set_speed(NXT_PORT_C, -((sonar2+40)*1.052), 1);
					//this is a test
					printstr3="FORWARD";
					printstr3clr=0;
				}
				else if(sonar2<bottom_motor)
				{
					nxt_motor_set_speed(NXT_PORT_C, 50, 1);
					//same here
					printstr3="BACKWARD";
					printstr3clr=0;
				}
				else
				{
					nxt_motor_set_speed(NXT_PORT_C, 0, 1);
					//same here
					printstr3="STOP";
					printstr3clr=0;
				}
			}
		}
		//if the robot sees nothing
		else
		{
			nxt_motor_set_speed(NXT_PORT_A, -100, 1);
			nxt_motor_set_speed(NXT_PORT_C, 100, 1);
			printstr3="SEEK & DESTROY";
			printstr3clr=0;
		}
		
		//==========DISPLAY SOME STUFF=============
		char *sonar1buff=b_int_to_string(sonar1);
		char *printstr1=concat("sonar1: ",sonar1buff,NULL);
		
		char *sonar2buff=b_int_to_string(sonar2);
		char *printstr2=concat("sonar2: ",sonar2buff,NULL);
		
		//clear display and show the text
		display_clear(0);
		display_goto_xy(0, 0);
		display_string(printstr1);
		display_goto_xy(0, 1);
		display_string(printstr2);
		
		if(printstr3)
		{
			display_goto_xy(0, 2);
			display_string(printstr3);
		}
		
		display_update();
		
		//free the memmory
		free(sonar1buff);
		free(printstr1);
		free(sonar2buff);
		free(printstr2);
		
		if(printstr3clr)
		{
			free(printstr3);
		}
		
		//wait so the sensors will work correctly, this is usable if implementing a PID.
		systick_wait_ms(REGULATOR_DT);
		
	}
	
	//does the terminating stuff, this line is kinda useless, but feels nice
	TerminateTask();
}
