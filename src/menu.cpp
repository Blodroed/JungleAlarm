//
// Created by lars_ on 15.05.2024.
//

#include "menu.h"
#include "mbed.h"
#include "json.h"

void sending_request(){

    printf("Attempting network connection\n");
    auto *network = NetworkInterface::get_default_instance();
    if (!network || network->connect() != NSAPI_ERROR_OK) {
        printf("Failed to initialize the network connection\n");
        return -1;
    }
    printf("Network connection successful\n");

    TCPSocket socket;
    if (socket.open(network) != NSAPI_ERROR_OK) {
        printf("Failed to open socket\n");
        return -1;
    }

    SocketAddress address;
    if (network->gethostbyname("api.ipgeolocation.io/", &address) != NSAPI_ERROR_OK) {
        printf("Failed to resolve host name\n");
        return -1;
    }
    address.set_port(80);

    if (socket.connect(address) != NSAPI_ERROR_OK) {
        printf("Server connection failed\n");
        return -1;
    }
    printf("Server connection succesful\n");

    //sending request
    const char http_request[] = "GET /60eb89be4944453b80026afef04306d6 HTTP/1.1\r\n"
                                "Host: ikt104.andersenitc.no\r\n"
                                "Connection: close\r\n\r\n";

    nsapi_size_t bytes_to_send = strlen(http_request);
    nsapi_size_or_error_t sent_bytes = 0;

    printf("\nSending message: \n%s", http_request);

    //Looping while there is more data
    while (bytes_to_send) {
        sent_bytes = socket.send(http_request + sent_bytes, bytes_to_send);


        //Error
        if (sent_bytes < 0) {
            printf("Failed to send HTTP request: %d\n", sent_bytes);
            return -1;
        } else {
            printf("Sent %d bytes\n", sent_bytes);
        }

        bytes_to_send -= sent_bytes;
    }

    printf("Complete message sent\n");


    //Array for storing response
    const int total_file_size = 1000;
    char response[total_file_size] = {0};
    nsapi_size_or_error_t totalReceived = 0;

    while (totalReceived < total_file_size) {
        nsapi_size_or_error_t chunkSize = std::min(static_cast<nsapi_size_or_error_t>(100), total_file_size - totalReceived);
        nsapi_size_or_error_t result = socket.recv(response + totalReceived, chunkSize);

        //If there is no more data
        if (result == 0) {
            break;
        }

        //Error
        if (result < 0) {
            printf("Failed to receive data: %d\n", result);
            return result;
        }

        //For printing three dots after each received chunk
        for (int i = 0; i < result; ++i) {
            printf("%c", response[totalReceived + i]);
        }
        printf("...\n");

        totalReceived += result;
    }

    //Printing the response
    printf("\nComplete response received (%d bytes):\n", totalReceived);
    for (int i = 0; i < totalReceived; ++i) {
        putchar(response[i]);
    }
    printf("\n");


    //String conversion to allow for JSON parsing
    std::string responseStr(response, totalReceived);

    json parsedJson = json::parse(responseStr, nullptr, false);

    if(parsedJson.is_discarded()) {
        printf("Parsing failed or there is no more data to parse\n");
    } else {
        if(parsedJson.contains("first name") && parsedJson["first name"].is_string()) {
            std::string firstName = parsedJson["first name"];
            printf("First Name: %s\n", firstName.c_str());
        } else {
            printf("First Name not found or is not a string.\n");
        }
    }
}

void longitude_latitude(){

}


void geolocation(){

}


void unix_epoch_time(){

}
