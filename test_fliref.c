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
    f2 = ouvrir("src_fliref.txt", 'L');
    fliref(f2, "\ttest %c end %s", &c, my_word);
    fecriref(stdout, "Read '%c' and '%s' from %s\n", c, my_word, "src_fliref.txt");
    fliref(f2, "end %d %s", &d, my_word);
    fecriref(stdout, "Now read '%d' and '%s' from %s\n", d, my_word, "src_fliref.txt");

    vider(stdout);
    fermer(f2);
}


