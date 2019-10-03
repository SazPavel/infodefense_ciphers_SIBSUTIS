#include "infodef.h"
#include "randombytes.h"
#include <locale.h>

void sham_generate_p(int_least64_t *p)
{
    randombytes(p, sizeof(*p));
    *p = fabs(*p % (int_least64_t)1e9) + 256;
    if(*p % 2 == 0)
        *p += 1;
    for(;; *p += 2)
    {
        if(prime_test(*p))
        {
            break;
        }
    }
}

void sham_generate_en(int_least64_t p, int_least64_t *ca, int_least64_t *da)
{
    int_least64_t am[3], bm[3];
    do
    {
        randombytes(ca, sizeof(*ca));
        *ca = fabs(*ca % (int_least64_t)1e9) + 2;
        am[0] = p - 1; am[1] = 1; am[2] = 0;
        bm[0] = *ca; bm[1] = 0; bm[2] = 1;
        gcd_v(am, bm);
    }while(am[0] != 1);
    if(am[2] < 0)
        am[2] += (p - 1);
    *da = am[2];
}

void sham_generate_de(int_least64_t p, int_least64_t *cb, int_least64_t *db)
{
    int_least64_t am[3], bm[3];
    do
    {
        randombytes(cb, sizeof(*cb));
        *cb = fabs(*cb % (int_least64_t)1e9) + 2;
        am[0] = p - 1; am[1] = 1; am[2] = 0;
        bm[0] = *cb; bm[1] = 0; bm[2] = 1;
        gcd_v(am, bm);
    }while(am[0] != 1);
    if(am[2] < 0)
        am[2] += (p - 1);
    *db = am[2];
}

void sham_first_en(int_least64_t p, int_least64_t ca, char *in, char *out)
{
    int_least64_t tmp;
    FILE *fin = fopen(in, "r");
    FILE *fout = fopen(out, "w");
    if(fin == NULL || fout == NULL)
    {
        perror("file encrypt 1");
        exit(-1);
    }
    while((tmp = fgetc(fin)) != EOF)
    {
        fprintf(fout, "%"PRId64" ", modpow(tmp, ca, p));
    }
    fclose(fin);
    fclose(fout);
}

void sham_temp_encrypt(int_least64_t p, int_least64_t b, char *in, char *out)
{
    int_least64_t tmp;
    FILE *fin = fopen(in, "r");
    FILE *fout = fopen(out, "w");
    if(fin == NULL || fout == NULL)
    {
        perror("file decrypt 1");
        exit(-1);
    }
    while(fscanf(fin, "%"PRId64, &tmp) != EOF)
    {
        fprintf(fout, "%"PRId64" ", modpow(tmp, b, p));
    }
    fclose(fin);
    fclose(fout);
}

void sham_second_de(int_least64_t p, int_least64_t db, char *in, char *out)
{
    int_least64_t tmp;
    FILE *fin = fopen(in, "r");
    FILE *fout = fopen(out, "w");
    if(fin == NULL || fout == NULL)
    {
        perror("file decrypt 2");
        exit(-1);
    }
    while(fscanf(fin, "%"PRId64, &tmp) != EOF)
    {
        fprintf(fout, "%c", (char)modpow(tmp, db, p));
    }
    fclose(fin);
    fclose(fout);
}

int main(int argc, char *argv[])
{
    setlocale (LC_ALL, "Rus");
    int_least64_t  p, ca, da, cb, db;
    if(argc < 2)
    {
        printf("Input name\n");
        exit(0);
    }    
    sham_generate_p(&p);
    sham_generate_en(p, &ca, &da);
    sham_generate_de(p, &cb, &db);
    FILE *fout = fopen("sham_key", "w");
    printf("p %"PRId64" ca %"PRId64" da %"PRId64" cb %"PRId64" db %"PRId64" \n", p, ca, da, cb, db);
    fclose(fout);
    sham_first_en(p, ca, argv[1], "tmp/sham1");
    sham_temp_encrypt(p, cb, "tmp/sham1", "tmp/sham2");
    sham_temp_encrypt(p, da, "tmp/sham2", "tmp/sham3");
    sham_second_de(p, db, "tmp/sham3", "sham_result");

    //execlp("rm", "", "tmp/sham1", "tmp/sham2", "tmp/sham3", NULL);
    exit(0);
}
