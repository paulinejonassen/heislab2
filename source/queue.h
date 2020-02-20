
/**
 * @file
 * @brief Module for queue.
 *
 *
 * Pauline M. Jonassen & Helene E. Haugen
 */

/**
 * @brief checking if there are any orders at current floor.
 * @return 1 if there are orders at current floor, return 0 otherwise.
 */
int check_order_at_current_floor();
/**
 * @brief Array for inside queue, with @p HARDWARE_NUMBER_OF_FLOORS elements.
 */
int inside_queue[HARDWARE_NUMBER_OF_FLOORS];

/**
 * @brief Array for down queue, with @p HARDWARE_NUMBER_OF_FLOORS elements.
 */
int down_queue[HARDWARE_NUMBER_OF_FLOORS];

/**
 * @brief Array for the up queue with @p HARDWARE_NUMBER_OF_FLOORS elements.
 */
int up_queue[HARDWARE_NUMBER_OF_FLOORS];

int any_order();

/**
 * @brief function which are checking if any buttons are pushed at, if so it sets the right element in the right queue to 1.
 * @return 1 if there are any orders and 0 if not.
 */
void add_to_queue();

/**
 * @brief function checking which floor you are in. If there are any orders at current floor, the right element in the right floor is set to 0.
 */
void delete_order();


/**
 * @brief function setting every element in every queue to 0.
 */
void delete_all_orders();

/**
 * @brief function to remember the last floor the elvator was in.
 * @return returns a int with the last floor. 
 */
int last_floor();

/**
 * @brief function to check if there is any orders in a list. 
 * @return returns the floor if it is requierd [0-3] and -1 if there is no requierments. 
 */
int check_and_return_floor_inside();

int check_and_return_floor_up();

int check_and_return_floor_down();