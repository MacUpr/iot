#define TRIGGER D2 // Define o pino D2 como o pino de saída para o sinal de trigger do sensor ultrassônico. 
#define ECHO D3    // Define o pino D3 como o pino de entrada para o sinal de eco do sensor ultrassônico.
#define BLYNK_TEMPLATE_ID "TMPL2R_BxE1sK"
#define BLYNK_TEMPLATE_NAME "abe"

#define BLYNK_PRINT Serial // Habilita a impressão de mensagens de depuração no monitor serial para o Blynk.

#include <ESP8266WiFi.h> // Inclui a biblioteca para conectividade Wi-Fi com ESP8266.
#include <BlynkSimpleEsp8266.h> // Inclui a biblioteca do Blynk para ESP8266.

char auth[] = "E_QYsxdxh4-SCCLfHFDd_x-Q3wsqFPUZ"; // Token de autenticação do Blynk.

char ssid[] = "abe"; // Nome da rede Wi-Fi.
char pass[] = "12345678"; // Senha da rede Wi-Fi.

void setup() {
  Serial.begin(9600); // Inicializa a comunicação serial com uma taxa de 9600 baud.
  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80); // Conecta o ESP8266 ao servidor Blynk com o token de autenticação e as credenciais Wi-Fi.
  pinMode(TRIGGER, OUTPUT); // Configura o pino TRIGGER como saída.
  pinMode(ECHO, INPUT); // Configura o pino ECHO como entrada.
  pinMode(LED_BUILTIN, OUTPUT); // Configura o LED embutido do ESP8266 como saída (usado para indicar status, se necessário).
}

void loop() {
  long duration, distance; // Declara variáveis para armazenar a duração do pulso e a distância calculada.
  digitalWrite(TRIGGER, LOW); // Garante que o pino TRIGGER esteja baixo antes de enviar o pulso.
  delayMicroseconds(2); // Aguarda 2 microsegundos para garantir que o TRIGGER esteja em nível baixo.

  digitalWrite(TRIGGER, HIGH); // Envia um pulso de 10 microsegundos no pino TRIGGER para iniciar a medição.
  delayMicroseconds(10); // Mantém o pulso por 10 microsegundos.
  digitalWrite(TRIGGER, LOW); // Define o pino TRIGGER para nível baixo novamente.

  // Lê a duração do pulso no pino ECHO. O pulso permanece HIGH enquanto o som viaja até o objeto e volta.
  duration = pulseIn(ECHO, HIGH);

  // Calcula a distância em centímetros. A fórmula divide a duração por 2 (ida e volta) e converte em cm usando 29.1 (aproximação).
  distance = (duration / 2) / 29.1;

  // Verifica a distância e aciona os LEDs virtuais no Blynk.
  Blynk.virtualWrite(V0, distance); // Exibe a distância medida em V0.

  if (distance <= 50) {
    Blynk.virtualWrite(V1, 255); // Liga o LED vermelho (perigo) em V1.
    Blynk.virtualWrite(V2, 0);   // Desliga o LED amarelo (alerta) em V2.
  } else if (distance > 50 && distance <= 100) {
    Blynk.virtualWrite(V2, 255); // Liga o LED amarelo (alerta) em V2.
    Blynk.virtualWrite(V1, 0);   // Desliga o LED vermelho (perigo) em V1.
  } else {
    Blynk.virtualWrite(V1, 0); // Desliga o LED vermelho em V1.
    Blynk.virtualWrite(V2, 0); // Desliga o LED amarelo em V2.
  }

  // Imprime a distância medida no monitor serial.
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  delay(100); // Aguarda 500 ms antes da próxima medição.
  Blynk.run(); // Mantém o Blynk rodando e a conexão ativa.
}
