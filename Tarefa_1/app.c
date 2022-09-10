// ************************************************************************************************************
// Disciplina: IoT011                                                                                         *
// Aluno: Rodrigo Policarpo Barreto                                                                           *
// Exercicio 1                                                                                                *
//                                                                                                            *                                                      
// Projeto pensado da seguinte maneira:                                                                       *
//                                                                                                            *
// 1 - A partir do exemplo Demo_4, criei 4 tarefas: 1) Leitura do teclado; 2) LED verde piscando regularmente *
// 3) LED vermelho acendendo sempre que um numero for digitado 4) LED vermelho ficando aceso. Montei toda a   *
// estrutura das tasks conforme entendi corretas, mas nao consegui que o sistema funcionasse. Até onde conse  *
// gui verificar, as tasks foram corretamente criadas. Posso estar enganado mas o que acredito é que haja al  *
// guma falha no processo de notificacao entre as tarefas.                                                    *
//                                                                                                            *
//    Funcionamento proposto e esperado:                                                                      *                                                                      *
//                                                                                                            *
//   > LED verde pisca de forma contínua com intervalo de 500ms.                                              *
//   > LED vermelho pisca por 100ms sempre que for digitado um numero.                                        *
//   > Sempre que o usuario digitar * o LED vermelho deverá ficar aceso e o verde deverá apagar.              *
//   > O sistema volta ao funcionamento normal sempre que for digitado +.                                     *
//                                                                                                            *
// ************************************************************************************************************


#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <termios.h>

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "semphr.h"

/* Local includes. */
#include "console.h"

#define TASK1_PRIORITY 1 // Tarefa do LED verde piscando
#define TASK2_PRIORITY 1 // Tarefa do LED vermelho acendendo sempre que digitar um numero
#define TASK3_PRIORITY 1 // Tarefa do LED vermelho ficando aceso
#define TASK4_PRIORITY 0 // Tarefa de leitura do teclado

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
TaskHandle_t greenTask_hdlr, redTask_hdlr, redTask_lit_hdlr;    // handlers das tarefas 1, 2 e 3

#include <termios.h>

static void prvTask_getChar(void *pvParameters)
{
    char key;
    int n;
    uint32_t notificationValue_getChar;

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
        key = getchar();            // Testa a tecla digitada. Se for numero pisca o LED mermelho por 100ms. O verde continua piscando
                                    // Se o usuario digitar * o LED verde apaga e o vermelho acende e permanece aceso
                                    // Se o usuario digitar + o sistema volta a funcionar como antes. Verde piscando e vermelho apagado

        switch (key)
        {
        case '1':
            xTaskNotify(redTask_hdlr, 1, eSetValueWithOverwrite);
            break;
        case '2':
            xTaskNotify(redTask_hdlr, 1, eSetValueWithOverwrite);
            break;
        case '3':
            xTaskNotify(redTask_hdlr, 1, eSetValueWithOverwrite);
            break;
        case '4':
            xTaskNotify(redTask_hdlr, 1, eSetValueWithOverwrite);
            break;
        case '5':
            xTaskNotify(redTask_hdlr, 1, eSetValueWithOverwrite);
            break;
            case '6':
            xTaskNotify(redTask_hdlr, 1, eSetValueWithOverwrite);
            break;
            case '7':
            xTaskNotify(redTask_hdlr, 1, eSetValueWithOverwrite);
            break;
            case '8':
            xTaskNotify(redTask_hdlr, 1, eSetValueWithOverwrite);
            break;
            case '9':
            xTaskNotify(redTask_hdlr, 1, eSetValueWithOverwrite);
            break;
            case '0':
            xTaskNotify(redTask_hdlr, 1, eSetValueWithOverwrite);
            break;
            case '*':
            vTaskNotify(greenTask_hdlr, 0, eSetValueWithOverwrite);
            vTaskNotify(redTask_hdlr, 0, eSetValueWithOverwrite);
            break;
            case '+':
            vTaskSuspend(greenTask_hdlr);
            vTaskNotify(redTask_lit_hdlr, 0, eSetValueWithOverwrite);
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
static void prvTask_led_green(void *pvParameters)         // Task do LED verde piscando
{
    uint32_t notificationValue_green
    
    // pvParameters contains LED params
    st_led_param_t *led = (st_led_param_t *)pvParameters;
    portTickType xLastWakeTime = xTaskGetTickCount();
    for (;;)
    {
        if (xTaskNotifyWait(
                ULONG_MAX,
                ULONG_MAX,
                &notificationValue_green,
                portMAX_DELAY))
        {
            if (notificationValue_red == 1)
            {
        gotoxy(led->pos, 2);
        printf("%s⬤", led->color);
        fflush(stdout);
        vTaskDelay(led->period_ms / portTICK_PERIOD_MS);
        // vTaskDelayUntil(&xLastWakeTime, led->period_ms / portTICK_PERIOD_MS);

        gotoxy(led->pos, 2);
        printf("%s ", BLACK);
        fflush(stdout);
        // vTaskDelay(led->period_ms / portTICK_PERIOD_MS);
        // vTaskDelayUntil(&xLastWakeTime, led->period_ms / portTICK_PERIOD_MS);
    }}

    vTaskDelete(NULL);
}
static void prvTask_led_red(void *pvParameters)         // Task do LED vermelho acendendo ao teclar um numero
{
    uint32_t notificationValue_red

// pvParameters contains LED params
    st_led_param_t *led = (st_led_param_t *)pvParameters;
    portTickType xLastWakeTime = xTaskGetTickCount();
    for (;;)
    {
         if (xTaskNotifyWait(
                ULONG_MAX,
                ULONG_MAX,
                &notificationValue_red,
                portMAX_DELAY))
        {
            if (notificationValue_red == 1)
            {
        gotoxy(led->pos, 2);
        printf("%s⬤", led->color);
        fflush(stdout);
        vTaskDelay(led->period_ms / portTICK_PERIOD_MS);
        // vTaskDelayUntil(&xLastWakeTime, led->period_ms / portTICK_PERIOD_MS);

        gotoxy(led->pos, 2);
        printf("%s ", BLACK);
        fflush(stdout);
        // vTaskDelay(led->period_ms / portTICK_PERIOD_MS);
        // vTaskDelayUntil(&xLastWakeTime, led->period_ms / portTICK_PERIOD_MS);
            }
     }

    vTaskDelete(NULL);
}
static void prvTask_led_lit(void *pvParameters)         // Task do LED vermelho aceso
{
    uint32_t notificationValue_lit
    // pvParameters contains LED params
    st_led_param_t *led = (st_led_param_t *)pvParameters;
    portTickType xLastWakeTime = xTaskGetTickCount();
    for (;;)
    {
         if (xTaskNotifyWait(
                0,
                0,
                &notificationValue_lit,
                portMAX_DELAY))

        // console_print("@");
        gotoxy(led->pos, 2);
        printf("%s⬤", led->color);
        fflush(stdout);
        vTaskDelay(portMAX_DELAY);
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
    xTaskCreate(prvTask_led_lit, "LED_red", configMINIMAL_STACK_SIZE, &red, TASK3_PRIORITY, &redTask_lit_hdlr);
    vTaskSuspend(redTask_lit_hdlr);
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