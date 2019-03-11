#include <WiFi.h> //funcionalidades de conexão wifi, como AP e WebServer
#include <ESP_ClientMacaddress.h>

//Crendenciais do ponto de acesso
const char *ssid = "meu_ssid";
const char *senha = "minha_senha";
WiFiServer server(80); //Porta padrão 80

#define NUM_DISPOSITIVOS 3

uint8_t macList[NUM_DISPOSITIVOS][6] = {
  {0xA8,0x16,0xD0,0xA6,0x45,0x3C},
  {0xA8,0x16,0xD0,0xA6,0x45,0x3C}, 
  {0xA8,0x16,0xD0,0xA6,0x45,0x3C} 
};

bool mac_conhecido;

ClientMacaddress clientMac(macList, NUM_DISPOSITIVOS);

void run_html(WiFiClient client){ 
   String html_content = \
    "<!DOCTYPE html><html>" \
        "<head><style media='screen' type='text/css'>" \
             "html{margin:10px auto;text-align:center;}" \
        "</style></head>" \
        "<body>" \
            "<h1 style='font-size:80px'>ACIONAMENTO LED</h1>";
        if(mac_conhecido){
            html_content += \
            "<p><a href='/LED_ON'>" \
                "<button style='width:200px;font-size:80px'>ON</button>" \
            "</a></p>" \
            "<p><a href='/LED_OFF'>" \
                "<button style='width:200px;font-size:80px'>OFF</button>" \
           "</a></p>";
        }else{
            html_content += \
            "<p style='color:red;font-size:80px'>DISPOSITIVO N&AtildeO AUTORIZADO</p>";
        }
        html_content += \    
        "</body>" \
    "</html>";
    client.println(html_content);
}

void setup(){
    //Configura pino conectado ao LED como saída
    pinMode(2, OUTPUT); 
    //Inicializa serial
    Serial.begin(115200);
    Serial.println();

    //Configura ESP no modo AP
    Serial.printf("Configurando ponto de acesso '%s'\n", ssid);
    WiFi.softAP(ssid, senha);
    server.begin();
    Serial.println("Configuração conluída");
}

void loop() {
    //Verifica se existe algum client na rede 
    WiFiClient client = server.available();   
    //Caso positivo, imprime "New Client" no monitor
    if (client){                              
        Serial.println("New Client");  
        //Enquanto client conectado, verifica se existem bytes a serem lidos     
        //e concatena os bytes recebidos na String cabecalho
        String cabecalho;    
        while (client.connected()){     
            if (client.available()){
                cabecalho += (char)client.read();  
                //Se receber nova linha em branco, encerrou leitura dos dados
                if (cabecalho.endsWith("\n\r\n")){
                    Serial.println(cabecalho); //imprime cabeçalhos http recebidos
                    // iniciamos a resposta http com o código OK(200), 
                    // o tipo de conteúdo a ser enviado e tipo de conexão.
                    client.println("HTTP/1.1 200 OK");
                    client.println("Content-Type:text/html");
                    client.println("Connection: close");
                    client.println();
                    // INSIRA AQUI SUA APLICAÇÃO

                    //Armazena na variável 'm' o endereço MAC do client
                    uint8_t *m = clientMac.getAddr(client);
                    Serial.printf("Macaddress:%.2X:%.2X:%.2X:%.2X:%.2X:%.2X\n",
                                            m[0],m[1],m[2],m[3],m[4],m[5]);

                    //determina se o endereço MAC do client é conhecido
                    mac_conhecido = clientMac.isKnown(m);

                    //envia ao client conteúdo HTML
                    run_html(client);

                    //se client possui MAC conhecido, libera acesso para controlar o LED
                    if(mac_conhecido){
                        Serial.println("mac ok");
                        if(cabecalho.indexOf("GET /LED_ON")>= 0){
                            digitalWrite(2, true);
                        }else if(cabecalho.indexOf("GET /LED_OFF")>= 0){
                            digitalWrite(2, false);
                        }
                    }else{
                        Serial.println("mac não autorizado");  
                    }
                                  
                    break; //sai do while loop
                }
            }
        }
        cabecalho = ""; //ao encerrar conexão, limpa variável cabecalho
        client.flush(); client.stop();
        Serial.println("Client desconectado."); Serial.println();
    }
}
