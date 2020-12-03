#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h> // for open
#include <unistd.h> // for close
#include <string.h>
#include <stdarg.h>
#include "stdes.h"


int main(int argc, char *argv[]){
    FICHIER *f2;

    char c;
    int  d;
    char my_word[128];

    f2 = ouvrir (argv[1], 'L');

    fliref (f2, "  test  %c end %s", &c, my_word);
    ecriref ("Read '%c' and '%s' from %s\n", c, my_word, argv[1]);

    fliref (f2, "end %d %s", &d, my_word);
    ecriref ("Now read '%d' and '%s' from %s\n", d, my_word, argv[1]);

    vider(stdout);
    fermer(f2);
}


