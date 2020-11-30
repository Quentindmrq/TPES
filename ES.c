
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h> // for open
#include <unistd.h> // for close
#include <string.h>
#include <stdarg.h>

#if !defined(TAILLE_BUFF)
#define TAILLE_BUFF 16
#endif

struct _ES_FICHIER {
    int openNb;
    char wbuff[TAILLE_BUFF];
    char rbuff[TAILLE_BUFF];
    int next_oct_libre_w;
    int next_oct_to_read;
    int taille_lue;
};

typedef struct _ES_FICHIER FICHIER;

const char buff[TAILLE_BUFF];

FICHIER stdoutinit = {
        .openNb = 1,
        .next_oct_libre_w = 0,
        .taille_lue = 0,
        .wbuff = {},
};

FICHIER* stdout = &stdoutinit;

FICHIER stderrinit = {
        .openNb = 2,
        .next_oct_libre_w = 0,
        .taille_lue = 0,
        .wbuff = {},
};

FICHIER* stderr = &stderrinit;

FICHIER* ouvrir(char* nom, char mode){
    FICHIER* f = malloc(sizeof(FICHIER));
    int flag;
    switch(mode){
        case 'L':
            flag = O_RDONLY;
            break;
        case 'E':
            flag = O_WRONLY;
            break;
        default:
            return NULL;

    }

    f->openNb = open(nom, flag);
    if(f->openNb == -1){
        return NULL;
    }
    f->next_oct_libre_w = 0;
    f->next_oct_to_read = TAILLE_BUFF;
    f->taille_lue = 0;
    return f;
}

int fermer(FICHIER *f){
    write(f->openNb, f->wbuff, f->next_oct_libre_w);
    write(1, stdout->wbuff, stdout->next_oct_libre_w);
    write(2, stderr->wbuff, stderr->next_oct_libre_w);
    stdout->next_oct_libre_w = 0;
    stderr->next_oct_libre_w = 0;
    close(f->openNb);
    free(f);
    return 0;

}

int fecriref (FICHIER *f, const char *format, ...){

}
int ecriref (const char *format, ...){
    
}
int fliref (FICHIER *f, const char *format, ...){

}
int ecrire(const void *p, unsigned int taille, unsigned int nbelem, FICHIER *f){
    int written = 0;
    while(TAILLE_BUFF - f->next_oct_libre_w >= taille && written < nbelem) {

        memcpy(f->wbuff + f->next_oct_libre_w, p + (written * taille), taille);
        f->next_oct_libre_w += taille;
        written++;
    }

    if(written < nbelem || f->next_oct_libre_w == TAILLE_BUFF){
        write(f->openNb, f->wbuff, f->next_oct_libre_w);
        f->next_oct_libre_w = 0;
    }
    return written;
}

int lire(void *p, unsigned int taille, unsigned int nbelem, FICHIER *f){
    int ret = 0;

    if(taille > TAILLE_BUFF - f->next_oct_to_read){
        memcpy(f->rbuff, f->rbuff + f->next_oct_to_read, TAILLE_BUFF - f->next_oct_to_read);
        f->taille_lue = read(f->openNb, f->rbuff + (TAILLE_BUFF - f->next_oct_to_read), f->next_oct_to_read);
        if(f->taille_lue == 0){
            return 0;
        }
        f->next_oct_to_read = 0;
    }

    while(ret < nbelem && taille <= f->taille_lue - f->next_oct_to_read){
        memcpy(p + (ret * taille), f->rbuff + f->next_oct_to_read, taille);
        f->next_oct_to_read += taille;
        ret ++;
    }
    return ret;
}

int vider(FICHIER *f){
    return 0;

}