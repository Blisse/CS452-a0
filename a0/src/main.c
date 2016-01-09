#define FOREVER for( ; ; )

#include <pio.h>
#include <clock.h>
#include <error.h>
#include <escape_codes.h>

int main(int argc, char* argv[]) {

    pio_setup();
    clock_setup();

    pio_set_fifo(COM2, FIFO_ON);
    pio_set_speed(COM1, 2400);
    pio_set_speed(COM2, 115200);

    pio_print_f(COM2, "RTOS\r\n");
    pio_flush(COM2);

    timer_enable();
    timer_update();
    pio_print_f(COM2, "Timer Enabled\r\n");
    pio_print_f(COM2, "First Tick: %u \r\n", timer_get_ticks());
    pio_flush(COM2);

    pio_print_f(COM2, "Tests Started\r\n");
    pio_print_f(COM2, "Number Test 1: %d\r\n", 1);
    pio_print_f(COM2, "Number Test 2: %d %d\r\n", 2, 2);
    pio_print_f(COM2, "Number Test 3: %d %u %u\r\n", 3, 3, 3);
    pio_print_f(COM2, "Number Test 4: %u %d %u %d\r\n", 4, 3, 2, 1);
    pio_print_f(COM2, "Tests Completed\r\n");
    pio_flush(COM2);

    FOREVER {

        timer_update();

        if (clock_update() == SUCCESS) {
            pio_print_f(COM2, DELETE_LINE);
            pio_print_f(COM2, "\rt:");
            pio_print_f(COM2, "%u", (((clock_get_hundred_ms() / 10) / 60) % 60));
            pio_print_f(COM2, ":");
            pio_print_f(COM2, "%u", (clock_get_hundred_ms() / 10) % 60);
            pio_print_f(COM2, ":");
            pio_print_f(COM2, "%u", clock_get_hundred_ms() % 10);
        }

        pio_flush(COM2);

    }

    return SUCCESS;
}
