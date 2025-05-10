#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "ssd1306_up.h"
#include "pico/stdlib.h"

#define LARGURA_TELA 128
#define ALTURA_TELA 64
#define I2C_PORT i2c0

#define I2C_SDA 14
#define I2C_SCL 15
#define OLED_ADDR 0x3C
#define numero 5

typedef struct Bola
{
    int x; // largura
    int y; // altura
    int r; //raio
    int ativo; // se a bola está no topo
} Bola;        

typedef struct Pino
{
    int x; // largura
    int y; // altura
    int r; //raio
} Pino; 

typedef struct Galton
{
    int n_pinos;
    Pino pinos[15];
} Galton;

const int vertical= 8;
const int horizontal= 24;
const int raio= 2;
static int bolas_caidas=0;
static int v[5] = {0};
static Galton galton;
static Bola bola;

void draw_circle(ssd1306_t *disp, int cy, int cx, int radius)
{

    ssd1306_draw_pixel(disp, cx, cy);
    for (int y = -radius; y <= radius; y++)
    {
        int y_pos = cy + y;
        if (y_pos < 0 || y_pos >= ALTURA_TELA)
            continue;

        int dx = (int)sqrt(radius * radius - y * y);
        int x_start = cx - dx;
        int x_end = cx + dx;

        if (x_start < 0)
            x_start = 0;
        if (x_end >= LARGURA_TELA)
            x_end = LARGURA_TELA - 1;

        for (int x = x_start; x <= x_end; x++)
        {
            ssd1306_draw_pixel(disp, x, y_pos);
        }
    }
}
void update_display(ssd1306_t *disp) {
    ssd1306_clear(disp); // limpa o display
    //Mostra a bola
    draw_circle(disp,bola.y, bola.x, bola.r);
    // Mostra o número de bolas que já caíram
    char balls_number[6];
    sprintf(balls_number, "%d", bolas_caidas);

    ssd1306_draw_string(disp, 15, 0, 1, balls_number);

    // Mostra todos os pinos que estão na Galton Board
    for(int i=0; i<galton.n_pinos;i++){
        Pino pino = galton.pinos[i];
        draw_circle(disp, pino.y, pino.x, pino.r);
    }

    // Mostra o histograma
    int max_value = -1;
    for(int i = 0; i < numero; i++)
        if(v[i] > max_value) max_value = v[i];

    int display_result[numero];
    for(int i = 0; i < numero; i++) display_result[i] = v[i];

    if(max_value > 18)
    {
        double scale_factor = 18.0/max_value;
        for(int i = 0; i < numero; i++)
        {
            display_result[i] = (int) (v[i] * scale_factor);
        }
    }

    for(int i=0; i<numero; i++)
    {
        for(int j = 2+(21*i); j <= 21+(21*i); j++)
        {
            for(int k = 64; k > 63 - display_result[i]; k--)
            {
                ssd1306_draw_pixel(disp, j, k);
            }
        }
    } 
    ssd1306_show(disp);
}


int main()
{
    stdio_init_all(); //inicia
    ssd1306_t disp;
    display_init(&disp);
    bola.x=LARGURA_TELA/2;
    bola.y=0;
    bola.r=1;

    ssd1306_clear(&disp);
    ssd1306_show(&disp);

    srand(time(NULL)); // aleatoriza a posição
    int value;
    int direita = 5;
    int estaleca = 0;
    galton.n_pinos=0;

    // Inicialização dos pinos
    for(int i=0; i<4; i++){
        int limit=i+1;
        for(int j=0; j<limit;j++){
            Pino pino;
            pino.x=-i*(horizontal/2)+(j*horizontal)+LARGURA_TELA/2;;
            pino.y=(i+1)*vertical;
            pino.r=raio;
            galton.pinos[galton.n_pinos]=pino;
            galton.n_pinos++;

        }
    }

    for (int j = 0; j < 10000; j++)
    {
        estaleca = 0;
        direita = 0;
        for (int i = 0; i < 4; i++)
        {
            bola.y=(i+1)*vertical-raio-1;
            bola.x=-i*(horizontal/2) + direita*horizontal + LARGURA_TELA/2;
            value = rand() % 2;
            if (value == 0)
            {
                direita += 0;
            }
            else if (value == 1)
            {
                direita += 1;
            }
            sleep_ms(100);
            update_display(&disp);
        } 
        bolas_caidas++;
        estaleca = direita;
        v[estaleca] = v[estaleca] + 1;
        bola.x=LARGURA_TELA/2;
        bola.y=0;
        update_display(&disp);
        sleep_ms(100);
    
    }

    while (1)
    {
        tight_loop_contents();
    }
    return 0;
}
