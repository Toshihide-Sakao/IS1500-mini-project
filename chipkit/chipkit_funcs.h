// heart     128
// blob1     129
// blob2     130
// downarrow 131
// select    132
static const uint8_t const font[133*8];
extern char textbuffer[4][16];
const uint32_t const pistol[15 * 4];
const uint32_t const pistol_border[15];
const uint32_t const enemy[30 * 4];
const uint32_t const enemy_border[30];

void delay(int cyc);
uint8_t spi_send_recv(uint8_t data);
void display_init();
void display_string(int line, char *s);
void display_update(const uint32_t *map);
void display_reset();
void display_update_text();
void conv_normal_to_disp(uint32_t *map, uint8_t *disp);

int getsw(void);
int getbtns(void);
