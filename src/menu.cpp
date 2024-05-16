//
// Created by lars_ on 15.05.2024.
//

#include "menu.h"
#include "mbed.h"
#include "json.hpp"
#include "ISM43362.h"
#include "WiFiInterface.h"
#include "ipgeolocation_certificate.h"

using json = nlohmann::json;

#define JSON_NOEXCEPTION
#define HTTP_RESPONSE_BUFFER_SIZE (4000)

#define wait_time_ms (1000ms)

static const char *get_nsapi_error_string(nsapi_error_t err);
// Må tenke over hvordan jeg kommer til å sende requesten her, og 
/* hvordan jeg kommer til å sende requesten til API'en.
 * 
 * Må tenke litt på hvordan jeg kommer til å koble til internettet  
 * med disse header filene
 *
 */
void sending_request(){
    //må huske at dette trenger bare å bli eksekvert en gang
        // aka. trenger ikke å kjøre dette flere ganger...
    ////URL Branch : api.ipgeolocation.io/ipgeo?apiKey=YOUR_API_KEY

    // API KEY : 60eb89be4944453b80026afef04306d6

    // Full URL : 
      //https://api.ipgeolocation.io/ipgeo?apiKey=60eb89be4944453b80026afef04306d6

    NetworkInterface *network = nullptr;

    printf("Get default network interface");

    network = NetworkInterface::get_default_instance();
    ThisThread::sleep_for(1000ms);
    if(!network){
        printf("Failed to get default network interface\n");
        while(true);
    }


    nsapi_size_or_error_t result;

    printf("Connecting to the network");
    result = network->connect();

    if(result != NSAPI_ERROR_OK){
        printf("Failed to connect to network: %d %s\n", result,
            get_nsapi_error_string(result));

        while(true);    
    }
    SocketAddress address;

    printf("Get local IP address\n");
    result = network->get_ip_address(&address);
    
    if(result != NSAPI_ERROR_OK){
        printf("Failed to get local IP address: d% %s\n",
            result, get_nsapi_error_string(result));

        while(true);
    }
    

    //seems like I gotta use something that verifies the stuff to do
    
    printf("Connected to WLAN and gathered IP address %s\n",
        address.get_ip_address());

    while(true){
        printf("Remaining stack space = %u bytes\n",
            osThreadGetStackSpace(ThisThread::get_id()));

        ThisThread::sleep_for(wait_time_ms);


        //TLSSocket is used for HTTPS 
        TLSSocket socket;

        socket.set_timeout(1000);

        result = socket.open(network);

        if(result != NSAPI_ERROR_OK){
            printf("Failed to open TLS socket: %d %s", result,
                    get_nsapi_error_string(result));
                    continue;
        }

        const char host[] = "api.ipgeolocation.io";

        result = network->gethostbyname(host, &address);

        if (result != NSAPI_ERROR_OK){
            printf("Failed to get IP adress of the host %s: %d %s\n",
            host, result, get_nsapi_error_string(result));
            continue;
        }

        printf("IP address of server %s is %s\n",
            host, address.get_ip_address());


        address.set_port(443);
        result = socket.set_root_ca_cert(ipGeolocation_root_certificate);

        if (result != NSAPI_ERROR_OK){
            printf("Failed root certificate for website: %d %s\n",
                result, get_nsapi_error_string(result));
        }

        socket.set_hostname(host);

        result = socket.connect(address);

        if (result != NSAPI_ERROR_OK){
            printf("Failed to connect to server at %s: %d %s\n",
                host, result, get_nsapi_error_string(result));
        }

        const char http_request[] = "GET /ipgeo?apiKey=60eb89be4944453b80026afef04306d6/\r\n"
                                    "Host: api.ipgeolocation.io\r\n"
                                    "Connection: close\r\n"
                                    "\r\n";

        nsapi_size_t bytes_to_send = strlen(http_request);
        nsapi_size_or_error_t sent_bytes = 0;

        printf("\nSending message: \n%s\n", http_request);

        while (bytes_to_send){
            sent_bytes = socket.send(http_request + sent_bytes, bytes_to_send);
            
            if(sent_bytes < 0){
                break;
            } else{
                printf("sent %d bytes\n", sent_bytes);
            }

            bytes_to_send -=sent_bytes;
        }

        if(sent_bytes < 0){
            printf("Failed to send HTTP request: %d %s\n", sent_bytes,
            get_nsapi_error_string(sent_bytes));
            continue;
        }
 
    

    //seems like the main reason for why these functions won't 
    //work, is that these are just 

    printf("Complete message sent\n");

    static char http_response[HTTP_RESPONSE_BUFFER_SIZE];

    memset(http_response, 0, sizeof(http_response));

    nsapi_size_t remaining_bytes = HTTP_RESPONSE_BUFFER_SIZE;
    nsapi_size_or_error_t received_bytes = 0;

    while(remaining_bytes > 0){
        nsapi_size_or_error_t result = 
        socket.recv(http_response + received_bytes, remaining_bytes);

        if(result < 0){
            received_bytes = result;
            break;
        }   
        else{
            printf("Received %d bytes\n", result);
        }
        if (result==0){
            break;
        }

        received_bytes += result;
        remaining_bytes -= result;
    }

    if (received_bytes < 0) {
        printf("Failed to receive HTTP response: %d %s\n",
            received_bytes, get_nsapi_error_string(received_bytes));
        continue;    
    }

    printf("\nReceived %d bytes with HTTP status code: %.*s\n",
    received_bytes, strstr(http_response, "\n") - http_response,
    http_response);

    //
    char *json_begin = strchr(http_response, '{');
    char *json_end = strrchr(http_response, '}');

    if(json_begin == nullptr || json_end == nullptr){
        printf("Failed to find json in response\n");
        continue;
    }

    json_end[1] = 0;

    json document = json::parse(json_begin);

    if (document.is_discarded()){
        printf("The input is invalid JSON\n");
        continue;
    }

    //where we actually end up finding the IP..
    std::string ip;
    document["ip"].get_to(ip);
    
    printf("\nIP from JSON data: %s\n", ip.c_str());

    
    } 
}


