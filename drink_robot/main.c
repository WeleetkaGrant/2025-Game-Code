#include <kipr/wombat.h>
#include <botballLibraryV2.h>

int main()
{
    printf("Hello World\n");
    int open = 1100;
    int cup = 1800;
    int drink = 2000;
    //checkBattery();//startup
    camera_open_at_res(LOW_RES);
    slowServo(0, 450, 10, 1);
    slowServo(0, 1300, 10, 0);
    slowServo(1, 1100, 10, 0);
    
    cameraFollow(0, 0, 2200, 600, 100);//Step 1
    turn(10, 300, "left");
    drive(3, 1000);
    slowServo(1, cup, 10, 1);
    
    drive(5, -1000);//step 2
    slowServo(0, 700, 10, 0);
    turn(90, 1000, "left");
    squareUp(black, -500);
    drive(4, 1000);
    turn(90, 1000, "left");
    BlackPeopleTerminator(500);
    slowServo(0, 1300, 10, 0);
    int i;

    for (i = 0; i < 2; i++) {
      printf("%d in drink(s) cup\n", i + 1);
    
        slowServo(1, open, 10, 1);
        slowServo(0, 450, 10, 0);
        turn(90, 1000, "right");//Step 3
        squareUp(black, -500);
        drive(3, -700);
        turn(90, 1000, "right");
        squareUp(black, -500);
        slowServo(0, 1300, 10, 1);
        slowServo(1, open, 10, 0);
        cameraFollow(0, 0, 2200, 600, 100);
        slowServo(0, 1500, 10, 0);
        turn(5, 1000, "left");
        drive(3, 1000);
        turn(5, 1000, "right");
        slowServo(1, drink, 10, 1);
    
        drive(8, -1500);//Step 4
        turn(90, 1000, "left");
        drive(4, 1000);
        squareUp(black, -500);
        drive(4, 500);
        turn(90, 1000, "left");
        slowServo(0, 450, 10, 1);
        BlackPeopleTerminator(500);
        //turn(5, 1000, "right");
        //drive(1, 1000);
        slowServo(0, 900, 10, 1);
        slowServo(1, open, 10, 0);
        //drive(1, -1000);
        //turn(5, 1000, "left");
        //reset cup
        if (i == 0) {
        slowServo(0, 1300, 10, 0);
        drive(3, 1000);
        slowServo(1, cup, 10, 1);
        drive(8, -1000);
        BlackPeopleTerminator(500);
        }
    }

    
    slowServo(0, 1300, 10, 0);//step 5
    slowServo(1, cup, 10, 1);
    slowServo(0, 700, 10, 0);
    drive(2, 1000);
    lineFollow(8, 500, 2);
    drive(4, 1000);
    turn(90, 1000, "left");
    lineFollow(5, 500, 2);
    slowServo(0, 1300, 10, 0);
    slowServo(1, 1100, 10, 0);
    slowServo(0, 450, 10, 1);
    return 0;
}

void BlackPeopleTerminator(int speed){
	cmpc(0);
    while(rightSensor<black){
        int error = leftSensor - black;//error > 0 = black, error < 0 = white
        //float speed_modifier = error * 0.7;
        mav(0,speed-error);
        mav(3,speed+error);
    }
	stop();
}

void BlackPeopleTerminatorleft(int speed){
	cmpc(0);
    while(rightSensor<black){
        int error = leftSensor - black;//error > 0 = black, error < 0 = white
        //float speed_modifier = error * 0.7;
        mav(3,speed-error);
        mav(0,speed+error);
    }
	stop();
}