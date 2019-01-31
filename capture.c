#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pcap.h>

void packet_callback(u_char *args, const struct pcap_pkthdr *pkthdr, const u_char *packet) {
    if (pkthdr->len == 65) {
        if (packet[64] == 0x0d) {
            printf("received mute\n");
            system("pactl set-sink-mute @DEFAULT_SINK@ toggle");
        } else if (packet[64] == 0x0f) {
            printf("received left turn\n");
            system("pactl set-sink-volume @DEFAULT_SINK@ -5%");
        } else if (packet[64] == 0x10) {
            printf("received right turn\n");
            system("pactl set-sink-volume @DEFAULT_SINK@ +5%");
        }
    }
}

int main(int argc, const char **argv) {
    char interface[32] = {0x00};
    FILE *fp = popen("lsusb | awk '/Creative Technology/{print \"usbmon\"int($2)}'", "r");
    fscanf(fp, "%31s", &interface);
    pclose(fp);
    printf("Using interface \'%s\'\n", interface);

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
