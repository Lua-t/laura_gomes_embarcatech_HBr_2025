#include <stdio.h>
#include "pico/cyw43_arch.h"

void connect_to_wifi(const char *ssid, const char *password){
// Inicializa o driver Wi-Fi (CYW43). Retorna 0 se for bem-sucedido.
if (cyw43_arch_init()) {
printf("Erro ao iniciar Wi-Fi\n");
return;
}
}