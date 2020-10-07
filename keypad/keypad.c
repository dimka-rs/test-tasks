

void gpio_low(int gpio_num)
{
    // set gpio out low
}

void gpio_high(int gpio_num)
{
    // set gpio out high
}

int gpio_read(int gpio_name)
{
    // returns gpio input state
    return 1;
}

void os_delay_ms(int delay)
{
    // delay in ms
}

void main()
{

    enum
    {
        BTN_SHORT_MIN = 100,    // minimum short press time in ms
        BTN_SHORT_MAX = 500,    // maximum short press time in ms
        BTN_LONG_MIN = 1000,    // long press time in ms
        BTN_LONG_MAX = 5000,    // just limit this counter
        BTN_NUM = 12,           // number of buttons
        BTN_SHORT_MARK = -1,    // indicate unhandled short press
        BTN_LONG_DONE = -2,     // indicate handled long press
        SCAN_DELAY_MS = 1,      // delay between scans
        HANDLE_DELAY_MS = 10    // handle data once in X scans
    };

    // buttons names
    char names[BTN_NUM] = {'1', '2', '3',
                    '4', '5', '6',
                    '7', '8', '9',
                    '*', '0', '#'};

    // pressed buttons
    char pressed_short[BTN_NUM];
    char pressed_long[BTN_NUM];

    // gpio numbers for rows
    int rows[] = {0, 1, 2, 3};

    // gpio numbers for cols
    int cols[] = {4, 5, 6};

    // counters for buttons
    int buttons[12] = {0};

    // pointer to current value
    int *button;

    // counters for events
    int short_cnt, long_cnt, scan_cnt = 0;

    while (1)
    {
        // scan all buttons
        for (int row = 0; row < sizeof(rows); row++)
        {
            // set all rows high
            for (int i = 0; i < sizeof(rows); i++)
            {
                gpio_high(rows[i]);
            }

            // set current row low
            gpio_low(rows[row]);

            // read columns in current row
            for (int col = 0; col < sizeof(cols); col++)
            {
                // pointer to current button
                button = &buttons[row * sizeof(col) + col];

                if (gpio_read(cols[col]))
                {
                    // button released
                    if (*button > 0 && *button <= BTN_SHORT_MIN)
                    {
                        // too short press, reset counter
                        *button = 0;
                    }
                    else if (*button > 0 && *button >= BTN_SHORT_MAX)
                    {
                        // mark short press
                        *button = BTN_SHORT_MARK;
                    }

                    if (*button == BTN_LONG_DONE)
                    {
                        // depressed after long press, reset counter
                        *button = 0;
                    }
                }
                else
                {
                    // button pressed, increment counter
                    if (*button >= 0 && *button < BTN_LONG_MAX)
                    {
                        *button += 1;
                    }
                }
            }
        }

        // count available events and store pressed buttons
        scan_cnt++;
        if (scan_cnt >= SCAN_DELAY_MS)
        {
            scan_cnt = 0;
            short_cnt = 0;
            long_cnt = 0;

            for (int btn = 0; btn < sizeof(buttons); btn++)
            {
                if (buttons[btn] == BTN_SHORT_MARK)
                {
                    buttons[btn] = 0;
                    pressed_short[short_cnt] = names[btn];
                    short_cnt++;
                }

                if (buttons[btn] >= BTN_LONG_MIN)
                {
                    buttons[btn] = BTN_LONG_DONE;
                    pressed_long[long_cnt] = names[btn];
                    long_cnt++;
                }
            }

            // call short handlers
            switch (short_cnt)
            {
                case 0:
                    // nothing to do
                    break;

                case 1:
                    // just one short press
                    single_key_pressed(pressed_short[0]);
                    break;

                default:
                    // 2+ buttons pressed
                    double_key_pressed(pressed_short[0], pressed_short[1]);
                    break;
            }

            // call long handlers
            switch (long_cnt)
            {
                case 0:
                    // nothing to do
                    break;

                case 1:
                    // just one long press
                    single_key_long_pressed(pressed_long[0]);
                    break;

                default:
                    // 2+ buttons pressed
                    double_key_long_pressed(pressed_long[0], pressed_long[1]);
                    break;
            }
        }
    }

    // wait before next scan
    os_delay_ms(SCAN_DELAY_MS);
}
