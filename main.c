#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#include "McGoogles.h"

// Feel free to play with these numbers! This is a great way to
// test your implementation.
#define MCGOOGLES_SIZE 3
#define NUM_CUSTOMERS 50
#define NUM_COOKS 5
#define ORDERS_PER_CUSTOMER 100
#define EXPECTED_NUM_ORDERS NUM_CUSTOMERS * ORDERS_PER_CUSTOMER

// Global variable for the restaurant.
McGoogles *mcg;

/**
 * Thread funtion that represents a customer. A customer should:
 *  - allocate space (memory) for an order.
 *  - select a menu item.
 *  - populate the order with their menu item and their customer ID.
 *  - add their order to the restaurant.
 */

void* McGooglesCustomer(void* tid) {
    int customer_id = *(int*) tid;
    int orders_made = 0;
    
    while (orders_made < ORDERS_PER_CUSTOMER) {
        Order *order = malloc(sizeof(Order));
        order->menu_item = PickRandomMenuItem();
        order->customer_id = customer_id;
        order->next = NULL;

        AddOrder(mcg, order);
//         printf("Customer #%d added 1 order\n", customer_id);
        orders_made++;
    }
    
	return NULL;
}

/**
 * Thread function that represents a cook in the restaurant. A cook should:
 *  - get an order from the restaurant.
 *  - if the order is valid, it should fulfill the order, and then
 *    free the space taken by the order.
 * The cook should take orders from the restaurants until it does not
 * receive an order.
 */

void* McGooglesCook(void* tid) {
    int cook_id = *(int*) tid;
	int orders_fulfilled = 0;
    
    while (mcg->orders_handled < mcg->expected_num_orders) {
        Order *order;
        if (order = GetOrder(mcg)) {
            free(order);
            orders_fulfilled++;
        }
    }
    
    printf("Cook #%d fulfilled %d orders\n", cook_id, orders_fulfilled);
    
	return NULL;
}

/**
 * Runs when the program begins executing. This program should:
 *  - open the restaurant
 *  - create customers and cooks
 *  - wait for all customers and cooks to be done
 *  - close the restaurant.
 */
int main() {
    // Open the restaurant.
    mcg = OpenRestaurant(MCGOOGLES_SIZE, EXPECTED_NUM_ORDERS);
    
    // Create customers and cooks.
    pthread_t customers[NUM_CUSTOMERS];
    pthread_t cooks[NUM_COOKS];

    int customer_counter[NUM_CUSTOMERS];
    int cook_counter[NUM_COOKS];
    for(int i = 0; i < NUM_CUSTOMERS; i++) {
        customer_counter[i] = i;
        pthread_create(&customers[i], NULL, McGooglesCustomer, (void*) &customer_counter[i]);
    }
    
    for(int i = 0; i < NUM_COOKS; i++) {
        cook_counter[i] = i;
        pthread_create(&cooks[i], NULL, McGooglesCook, (void*) &cook_counter[i]);
    }
    
    // Wait for all customers and cooks to be done.
    for (int i = 0; i < NUM_CUSTOMERS; i++) {
        pthread_join(customers[i], NULL);
    }
    
    for (int i = 0; i < NUM_COOKS; i++) {
        pthread_join(cooks[i], NULL);
    }
    
    // Close the restaurant.
    CloseRestaurant(mcg);
    return 0;
}
