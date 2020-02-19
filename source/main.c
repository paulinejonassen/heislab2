#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "hardware.h"
#include "lights.h"
#include "door.h"
#include "stop.h"
#include "queue.h"

static void sigint_handler(int sig){
    (void)(sig);
    printf("Terminating elevator\n");
    hardware_command_movement(HARDWARE_MOVEMENT_STOP);
    exit(0);
}

int main(){
    int error = hardware_init();
    if(error != 0){
        fprintf(stderr, "Unable to initialize hardware\n");
        exit(1);
    }

    signal(SIGINT, sigint_handler);

    printf("=== Example Program ===\n");
    printf("Press the stop button on the elevator panel to exit\n");

    hardware_command_movement(HARDWARE_MOVEMENT_UP);
    


    while(1){
        if(hardware_read_stop_signal()){
            hardware_command_movement(HARDWARE_MOVEMENT_STOP);
            break;
        }
        
       

        if(hardware_read_floor_sensor(0)){
            hardware_command_movement(HARDWARE_MOVEMENT_UP);
            set_start_time();

        }
        if(counting_3seconds()){
            hardware_command_door_open(1);
        }
        else {
        hardware_command_door_open(0);
        }

        if(hardware_read_floor_sensor(HARDWARE_NUMBER_OF_FLOORS - 1)){
            hardware_command_movement(HARDWARE_MOVEMENT_DOWN);
        }
        door_obstruction();
        set_order_light_on();
        set_order_light_off();
        //set_floor_stop(1);
        set_floor_light();
        set_stop_light();
        add_to_queue();
        delete_order();
        
        for (int i= 0; i< 4; i++){
            int floor =down_queue[i];
            printf ("%d",floor);
        }

    }
}