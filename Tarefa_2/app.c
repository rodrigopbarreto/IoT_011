// ************************************************************************************************************
// Disciplina: IoT011                                                                                         *
// Aluno: Rodrigo Policarpo Barreto                                                                           *
// Exercicio final                                                                                            *
//                                                                                                            *                                                      
// Projeto pensado da seguinte maneira:                                                                       *
// 1 - À partir do exemplo Demo_4, criei 4 tarefas: 1) Leitura do teclado; 2) Montagem qa queue com as letras *
// digitadas 3) Codificacao da queue em pontos e tracos 4) LED verde piscando conforme o codigo de pontos e   *
// tracos criado na tarefa anterior                                                                           *
// Montei a estrutura das tasks conforme entendi corretas, mas nao consegui que o sistema funcionasse. Estou  *
// ainda tentando entender o funcionamento conjunto das queues e notificacoes. Alem disso estou encontrando   *
// grande dificuldade em programar em C.                                                                      *
//                                                                                                            *
//    Funcionamento proposto e esperado:                                                                      *                                                                      *
//                                                                                                            *
//   > Usuario digita uma frase ou conjunto de caracteres e tecla enter indicando o fim da queue              *
//   > Tarefa 2 monta a string com todas as letras digitadas                                                  *
//   > Tarefa 3 decodifica a string em simbolos Morse "." e "-" criando uma nova string de simbolos           *
//   > O LED pisca por 500ms para cada simbolo "-" e por 200 ms para cada simbolo "."                         *
//                                                                                                            *
// ************************************************************************************************************

#include <stdio.h>
#include <pthread.h>

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "semphr.h"

/* Local includes. */
#include "console.h"

#define KEY_ESC     0x1B // Tecla ESC - em caso de pressionada termina o programa
#define KEY_ENTER   0x0A // Tecla Entr para indicar o fim da mensagem a ser decodificada

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

st_led_param_t green = {        // Intervalo do LED verde para ".""
    6,
    GREEN,
    500};
// st_led_param_t red = {          // Intervalo do LED verde para "-"
 //    13,
 //   RED,
 //   200};
TaskHandle_t Tecla_hdlr, MorseLEDTask_hdlr, greenTask_lit_hldr, Frase_hdlr, MorseDecode_hdlr;    // handlers das tarefas 1, 2 e 3
QueueHandle_t TeclaQueue = NULL, MorseQueue = NULL, LEDQueue = NULL; // Declaracao dos handlers das queues
#include <termios.h>

