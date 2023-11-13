static const uint8_t const font[128*8];
const uint8_t const dog_icon[128];
extern char textbuffer[4][16];

void delay(int cyc);
uint8_t spi_send_recv(uint8_t data);
void display_init();
void display_string(int line, char *s);
void display_update(const uint8_t *data);
void display_reset();
void display_update_text();
void conv_normal_to_disp(uint32_t *map, uint8_t *disp);

int getsw(void);
int getbtns(void);
