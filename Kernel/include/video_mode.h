void BgaWriteRegister(unsigned short IndexValue, unsigned short DataValue);
unsigned short BgaReadRegister(unsigned short IndexValue);
int BgaIsAvailable(void);
void BgaSetVideoMode(unsigned int Width, unsigned int Height, unsigned int BitDepth, int UseLinearFrameBuffer, int ClearVideoMemory);
void BgaSetBank(unsigned short BankNumber);
int SetVideoMode(void);
void paintPixel(int x, int y, uint8_t blue, uint8_t green, uint8_t red);