#include "image_converter.h"

void wurfl_query(const char *user_agent, int *width, int *heigh) {
    const char *root = "/home/emanuele/Scrivania/WebServer/wurfl.xml";

    printf("%s\n", user_agent);

    wurfl_t *wurfl = wurfl_init(root, NULL);

    device_t *device = wurfl_match(wurfl, user_agent);

    char **capabilities = device_capabilities(device, NULL);
    char **caps_ptr = capabilities;

    *width = -1;
    *heigh = -1;

    while (caps_ptr != NULL && *caps_ptr != NULL) {
        if (strcmp(*caps_ptr, "resolution_width") == 0) {
            *width = atoi(caps_ptr[1]);
        } else if (strcmp(*caps_ptr, "resolution_height")) {
            *heigh = atoi(caps_ptr[1]);
        }
        caps_ptr += 2;
    }

    printf("width: %d, heigh: %d\n", *width, *heigh);

}

int main(){

    int *width, *heigh;
    wurfl_query("Mozilla/5.0 (iPad; U; CPU OS 3_2_1 like Mac OS X; en-us) AppleWebKit/531.21.10 (KHTML, like Gecko) Mobile/7B405",
    width,heigh);


    return 0;
}