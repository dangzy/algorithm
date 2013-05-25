#include <iostream>
#include <cstring>
#include <cstdio>
#include <cmath>
#include <arpa/inet.h>

using namespace std;

typedef void (*myfun)(unsigned int*, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int);
#define ROTATE_LEFT(x, n) (((x) << (n)) | ((x) >> (32-(n))))


unsigned int F(unsigned int a, unsigned int b, unsigned int c){
    return (a&b)|((~a)&c);
}

void FF(unsigned int* a, unsigned int b, unsigned int c, unsigned int d, unsigned int Mj, unsigned int s, unsigned int ti){
    unsigned int tmp=*a + F(b,c,d) + Mj + ti;
    *a=b+ROTATE_LEFT(tmp,s);
}

unsigned int G(unsigned int a, unsigned int b, unsigned int c) {
    return (a&c)|(b&(~c));
}

void GG(unsigned int* a, unsigned int b, unsigned int c, unsigned int d, unsigned int Mj, unsigned int s, unsigned int ti){
    unsigned int tmp=*a + G(b,c,d) + Mj + ti;
    *a=b+ROTATE_LEFT(tmp,s);
}

unsigned int H(unsigned int a, unsigned int b, unsigned int c) {
    return a^b^c;
}

void HH(unsigned int* a, unsigned int b, unsigned int c, unsigned int d, unsigned int Mj, unsigned int s, unsigned int ti){
    unsigned int tmp=*a + H(b,c,d) + Mj + ti;
    *a=b+ROTATE_LEFT(tmp,s);
}

unsigned int I(unsigned int a, unsigned int b, unsigned int c) {
    return b^(a|(~c));
}

void II(unsigned int* a, unsigned int b, unsigned int c, unsigned int d, unsigned int Mj, unsigned int s, unsigned int ti){
    unsigned int tmp=*a + I(b,c,d) + Mj + ti;
    *a=b+ROTATE_LEFT(tmp,s);
}

