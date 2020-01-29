#ifndef DATA_H_
#define DATA_H_

/*
 * data.h is essential for control data
 * All camera data, monitoring variables and control variables must be declared in data.h
 * This file has 3 sections delimited by camera data, monitoring and control variable
 * This format must be respect for the Java interface to work
 * There are maximum 2 camera data array of 128 char
 * There are maximum 20 monitoring variables
 * There can be as many control variable as the user need
 * Monitoring and control variables MUST be declared as INT or FLOAT since they both take up 4 bytes of data
 * The camera_data array must have data type CHAR
 * Note that the start of 
 */

struct Debug_Data {
	/* CAMERA DATA */
	char camera_data_1[128];
	char camera_data_2[128];
	/* END CAMERA DATA */
	/* MONITORING */
	int A0;
	float B1;
	int C2; 
	float D3;
	int E4;
	float F5;
	int G6; 
	float H7;
	int I8;
	float J9;
	int K10; 
	float L11;
	int M12;
	float N13;
	int O14; 
	float P15;
	int Q16;
	float R17;
	int S18; 
	float T19;
	/* END MONITORING */
	/* CONTROL VARIABLES */
	int CONTROL_0;
	int CONTROL_1;
	int CONTROL_2;
	int CONTROL_3;
	int CONTROL_4;
	float CONTROL_5;
	float CONTROL_6;
	float CONTROL_7;
	float CONTROL_8;
	float CONTROL_9;
	/* END CONTROL VARIABLES */
};

union Debug_Data_Union {
	unsigned char byte_block[592];
	struct Debug_Data data;
} debug_data;

#endif /* DATA_H_ */
