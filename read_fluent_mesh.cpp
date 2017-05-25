#include<stdio.h>
#include<math.h>
#include<string.h>
#include<stdlib.h>
#include <fstream>

using namespace std;

typedef struct cell{int type;}CELL;
typedef struct face{long int n1,n2,n3,n4,rcell,lcell;}FACE;
typedef struct node{double x,y,z;}NODE;

int readDimension(ifstream&);
void readNodes(ifstream&, NODE *nodes, long int numNodes);
void readFaces(ifstream&, FACE *faces, long int facel1, long int facel2);
void readCells(ifstream&, CELL *cells, long int numCells);

/* ******************************************* */
/* Main function                             */
int main(int argc, char **argv)
{
    ifstream meshFile ;
    char ch[2]="", ch1[12];//*hex1;
    long int dec1, n1, numNodes, *faceid, *facel, facev, facen, *cellid, numCells;
    int i, dimv, dimswt=0, nodeswt=0, cellswt=0, faceswt=0, fzone;
    NODE *nodes;
    FACE *faces;
    CELL *cells;
    //int brkswt=0;
    //hex1 = "41D";
    //dec1 = hex2dec(hex1);
    //printf("\n %ld \n", dec1);
 //   meshFile = fopen("fluent.msh","r");
     meshFile.open("fluent.msh", ifstream::in);

    if (meshFile == NULL)
    {
        printf("Cannot open file \n");
        //printf("\n %d \n",11111);
        return 0;
    }
    //printf("\n %d \n",11111);
    //fscanf(meshFile,"%c",&ch);
    *ch = meshFile.get();
    while (*ch!=EOF)
    {
        //printf ("%c", ch);
        if (*ch=='(')
        {
            //brkswt = 1;
            //fscanf(meshFile,"%c",&ch);
            *ch = meshFile.get();
            if(*ch=='2' && dimswt==0)
            {
                //printf("\n %d \n",11111);
                dimv = readDimension(meshFile);
                dimswt = 1;
                if(dimv!=3)
                {
                    fprintf(stderr,"This code is for 3D only\n");
                    fprintf(stderr,"Process Terminated\n");
                    return 0;
                }
                *ch = meshFile.get();
            }
            if(*ch=='1')
            {
                *ch = meshFile.get();
                //printf("%c\n",ch);
                if(*ch=='0' && nodeswt==0)
                {
                    //Nodes
                    *ch = meshFile.get(); *ch = meshFile.get(); *ch = meshFile.get(); *ch = meshFile.get(); *ch = meshFile.get(); *ch = meshFile.get();
                    meshFile >>  ch1;
                    numNodes = strtoul(ch1, (char**)0, 16);
                    fprintf(stderr,"The total number of nodes are %ld\n", numNodes);
                    if((nodes=(NODE *)malloc(numNodes*sizeof(NODE)))==NULL)
                    {
                        fprintf(stderr,"Memory Error for nodes\n");
                        fprintf(stderr,"Process Terminated\n");
                        return 0;
                    }
                    readNodes(meshFile, nodes, numNodes);
                    //for(i=0;i<numNodes;i++)
                    //{
                    //    fprintf(stderr,"%lf,%lf,%lf\n",nodes[i].x,nodes[i].y,nodes[i].z);
                    //}
                    nodeswt = 1;
                }
                if(*ch=='2' && cellswt==0)
                {
                    //Cells
                    *ch = meshFile.get(); *ch = meshFile.get(); *ch = meshFile.get(); *ch = meshFile.get(); *ch = meshFile.get();
                    meshFile >> ch1;
                    numCells = strtoul(ch1, (char**)0, 16);
                    fprintf(stderr,"The total number of cells are %ld\n", numCells);
                    if((cells=(CELL *)malloc(numCells*sizeof(CELL)))==NULL)
                    {
                        fprintf(stderr,"Memory Error for cells\n");
                        fprintf(stderr,"Process Terminated\n");
                        return 0;
                    }
                    cellid = (long int *)malloc(sizeof(long int));
                    fzone = 0;
                    while(fzone!=1)
                    {
                        *ch = meshFile.get();
                        //fprintf(stderr,"%s\n",&ch);
                        if(*ch=='('){*ch = meshFile.get(); if(*ch=='1'){ fzone = 1;}}
                    }
                    *ch = meshFile.get();
                    while(*ch!='(')
                    {
                        *ch = meshFile.get();
                    }
                    *ch = meshFile.get(); *cellid = *ch-'0';
                    readCells(meshFile, cells, numCells);
                    //for(i=0;i<numCells;i++)
                    //{
                    //    fprintf(stderr,"%d\n",cells[i].type);
                    //}
                    cellswt = 1;
                }
                if(*ch=='3' && faceswt==0)
                {
                    // Faces
                    *ch = meshFile.get(); *ch = meshFile.get(); *ch = meshFile.get(); *ch = meshFile.get(); *ch = meshFile.get();
                    meshFile >> ch1;
                    facev = strtoul(ch1, (char**)0, 16);
                    fprintf(stderr,"The total number of faces are %ld\n", facev);
                    if((faces=(FACE *)malloc(facev*sizeof(FACE)))==NULL)
                    {
                        fprintf(stderr,"Memory Error for faces\n");
                        fprintf(stderr,"Process Terminated\n");
                        return 0;
                    }
                    faceid = (long int *)malloc(sizeof(long int));
                    facel = (long int *)malloc(sizeof(long int));
                    *faceid = 0;
                    *facel = 0;
                    facen = 0;
                    while(*(facel+facen)!=facev)
                    {
                        facen++;
                        faceid = (long int *)realloc(faceid,sizeof(long int)*(facen+1));
                        facel = (long int *)realloc(facel,sizeof(long int)*(facen+1));
                        fzone = 0;
                        while(fzone!=1)
                        {
                            *ch = meshFile.get();
                            //fprintf(stderr,"%s\n",&ch);
                            if(*ch=='('){*ch = meshFile.get(); if(*ch=='1'){ fzone = 1;}}
                        }
                        *ch = meshFile.get();
                        while(*ch!='(')
                        {
                            *ch = meshFile.get();
                        }
                        *ch = meshFile.get(); *(faceid+facen) = *ch-'0';
                        *ch = meshFile.get();
                        meshFile >> ch1;
                        //fprintf(stderr,"%s\n",&ch1);
                        n1 = strtoul(ch1, (char**)0, 16);
                        *(facel+facen) = n1;
                        *ch = meshFile.get();
                        meshFile >> ch1;
                        //fprintf(stderr,"%s\n",&ch1);
                        n1 = strtoul(ch1, (char**)0, 16);
                        *(facel+facen) = n1;
                        fprintf(stderr,"The total number of faces in Zone Id %ld is %ld\n", *(faceid+facen), *(facel+facen)-*(facel+facen-1));
                        readFaces(meshFile, faces, *(facel+facen-1), *(facel+facen));
                    }
                    //for(i=0;i<facev;i++)
                    //{
                    //    fprintf(stderr,"%ld,%ld,%ld,%ld,%ld,%ld\n",faces[i].n1,faces[i].n2,faces[i].n3,faces[i].n4,faces[i].rcell,faces[i].lcell);
                    //}
                    faceswt = 1;
                }
            }
        }
        *ch = meshFile.get();
    }
    meshFile.close();
    free(nodes);
    free(faces);
    return 0;
}
/* Main Function ends                        */
/* ******************************************* */
/* ******************************************* */
int readDimension(ifstream& meshFile)
{
    int dim;
    meshFile >> dim;
    //printf("\n %d \n",dimv);
    return dim;
    //return the dimension of the grid
}
/* ******************************************* */
void readNodes(ifstream& meshFile, NODE *nodes, long int numNodes)
{
    int i, runtwice = 0;
    double x,y,z;
    char ch;
    ch = meshFile.get();
    while (runtwice!=3)
    {
        ch = meshFile.get();
        if(ch=='(')
        {
            runtwice++;
        }
    }
    for(i=0;i<numNodes;i++)
    {
        meshFile >> x >> y >>z;
        nodes[i].x = x;
        nodes[i].y = y;
        nodes[i].z = z;
    }
}
/* ******************************************* */
void readFaces(ifstream& meshFile, FACE *faces, long int facel1, long int facel2)
{
    int i, ftype, ftype1;
    char n1[12],n2[12],n3[12],n4[12],n5[12],n6[12];
    char ch[2]="";
    long int val1;
    *ch = meshFile.get();
    while (*ch!=')')
    {
        ftype1 = *ch-'0';
        *ch = meshFile.get();
        //fprintf(stderr,"%s\n",ch);
    }
    while (*ch!='(')
    {
        *ch = meshFile.get();
        //fprintf(stderr,"%s\n",ch);
    }
    //fprintf(stderr,"%ld,%ld\n",facel1,facel2);
    i = facel1;
    if(ftype1==0)
    {
        while(i<facel2)
        {
            //fprintf(stderr,"%d\n",i);
            *ch = meshFile.get();
            ftype = *ch-'0';
            if(ftype==3)
            {
                meshFile >> n1 >> n2 >> n3 >> n4 >> n5;
                //fprintf(stderr,"%s %s %s %s %s\n", n1, n2, n3, n4, n5);
                val1 = strtoul(n1, (char**)0, 16);
                faces[i].n1 = val1;
                val1 = strtoul(n2, (char**)0, 16);
                faces[i].n2 = val1;
                val1 = strtoul(n3, (char**)0, 16);
                faces[i].n3 = val1;
                val1 = strtoul(n4, (char**)0, 16);
                faces[i].n4 = val1;
                val1 = strtoul(n5, (char**)0, 16);
                faces[i].rcell = val1;
                val1 = strtoul(n5, (char**)0, 16);
                faces[i].lcell = val1;
                i++;
            }
            if(ftype==4)
            {
                meshFile >> n1 >> n2 >> n3 >> n4 >> n5 >> n6;
                //fprintf(stderr,"%s %s %s %s %s %s\n", n1, n2, n3, n4, n5, n6);
                val1 = strtoul(n1, (char**)0, 16);
                faces[i].n1 = val1;
                val1 = strtoul(n2, (char**)0, 16);
                faces[i].n2 = val1;
                val1 = strtoul(n3, (char**)0, 16);
                faces[i].n3 = val1;
                val1 = strtoul(n4, (char**)0, 16);
                faces[i].n4 = val1;
                val1 = strtoul(n5, (char**)0, 16);
                faces[i].rcell = val1;
                val1 = strtoul(n6, (char**)0, 16);
                faces[i].lcell = val1;
                i++;
            }
        }//while
    }//If
    if(ftype1==3)
    {
        while(i<facel2)
        {
            meshFile >> n1 >> n2 >> n3 >> n4 >> n5;
            //fprintf(stderr,"%s %s %s %s %s\n", n1, n2, n3, n4, n5);
            val1 = strtoul(n1, (char**)0, 16);
            faces[i].n1 = val1;
            val1 = strtoul(n2, (char**)0, 16);
            faces[i].n2 = val1;
            val1 = strtoul(n3, (char**)0, 16);
            faces[i].n3 = val1;
            val1 = strtoul(n4, (char**)0, 16);
            faces[i].n4 = val1;
            val1 = strtoul(n5, (char**)0, 16);
            faces[i].rcell = val1;
            val1 = strtoul(n5, (char**)0, 16);
            faces[i].lcell = val1;
            i++;
        }//while
    }//If
    if(ftype1==4)
    {
        while(i<facel2)
        {
            meshFile >> n1 >> n2 >> n3 >> n4 >> n5 >> n6;
            //fprintf(stderr,"%s %s %s %s %s %s\n", n1, n2, n3, n4, n5, n6);
            val1 = strtoul(n1, (char**)0, 16);
            faces[i].n1 = val1;
            val1 = strtoul(n2, (char**)0, 16);
            faces[i].n2 = val1;
            val1 = strtoul(n3, (char**)0, 16);
            faces[i].n3 = val1;
            val1 = strtoul(n4, (char**)0, 16);
            faces[i].n4 = val1;
            val1 = strtoul(n5, (char**)0, 16);
            faces[i].rcell = val1;
            val1 = strtoul(n6, (char**)0, 16);
            faces[i].lcell = val1;
            i++;
        }//while
    }//If
}
/* ******************************************* */
void readCells(ifstream& meshFile, CELL *cells, long int numCells)
{
    char ch[2]="";
    int ftype1, n1, i=0;
    *ch = meshFile.get();
    while (*ch!=')')
    {
        ftype1 = *ch-'0';
        *ch = meshFile.get();
        //fprintf(stderr,"%s\n",ch);
    }
    if(ftype1==0)
    {
        while (*ch!='(')
        {
            *ch = meshFile.get();
            //fprintf(stderr,"%s\n",ch);
        }
        while(i<numCells)
        {
            meshFile >> n1;
            cells[i].type = n1;
            i++;
        }
    }
    if(ftype1==2)
    {
        while(i<numCells)
        {
            cells[i].type = 2;
            i++;
        }
    }
    if(ftype1==4)
    {
        while(i<numCells)
        {
            cells[i].type = 2;
            i++;
        }
    }
    if(ftype1==6)
    {
        while(i<numCells)
        {
            cells[i].type = 2;
            i++;
        }
    }
}
/* ******************************************* */



