#include <cstdio>
#include <cstring>
#include <bitset>
#include <deque>
#include <algorithm>
using namespace std;

#define HUFFMAN_DEBUG 0

struct node{
    char key;//data
    int weit;//weight
    int bits;//huffcode
    char bitslen;//buffcode len
    node *leftleaf;
    node *rightleaf;
    node *parent;
    bool isdata;
    node(char mykey){
        key=mykey;
        weit=1;
        leftleaf=NULL;
        rightleaf=NULL;
        parent=NULL;
        bits=0;
        bitslen=0;
        isdata=true;
    }
    node(int tweit){
        key=0xff;
        weit=tweit;
        isdata=false;
        bits=0;
        bitslen=0;
    }
};

bool compare(node *n1, node *n2){
    return n1->weit < n2->weit;
}

void huffmandebug(char* fmt,...){
#if HUFFMAN_DEBUG
    printf(fmt,...);
#endif
}

class Hufftree{
    public:
        Hufftree(char* data){
            mytree=NULL;
            int i;
            int len=strlen(data);
            //getdata
            for(i=0;i<len;i++){
                node *old=findnode(data[i]);
                //huffmandebug("data=%c,addr=%p\n",data[i],old);
                if(NULL==old){
                    node *tmp=new node(data[i]);
                    mynodes.push_back(tmp);
                }else{
                    old->weit++;
                }
            }
        }

        void codingcode(){
            node *tmp=NULL;
            deque<node*> tmpque=mynodes;
            deque<node*>::iterator it1=tmpque.begin();

            //huffmandebug("size:%d\n",mynodes.size());
            while(tmpque.size()>=2){
                sort(tmpque.begin(),tmpque.end(),compare);
                //huffmandebug("tmp0:%c;tmp1:%c\n",tmpque[0]->key,tmpque[1]->key);
                tmp=new node(tmpque[0]->weit + tmpque[1]->weit);
                tmp->leftleaf=tmpque[0];
                tmp->rightleaf=tmpque[1];
                tmpque[0]->parent=tmp;
                tmpque[1]->parent=tmp;
                tmpque.pop_front();
                tmpque.pop_front();
                tmpque.push_back(tmp);
                mynodes.push_back(tmp);
            }

            sort(mynodes.begin(),mynodes.end(),compare);
            buildbits(tmp);
            //huffmandebug("size:%d\n",mynodes.size());
        }

        ~Hufftree(){
        }

        bool getdatabits(char data,int* pbits,char* pbitslen){
            node *tmp=findnode(data);
            if(!tmp)
                return false;
            *pbits=tmp->bits;
            *pbitslen=tmp->bitslen;
            return true;
        }

        bool datahas1char(){
            if(mynodes.size()==1)
                return true;
            return false;
        }

        char *findchar(int bits,char bitslen){
            deque<node*>::iterator it1=mynodes.begin();
            node *tmp=NULL;
            while(it1!=mynodes.end()){
                tmp=*it1;
                //huffmandebug("data=%c;key=%c\n",data,tmp->key);
                if(tmp->bits==bits&&tmp->isdata&&tmp->bitslen==bitslen)
                    return &(tmp->key);
                it1++;
            }
            return NULL;
        }

    private:
        node *findnode(char data){
            deque<node*>::iterator it1=mynodes.begin();
            node *tmp=NULL;
            while(it1!=mynodes.end()){
                tmp=*it1;
                //huffmandebug("data=%c;key=%c\n",data,tmp->key);
                if(tmp->key==data&&tmp->isdata)
                    return tmp;
                it1++;
            }
            return NULL;
        }

