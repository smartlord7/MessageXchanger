//
// Created by joao on 24/05/21.
//

#ifndef MESSAGEXCHANGER_CLIENT_H
#define MESSAGEXCHANGER_CLIENT_H

#include "tcp_ip/udp/udp.h"
#include "util/strings/strings.h"
#include "util/numbers/numbers.h"
#include "tcp_ip/tcp/tcp.h"

#define IP "125.234.233.213"
#define PORT 9000
#define NUM_PERMITS 3

#define END "END"

#define true 1
#define false 0

#define REJECTED "000"

enum permits {
    SERVER_COMMS,
    P2P,
    GROUP_COMMS,
};

#endif //MESSAGEXCHANGER_CLIENT_H
