#ifndef SIK_BOMBERMAN_CLIENT_CONNECTIONS_H
#define SIK_BOMBERMAN_CLIENT_CONNECTIONS_H

#include <memory>
#include "../utils/messages/InputMessage.h"
#include "ServerConnector.h"

size_t pass_from_gui_to_server(std::shared_ptr<InputMessage> &input_message, ServerConnector &connector);

#endif //SIK_BOMBERMAN_CLIENT_CONNECTIONS_H
