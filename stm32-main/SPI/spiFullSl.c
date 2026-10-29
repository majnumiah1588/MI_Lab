/**
 * SPI TWO-WAY SWITCH — SLAVE — Full-duplex — NUCLEO-F446RE
 * প্রতিটা transfer-এ MISO-তে আমার বাটন-স্টেট+toggle, MOSI-তে master-এর।
 * কোনো handshake/ack নেই; slave-ও non-blocking blink করে।
 */
#define BLINK_MS        200
#define LITTLE_ON_MS    120

#define RCC_AHB1ENR  (*(volatile unsigned int *)0x40023830)
#define RCC_APB1RSTR (*(volatile unsigned int *)0x40023820)
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

static void spi_reset(void) {
    RCC_APB1RSTR |=  (1 << 14);
    RCC_APB1RSTR &= ~(1 << 14);
    SPI2_CR1 = (1 << 9) | (1 << 6);       /* slave, SSM, enable */
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
    GPIOB_PUPDR &= ~((3 << 26) | (3U << 30));
    GPIOB_PUPDR |=  ((2 << 26) | (2U << 30));                      /* SCK+MOSI pull-down */
    GPIOC_PUPDR &= ~(3 << 26);  GPIOC_PUPDR |= (1 << 26);          /* PC13 pull-up */

    spi_reset();                                     /* clean start as slave */

    /* ---- state ---- */
    unsigned int  my_toggle  = 0;
    int           my_prev    = 0;
    unsigned int  rx_toggle  = 0;
    int           first_rx   = 1;

    int           blink_remaining = 0;
    unsigned int  blink_timer     = 0;
    int           led_on          = 0;

    /* প্রথম transfer-এর আগে একবার reply লোড করা */
    unsigned char reply = (pressed() ? 1u : 0u) | (my_toggle << 1);
    while (!(SPI2_SR & (1 << 1)));
    SPI2_DR = reply;

    while (1) {
        /* ---- 1) master একটি byte পাঠালে: সেটা পড়ো ---- */
        while (!(SPI2_SR & (1 << 0)));               /* RXNE */
        unsigned int  sr  = SPI2_SR;
        unsigned char cmd = (unsigned char)SPI2_DR;

        if (!(sr & (1 << 1))) spi_reset();           /* one byte late হলে রিসেট */

        /* ---- 2) নিজের বাটন-edge → toggle ফ্লিপ ---- */
        int now = pressed();
        if (now && !my_prev) my_toggle ^= 1u;
        my_prev = now;

        /* ---- 3) master-এর reply থেকে তার বাটন-event পড়া ---- */
        unsigned int r_tgl = (cmd >> 1) & 1u;
        if (!first_rx && r_tgl != rx_toggle) {
            blink_remaining = 5 * 2;                 /* ৫ বার on/off */
            blink_timer     = 0;
            led_on          = 0;
        }
        rx_toggle = r_tgl;
        first_rx  = 0;

        /* ---- 4) next reply লোড করা (নিজের সবশেষ স্টেট) ---- */
        unsigned char out = (pressed() ? 1u : 0u) | (my_toggle << 1);
        while (!(SPI2_SR & (1 << 1)));               /* TXE */
        SPI2_DR = out;

        /* ---- 5) non-blocking blink (master-এর পরের transfer-এও সাড়া দেয়) ---- */
        if (blink_remaining > 0) {
            if (blink_timer == 0) {
                led_on = !led_on;
                if (led_on) GPIOA_ODR |=  (1 << 5);
                else        GPIOA_ODR &= ~(1 << 5);
                blink_remaining--;
                blink_timer = BLINK_MS;
            } else {
                blink_timer--;
            }
        } else {
            GPIOA_ODR &= ~(1 << 5);
        }
    }
}