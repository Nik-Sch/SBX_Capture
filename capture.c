#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pcap.h>

void packet_callback(u_char *args, const struct pcap_pkthdr *pkthdr, const u_char *packet) {
    if (pkthdr->len == 65) {
        if (packet[64] == 0x0d) {
            printf("received mute\n");
            system("pactl set-sink-mute 1 toggle");
        } else if (packet[64] == 0x0f) {
            printf("received left turn\n");
            system("pactl set-sink-volume 1 -5%");
        } else if (packet[64] == 0x10) {
            printf("received right turn\n");
            system("pactl set-sink-volume 1 +5%");
        }
    }
}

int main(int argc, const char **argv) {
    const char *interface;
    if (argc != 2) {
        interface = "usbmon3";
        printf("Using default interface \'%s\'\n", interface);
    } else {
        interface = argv[1];
        printf("Using interface \'%s\'\n", interface);
    }

    pcap_t *handle;
    char errbuf[PCAP_ERRBUF_SIZE];
    errbuf[0] = 0x00;
    if ((handle = pcap_open_live(interface, 65, 1, -1, errbuf)) == NULL)
        fprintf(stderr, "%s\n", errbuf);
    else if (errbuf[0] != 0x00)
        printf("%s\n", errbuf);

    pcap_loop(handle, -1, packet_callback, NULL);

    pcap_close(handle);

    return EXIT_SUCCESS;
}
