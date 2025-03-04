#include <stdio.h>
#include <stdlib.h>

#define rightMotor 3
#define leftMotor 0
#define leftSensor analog(1)
#define rightSensor analog(2)
#define black 3800
#define inches 220

void stop() {
	freeze(leftMotor);
    freeze(rightMotor);
    msleep(500);
    ao();
}

void drive(int distance, int speed){
	cmpc(0);
	while(abs(gmpc(0)) < distance * inches){
        mav(0,speed);
        mav(3,speed);
    }
    mav(0,0);
    mav(3,0);
    msleep(20);
}

void driveET(int port, int distance, int speed){
	while(analog(port)<distance){
    	mav(0, speed);
        mav(3, speed);
    }
}


void turn(int degrees, int speed, char* direction) {
    // Constants (adjust these based on testing)
    const int TICKS_PER_DEGREE = 11;  // Example: Encoder ticks per degree, calibrate as needed
    
    int target_ticks = abs(degrees) * TICKS_PER_DEGREE;
    
    cmpc(rightMotor); // Reset right motor encoder
    cmpc(leftMotor); // Reset left motor encoder
    
    if (strcmp(direction, "left") == 0) {
        mav(rightMotor, speed);  // Right wheel forward
        mav(leftMotor, -speed); // Left wheel backward
        while (gmpc(rightMotor) < target_ticks && gmpc(leftMotor) > -target_ticks); // Wait until target is reached
    } else if (strcmp(direction, "right") == 0) {
        mav(rightMotor, -speed); // Right wheel backward
        mav(leftMotor, speed);  // Left wheel forward
        while (gmpc(rightMotor) > -target_ticks && gmpc(leftMotor) < target_ticks); // Wait until target is reached
    } else {
        printf("Invalid direction. Use 'right' or 'left'.\n");
        return;
    }
    stop();
}


void lineFollow(int distance, int speed, int port){
	cmpc(0);
    while(abs(gmpc(0))<distance*220){
        int error = analog(port) - black;//error > 0 = black, error < 0 = white
        //float speed_modifier = error * 0.7;
        mav(0,speed-error);
        mav(3,speed+error);
    }
	stop();
}

#define CENTER_X 80  // Assuming a 160x120 resolution
#define TOLERANCE 5 // Allowable error for centering
void cameraFollow(int port, int channel, int ET, int speed, int turnSpeed) {
    while(analog(port)<ET) {
        camera_update();
       
        if(get_object_count(channel) > 0) { // If the object is detected
            int obj_x = get_object_center_x(channel, 0); // X position of largest object
           
            if(obj_x < CENTER_X - TOLERANCE) { // Object is to the left
                mav(leftMotor, speed - turnSpeed);
                mav(rightMotor, speed + turnSpeed);
            }
            else if(obj_x > CENTER_X + TOLERANCE) { // Object is to the right
                mav(leftMotor, speed + turnSpeed);
                mav(rightMotor, speed - turnSpeed);
            }
            else { // Object is centered
                mav(leftMotor, speed);
                mav(rightMotor, speed);
            }
        }
        else { // If no object is detected, stop
            mav(leftMotor, 0);
            mav(rightMotor, 0);
        }
        msleep(100);
    }
    stop();
}


void cameraFollowDistance(int channel, int distance, int speed, int turnSpeed){
    cmpc(0);
	while(abs(gmpc(0)) < distance * inches) {
        camera_update();
       
        if(get_object_count(channel) > 0) { // If the object is detected
            int obj_x = get_object_center_x(channel, 0); // X position of largest object
           
            if(obj_x < CENTER_X - TOLERANCE) { // Object is to the left
                mav(leftMotor, speed - turnSpeed);
                mav(rightMotor, speed + turnSpeed);
            }
            else if(obj_x > CENTER_X + TOLERANCE) { // Object is to the right
                mav(leftMotor, speed + turnSpeed);
                mav(rightMotor, speed - turnSpeed);
            }
            else { // Object is centered
                mav(leftMotor, speed);
                mav(rightMotor, speed);
            }
        }
        else { // If no object is detected, stop
            mav(leftMotor, 0);
            mav(rightMotor, 0);
        }
        msleep(100);
    }
    stop();
}


