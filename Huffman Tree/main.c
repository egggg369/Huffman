//
//  main.c
//  Huffman Tree
//
//  Created by 郭红乐 on 2020/12/28.
//  Copyright © 2020 无. All rights reserved.
//

#include<stdio.h>
#include<string.h>
#include<stdlib.h>

struct Huffman {
    int weight;
    int parent, LChild, RChild;
    char s;
};

typedef char * Huffmancode[150];



//统计字符出现的频率
int Count(FILE *file, int *number, char *charnumber) {
    int times[26] = {0};
    FILE *fp = file;
    char ch;
    
    while(!feof(fp)) {
        ch = fgetc(fp);
        printf("%c ",ch);
        printf("%d ", ch-'a');
        times[(ch - 'a')]++;
        printf("%d \n", times[(ch - 'a')]);
        //break;
        //ch = fgetc(fout);
    }
    
    int j = 0;
    for(int i = 0; i < 26; i++) {
        
        if(times[i] != 0) {
                number[j] = times[i];
                charnumber[j] = (i + 'a');
            j++;
        }
    }
    
    for(int k = 0; k < j; k++) {
        printf("%c:%d\n", charnumber[k],number[k]);
    }
    
    fclose(fp);
    return j;
}

//select函数，在i-1项中选双亲为0且权值最小的两结点s1,s2.m p
int min (struct Huffman *huffman, int i) {
    int j;
    int m = 0;
    unsigned int k = 99;
    for(j = 1; j < i; ++j)
    {
        if ((huffman + j)->weight <= k && (huffman + j)->parent == 0)
        {
            k = (huffman + j)->weight;
            m = j;
        }
    }
    (huffman + m)->parent = 1;
    return m;
}

void selected(struct Huffman *huffman, int n, int *s1, int *s2) {
    int j;
    *s1 = min(huffman,n);
    *s2 = min(huffman,n);
    if(*s1 > *s2)
    {
        j = *s1;
        *s1 = *s2;
        *s2 = j;
    }
}


//创建哈夫曼树（即便权值的输入顺序不一样，得到的哈夫曼树和三叉链表也是一样的，那么最后得到的哈夫曼编码应该也是一样的）
void CrtHuffmanTree(struct Huffman *huffman, int *number, char *charnumber,int n) {
    int m = 2 * n - 1;
    for(int i = 1; i <= n ;i++) {
        (huffman + i)->LChild = 0;
        (huffman + i)->RChild = 0;
        (huffman + i)->parent = 0;
        (huffman + i)->weight = number[i-1];
        (huffman + i)->s = charnumber[i-1];
    }
    for(int i = n + 1; i <= m; i++) {
        (huffman + i)->LChild = 0;
        (huffman + i)->RChild = 0;
        (huffman + i)->parent = 0;
        (huffman + i)->weight = 0;
        (huffman + i)->s = 0;
    }
    for(int i = n + 1; i <= m; i++) {
        int s1 = 0, s2 = 0;
        selected(huffman, i, &s1, &s2);
        (huffman + i)->weight = (huffman + s1)->weight + (huffman + s2)->weight;
        (huffman + i)->LChild = s1; (huffman + i)->RChild = s2;
        (huffman + s1)->parent = i; (huffman + s2)->parent = i;
    }
    
    printf("三叉链表 \n         下标     weight    parent    LChild    RChild\n");
    for(int j = 1; j <= m; j++) {
        printf("%c      %d          %d        %d         %d        %d\n",(huffman + j)->s, j, (huffman + j)->weight, (huffman+j)->parent, (huffman+j)->LChild, (huffman+j)->RChild);
    }
    
    
}

//构造哈夫曼编码
void CrtHuffmanCode1(struct Huffman *huffman,Huffmancode hc, int n)
{
    //从叶子到根，逆向求个叶子结点的编码
    char *cd;
    int start;
    cd = (char *)malloc(n * sizeof(char));  //临时编码数组
    cd[n-1] = '\0';   //从后往前逐位求编码，首先放编码结束符
    for(int i = 1; i <= n; i++) {  //从每个叶子开始，求相应的哈夫曼编码
        start = n -1;
        int c = i;   //c是当前节点
        printf("%c",(huffman + c)->s);
        int p = (huffman + i)->parent;  //p是双亲
        while(p != 0)
        {
            --start;
            if((huffman + p)->LChild == c) {
                cd[start] = '0';    //左分支得0
            }else {
                cd[start] = '1';    //右分支得1
            }
            c = p;
            p = (huffman + p)->parent;
        }
        hc[i] = (char *) malloc((n-start) * sizeof(char));
        //strcpy(hc[i], &(huffman + i)->s);
        //printf("%s\n",&cd[start]);
        strcpy(hc[i], &cd[start]);
        printf("%s\n",hc[i]);
    }
    free(cd);
}

