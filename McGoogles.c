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
    return NULL;
}

McGoogles* OpenRestaurant(int max_size, int expected_num_orders) {
    printf("Restaurant is open!\n");
    return NULL;
}

void CloseRestaurant(McGoogles* mcg) {
    printf("Restaurant is closed!\n");
}

int AddOrder(McGoogles* mcg, Order* order) {
    return -1;
}

Order *GetOrder(McGoogles* mcg) {
    return NULL;
}

// Optional helper functions (you can implement if you think they would be useful)
bool IsEmpty(McGoogles* mcg) {
  return false;
}

bool IsFull(McGoogles* mcg) {
  return false;
}

void AddOrderToBack(Order **orders, Order *order) {}
