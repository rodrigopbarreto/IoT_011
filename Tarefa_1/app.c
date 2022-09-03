#include <stdio.h>
#include <pthread.h>

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "semphr.h"

/* Local includes. */
#include "console.h"

#define TASK1_PRIORITY 0 // Tarefa do LED verde piscando
#define TASK2_PRIORITY 0 // Tarefa do LED vermelho acendendo sempre que digitar um numero
#define TASK3_PRIORITY 0 // Tarefa do LED vermelho ficando aceso
#define TASK4_PRIORITY 1 // Tarefa de leitura do teclado

#define BLACK "\033[30m" /* Black */
#define RED "\033[31m"   /* Red */
#define GREEN "\033[32m" /* Green */
#define DISABLE_CURSOR() printf("\e[?25l")
#define ENABLE_CURSOR() printf("\e[?25h")

#define clear() printf("\033[H\033[J")
#define gotoxy(x, y) printf("\033[%d;%dH", (y), (x))

typedef struct
{
    int pos;
    char *color;
    int period_ms;
} st_led_param_t;

st_led_param_t green = {        // LED verde piscando por 500ms
    6,
    GREEN,
    500};
st_led_param_t red = {          // LED vermelho piscando quando se tecla um numero
    13,
    RED,
    100};
TaskHandle_t greenTask_hdlr, redTask_hdlr, redTask_lit_hldr;    // handlers das tarefas 1, 2 e 3

#include <termios.h>

static void prvTask_getChar(void *pvParameters)
{
    char key;
    int n;

    /* I need to change  the keyboard behavior to
    enable nonblock getchar */
    struct termios initial_settings,
        new_settings;

    tcgetattr(0, &initial_settings);

    new_settings = initial_settings;
    new_settings.c_lflag &= ~ICANON;
    new_settings.c_lflag &= ~ECHO;
    new_settings.c_lflag &= ~ISIG;
    new_settings.c_cc[VMIN] = 0;
    new_settings.c_cc[VTIME] = 1;

    tcsetattr(0, TCSANOW, &new_settings);
    /* End of keyboard configuration */
    for (;;)
    {
        int stop = 0;
        key = getchar();            // Leitura do teclado se for numero pisca o LED vermelho que esta apagado por 100ms
                                    // Se for '*' LED vermelho acesso e LED verde apagado
                                    // Se for '+' tudo volta ao normal

        switch (key)
        {
        case '1':
            vTaskResume(redTask_hdlr);
            break;
        case '2':
            vTaskResume(redTask_hdlr);
            break;
        case '3':
            vTaskResume(redTask_hdlr);
            break;
        case '4':
            vTaskResume(redTask_hdlr);
            break;
        case '5':
            vTaskResume(redTask_hdlr);
            break;
            case '6':
            vTaskResume(redTask_hdlr);
            break;
            case '7':
            vTaskResume(redTask_hdlr);
            break;
            case '8':
            vTaskResume(redTask_hdlr);
            break;
            case '9':
            vTaskResume(redTask_hdlr);
            break;
            case '0':
            vTaskResume(redTask_hdlr);
            break;
            case '*':
            vTaskSuspend(greenTask_hdlr);
            vTaskResume(redTask_lit_hldr);
            break;
            case '+':
            vTaskResume(greenTask_hdlr);
            vTaskSuspend(redTask_lit_hldr);
            break;
        }
        if (stop)
        {
            break;
        }
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
    tcsetattr(0, TCSANOW, &initial_settings);
    ENABLE_CURSOR();
    exit(0);
    vTaskDelete(NULL);
}
static void prvTask_led_lit(void *pvParameters)         // Task do LED vermelho aceso
{
    // pvParameters contains LED params
    st_led_param_t *led = (st_led_param_t *)pvParameters;
    portTickType xLastWakeTime = xTaskGetTickCount();
    for (;;)
    {
        // console_print("@");
        gotoxy(led->pos, 2);
        printf("%s⬤", RED);
        fflush(stdout);
        xTaskNotify(redTask_hdlr, 1UL, eSetValueWithOverwrite);

        // gotoxy(led->pos, 2);
        // printf("%s ", BLACK);
        // fflush(stdout);
        // vTaskDelay(led->period_ms / portTICK_PERIOD_MS);
        // vTaskDelayUntil(&xLastWakeTime, led->period_ms / portTICK_PERIOD_MS);
    }

    vTaskDelete(NULL);
}
static void prvTask_led_green(void *pvParameters)         // Task do LED verde piscando
{
    // pvParameters contains LED params
    st_led_param_t *led = (st_led_param_t *)pvParameters;
    portTickType xLastWakeTime = xTaskGetTickCount();
    for (;;)
    {
        // console_print("@");
        gotoxy(led->pos, 2);
        printf("%s⬤", GREEN);
        fflush(stdout);
        vTaskDelay(led->period_ms / portTICK_PERIOD_MS);
        // vTaskDelayUntil(&xLastWakeTime, led->period_ms / portTICK_PERIOD_MS);

        gotoxy(led->pos, 2);
        printf("%s ", BLACK);
        fflush(stdout);
        vTaskDelay(led->period_ms / portTICK_PERIOD_MS);
        // vTaskDelayUntil(&xLastWakeTime, led->period_ms / portTICK_PERIOD_MS);
    }

    vTaskDelete(NULL);
}
static void prvTask_led_red(void *pvParameters)         // Task do LED vermelho acendendo ao teclar numero
{
    // pvParameters contains LED params
    st_led_param_t *led = (st_led_param_t *)pvParameters;
    portTickType xLastWakeTime = xTaskGetTickCount();
    for (;;)
    {
        // console_print("@");
        gotoxy(led->pos, 2);
        printf("%s⬤", RED);
        fflush(stdout);
        vTaskDelay(led->period_ms / portTICK_PERIOD_MS);
        // vTaskDelayUntil(&xLastWakeTime, led->period_ms / portTICK_PERIOD_MS);

        gotoxy(led->pos, 2);
        printf("%s ", BLACK);
        fflush(stdout);
        // vTaskDelay(led->period_ms / portTICK_PERIOD_MS);
        // vTaskDelayUntil(&xLastWakeTime, led->period_ms / portTICK_PERIOD_MS);
    }

    vTaskDelete(NULL);
}
void app_run(void)
{

    clear();
    DISABLE_CURSOR();
    printf(
        "╔═════════════════╗\n"
        "║                 ║\n"
        "╚═════════════════╝\n");

    xTaskCreate(prvTask_led_green, "LED_green", configMINIMAL_STACK_SIZE, &green, TASK1_PRIORITY, &greenTask_hdlr);
    xTaskCreate(prvTask_led_red, "LED_red", configMINIMAL_STACK_SIZE, &red, TASK2_PRIORITY, &redTask_hdlr);
    vTaskSuspend(redTask_hdlr);
    xTaskCreate(prvTask_led_lit, "LED_red", configMINIMAL_STACK_SIZE, &red, TASK3_PRIORITY, &redTask_lit_hldr);
    vTaskSuspend(redTask_lit_hldr);
    xTaskCreate(prvTask_getChar, "Get_key", configMINIMAL_STACK_SIZE, NULL, TASK4_PRIORITY, NULL);

    /* Start the tasks and timer running. */
    vTaskStartScheduler();

    /* If all is well, the scheduler will now be running, and the following
     * line will never be reached.  If the following line does execute, then
     * there was insufficient FreeRTOS heap memory available for the idle and/or
     * timer tasks      to be created.  See the memory management section on the
     * FreeRTOS web site for more details. */
    for (;;)
    {
    }
}