static const char *get_nsapi_error_string(nsapi_error_t err) {
  switch (err) {
  case NSAPI_ERROR_OK:
    return "NSAPI_ERROR_OK";
  case NSAPI_ERROR_WOULD_BLOCK:
    return "NSAPI_ERROR_WOULD_BLOCK";
  case NSAPI_ERROR_UNSUPPORTED:
    return "NSAPI_ERROR_UNSUPPORTED";
  case NSAPI_ERROR_PARAMETER:
    return "NSAPI_ERROR_PARAMETER";
  case NSAPI_ERROR_NO_CONNECTION:
    return "NSAPI_ERROR_NO_CONNECTION";
  case NSAPI_ERROR_NO_SOCKET:
    return "NSAPI_ERROR_NO_SOCKET";
  case NSAPI_ERROR_NO_ADDRESS:
    return "NSAPI_ERROR_NO_ADDRESS";
  case NSAPI_ERROR_NO_MEMORY:
    return "NSAPI_ERROR_NO_MEMORY";
  case NSAPI_ERROR_NO_SSID:
    return "NSAPI_ERROR_NO_SSID";
  case NSAPI_ERROR_DNS_FAILURE:
    return "NSAPI_ERROR_DNS_FAILURE";
  case NSAPI_ERROR_DHCP_FAILURE:
    return "NSAPI_ERROR_DHCP_FAILURE";
  case NSAPI_ERROR_AUTH_FAILURE:
    return "NSAPI_ERROR_AUTH_FAILURE";
  case NSAPI_ERROR_DEVICE_ERROR:
    return "NSAPI_ERROR_DEVICE_ERROR";
  case NSAPI_ERROR_IN_PROGRESS:
    return "NSAPI_ERROR_IN_PROGRESS";
  case NSAPI_ERROR_ALREADY:
    return "NSAPI_ERROR_ALREADY";
  case NSAPI_ERROR_IS_CONNECTED:
    return "NSAPI_ERROR_IS_CONNECTED";
  case NSAPI_ERROR_CONNECTION_LOST:
    return "NSAPI_ERROR_CONNECTION_LOST";
  case NSAPI_ERROR_CONNECTION_TIMEOUT:
    return "NSAPI_ERROR_CONNECTION_TIMEOUT";
  case NSAPI_ERROR_ADDRESS_IN_USE:
    return "NSAPI_ERROR_ADDRESS_IN_USE";
  case NSAPI_ERROR_TIMEOUT:
    return "NSAPI_ERROR_TIMEOUT";
  case NSAPI_ERROR_BUSY:
    return "NSAPI_ERROR_BUSY";
  default:
    return "NSAPI_ERROR_UNKNOWN";
  }
}

void longitude_latitude(){

}


void geolocation(){

}


void unix_epoch_time(){

}
