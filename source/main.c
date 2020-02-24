#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "hardware.h"
#include "lights.h"
#include "door.h"
#include "stop.h"
#include "queue.h"
#include "fsm.h"

static void sigint_handler(int sig){
    (void)(sig);
    printf("Terminating elevator\n");
    hardware_command_movement(HARDWARE_MOVEMENT_STOP);
    exit(0);
}
int c;
static int lastfloor;
static int lastMotorDirection;
int a;
int b;
int main(){
 

    
    
    printf("=== Example Program ===\n");
    printf("Press the stop button on the elevator panel to exit\n");

     int error = hardware_init();
        if(error != 0){
            fprintf(stderr, "Unable to initialize hardware\n");
            exit(1);
        }

    State change_state = INIT;
    
    while(1){
    signal(SIGINT, sigint_handler);
        
    switch (change_state)
    {
    case INIT:
        hardware_command_movement(HARDWARE_MOVEMENT_UP);
        lastMotorDirection =1;

        for( int i = 0; i<HARDWARE_NUMBER_OF_FLOORS; i++){
            if (hardware_read_floor_sensor(i)){
                hardware_command_movement(HARDWARE_MOVEMENT_STOP); 
                current_floor();
                change_state = STILL;
            }
        }
        break;
   
    case STILL:
        add_to_queue();
        set_floor_light();
        set_order_light_on();
        current_floor();
        update_last_floor(&lastfloor);
      
        if((current_floor()==-1) && (order_at_last_floor(&lastfloor))){
            if(lastMotorDirection == 1){
                hardware_command_movement(HARDWARE_ORDER_DOWN);
                lastMotorDirection = 0;
                change_state = MOVING;
            }
            else if(lastMotorDirection == 0){
                hardware_command_movement(HARDWARE_ORDER_UP);
                lastMotorDirection = 1;
                change_state = MOVING;
            }
        }
        else if(order_in_queues()){
            change_state = MOVING;
        } 
        else if(hardware_read_stop_signal()){
            change_state = EMERGENCY_STOP;
        }
        else if(check_order_at_current_floor()){
            set_start_time();
            change_state = DOOR;
        }
        break;

    case MOVING:
        add_to_queue();
        make_required_floors();
        update_last_floor(&lastfloor);
        set_order_light_on();
        set_floor_light();
        
        if (lastMotorDirection){
            if(check_order_above(&lastfloor)){
                hardware_command_movement(HARDWARE_MOVEMENT_UP);
                lastMotorDirection = 1; 
            }
            else if (check_order_below(&lastfloor)){
                hardware_command_movement(HARDWARE_MOVEMENT_DOWN);
                lastMotorDirection = 0;
            }
        }
        if (!lastMotorDirection){
            if(check_order_below(&lastfloor)){
                hardware_command_movement(HARDWARE_MOVEMENT_DOWN);
                lastMotorDirection = 0;
            }
            else if(check_order_above(&lastfloor)){
                hardware_command_movement(HARDWARE_MOVEMENT_UP);
                lastMotorDirection = 1;
            }
        }
        if(cab_button_at_current_floor()){
            set_start_time();
            change_state = DOOR;
        }
        else if(lastMotorDirection){
            if(up_button_at_current_floor()){
                set_start_time();
                change_state= DOOR;
            }
        
            if(!check_order_above(&lastfloor)){
                if(current_floor()>=0){
                    set_start_time();
                    change_state = DOOR;
                }
            }
            
        }
        else if(!lastMotorDirection){
            if(down_button_at_current_floor()){
                if(current_floor()>=0){
                    set_start_time();
                    change_state = DOOR;
                }
            }
            if(!check_order_below(&lastfloor)){
                set_start_time();
                change_state = DOOR; 
            }
            
        }
        
        if(hardware_read_stop_signal()){
            change_state = EMERGENCY_STOP;
        }

        break;

    case DOOR:
        delete_order();
        delete_required_floors(&lastfloor);
        
        set_order_light_off();
        set_order_light_on();
        add_to_queue();
        hardware_command_movement(HARDWARE_MOVEMENT_STOP);
        if(hardware_read_stop_signal()){
            change_state = EMERGENCY_STOP;
        }
        if(counting_3seconds()){
            hardware_command_door_open(1);
            if (hardware_read_obstruction_signal()) {
                hardware_command_door_open(1);
                set_start_time();
                change_state = DOOR;
            }
        }
        
        else {
        hardware_command_door_open(0);
        change_state = STILL;
        }
        break;

    case EMERGENCY_STOP:
        clear_all_order_lights();
        set_emergency_stop();
        delete_all_orders();
        set_stop_light();
        if(current_floor()>= 0){
            hardware_command_door_open(1);
        }
        if(hardware_read_stop_signal()){
            change_state = EMERGENCY_STOP; 
        }
        else if(current_floor()>= 0){
            clear_stop_light();
            set_start_time();
            change_state = DOOR;
        }
        else{
            clear_stop_light();
            change_state = STILL;
        }
        break;

    default:
        break;
    }
}           
}