//生成压缩文件B
void compression(Huffmancode hc, int *number, char *charnumber, int n, FILE *fp)
{
    FILE *ftp = fopen("/Users/guohongle/Desktop/taskzip.txt", "wb+");
    if(ftp == NULL)
    {
        printf("创建压缩文件失败");
    }
    fwrite(&n, sizeof(int), 1, ftp);
    //首部
    for(int i = 0; i < n; i++)
    {
        fwrite(&(number[i]), sizeof(int), 1, ftp);
        fwrite(&(charnumber[i]), sizeof(char), 1, ftp);
    }
    int t = 0;
    unsigned char temp = 0;
    while(!feof(fp)) {
        char c = fgetc(fp);
        printf("<%c>", c);
        for(int i = 1; i <= n; i++)
        {
            if(c == charnumber[i-1]) {
                printf("%s\n",hc[i]);
            for(int j = 0; j < strlen((hc[i])); j++)
            {
                if(t == 8) {
                    fwrite(&temp, sizeof(char), 1, ftp);
                    //printf("%o  \n",temp);
                    t = 0;
                    temp = 0;
                }
                if(hc[i][j] == '1') {
                    temp = (temp<<1)|1;
                }else {
                    temp = temp<<1;
                }
                t++;
            }
            
        }
    }
    }
    
    //文件尾
    while(t<8 && t!= 0) {
        temp = temp<<1;
        t++;
    }
    
    fwrite(&temp, sizeof(char), 1, ftp);
    fclose(ftp);
}

//判断压缩文件的二进制位是0还是1
int GetBit(FILE *ftp)
{
    static int i = 7;
    static unsigned char Bchar;
    //int x;
    unsigned char bit[8] = {128, 64, 32, 16, 8, 4, 2, 1};
    i++;
    if(i == 8) {
        Bchar = fgetc(ftp);
        i = 0;
    }
    return (Bchar&bit[i]);
}

//解压缩
void uncompress(FILE *ftp)
{
    int n = 0;
    fread(&n, sizeof(int), 1, ftp);
    char arrayChar[100] = {0};
    int arraynumber[100] = {0};
    int sumofChar = 0;
    for(int i = 0; i < n; i++) {
        fread(&arraynumber[i], sizeof(int), 1, ftp);
        fread(&arrayChar[i], sizeof(char), 1, ftp);
        sumofChar = sumofChar + arraynumber[i];
    }
    //重建哈夫曼树
    struct Huffman unCompresshuffman[150];
    CrtHuffmanTree(unCompresshuffman, arraynumber, arrayChar, n);
    Huffmancode HC2;
    CrtHuffmanCode1(unCompresshuffman, HC2 , n);
    
    //解译
    int x,c;
    char unPackChar[150] = {0};
    int unPackCharOfTails = 0;
    for(x = 0; x < sumofChar; x++) {
        c = 2 * n - 1;      //回到哈夫曼树的根节点
        while((unCompresshuffman+c)->LChild) {  //当结点不是叶子结点，重复一下操作
            if(GetBit(ftp)) {  //判断压缩文件的二进制位是0还是1
                c = (unCompresshuffman + c)->RChild;  //当前二进制是1进入右孩子
            }
            else {
                c = (unCompresshuffman + c)->LChild;  //当前二进制位是0，进入左孩子
            }
            
        }
        printf("%c", (unCompresshuffman + c)->s);
        unPackChar[unPackCharOfTails++] = (unCompresshuffman + c)->s;
    }
    FILE *upftp = fopen("/Users/guohongle/Desktop/taskout.text", "wb");
    for(x = 0; x < sumofChar; x++)
    {
        fputc(unPackChar[x], upftp);  //将译出的字符存入解压缩文件
    }
    fclose(upftp);
}

int main(int argc, const char * argv[]) {
    FILE *fp;
    fp = fopen("/Users/guohongle/Desktop/task.txt", "r");
    if(fp == NULL) {
        printf("文件打开失败");
    }
    int number[26] = {0};
    char charnumber[26] = {0};
    int n = Count(fp, number, charnumber);
    struct Huffman huffman[100];
    CrtHuffmanTree(huffman, number, charnumber, n);
    //HuffmanCode HC = NULL;
    Huffmancode HC;
    CrtHuffmanCode1(huffman, HC , n);
    //可以输出哈夫曼编码
//     for(int i = 1; i <= n; i++) {
//         printf("%s\n",HC[i]);
//     }
    
    FILE *ftp = fopen("/Users/guohongle/Desktop/taskzip.txt","wb+");
    if(ftp == NULL)
    {
        printf("创建压缩文件失败");
    }
    fclose(ftp);
    
    fp = fopen("/Users/guohongle/Desktop/task.txt", "r");
    compression(HC, number, charnumber, n, fp);
    fclose(fp);
    
    FILE *fdp = fopen("/Users/guohongle/Desktop/taskzip.txt", "rb+");
    uncompress(fdp);
    
    printf("\n解压完成！\n");
    
    return 0;
}
