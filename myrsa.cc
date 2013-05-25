//Only can work with a--z charactors
#include <cstdio>
#include <malloc.h>
#include <cmath>
#include <cstring>

void getpq(int *p, int *q){
    *p=3;
    *q=11;
}

int gete(int f){
    return 3;
}


int getd(int e, int f){
    int i;
    for(i=0;;i++){
        //printf("%d*%d/%d==%d\n",e,i,f,e*i%f);
        if(1==(e*i%f))
            break;
    }
    return i;
}

char *encinfo(char* msg, int e, int n){
    int len=strlen(msg);
    char *enc=(char*)malloc(len+1);
    int i,j;
    for(i=0;i<len;i++){
        for(j=0;;j++){
            //printf("%02x==pow(%d,%d)%2x==%02x\n",msg[i]-'a',j,e,n,((long long)pow(j,e))%n);
            if((msg[i]-'a')==(((long long)pow(j,e))%n))
                break;
        }
        enc[i+1]=j;
    }
    enc[0]=len;
    return enc;
}

char *decinfo(char* msg, int d, int n){
    int len=msg[0];
    msg++;
    char *dec=(char*)malloc(len+1);
    int i,j;
    for(i=0;i<len;i++){
        for(j=0;;j++){
            //printf("%02x==pow(%d,%d)%2x==%02x\n",msg[i],j,d,n,((long long)pow(j,d))%n);
            if(msg[i]==(((long long)pow(j,d))%n))
                break;
        }
        dec[i]=j+'a';
    }
    dec[i]=0;
    return dec;
}

void dumpinfo(char *str){
    int len=strlen(str);
    int i;
    for(i=0;i<len;i++)
        printf("%02x ",str[i]);
    printf("\n");
}

int main(int argc, char** argv){
    if(argc!=2){
        printf("input str\n");
        return -1;
    }

    int p,q;
    getpq(&p,&q);
    int n=p*q;

    int f=(p-1)*(q-1);
    int e=gete(f);
    
    int d=getd(e,f);

    //printf("%d:%s\n",d,argv[1]);
    //dumpinfo(argv[1]);
    char *enc=encinfo(argv[1],e,n);
    //dumpinfo(enc);
    char *dec=decinfo(enc,d,n);
    //dumpinfo(dec);
    printf("%s\n",dec);
    return 0;
}