static void prvTask_getChar(void *pvParameters)
{
    char tecla;
    // int n;

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

    char a[5];
    char b[5];
    char c[5];
    char d[5];
    char e[5];
    char f[5];
    char g[5];
    char h[5];
    char i[5];
    char j[5];
    char k[5];
    char l[5];
    char m[5];
    char n[5];
    char o[5];
    char p[5];
    char q[5];
    char r[5];
    char s[5];
    char t[5];
    char u[5];
    char v[5];
    char w[5];
    char x[5];
    char y[5];
    char z[5];
    char A[5];
    char B[5];
    char C[5];
    char D[5];
    char E[5];
    char F[5];
    char G[5];
    char H[5];
    char I[5];
    char J[5];
    char K[5];
    char L[5];
    char M[5];
    char N[5];
    char O[5];
    char P[5];
    char Q[5];
    char R[5];
    char S[5];
    char T[5];
    char U[5];
    char V[5];
    char W[5];
    char X[5];
    char Y[5];
    char Z[5];
    char 1[5];
    char 2[5];
    char 3[5];
    char 4[5];
    char 5[5];
    char 6[5];
    char 7[5];
    char 8[5];
    char 9[5];
    char 0[5];

    sprintf(a, ".-");
    sprintf(b, "-...");
    sprintf(c, "-.-.");
    sprintf(d, "-..");
    sprintf(e, ".");
    sprintf(f, "..-.");
    sprintf(g, "--.");
    sprintf(h, "....");
    sprintf(i, "..");
    sprintf(j, ".---");
    sprintf(k, "-.-");
    sprintf(l, ".-..");
    sprintf(m, "--");
    sprintf(n, "-.");
    sprintf(o, "---");
    sprintf(p, ".--.");
    sprintf(q, "--.-");
    sprintf(r, ".-.");
    sprintf(s, "...");
    sprintf(t, "-");
    sprintf(u, "..-");
    sprintf(v, "...-");
    sprintf(w, ".--");
    sprintf(x, "-..-");
    sprintf(y, "-.--");
    sprintf(z, "--..");
    sprintf(A, ".-");
    sprintf(B, "-...");
    sprintf(C, "-.-.");
    sprintf(D, "-..");
    sprintf(E, ".");
    sprintf(F, "..-.");
    sprintf(G, "--.");
    sprintf(H, "....");
    sprintf(I, "..");
    sprintf(J, ".---");
    sprintf(K, "-.-");
    sprintf(L, ".-..");
    sprintf(M, "--");
    sprintf(N, "-.");
    sprintf(O, "---");
    sprintf(P, ".--.");
    sprintf(Q, "--.-");
    sprintf(R, ".-.");
    sprintf(S, "...");
    sprintf(T, "-");
    sprintf(U, "..-");
    sprintf(V, "...-");
    sprintf(W, ".--");
    sprintf(X, "-..-");
    sprintf(Y, "-.--");
    sprintf(Z, "--..");
    sprintf(1, ".----");
    sprintf(2, "..---");
    sprintf(3, "...--");
    sprintf(4, "....-");
    sprintf(5, ".....");
    sprintf(6, "-....");
    sprintf(7, "--...");
    sprintf(8, "---..");
    sprintf(9, "----.");
    sprintf(0, "-----");
    
     
    while(!stop)
    {
        tecla = getchar();
        if (tecla > 0)
        {
            xQueueSend(TeclaQueue, &tecla, 0);
        }
    }
 
 
    tcsetattr(0, TCSANOW, &new_settings);
    vTaskDelay(100 / portTICK_PERIOD_MS);
    }
    tcsetattr(0, TCSANOW, &initial_settings);
    ENABLE_CURSOR();
    exit(0);
    vTaskDelete(NULL);
}
static void prvTask_Frase(void *pvParameters)         // Task 2  para enfileirar as teclas digitadas
{
    // pvParameters contains LED params
    st_led_param_t *led = (st_led_param_t *)pvParameters;
    portTickType xLastWakeTime = xTaskGetTickCount();
    char Tecla;
    
    for (;;)
    {
        
    if (xQueueReceive(TeclaQueue, simbolo, 0 )) ==pdpass
        {
            switch(Tecla)
            {
                case KEY_ESC
                stop = 1;
                break;
                case KEY_ENTER
                vTaskResume(Frase_hdlr);
                break;
                case 'a'
                xQueueSend(FraseQueue, Tecla, 0)
                case 'b'
                xQueueSend(FraseQueue, Tecla, 0)
                case 'c'
                xQueueSend(FraseQueue, Tecla, 0)
                case 'd'
                xQueueSend(FraseQueue, Tecla, 0)
                case 'e'
                xQueueSend(FraseQueue, Tecla, 0)
                case 'f'
                xQueueSend(FraseQueue, Tecla, 0)
                case 'g'
                xQueueSend(FraseQueue, Tecla, 0)
                case 'h'
                xQueueSend(FraseQueue, Tecla, 0)
                case 'i'
                xQueueSend(FraseQueue, Tecla, 0)
                case 'j'
                xQueueSend(FraseQueue, Tecla, 0)
                case 'k'
                xQueueSend(FraseQueue, Tecla, 0)
                case 'l'
                xQueueSend(FraseQueue, Tecla, 0)
                case 'm'
                xQueueSend(FraseQueue, Tecla, 0)
                case 'n'
                xQueueSend(FraseQueue, Tecla, 0)
                case 'o'
                xQueueSend(FraseQueue, Tecla, 0)
                case 'p'
                xQueueSend(FraseQueue, Tecla, 0)
                case 'q'
                xQueueSend(FraseQueue, Tecla, 0)
                case 'r'
                xQueueSend(FraseQueue, Tecla, 0)
                case 's'
                xQueueSend(FraseQueue, Tecla, 0)
                case 't'
                xQueueSend(FraseQueue, Tecla, 0)
                case 'u'
                xQueueSend(FraseQueue, Tecla, 0)
                case 'v'
                xQueueSend(FraseQueue, Tecla, 0)
                case 'w'
                xQueueSend(FraseQueue, Tecla, 0)
                case 'x'
                xQueueSend(FraseQueue, Tecla, 0)
                case 'y'
                xQueueSend(FraseQueue, Tecla, 0)
                case 'z'
                xQueueSend(FraseQueue, Tecla, 0)
                case 'A'
                xQueueSend(FraseQueue, Tecla, 0)
                case 'B'
                xQueueSend(FraseQueue, Tecla, 0)
                case 'C'
                xQueueSend(FraseQueue, Tecla, 0)
                case 'D'
                xQueueSend(FraseQueue, Tecla, 0)
                case 'E'
                xQueueSend(FraseQueue, Tecla, 0)
                case 'F'
                xQueueSend(FraseQueue, Tecla, 0)
                case 'G'
                xQueueSend(FraseQueue, Tecla, 0)
                case 'H'
                xQueueSend(FraseQueue, Tecla, 0)
                case 'I'
                xQueueSend(FraseQueue, Tecla, 0)
                case 'J'
                xQueueSend(FraseQueue, Tecla, 0)
                case 'K'
                xQueueSend(FraseQueue, Tecla, 0)
                case 'L'
                xQueueSend(FraseQueue, Tecla, 0)
                case 'M'
                xQueueSend(FraseQueue, Tecla, 0)
                case 'N'
                xQueueSend(FraseQueue, Tecla, 0)
                case 'O'
                xQueueSend(FraseQueue, Tecla, 0)
                case 'P'
                xQueueSend(FraseQueue, Tecla, 0)
                case 'Q'
                xQueueSend(FraseQueue, Tecla, 0)
                case 'R'
                xQueueSend(FraseQueue, Tecla, 0)
                case 'S'
                xQueueSend(FraseQueue, Tecla, 0)
                case 'T'
                xQueueSend(FraseQueue, Tecla, 0)
                case 'U'
                xQueueSend(FraseQueue, Tecla, 0)
                case 'V'
                xQueueSend(FraseQueue, Tecla, 0)
                case 'W'
                xQueueSend(FraseQueue, Tecla, 0)
                case 'X'
                xQueueSend(FraseQueue, Tecla, 0)
                case 'Y'
                xQueueSend(FraseQueue, Tecla, 0)
                case 'Z'
                xQueueSend(FraseQueue, Tecla, 0)
                case '1'
                xQueueSend(FraseQueue, Tecla, 0)
                case '2'
                xQueueSend(FraseQueue, Tecla, 0)
                case '3'
                xQueueSend(FraseQueue, Tecla, 0)
                case '4'
                xQueueSend(FraseQueue, Tecla, 0)
                case '5'
                xQueueSend(FraseQueue, Tecla, 0)
                case '6'
                xQueueSend(FraseQueue, Tecla, 0)
                case '7'
                xQueueSend(FraseQueue, Tecla, 0)
                case '8'
                xQueueSend(FraseQueue, Tecla, 0)
                case '9'
                xQueueSend(FraseQueue, Tecla, 0)
                case '0'
                xQueueSend(FraseQueue, Tecla, 0)
                break;
                }

    vTaskDelete(NULL);
}
static void prvTask_Frase(void *pvParameters)         // Task 3 para decodificar as teclas em simbolos morse
{
   
   char simbolo;

   while(xQueueReceive(FraseQueue, &simbolo, 0) == pdPASS)
    {
        if(simbolo =! 0)
        // Não consegui bolar um codigo em C para transformar a string de caracteres em simbolos Morse
        // Pensei em usar algum comando que envolvesse strcpy.

    }
xQueueSend(MorseQueue, simbolo, 0)
    vTaskDelete(NULL);
}

static void prvTask_MorseLED(void *pvParameters)         // Task 4 - LED piscando de acordo com a mensagem em Morse
{
    // pvParameters contains LED params
    st_led_param_t *led = (st_led_param_t *)pvParameters;
    portTickType xLastWakeTime = xTaskGetTickCount();
    char simbolo_morse;
    int aceso;

    
    for (;;)
    {
        if (xQueueReceive(MorseQueue, simbolo_morse, 0 )) ==pdpass
        {
            if (simbolo_morse = '-')
            aceso = 500;
            else
            if (simbolo_morse = '.')
            aceso = 250;
        // console_print("@");
        gotoxy(6, 2);
        printf("%s⬤", led->color);
        fflush(stdout);
        vTaskDelay(aceso / portTICK_PERIOD_MS);
        // vTaskDelayUntil(&xLastWakeTime, led->period_ms / portTICK_PERIOD_MS);

        gotoxy(6, 2);
        printf("%s ", BLACK);
        fflush(stdout);
        vTaskDelay(aceso / portTICK_PERIOD_MS);
        // vTaskDelayUntil(&xLastWakeTime, led->period_ms / portTICK_PERIOD_MS);
    }

    vTaskDelete(NULL);
}

void app_run(void)
{
    TeclaQueue = xQueueCreate (200, 0)
    MorseQueue = xQueueCreate (200, 0)
    LEDQueue = xQueueCreate (2000, 0)

    if (TeclaQueue == NULL)
    {
        printf("Falha ao criar a queue\n");
        exit(1);
    }

    clear();
    DISABLE_CURSOR();
    printf(
        "╔═════════════════╗\n"
        "║                 ║\n"
        "╚═════════════════╝\n");

    xTaskCreate(prvTask_MorseLED, "LED_green", configMINIMAL_STACK_SIZE, &green, TASK1_PRIORITY, &MorseLEDTask_hdlr); // Task para acender o LED de acordo com o código Morse
    vTaskSuspend(MorseLEDTask_hdlr);
    xTaskCreate(prvTask_Frase, "Montagem", configMINIMAL_STACK_SIZE, &red, TASK2_PRIORITY, &Frase_hdlr); // Task para enfileirar as teclas digitadas
    xTaskCreate(prvTask_Decode, "MorseDecode", configMINIMAL_STACK_SIZE, &red, TASK3_PRIORITY, &MorseDecode_hldr); // Task para codificar a frase em Morse
    xTaskCreate(prvTask_getChar, "Get_key", configMINIMAL_STACK_SIZE, NULL, TASK4_PRIORITY, NULL); // Task para leitura do teclado

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