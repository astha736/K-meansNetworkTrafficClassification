#include <cstdlib>
#include <iostream>
#include <fstream>
#include <cstring>
#include <sstream>
#include <string>
#include <stdlib.h>
#include "KmeansML.hpp"
using namespace std;

int compair(char buffer[]);
string  cluster_Name(int i);
void print_Output(int no_of_cluster,double* output);

int main()
{
int no_of_dimension = 6;
int no_of_sample = 216;
int no_of_cluster = 10;
int it_max = 100;
char  buffer[20];
string STRING;
int line_no =1;
int j = 0;
int it_num = 10;

ifstream myReadFile ;
ifstream myResult;

 myReadFile.open("2012C6PS736_D2_1.txt");

//******************************GENERAL CHECKS*******************************
 if (!myReadFile.is_open()) {
    cout << "\t ...Opps could not open file ...\n";
    return 0;
 }
//******************************END OF CHECKS*******************************
//******************************READ THE FIRST LINE*************************
    if(line_no == 1 && !myReadFile.eof())
    {
        getline(myReadFile,STRING);
        istringstream iss(STRING);
        cout << "\n....... Features are ........\n\n";
        while (iss)
        {
            string sub;
            iss >> sub;
            cout << sub<< "\n";
            j++; //count the number of fields
        }
        line_no++;
    }
    else
    {
        cout << "Some error with the file";
    }
//******************************END OF THE FIRST LINE************************

//******************************READ THE REST OF DATA************************
    double **sample;
    sample=(double**)malloc(no_of_sample*sizeof(double*));
    int m;
    for(m=0;m<no_of_sample;m++)
    {
        sample[m]=(double*)malloc(sizeof(double)*no_of_dimension);
    }

    if(line_no > 1)
    {
    int i;
    char* mum;
    j = 0;


     while(!myReadFile.eof() && j< no_of_sample)
        {
        i = 0;                              //index for each double in that  line
                while(i<no_of_dimension){

                if(i<no_of_dimension && i > -1)
                    {
                        myReadFile >> buffer;
                        sample[j][i] = atof(buffer);
                    }
                    i++;
                }
            j++;
            line_no++;
        }
    }
    else
    {
        myReadFile.close();
        return 0;
    }

//******************************END OF THE REST OF DATA************************

//creating array for cluster no for each sample
int cluster[no_of_sample];

//creating array to store properties of cluster center
double **cluster_center;
    cluster_center=(double**)malloc(no_of_cluster*sizeof(double*));
    for(m=0;m<10;m++)
    {
        cluster_center[m]=(double*)malloc(sizeof(double)*no_of_dimension);
    }
//initializing cluster center
for( int i =0; i< no_of_cluster;i++)
    for(int j =0;j<no_of_dimension;j++)
    cluster_center[i][j] = i+j;
//creating array for no of samples(population) cluster contains
int cluster_population[no_of_cluster];
//creating array for weight or entropy of each cluster
double cluster_entropy[no_of_cluster];

kmeans( no_of_dimension,no_of_sample, no_of_cluster,it_max, it_num ,(double**)sample,cluster ,(double **)cluster_center ,cluster_population ,cluster_entropy);

myResult.open("2012C6PS736_D2_2.txt");
//******************************GENERAL CHECKS*******************************
 if (!myResult.is_open())
 {
    cout << "\t ...Opps could not open file ...\n";
    return 0;
 }
//******************************END OF CHECKS*******************************
double *result;
result=(double*)malloc(no_of_sample*sizeof(double));

//Initialize with the given output
for(int i = 0;i<no_of_sample;i++){
    if(myResult.eof())
        {
            cout<<"\nReached end of file";
            break;
        }
        myResult>> buffer;
        result[i] = compair(buffer);
}
//pred will store frequency of occurrence of class in a cluster
double **pred;
    pred=(double**)malloc(no_of_cluster*sizeof(double*));
    for(m=0;m<no_of_cluster;m++)
    {
        pred[m]=(double*)malloc(sizeof(double)*no_of_cluster);
    }

init_array2D_DOUBLE ( no_of_cluster, no_of_cluster, pred);

for(int i =0;i<no_of_cluster;i++)
{
    for(int k =0;k<no_of_sample;k++)
    {
        if( result[k] == i)             //initialize for a given kind of cluster
        {
            j = cluster[k];
            pred[i][j] = pred[i][j] + 1;
        }
    }
    //pred has the values
}
free(result);
double *output;
output=(double*)malloc(no_of_sample*sizeof(double));

int maximum;
int n;
double total;
for(int i =0;i<no_of_cluster;i++)
{   maximum = -1;
    m = -1;
    n = -1;
    total = 0;
    for(int k =0;k<no_of_cluster;k++)
    {
        if( maximum < pred[i][k])
        {
            maximum = pred[i][k];
            m = i ;
            n = k ;
        }
        if(pred[i][k]!= -1)
        {
            total = total + pred[i][k];
        }
    }
    if( m != -1 && n != -1)
    {
        if(output[n] != -1)
        {
            output[i] = (double)pred[m][n]/(double)total; // mapping cluster number to an class
        }
    }
}

cout<<"......Final Output is.......\n\n";
print_Output(no_of_cluster,output);

return 0;
}

int compair(char buffer[])
{
    if(!strcmp(buffer,"ATTACK")) //string compair gives zero for true
        {
          return 0;
        }
        else if(!strcmp(buffer,"DATABASE"))
        {
          return 1;
        }
        else if(!strcmp(buffer,"FTP-CONTROL"))
        {
          return 2;
        }
        else if(!strcmp(buffer,"FTP-DATA"))
        {
          return 3;
        }
        else if(!strcmp(buffer,"FTP-PASV"))
        {
          return 4;
        }
        else if(!strcmp(buffer,"MAIL"))
        {
         return 5;
        }
        else if(!strcmp(buffer,"MULTIMEDIA"))
        {
          return 6;
        }
        else if(!strcmp(buffer,"P2P"))
        {
          return 7;
        }
        else if(!strcmp(buffer,"SERVICES"))
        {
          return 8;
        }
        else if(!strcmp(buffer,"WWW"))
        {
          return 9;
        }
        else
        {
           return -1;
        }
        return -1;
}
string  cluster_Name(int i)
{
   switch(i)
   {
    case 0:
        return "ATTACK";
    case 1:
          return "DATABASE";
    case 2:
          return "FTP-CONTROL";
    case 3:
          return "FTP-DATA";
    case 4:
          return "FTP-PASV";
    case 5:
         return "MAIL";
    case 6:
          return "MULTIMEDIA";
    case 7:
          return "P2P";
    case 8:
          return "SERVICES";
    case 9:
          return "WWW";
    default:
           return "NULL";

   }
}

void print_Output(int no_of_cluster,double* output)
{
    for(int i = 0 ; i < no_of_cluster ; i++ )
{
        cout<<"\n"<<i<<" "<<cluster_Name(i)<<":"<<output[i];
}
}
