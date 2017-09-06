#ifndef GPIO_H
#define GPIO_H


int GPIO_OutEnable(int fd, unsigned int dwEnBits);

int GPIO_OutDisable(int fd, unsigned int dwDisBits);

int GPIO_OutSet(int fd, unsigned int dwSetBits);

int GPIO_OutClear(int fd, unsigned int dwClearBits);
void SetLED();
void ClearLED();


int GPIO_PinState(int fd, unsigned int* pPinState);

#endif // GPIO_H
