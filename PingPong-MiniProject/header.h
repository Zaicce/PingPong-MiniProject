//Header file


//declare functions in functions.c

void init(void);
void gameSettings(int);
void interruptInit(void);
void screen(void);
void scoreScreen(void);
void gameSettingScreen(void);
//void getbtns(void);
void gameSelection(void);
void difficulty(void);
void game();
void aiGame(void);
void aiScreen(void);
void aiScoreScreen(void);
void quickPlay(void);
//int rand();
//void srand (int);



//declare functions in main.c
void delay(int);
void display_image(int x, const uint8_t *data);
void display_init(void);
void display_string(int line, char *s);
void display_update(void);
uint8_t spi_send_recv(uint8_t data);