void slowServo(int port, int position, int speed, int disable){
    enable_servo(port);
    int counter = get_servo_position(port);
    
    // Move the servo up (to a higher position)
    if (counter < position) {
        while (counter < position) {
            msleep(10);
            counter += speed;  // Increment the counter by the speed value
            if (counter > position) {
                counter = position; // Prevent overshooting
            }
            set_servo_position(port, counter);
        }
    }
    // Move the servo down (to a lower position)
    else {
        while (counter > position) {
            msleep(10);
            counter -= speed;  // Decrement the counter by the speed value
            if (counter < position) {
                counter = position; // Prevent overshooting
            }
            set_servo_position(port, counter);
        }
    }
    if (disable == 0){
    	disable_servo(port);
    }
}



void checkBattery() {
    float battery_level = power_level(); // Get battery voltage
    float low_battery_threshold = 20.0; // 20% battery level (approximate)
    
    if (battery_level < low_battery_threshold) {
        beep(); // Play an audible beep
        printf("Warning: Battery low (%.2fV)\n", battery_level);
        
        // Draw warning symbol on screen
        graphics_open(320, 240); // Open graphics at default screen size
        
        graphics_fill(255, 255, 255); // White background
        graphics_circle_fill(160, 120, 50, 255, 0, 0); // Red circle
        
        // Draw black exclamation mark
        graphics_rectangle_fill(155, 90, 165, 140, 0, 0, 0); // Vertical bar
        graphics_circle_fill(160, 155, 5, 0, 0, 0); // Dot at bottom
        
        graphics_update(); // Refresh screen
        msleep(3000); // Display for 3 seconds
        graphics_close(); // Close graphics window
    }
}



// Tuned PID constants
#define Kp 0.8
#define Ki 0.002
#define Kd 0.3

// Motor bias correction
#define RIGHT_BIAS .965
#define LEFT_BIAS 1.0

double get_gyro_offset() {
    double sum = 0;
    int samples = 50;

    for (int i = 0; i < samples; i++) {
        sum += gyro_z();
        msleep(5);
    }

    return sum / samples;
}

void driveStraight(int speed, double distance) {
    double gyro_offset = get_gyro_offset();
    double previous_error = 0, integral = 0;

    int target_ticks = distance * inches; // Convert inches to encoder ticks

    // Reset motor encoders
    clear_motor_position_counter(leftMotor);
    clear_motor_position_counter(rightMotor);

    while (gmpc(leftMotor) < target_ticks && gmpc(rightMotor) < target_ticks) {
        double gyro_reading = gyro_z() - gyro_offset;

        // PID calculations
        double error = gyro_reading;
        integral += error;
        double derivative = error - previous_error;

        // Compute correction
        double correction = (Kp * error) + (Ki * integral) + (Kd * derivative);

        // Adjust motor speeds with bias correction
        int left_speed = (speed - correction) * LEFT_BIAS;
        int right_speed = (speed + correction) * RIGHT_BIAS;

        // Prevent negative speeds
        left_speed = left_speed < 0 ? 0 : left_speed;
        right_speed = right_speed < 0 ? 0 : right_speed;

        // Set motor speeds
        mav(leftMotor, left_speed);
        mav(rightMotor, right_speed);

        previous_error = error;
        msleep(50);
    }
    stop(); // Stop motors after reaching distance
}



void squareUp(int threshold, int speed) {
    int leftOnLine = 0;
    int rightOnLine = 0;

    while (!(leftOnLine && rightOnLine)) {  
        leftOnLine = leftSensor > threshold;  // Check if left sensor detects black  
        rightOnLine = rightSensor > threshold; // Check if right sensor detects black  

        if (!leftOnLine) {
            mav(leftMotor, speed);  // Move left wheel forward until it finds the line  
        } else {
            mav(leftMotor, 0);  // Stop left wheel  
        }

        if (!rightOnLine) {
            mav(rightMotor, speed); // Move right wheel forward until it finds the line  
        } else {
            mav(rightMotor, 0); // Stop right wheel  
        }
        
        msleep(50); // Small delay to prevent excessive CPU usage  
    }
    stop();
}
