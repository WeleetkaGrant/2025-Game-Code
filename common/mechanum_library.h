#include <kipr/botball.h>
#include <stdio.h>
#include <math.h>

#define WHEEL_DIAMETER 4.0 // Wheel diameter in inches
#define ROBOT_WIDTH 12.0 // Distance between left and right wheels in inches
#define TICKS_PER_INCH 140 // Fine-tuned conversion factor for forward/backward movement
#define STRAFE_TICKS_PER_INCH 150 // Adjusted conversion factor for strafing movement
#define TURN_SCALING_FACTOR 1.1 // Adjust this to fine-tune turning accuracy

void drive(float strafe_inches, float turn_degrees, float distance_inches, float speed, int motor_ports[4]) {
    /*
    Drives a KIPR robot using mecanum wheels.
    
    Parameters:
    strafe_inches (float): Left/Right movement in inches (negative for left, positive for right)
    turn_degrees (float): Rotation in degrees (negative for left turn, positive for right turn)
    distance_inches (float): Forward/Backward movement in inches (negative for reverse, positive for forward)
    speed (float): Speed factor (0 to 1, where 1 is max speed)
    motor_ports (int[4]): Array containing motor ports in the order:
        [front_left, front_right, back_left, back_right]
    */
    
    float turn_circumference = M_PI * ROBOT_WIDTH;
    float turn_distance = (turn_degrees / 360.0) * turn_circumference * TURN_SCALING_FACTOR;
    
    float front_left_speed = (distance_inches + strafe_inches + turn_distance);
    float front_right_speed = (distance_inches - strafe_inches - turn_distance);
    float back_left_speed = (distance_inches - strafe_inches + turn_distance);
    float back_right_speed = (distance_inches + strafe_inches - turn_distance);
    
    float max_speed = fmax(fmax(fabs(front_left_speed), fabs(front_right_speed)), 
                           fmax(fabs(back_left_speed), fabs(back_right_speed)));
    if (max_speed > 1) {
        front_left_speed /= max_speed;
        front_right_speed /= max_speed;
        back_left_speed /= max_speed;
        back_right_speed /= max_speed;
    }
    
    int front_left_power = (int)(front_left_speed * speed * 100);
    int front_right_power = (int)(front_right_speed * speed * 100);
    int back_left_power = (int)(back_left_speed * speed * 100);
    int back_right_power = (int)(back_right_speed * speed * 100);
    
    int duration_ms = (int)((fabs(distance_inches) * TICKS_PER_INCH + fabs(strafe_inches) * STRAFE_TICKS_PER_INCH + fabs(turn_distance) * TICKS_PER_INCH) / speed);
    
    motor(motor_ports[0], front_left_power);
    motor(motor_ports[1], front_right_power);
    motor(motor_ports[2], back_left_power);
    motor(motor_ports[3], back_right_power);
    
    msleep(duration_ms);
    
    motor(motor_ports[0], 0);
    motor(motor_ports[1], 0);
    motor(motor_ports[2], 0);
    motor(motor_ports[3], 0);
}