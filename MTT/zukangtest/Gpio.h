#ifndef GPIO_H
#define GPIO_H


int GPIO_OutEnable(int fd, unsigned int dwEnBits);

int GPIO_OutDisable(int fd, unsigned int dwDisBits);

int GPIO_OutSet(int fd, unsigned int dwSetBits);

int GPIO_OutClear(int fd, unsigned int dwClearBits);

int GPIO_PinState(int fd, unsigned int* pPinState);


void SetLED();
void ClearLED();


#endif // GPIO_H
