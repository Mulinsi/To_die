#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#define MAX 200
//#define LENGTH(a)  (sizeof(a)/sizeof(a[0]))
//矩阵
typedef struct _graph
{
    char vexs[MAX];       // 顶点集合
    int vexnum;           // 顶点数
    int edgnum;           // 边数
    double matrix[MAX][MAX]; // 邻接矩阵
}Graph, *PGraph;
/*返回ch在matrix矩阵中的位置*/
static int get_position(Graph g, char ch)
{
    int i;
    for(i=0; i<g.vexnum; i++)
        if(g.vexs[i]==ch)
            return i;
    return -1;
}
//读取图
Graph* get_graph(char *a)
{
    FILE *in;
    char c1, c2;
    int v, e;
    int i, p1, p2;
    Graph* pG;
    if((in=fopen(a,"r"))==NULL)
    {
        printf("无法打开此文件\n");
        exit(0);
    }
    // 读入"顶点数"和"边数"
    fscanf(in,"%d %d",&v,&e);
    if ( v < 1 || e < 1 || (e > (v * (v-1))))
    {
        printf("input error: invalid parameters!\n");
        return NULL;
    }

    if ((pG=(Graph*)malloc(sizeof(Graph))) == NULL )
        return NULL;
    memset(pG, 0, sizeof(Graph));

    // 初始化"顶点数"和"边数"
    pG->vexnum = v;
    pG->edgnum = e;
    // 初始化"顶点"
    for (i = 0; i < pG->vexnum; i++)
    {

        pG->vexs[i] = fgetc(in);
    }

    // 初始化"边"
    for (i = 0; i < pG->edgnum; i++)
    {
        // 读取边的起始顶点和结束顶点

        c1 = fgetc(in);
        c2 = fgetc(in);

        p1 = get_position(*pG, c1);
        p2 = get_position(*pG, c2);
        if (p1==-1 || p2==-1)
        {
            printf("input error: invalid edge!\n");
            free(pG);
            return NULL;
        }

        pG->matrix[p1][p2] = 1;
    }
    fclose(in);
    return pG;
}
/*打印图*/
void print_graph(Graph G)
{
    int i,j;


    for (i = 0; i < G.vexnum; i++)
    {
        for (j = 0; j < G.vexnum; j++)
            printf("%lf\t", G.matrix[i][j]);
        printf("\n");
    }
    for(i=0;i<5;i++)
    printf("----------------------");
    printf("\n");

}
Graph * change_to_frac(Graph *G)
{
    int i,j,sum[MAX]={0};
    double temp[MAX][MAX];
    for(i=0; i< G->vexnum; i++)
    {
        for (j = 0; j < G->vexnum; j++)
        {
            sum[i]=G->matrix[i][j]+sum[i];
        }
    }
    for(i=0; i< G->vexnum; i++)
    {
        for (j = 0; j < G->vexnum; j++)
        {
            if(G->matrix[i][j]!=0)
            G->matrix[i][j]=1.0/sum[i];
            temp[i][j]=G->matrix[i][j];
        }
    }
     for(i=0; i< G->vexnum; i++)
    {
        for (j = 0; j < G->vexnum; j++)
        {
            G->matrix[i][j]=temp[j][i];
        }
    }
    return G;
}
void arymul1(Graph G,double (*b)[1],double (*c)[1])
{
    int i, j, k;
    double temp= 0.0;
    for(i = 0; i < G.vexnum; i++)
    {
        for(k = 0; k < 1; k ++)

        {
            temp = 0.0;
        }
        for(j = 0; j < G.vexnum; j++) //当前行的每个元素
        {
            for(k = 0; k < 1; k++)
            {
                temp += 0.85*G.matrix[i][j] * b[j][k]+ 0.15*b[j][k];//temp += G.matrix[i][j] * b[j][k];
            }
        }
        c[i][0] = temp;
    }
}
void get_pagerank(Graph frapG)
{
    int i,j;
    double pagerank[MAX][1],pagerank0[MAX][1];//初始化
    for(i=0;i<frapG.vexnum;i++)
    {
        pagerank0[i][0]=(double)(1.0)/(frapG.vexnum);
    }
    while( ((pagerank0[0][0]-pagerank[0][0])>0.00002)||((pagerank0[0][0]-pagerank[0][0])<-0.00002))//for(j=0;j<1000;j++)
    {
        arymul1(frapG,pagerank0,pagerank);
        for(i=0;i<frapG.vexnum;i++)
        {
            pagerank0[i][0]=pagerank[i][0];
        }
    }
    printf("\t\t\t\t\t结\t果：\n");
    for(i=0;i<frapG.vexnum;i++)
    {

        printf("\t\t\t\t\t%c :   %lf\n",65+i,pagerank[i][0]);
    }

}
void main(int argc,char** argv)
{
    Graph* pG;
    Graph* frapG;
    pG = get_graph(*(argv+1));//自定义构建图
    printf("\t\t\t\t\tMartix Graph:\n");
    print_graph(*pG);       // 打印图
    frapG=change_to_frac(pG);
    printf("\t\t\t\t\tFraMartix Graph:\n");
    print_graph(*frapG);  //打印转置后的概率图
    get_pagerank(*frapG); //得到pagerank并打印
}
