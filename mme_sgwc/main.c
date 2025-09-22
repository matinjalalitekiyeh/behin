#include <stdio.h>
#include <errno.h>

#include <stdlib.h>
#include "gtpv2_packet_parser.h"

#define OUTPUT_FILE "gtpv2.pcap"


int main() {
    int sock_raw;
    struct sockaddr saddr;
    socklen_t saddr_size = sizeof(saddr);
    unsigned char buffer[BUFFER_SIZE];

    pcap_init(OUTPUT_FILE);

    sock_raw = socket_create_raw();
    if (sock_raw < 0) {
        pcap_close();
        exit(EXIT_FAILURE);
    }

    socket_set_timeout(sock_raw, 1);


    while (1) {
        int data_size = socket_receive(sock_raw, buffer, BUFFER_SIZE, &saddr, &saddr_size);
        if (data_size < 0) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) continue;
            perror("Recvfrom error");
            break;
        }

        // Check if packet matches GTPv2 traffic
        if (is_gtpv2_traffic(buffer, data_size)) {
                pcap_write_packet(buffer, (size_t)data_size);

//            // Parse and print GTPv2 information
//            struct ethhdr *eth = (struct ethhdr *)buffer;
//            struct iphdr *ip = (struct iphdr *)(buffer + sizeof(struct ethhdr));
//            int ip_header_len = ip->ihl * 4;
//            struct udphdr *udp = (struct udphdr *)(buffer + sizeof(struct ethhdr) + ip_header_len);
//            struct gtpv2_header *gtp = (struct gtpv2_header *)(buffer + sizeof(struct ethhdr) + ip_header_len + sizeof(struct udphdr));

//            print_gtpv2_info(gtp, ip, udp);
//            printf("Captured GTPv2 packet: %d bytes\n", data_size);
        }
    }

    socket_close(sock_raw);
    pcap_close();
    return 0;
}