        void buildbits(node *tmp){
            if(tmp->leftleaf==NULL&&tmp->rightleaf==NULL)
                return;
            if(tmp->leftleaf){
                tmp->leftleaf->bits=tmp->bits<<1;
                tmp->leftleaf->bitslen=tmp->bitslen+1;
                huffmandebug("left key:%c,bits:%d\n",tmp->leftleaf->key,tmp->leftleaf->bits);
            }
            if(tmp->rightleaf){
                tmp->rightleaf->bits=(tmp->bits<<1)+1;
                tmp->rightleaf->bitslen=tmp->bitslen+1;
                huffmandebug("right key:%c,bits:%d\n",tmp->rightleaf->key,tmp->rightleaf->bits);
            }
            buildbits(tmp->leftleaf);
            buildbits(tmp->rightleaf);
        }

        deque<node*> mynodes;
        node* mytree;
};

class Huffcode{
    public:
        Huffcode(Hufftree *tree, char *data){
            int len=strlen(data);
            m_orglen=len;
            m_tree=tree;
            int i,j,k;
            int bits;
            char bitslen;
            int totallen=0;
            for(i=0;i<len;i++){
                if(!tree->getdatabits(data[i],&bits,&bitslen)){
                    huffmandebug("find unkown char:%c\n",data[i]);
                    continue;
                }
                huffmandebug("data:%c,bits:%d,bitslen:%d\n",data[i],bits,bitslen);
                totallen+=bitslen;
            }
            m_totallen=totallen;

            char* encdata=(char*)malloc((8-totallen%8+totallen)/8);
            memset(encdata,0x0,(8-totallen%8+totallen)/8);
            printf("compress len:%d\n",totallen);
            int byteindex=0;
            int bitindex=0;
            for(i=0;i<len;i++){
                if(!tree->getdatabits(data[i],&bits,&bitslen)){
                    //huffmandebug("find unkown char:%c\n",data[i]);
                    continue;
                }
                for(j=0;j<bitslen;j++){
                    encdata[byteindex] |= (((bits>>(bitslen-j-1))&0x1)<<bitindex);
                    huffmandebug("i=%d;j=%d;bits:%d;bit:%d\n",i,j,bits,((bits>>(bitslen-j-1))&0x1));
                    if(bitindex==7){
                        bitindex=0;
                        byteindex++;
                    }else
                        bitindex++;
                }
            }
            m_encdata=encdata;
        }

        char* decode(){
            int byteindex=0;
            int bitindex=0;
            int unparse=m_totallen;
            char *myde=(char*)malloc(m_orglen+1);
            char *tmp1,*tmp2;
            char tmpdata=0;
            int i=0;
            int tmpdatalen=0;

            while(unparse>0){
                while(byteindex*8+bitindex+1<=m_totallen){
                    tmpdata=tmpdata<<1;
                    tmpdata+=(m_encdata[byteindex]>>bitindex)&0x1;
                    tmpdatalen++;
                    huffmandebug("byteindex:%d;bitindex=%d;tmpdata=%d\n",byteindex,bitindex,tmpdata);
                    tmp1=m_tree->findchar(tmpdata,tmpdatalen);
                    unparse--;
                    if(bitindex==7){
                        bitindex=0;
                        byteindex++;
                    }else
                        bitindex++;

                    if(tmp1)
                    {
                        tmpdata=0;
                        tmpdatalen=0;
                        break;
                    }
                }
                huffmandebug("code:%c:un:%d\n",*tmp1,unparse);
                myde[i++]=*tmp1;
            }
            myde[i]=0;
            printf("%s\n",myde);
        }
    private:
        int m_totallen;
        char *m_encdata;
        int m_orglen;
        Hufftree *m_tree;
};

int main(int argc, char** argv){
    if(argc!=2){
        printf("exe string\n");
        return -1;
    }

    int orgbitlen=strlen(argv[1])*8;
    printf("origin bit length:%d\n",orgbitlen);

    Hufftree *mytree=new Hufftree(argv[1]);
    if(mytree->datahas1char()){
        huffmandebug("string at least have 2 different chars\n");
        return 1;
    }
    mytree->codingcode();

    Huffcode *mycode=new Huffcode(mytree,argv[1]);
    mycode->decode();
    return 0;
}
