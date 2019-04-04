#include "McGoogles.h"

#include <assert.h>
#include <stdlib.h>
#include <time.h>

bool IsEmpty(McGoogles* mcg);
bool IsFull(McGoogles* mcg);
void AddOrderToBack(Order **orders, Order *order);

MenuItem McGooglesMenu[] = { 
    "GoogMac", 
    "GoogDouble", 
    "GoogChicken", 
    "GoogQuarterPounder", 
    "GoogNuggets",
    "GoogCrispyChicken",
    "GoogHamburger",
    "GoogCheeseBurger",
    "GoogSmokeHouseBurger",
    "GoogFiletOFish",
};

int McGooglesMenuLength = 10;

MenuItem PickRandomMenuItem() {
    int r = rand() % 10;      // Returns a pseudo-random integer between 0 and 9, inclusive.
    MenuItem item = McGooglesMenu[r];
    return item;
}

McGoogles* OpenRestaurant(int max_size, int expected_num_orders) {
    printf("\nRestaurant is open!\n");
    
    srand(time(NULL));   // Initialization, should only be called once.
    McGoogles* mcg = malloc(sizeof(McGoogles));
    mcg->orders = NULL;
    mcg->current_size = 0;
    mcg->max_size = max_size;
    mcg->next_order_number = 0;
    mcg->orders_handled = 0;
    mcg->expected_num_orders = expected_num_orders;
    pthread_mutex_init(&mcg->mutex, NULL);
    pthread_cond_init(&mcg->can_add_orders, NULL);
    pthread_cond_init(&mcg->can_get_orders, NULL);
    
    return mcg;
}

void CloseRestaurant(McGoogles* mcg) {
    if (mcg->orders) {
        printf("Some orders remaining!\n");
    } else {
        printf("All orders fulfilled!\n");
    }
    
    if (mcg->orders_handled == mcg->expected_num_orders) {
        printf("Expected number of orders handled!\n");
    } else {
        printf("Expected number of orders not handled!\n");
    }
    
    pthread_mutex_destroy(&mcg->mutex);
    pthread_cond_destroy(&mcg->can_add_orders);
    pthread_cond_destroy(&mcg->can_get_orders);
    free(mcg);
    
    printf("Restaurant is closed!\n\n");
}

int AddOrder(McGoogles* mcg, Order* order) {
    pthread_mutex_lock(&mcg->mutex);
    
    while (IsFull(mcg)) {
        pthread_cond_wait(&mcg->can_add_orders, &mcg->mutex);
    }
    
    AddOrderToBack(&mcg->orders, order);
    (mcg->current_size)++;
    
    pthread_cond_signal(&mcg->can_get_orders);
    pthread_mutex_unlock(&mcg->mutex);
    
    return -1;
}

Order *GetOrder(McGoogles* mcg) {
    pthread_mutex_lock(&mcg->mutex);
    
    // The second condition is to prevent all the remaining waiting 
    // cooks to be stuck in the loop when the final order is completed,
    // and a broadcast to wake up is sent.
    while (IsEmpty(mcg) &&
           mcg->orders_handled < mcg->expected_num_orders) {
        pthread_cond_wait(&mcg->can_get_orders, &mcg->mutex);
    }
    
    Order *head = mcg->orders;
    
    // If head is a valid order.
    if (head) {
        mcg->orders = head->next;
        (mcg->current_size)--;
        (mcg->orders_handled)++;
    }
    
    // If final order, broadcast can_get_orders to wake up all
    // the other waiting cooks.
    if (mcg->orders_handled == mcg->expected_num_orders) {
        pthread_cond_broadcast(&mcg->can_get_orders);
    } else {
        pthread_cond_signal(&mcg->can_add_orders);
    }
    pthread_mutex_unlock(&mcg->mutex);
    
    return head;
}

// Optional helper functions (you can implement if you think they would be useful)
bool IsEmpty(McGoogles* mcg) {
    // Less than is just for sanity check.
    return mcg->current_size <= 0;
}

bool IsFull(McGoogles* mcg) {
    // Greater than is just for sanity check.
    return mcg->current_size >= mcg->max_size;
}

void AddOrderToBack(Order **orders, Order *order) {
    Order *curr = *orders;
    if (!curr) {
        *orders = order;
    } else {
        while (curr->next)
            curr = curr->next;
        curr->next = order;
    }
}