int main(unsigned int argc, char** argv){
    if(argc!=2){
        cout<<"please input origin string"<<endl;
        return -1;
    }

    //form info
    long long originlen=strlen(argv[1]);
    unsigned int yushu=originlen*8%512;
    unsigned int buchong=0;
    if (yushu>448)
        buchong=512-yushu+448;
    else if(yushu<448)
        buchong=448-yushu;
    char* parinfo=new char[buchong/8+originlen+8];
    strncpy(parinfo,argv[1],originlen);
    unsigned int i;
    for(i=0;i<buchong/8;i++){
        if(0==i){
            parinfo[originlen+i]=0x80;
            continue;
        }
        parinfo[originlen+i]=0x00;
    }

    for(i=0;i<8;i++){
        char tmp=((originlen*8)>>(i*8))&0xff;
        parinfo[originlen+buchong/8+i]=tmp;
    }
#if 0
    for(i=0;i<(buchong/8+originlen+8);i++){
        printf("%02x ",parinfo[i]);
        if(!((i+1)%0x10))
            printf("\n");
    }
#endif
    //calc info
    unsigned int bigbuck=(buchong/8+originlen+8)/64;
    unsigned int A=0x67452301;
    unsigned int B=0xefcdab89;
    unsigned int C=0x98badcfe;
    unsigned int D=0x10325476;
    myfun funfun[4]={FF,GG,HH,II};
    unsigned int ses[][4]={{7,12,17,22},{5,9,14,20},{4,11,16,23},{6,10,15,21}};
    unsigned int myans[]={
        0xd76aa478,
        0xe8c7b756,
        0x242070db,
        0xc1bdceee,
        0xf57c0faf,
        0x4787c62a,
        0xa8304613,
        0xfd469501,
        0x698098d8,
        0x8b44f7af,
        0xffff5bb1,
        0x895cd7be,
        0x6b901122,
        0xfd987193,
        0xa679438e,
        0x49b40821,

        0xf61e2562,
        0xc040b340,
        0x265e5a51,
        0xe9b6c7aa,
        0xd62f105d,
        0x02441453,
        0xd8a1e681,
        0xe7d3fbc8,
        0x21e1cde6,
        0xc33707d6,
        0xf4d50d87,
        0x455a14ed,
        0xa9e3e905,
        0xfcefa3f8,
        0x676f02d9,
        0x8d2a4c8a,

        0xfffa3942,
        0x8771f681,
        0x6d9d6122,
        0xfde5380c,
        0xa4beea44,
        0x4bdecfa9,
        0xf6bb4b60,
        0xbebfbc70,
        0x289b7ec6,
        0xeaa127fa,
        0xd4ef3085,
        0x04881d05,
        0xd9d4d039,
        0xe6db99e5,
        0x1fa27cf8,
        0xc4ac5665,

        0xf4292244,
        0x432aff97,
        0xab9423a7,
        0xfc93a039,
        0x655b59c3,
        0x8f0ccc92,
        0xffeff47d,
        0x85845dd1,
        0x6fa87e4f,
        0xfe2ce6e0,
        0xa3014314,
        0x4e0811a1,
        0xf7537e82,
        0xbd3af235,
        0x2ad7d2bb,
        0xeb86d391
    };
    unsigned int a,b,c,d;
#define PARINFO(X) (*((unsigned int *)(parinfo+i*64)+X))
    for(i=0;i<bigbuck;i++){
        a=A;
        b=B;
        c=C;
        d=D;

        (funfun[0])(&a,b,c,d,PARINFO(0),ses[0][0],myans[0]);
        (funfun[0])(&d,a,b,c,PARINFO(1),ses[0][1],myans[1]);
        (funfun[0])(&c,d,a,b,PARINFO(2),ses[0][2],myans[2]);
        (funfun[0])(&b,c,d,a,PARINFO(3),ses[0][3],myans[3]);
        (funfun[0])(&a,b,c,d,PARINFO(4),ses[0][0],myans[4]);
        (funfun[0])(&d,a,b,c,PARINFO(5),ses[0][1],myans[5]);
        (funfun[0])(&c,d,a,b,PARINFO(6),ses[0][2],myans[6]);
        (funfun[0])(&b,c,d,a,PARINFO(7),ses[0][3],myans[7]);
        (funfun[0])(&a,b,c,d,PARINFO(8),ses[0][0],myans[8]);
        (funfun[0])(&d,a,b,c,PARINFO(9),ses[0][1],myans[9]);
        (funfun[0])(&c,d,a,b,PARINFO(10),ses[0][2],myans[10]);
        (funfun[0])(&b,c,d,a,PARINFO(11),ses[0][3],myans[11]);
        (funfun[0])(&a,b,c,d,PARINFO(12),ses[0][0],myans[12]);
        (funfun[0])(&d,a,b,c,PARINFO(13),ses[0][1],myans[13]);
        (funfun[0])(&c,d,a,b,PARINFO(14),ses[0][2],myans[14]);
        (funfun[0])(&b,c,d,a,PARINFO(15),ses[0][3],myans[15]);

        (funfun[1])(&a,b,c,d,PARINFO(1),ses[1][0],myans[16]);
        (funfun[1])(&d,a,b,c,PARINFO(6),ses[1][1],myans[17]);
        (funfun[1])(&c,d,a,b,PARINFO(11),ses[1][2],myans[18]);
        (funfun[1])(&b,c,d,a,PARINFO(0),ses[1][3],myans[19]);
        (funfun[1])(&a,b,c,d,PARINFO(5),ses[1][0],myans[20]);
        (funfun[1])(&d,a,b,c,PARINFO(10),ses[1][1],myans[21]);
        (funfun[1])(&c,d,a,b,PARINFO(15),ses[1][2],myans[22]);
        (funfun[1])(&b,c,d,a,PARINFO(4),ses[1][3],myans[23]);
        (funfun[1])(&a,b,c,d,PARINFO(9),ses[1][0],myans[24]);
        (funfun[1])(&d,a,b,c,PARINFO(14),ses[1][1],myans[25]);
        (funfun[1])(&c,d,a,b,PARINFO(3),ses[1][2],myans[26]);
        (funfun[1])(&b,c,d,a,PARINFO(8),ses[1][3],myans[27]);
        (funfun[1])(&a,b,c,d,PARINFO(13),ses[1][0],myans[28]);
        (funfun[1])(&d,a,b,c,PARINFO(2),ses[1][1],myans[29]);
        (funfun[1])(&c,d,a,b,PARINFO(7),ses[1][2],myans[30]);
        (funfun[1])(&b,c,d,a,PARINFO(12),ses[1][3],myans[31]);

        (funfun[2])(&a,b,c,d,PARINFO(5),ses[2][0],myans[32]);
        (funfun[2])(&d,a,b,c,PARINFO(8),ses[2][1],myans[33]);
        (funfun[2])(&c,d,a,b,PARINFO(11),ses[2][2],myans[34]);
        (funfun[2])(&b,c,d,a,PARINFO(14),ses[2][3],myans[35]);
        (funfun[2])(&a,b,c,d,PARINFO(1),ses[2][0],myans[36]);
        (funfun[2])(&d,a,b,c,PARINFO(4),ses[2][1],myans[37]);
        (funfun[2])(&c,d,a,b,PARINFO(7),ses[2][2],myans[38]);
        (funfun[2])(&b,c,d,a,PARINFO(10),ses[2][3],myans[39]);
        (funfun[2])(&a,b,c,d,PARINFO(13),ses[2][0],myans[40]);
        (funfun[2])(&d,a,b,c,PARINFO(0),ses[2][1],myans[41]);
        (funfun[2])(&c,d,a,b,PARINFO(3),ses[2][2],myans[42]);
        (funfun[2])(&b,c,d,a,PARINFO(6),ses[2][3],myans[43]);
        (funfun[2])(&a,b,c,d,PARINFO(9),ses[2][0],myans[44]);
        (funfun[2])(&d,a,b,c,PARINFO(12),ses[2][1],myans[45]);
        (funfun[2])(&c,d,a,b,PARINFO(15),ses[2][2],myans[46]);
        (funfun[2])(&b,c,d,a,PARINFO(2),ses[2][3],myans[47]);

        (funfun[3])(&a,b,c,d,PARINFO(0),ses[3][0],myans[48]);
        (funfun[3])(&d,a,b,c,PARINFO(7),ses[3][1],myans[49]);
        (funfun[3])(&c,d,a,b,PARINFO(14),ses[3][2],myans[50]);
        (funfun[3])(&b,c,d,a,PARINFO(5),ses[3][3],myans[51]);
        (funfun[3])(&a,b,c,d,PARINFO(12),ses[3][0],myans[52]);
        (funfun[3])(&d,a,b,c,PARINFO(3),ses[3][1],myans[53]);
        (funfun[3])(&c,d,a,b,PARINFO(10),ses[3][2],myans[54]);
        (funfun[3])(&b,c,d,a,PARINFO(1),ses[3][3],myans[55]);
        (funfun[3])(&a,b,c,d,PARINFO(8),ses[3][0],myans[56]);
        (funfun[3])(&d,a,b,c,PARINFO(15),ses[3][1],myans[57]);
        (funfun[3])(&c,d,a,b,PARINFO(6),ses[3][2],myans[58]);
        (funfun[3])(&b,c,d,a,PARINFO(13),ses[3][3],myans[59]);
        (funfun[3])(&a,b,c,d,PARINFO(4),ses[3][0],myans[60]);
        (funfun[3])(&d,a,b,c,PARINFO(11),ses[3][1],myans[61]);
        (funfun[3])(&c,d,a,b,PARINFO(2),ses[3][2],myans[62]);
        (funfun[3])(&b,c,d,a,PARINFO(9),ses[3][3],myans[63]);

        A+=a;
        B+=b;
        C+=c;
        D+=d;
    }

    printf("%08x",htonl(A));
    printf("%08x",htonl(B));
    printf("%08x",htonl(C));
    printf("%08x",htonl(D));
    cout<<endl;
            
    return 0;
}
