#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h> // for open
#include <unistd.h> // for close
#include <string.h>
#include <stdarg.h>

#if !defined(TAILLE_BUFF)
#define TAILLE_BUFF 1024
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

int vider(FICHIER *f){
    int i = write(f->openNb, f->wbuff, f->next_oct_libre_w);
    f->next_oct_libre_w = 0;
    return i;

}

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
    vider(stdout);
    vider(stderr);
    free(f);
    return 0;

}



int ecrire(const void *p, unsigned int taille, unsigned int nbelem, FICHIER *f){
    int written = 0;
    while(TAILLE_BUFF - f->next_oct_libre_w >= taille && written < nbelem) {

        memcpy(f->wbuff + f->next_oct_libre_w, p + (written * taille), taille);
        f->next_oct_libre_w += taille;
        written++;
    }

    if(written < nbelem || f->next_oct_libre_w == TAILLE_BUFF){
        vider(f);
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

int* decompose(int nombre){
    int base = 10;
    int div = 1;
    int nb = nombre;
    int pas = 1;
    int* ret = malloc(sizeof(int) * 17);
    if(nb >= 10){
        while(nb/div >= base){
            div *= base;
        }
        while(div != 1){
            ret[pas] = nb/div;
            nb = nb%div;
            div = div/base;
            pas ++;
        }
    }
    ret[pas] = nb;
    ret[0] = pas;
    return ret;
}


int fecriref (FICHIER *f, const char *format, ...){
    va_list ap;
    va_start(ap, format);
    char* text = format;
    int ret = 0;

    while(*text != '\0'){

        if(*text == '%'){

            text ++;
            char *s;
            int i;
            int nb;
            int stop;
            int *decom;
            char c;

            switch(*text){

                case 's':
                    s = va_arg(ap, char*);
                    i = 0;
                    ecrire(s, sizeof(char), strlen(s), f);
                    break;
                case 'd':
                    nb = va_arg(ap, int);
                    i = 0;
                    if(nb < 0){
                        c = '-';
                        ecrire(&c, sizeof(char), 1, f);
                        nb = nb *-1;
                    }
                    decom = decompose(nb);
                    stop = *decom;
                    while(i < stop){
                        decom ++;
                        if(*decom > 10){
                            exit(-1);
                        }
                        c = *decom + '0';
                        ecrire(&c, sizeof(char), 1, f);
                        i++;
                    }
                    break;
                case 'c':
                    i = va_arg(ap, int);
                    ecrire(&i, sizeof(char), 1, f);
                    break;
                default:
                    return -1;
            }
        }else{
            ecrire(text, sizeof(char), 1, f);
        }
        text++;
        ret ++;

    }

    va_end(ap);
    return ret;


}

int ecriref (const char *format, ...){
    va_list ap;
    va_start(ap, format);
    char* text = format;
    int ret = 0;

    while(*text != '\0'){

        if(*text == '%'){

            text ++;
            char *s;
            int i;

            switch(*text){

                case 's':
                    s = va_arg(ap, char*);
                    i = 0;
                    ecrire(s, sizeof(char), strlen(s), stdout);
                    break;
                case 'd':
                    s = "%d";
                    fecriref(stdout, s, va_arg(ap, int));
                    break;
                case 'c':
                    i = va_arg(ap, int);
                    ecrire(&i, sizeof(char), 1, stdout);
                    break;
                default:
                    return -1;
            }
        }else{
            ecrire(text, sizeof(char), 1, stdout);
        }
        text++;
        ret ++;

    }

    va_end(ap);
    return ret;
}

int fliref (FICHIER *f, const char *format, ...){
    int res = 0;
    va_list args;

    va_start(args, format);

    int* recup_int;
    char* recup_char;

    char* tmp = malloc(sizeof(char));
    int fin = 0;

    int compteur_format = 0;
    while(!fin && f->next_oct_to_read > 0 && format[compteur_format] != '\0') {
        switch(format[compteur_format]) {
            case '%':
                compteur_format++;
                switch (format[compteur_format]) {
                    case 'd':
                        recup_int = va_arg(args, int*);
                        lire(recup_int, sizeof(int), 1, f);
                        res++;
                        break;
                    case 'c':
                        recup_char = va_arg(args, char*);
                        lire(recup_char, sizeof(char), 1, f);
                        res++;
                        break;
                    case 's':
                        recup_char = va_arg(args, char*);

                        lire(tmp, sizeof(char), 1, f);
                        res++;
                        while (*tmp != format[compteur_format+1] && *tmp != '\n'
                                && *tmp != ' ' && *tmp != '\t' && *tmp != '\0') {
                            strcat(recup_char, tmp);
                            lire(tmp, sizeof(char), 1, f);
                            res++;
                        }
                        f->next_oct_to_read -= sizeof(char);
                        tmp = malloc(sizeof(char));
                        break;
                    default:
                        fecriref(stderr, "%s: Erreur : type de donnée inconnu.", __func__);
                        exit(-1);
                }
                break;
            case '\0':
                fin = 1;
                break;
            case ' ':
            case '\t':
                lire(tmp, sizeof(char), 1, f);
                if(*tmp == ' ' || *tmp == '\t') {
                    do {
                        lire(tmp, sizeof(char), 1, f);
                    } while(*tmp == ' ' || *tmp == '\t');
                    f->next_oct_to_read -= sizeof(char);
                    tmp = malloc(sizeof(char));

                    while(format[compteur_format] == ' ' || format[compteur_format] == '\t') {
                        compteur_format++;
                    }
                    compteur_format--;
                } else {
                    fecriref(stderr, "%s: Erreur : format et données d'entrée incohérents.", __func__);
                    exit(-1);
                }
                break;
            default:
                lire(tmp, sizeof(char), 1, f);
                res++;
                if((*tmp != format[compteur_format] && *tmp == '\n') || *tmp == '\0') {
                    fin = 1;
                } else if(*tmp != format[compteur_format]) {
                    fecriref(stderr, "%s: Erreur : format et données d'entrée incohérents.", __func__);
                    exit(-1);
                }
        }
        compteur_format++;
    }

    if(f->rbuff[f->next_oct_to_read] == '\n') {
        lire(tmp, sizeof(char), 1, f);
    }

    va_end(args);

    return res;
}

