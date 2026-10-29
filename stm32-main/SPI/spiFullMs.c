/**
 * SPI TWO-WAY SWITCH — MASTER — Full-duplex — NUCLEO-F446RE
 * প্রতি transfer-এ MOSI-তে আমার বাটন-স্টেট+toggle, MISO-তে slave-এর।
 * কোনো handshake delay নেই; একসাথে দুই বাটন চাপলেও দুই দিকের blink একসাথে।
 */
#define BLINK_MS        200
#define POLL_MS         1        /* প্রতি transfer-এর পরে ছোট গ্যাপ */
#define LITTLE_ON_MS    120

#define RCC_AHB1ENR  (*(volatile unsigned int *)0x40023830)
#define RCC_APB1ENR  (*(volatile unsigned int *)0x40023840)
#define GPIOA_MODER  (*(volatile unsigned int *)0x40020000)
#define GPIOA_ODR    (*(volatile unsigned int *)0x40020014)
#define GPIOB_MODER  (*(volatile unsigned int *)0x40020400)
#define GPIOB_PUPDR  (*(volatile unsigned int *)0x4002040C)
#define GPIOB_AFRH   (*(volatile unsigned int *)0x40020424)
#define GPIOC_PUPDR  (*(volatile unsigned int *)0x4002080C)
#define GPIOC_IDR    (*(volatile unsigned int *)0x40020810)
#define SPI2_CR1     (*(volatile unsigned int *)0x40003800)
#define SPI2_SR      (*(volatile unsigned int *)0x40003808)
#define SPI2_DR      (*(volatile unsigned int *)0x4000380C)

void SystemInit(void) __attribute__((weak));        void SystemInit(void) { }
void __libc_init_array(void) __attribute__((weak)); void __libc_init_array(void) { }

static void delay_ms(unsigned int ms) {
    for (unsigned int m = 0; m < ms; m++)
        for (volatile unsigned int i = 0; i < 1300; i++);
}
static int pressed(void) { return (GPIOC_IDR & (1 << 13)) == 0; }
static unsigned char spi_xfer(unsigned char b) {
    while (!(SPI2_SR & (1 << 1)));
    SPI2_DR = b;
    while (!(SPI2_SR & (1 << 0)));
    return (unsigned char)SPI2_DR;
}

int main(void)
{
    RCC_AHB1ENR |= (1 << 0) | (1 << 1) | (1 << 2);
    RCC_APB1ENR |= (1 << 14);

    GPIOA_MODER &= ~(3 << 10);  GPIOA_MODER |= (1 << 10);          /* PA5 LED */
    GPIOB_MODER &= ~((3 << 26) | (3 << 28) | (3U << 30));
    GPIOB_MODER |=  ((2 << 26) | (2 << 28) | (2U << 30));
    GPIOB_AFRH  &= ~((0xF << 20) | (0xF << 24) | (0xFU << 28));
    GPIOB_AFRH  |=  ((5 << 20) | (5 << 24) | (5U << 28));
    GPIOB_PUPDR &= ~(3 << 28);  GPIOB_PUPDR |= (2 << 28);          /* MISO pull-down */
    GPIOC_PUPDR &= ~(3 << 26);  GPIOC_PUPDR |= (1 << 26);          /* PC13 pull-up */

    SPI2_CR1 = (1 << 2) | (5 << 3) | (1 << 9) | (1 << 8) | (1 << 6);   /* master */

    /* ---- state ---- */
    unsigned int  my_toggle  = 0;     /* প্রতি নিজের rising-edge-এ ফ্লিপ */
    int           my_prev    = 0;
    unsigned int  rx_toggle  = 0;     /* slave-এর শেষ দেখা toggle */
    int           first_rx   = 1;     /* প্রথম reply-তে bogus event এড়াতে */

    /* ---- non-blocking blink state ---- */
    int           blink_remaining = 0;      /* কতগুলো অর্ধ-পিরিয়ড বাকি */
    unsigned int  blink_timer     = 0;
    int           led_on          = 0;

    while (1) {
        /* ---- 1) প্রতি লুপে এক transfer: full-duplex ---- */
        unsigned char tx = (pressed() ? 1u : 0u) | (my_toggle << 1);
        unsigned char rx = spi_xfer(tx);

        /* ---- 2) নিজের বাটনের edge → toggle ফ্লিপ ---- */
        int now = pressed();
        if (now && !my_prev) my_toggle ^= 1u;      /* নতুন event ঘোষণা */
        my_prev = now;

        /* ---- 3) slave-এর reply থেকে তার বাটন-event পড়া ---- */
        unsigned int r_tgl = (rx >> 1) & 1u;
        if (!first_rx && r_tgl != rx_toggle) {
            blink_remaining = 5 * 2;               /* ৫ বার on/off */
            blink_timer     = 0;
            led_on          = 0;
        }
        rx_toggle = r_tgl;
        first_rx  = 0;

        /* ---- 4) non-blocking blink: বাকি কাজ না থামিয়ে চালাও ---- */
        if (blink_remaining > 0) {
            if (blink_timer == 0) {
                led_on = !led_on;
                if (led_on) GPIOA_ODR |=  (1 << 5);
                else        GPIOA_ODR &= ~(1 << 5);
                blink_remaining--;
                blink_timer = (blink_remaining & 1) ? BLINK_MS : BLINK_MS;
            } else {
                blink_timer--;
            }
        } else {
            GPIOA_ODR &= ~(1 << 5);                /* idle: বন্ধ */
        }

        delay_ms(POLL_MS);
    }
}