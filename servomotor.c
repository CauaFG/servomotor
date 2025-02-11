#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h" 

#define SERVO 22  // Definindo a GPIO 22
#define PERIODO 20000  // Período de 20 ms (~50 Hz)
#define DIVISOR_CLOCK 125.0  // Divisor de clock para atingir 50 Hz
#define PULSO_180 2400  // 2.400 microssegundos para 180 graus
#define PULSO_90 1470   // 1.470 microssegundos para 90 graus
#define PULSO_0 500     // 500 microssegundos para 0 graus
#define INCREMENTO 5  // Incremento para movimentação suave
#define DELAY_MS 10     // Atraso de ajuste em milissegundos

void setup_pwm() {
    gpio_set_function(SERVO, GPIO_FUNC_PWM);
    uint slice = pwm_gpio_to_slice_num(SERVO);
    pwm_set_wrap(slice, PERIODO);  
    pwm_set_clkdiv(slice, DIVISOR_CLOCK);  
    pwm_set_enabled(slice, true);
}

// Define o ciclo ativo do PWM em microssegundos
void ciclo_ativo_pwm(uint16_t pulso) {
    uint slice = pwm_gpio_to_slice_num(SERVO);
    pwm_set_chan_level(slice, pwm_gpio_to_channel(SERVO), pulso);
}

// Movimento suave entre dois valores de ciclo ativo
void movimento_suave(uint16_t pulso_inicial, uint16_t pulso_final) {
    if (pulso_inicial < pulso_final) {
        for (uint16_t pulso = pulso_inicial; pulso <= pulso_final; pulso += INCREMENTO) {
            ciclo_ativo_pwm(pulso);
            sleep_ms(DELAY_MS);
        }
    } else {
        for (uint16_t pulso = pulso_inicial; pulso >= pulso_final; pulso -= INCREMENTO) {
            ciclo_ativo_pwm(pulso);
            sleep_ms(DELAY_MS);
        }
    }
}

int main() {
    stdio_init_all();
    setup_pwm(SERVO);

    // Ajusta para 180 graus e espera 5 segundos
    ciclo_ativo_pwm(PULSO_180);
    sleep_ms(5000);

    // Ajusta para 90 graus e espera 5 segundos
    ciclo_ativo_pwm(PULSO_90);
    sleep_ms(5000);

    // Ajusta para 0 graus e espera 5 segundos
    ciclo_ativo_pwm(PULSO_0);
    sleep_ms(5000);

    // Movimentação suave entre 0 e 180 graus
    while (true) {
        movimento_suave(PULSO_0, PULSO_180);  // Movimenta até 180 graus
        sleep_ms(500);
        movimento_suave(PULSO_180, PULSO_0);  // Retorna para 0 graus
        sleep_ms(500);
    }

    return 0;
}
