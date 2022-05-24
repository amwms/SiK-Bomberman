#include "client_connections.h"
#include <iostream>

void printm(const std::string &name, std::string buffer) {
    printf("\u001b[1m\u001b[31m- - - - - - - - - - - - \u001b[38;5;160mPRINT MESSAGE "
           "\u001b[1m\u001b[31m- - - - - - - - - - - - - - - -\u001b[0m \n");
    printf("\u001b[38;5;208m--------------------->  %s \u001b[0m\n", name.c_str());

    std::cout << "Message: \"" << buffer << "\" \n";
    printf("Message length: %zu \n", buffer.size());
    for (char & i : buffer) {
        if (i == 13) {
            i = (char) 255;
        }

        printf("(%c)[%d]", i, i);

        if (i == -1) {
            i = 13;
        }
    }

    printf("\n");
    printf("\u001b[1m\u001b[31m- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -\u001b[0m \n");
}

size_t pass_from_gui_to_server(std::shared_ptr<InputMessage> &input_message, ServerConnector &connector) {
    std::shared_ptr<ClientMessage> client_message = input_message->to_client_message();
    printm("from gui", input_message->serialize());
    printm("to server", client_message->serialize());
    connector.send_message(client_message->serialize());

    return client_message->serialize().size();
}