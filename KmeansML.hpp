#ifndef KMEANSML_H
#define KMEANSML_H 1

double *new_init_DOUBLE( int n );
void init_INT( int n, int array[] );
void init_DOUBLE( int n, double array[] );
void init_array2D_DOUBLE ( int d, int c, double *arr[] );
void kmeans( int no_of_dimension, int no_of_sample, int no_of_cluster, int it_max, int it_num, double *sample[], int cluster[], double *cluster_center[],int cluster_population[], double cluster_entropy[] );


#endif
