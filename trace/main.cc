// #include "pcap_module.h"
#include "socket_module.h"
#include "s1ap_module.h"
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <errno.h>
#define OUTPUT_FILE "s1ap_capture.pcap"

struct capture_for_file {
    uint8_t data[1024 * 64];
    uint32_t size;
};

capture_for_file m_cap_to_file[200];
struct trace_packet_pcap_t;

int main() {

//    std::cout << "hello world" << std::endl;

    int sock_raw;
    struct sockaddr saddr;
    socklen_t saddr_size = sizeof(saddr);
    unsigned char buffer[BUFFER_SIZE];

    // Initialize PCAP file
    pcap_init(OUTPUT_FILE);

    // Create raw socket
    sock_raw = socket_create_raw();
    if (sock_raw < 0) {
        pcap_close();
        exit(EXIT_FAILURE);
    }

    // Set timeout
    socket_set_timeout(sock_raw, 1);

    printf("Sniffing S1AP traffic between %s <-> %s:%d...\n",
           TARGET_CLIENT_IP, TARGET_SERVER_IP, TARGET_PORT);
    printf("Filtering out retransmissions...\n");
    printf("Saving S1AP packets to %s\n", OUTPUT_FILE);

    int packet_count = 0;
    //    int s1ap_count = 0;
    //    int retrans_count = 0;

    bool capture_to_file = false;

    while (1) {
        int data_size = socket_receive(sock_raw, buffer, BUFFER_SIZE, &saddr, &saddr_size);
        if (data_size < 0) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) continue;
            perror("Recvfrom error");
            break;
        }



        bool is_retransmission = false;
        uint32_t guti;
        // Check if packet contains S1AP traffic
        if (is_diameter_packet(buffer, data_size, is_retransmission,capture_to_file)) {
//            printf("salam");
            //
            if (!is_retransmission) {
                pcap_write_packet(buffer, data_size);
//                                pcap_write_packet(buffer, data_size);

                // m_cap_to_file[packet_count].size = data_size;
                // memcpy(&m_cap_to_file[packet_count].data, &buffer, data_size);
                // packet_count++;

                // if (capture_to_file == false) {
                // for (int i = 0 ; i < packet_count; i++) {
                // pcap_write_packet(m_cap_to_file[i].data, m_cap_to_file[i].size);
                // }
                // packet_count = 0;
                // capture_to_file = true;
                // }

//                if (capture_to_file)
//                {
// pcap_write_packet(buffer, data_size);


////                    std::cout << '\n' << '\n' << std::endl;
////                    auto packets = get_packet();


////                    for (int j = 0 ; j < packets.size(); j++) {
////                        if (packets.at(j).imsi == "999990123456780") {
////                            guti = packets.at(j).valid_tmsi;
////                            for (int i = 0; i < packets.at(j).count; i++) {
////                                pcap_write_packet(&packets.at(j).packet[i].packet[0], packets.at(j).packet[i].size);
////                            }
////                        } else if (packets.at(j).tmsi == guti) {
////                            for (int i = 0; i < packets.at(j).count; i++) {
////                                pcap_write_packet(&packets.at(j).packet[i].packet[0], packets.at(j).packet[i].size);
////                            }
////                        } else {
////                            continue;
////                        }
////                    }

//                    capture_to_file = false;



//                    //                s1ap_count++;
//                    //                printf("Captured S1AP packet %d (size: %d bytes)\n", s1ap_count, data_size);
//                } else {
//                    //                retrans_count++;
//                    //                printf("Filtered out retransmission (total filtered: %d)\n", retrans_count);
//                }
            }
        }

        // Print progress every 100 packets
        //        if (packet_count % 100 == 0) {
        // printf("Processed %d packets, S1AP: %d, Retrans: %d\n",
        //        packet_count, s1ap_count, retrans_count);
        //        }
    }

    socket_close(sock_raw);
    pcap_close();
    //    printf("Capture completed. Total packets: %d, S1AP packets: %d, Retransmissions filtered: %d\n",
    //           packet_count, s1ap_count, retrans_count);
    return 0;
}
