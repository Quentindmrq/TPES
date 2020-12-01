#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h> // for open
#include <unistd.h> // for close
#include <string.h>
#include <stdarg.h>
#include "stdes.h"


int main(int argc, char *argv[]){
    fecriref(stdout, "test du char %c \n", 'l');
    fecriref(stdout, "test de { %c \n", 123);
    fecriref(stdout, "test du int %d \n", 1);
    fecriref(stdout, "test du int %d \n", 12);
    fecriref(stdout, "test du int %d \n", 123);
    fecriref(stdout, "test du int %d \n", 1234);
    fecriref(stdout, "test du int %d \n", 12345);
    fecriref(stdout, "test du int %d \n", 123456);
    fecriref(stdout, "test du int %d \n", 1234567);
    fecriref(stdout, "test du int %d \n", 12345678);
    fecriref(stdout, "test du int %d \n", 945);

    fecriref(stdout, "test du string %s \n", "je suis le test");

    fecriref(stdout, "%d, ahah %d, %c, %s\n", 10, 1548, 123, "hola mamacitas");
    char* s = "fhezsfbds zefy bsudh fuqzevs fdh \n";
    ecrire(s, sizeof(char), strlen(s), stdout);

    ecriref("test du char %c \n", 'l');
    ecriref("test de { %c \n", 123);
    ecriref("test du int %d \n", 1);
    ecriref("test du int %d \n", 12);
    ecriref("test du int %d \n", 123);
    ecriref("test du int %d \n", 1234);
    ecriref("test du string %s \n", "test");

    ecriref("%d, ahah %d, %c, %s\n", 10, 1548, 123, "hola mamacitas");

    vider(stdout);



}


