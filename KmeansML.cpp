# include <cstdlib>
# include <iostream>
# include <fstream>
# include <iomanip>
# include <math.h>
# include <cmath>
# include <ctime>
# include <cstring>
# include <stdlib.h>

using namespace std;

# include "KmeansML.hpp"

double *new_init_DOUBLE( int n )
{
  double *array;
  int i;

  array = new double[n];

  for ( i = 0; i < n; i++ )
  {
    array[i] = 0.0;
  }
  return array;
}

void init_INT( int n, int array[] )
{
  int i;

  for ( i = 0; i < n; i++ )
  {
    array[i] = 0;
  }
  return;
}

void init_DOUBLE( int n, double array[] )
{
  int i;

  for ( i = 0; i < n; i++ )
  {
    array[i] = 0.0;
  }
  return;
}
void init_array2D_DOUBLE (int  no_of_dimension,int no_of_cluster, double *array[] )
{
  int i,j;
  i = 0;
  j=0;
  for ( i = 0; i < no_of_cluster; i++ )
  {
    for ( j = 0; j < no_of_dimension; j++ )
  {
    array[i][j]= 0.0;
  }
  }
  return;
}

void kmeans( int no_of_dimension, int no_of_sample, int no_of_cluster, int it_max, int it_num, double *sample[], int cluster[], double *cluster_center[],
  int cluster_population[], double cluster_entropy[] )
{
//  ******************************BEGIN: General Checks *****************************************
  if ( no_of_cluster < 1 ||  no_of_dimension < 1 || no_of_sample < 1)
  {
    cout << "\nKMEANS:\terror\n";
    cout << "Number of Clusters, Dimensions and Sample samples should be greater than 1.\n";
    exit ( 1 );
  }

//  ********************************END: General Checks *****************************************

//  ******************************BEGIN: Initialization *****************************************
  int i,j,k;
  double *sample_entropy;
  double sample_entropy_min;
  sample_entropy =(double*)malloc(sizeof(double)*no_of_cluster);
  int swap;

//    INITIALIZE CLUSTER FOR EACH sample
  for(i = 0 ; i < no_of_sample;i++)
  { 
    for(k=0;k<no_of_cluster;k++)
    {   
      for(j = 0; j < no_of_dimension; j++ )
      { 
        sample_entropy[k] = sample_entropy[k]+pow ( sample[i][j] - cluster_center[k][j], 2 );
      }
    }
    int c;
    int m;
    sample_entropy_min = 1.0E+30;
    c = -1;
    for(m=0;m<no_of_cluster;m++)
    {
      if ( sample_entropy[m] < sample_entropy_min )
      {
        sample_entropy_min = sample_entropy[m];
        c = m;
      }
    }
    cluster[i] = c;
  }

free(sample_entropy);

//    SET CLUSTER POPULATION
init_INT ( no_of_cluster, cluster_population );

  for ( j = 0; j < no_of_sample; j++ )
  {
    k = cluster[j];
    cluster_population[k] = cluster_population[k] + 1;
  }

// SET THE CENTER OR CLUSTERS
  init_array2D_DOUBLE ( no_of_dimension, no_of_cluster, (double**)cluster_center );

//find the aggregate
  for ( i = 0; i < no_of_sample; i++ )
  {
    k = cluster[i];
    for ( j = 0; j < no_of_dimension; j++ )
    {
      cluster_center[k][j] = cluster_center[k][j] + sample[i][j];
    }
  }

//divide the sum by total population
  for ( k = 0; k < no_of_cluster; k++ )
  {
    if ( 0 < cluster_population[k] )
    {
      for ( i = 0; i < no_of_dimension; i++ )
      {
        cluster_center[k][i] = cluster_center[k][i] /( double ) cluster_population[k];
      }
    }
  }

double* f;
  f = new_init_DOUBLE ( no_of_sample );

// SET THE entropy FOR EACH sample
  for ( i = 0; i < no_of_sample; i++ )
  {
    k = cluster[j];
    for ( j = 0; j < no_of_dimension; j++ )
    {
      f[i] = f[i] + pow ( sample[i][j] - cluster_center[k][j], 2 );
    }
  }

  init_DOUBLE ( no_of_cluster, cluster_entropy );
// SET CLUSTER entropy USING entropy FOR EACH sample
  for ( j = 0; j < no_of_sample; j++ )
  {
    k = cluster[j];
    cluster_entropy[k] = cluster_entropy[k] + f[j];
  }

//  Adjust the sample energies by a weight factor.
  for ( j = 0; j < no_of_sample; j++ )
  {
    k = cluster[j];
    if ( 1 < cluster_population[k] )
    {
      f[j] = f[j] * ( double ) ( cluster_population[k] )
        / ( double ) ( cluster_population[k] - 1 );
    }
  }

//  ********************************END: Initialization *****************************************

//  ******************************BEGIN: Iteration for clustering *******************************

  it_num = 0;
  int old_entropy;
  int new_entropy;
  int old_cluster_no;
  int new_cluster_no;
  int j2;

  while ( it_num < it_max )
  {
    it_num ++;
    swap = 0;

    for ( j = 0; j < no_of_sample; j++ ) //for each sample
    {
      old_cluster_no = cluster[j];
      new_cluster_no = old_cluster_no;

      if ( cluster_population[old_cluster_no] <= 1 )  // do nothing
      {
        continue;
      }

     old_entropy= f[j];

      for ( k = 0; k < no_of_cluster; k++ ) //for all clusters
      {
        if ( k != old_cluster_no )                    //where cluster is not equal to original
        {                                 //calculate entropy along with weight
          new_entropy = 0.0;
          for ( i = 0; i < no_of_dimension; i++ )
          {
            new_entropy = new_entropy + pow ( sample[j][i] - cluster_center[k][i], 2 );
          }
          new_entropy = new_entropy * ( double ) cluster_population[k]
             / ( double ) ( cluster_population[k] + 1 );

          if ( new_entropy <old_entropy)                  //if current entropy new_entropy is less than previous old_entropy
          {                               // then swap
           old_entropy= new_entropy;
            new_cluster_no = k;                       //if condition is satisfied old_cluster_no and new_cluster_no have difference
          }
        }
      }
// no change in cluster for a particular sample
      if ( new_cluster_no == old_cluster_no )
      {
        continue;
      }
// else if there is a change then reassign
      for ( i = 0; i < no_of_dimension; i++ )
      {
        cluster_center[old_cluster_no][i] = ( cluster_center[old_cluster_no][i]           //previous cluster center property will decrease
          * ( double ) ( cluster_population[old_cluster_no] ) - sample[j][i] )
          / ( double ) ( cluster_population[old_cluster_no] - 1 );

        cluster_center[new_cluster_no][i] = ( cluster_center[new_cluster_no][i]           //current cluster center property will increase
          * ( double ) ( cluster_population[new_cluster_no] ) + sample[j][i] )
          / ( double ) ( cluster_population[new_cluster_no] + 1 );
      }
      cluster_entropy[old_cluster_no] = cluster_entropy[old_cluster_no] - f[j];             //previous cluster center property will decrease
      cluster_entropy[new_cluster_no] = cluster_entropy[new_cluster_no] + old_entropy;               //current cluster center property will increase
      cluster_population[new_cluster_no] = cluster_population[new_cluster_no] + 1;        //previous cluster center property will decrease
      cluster_population[old_cluster_no] = cluster_population[old_cluster_no] - 1;        //current cluster center property will increase

      cluster[j] = new_cluster_no;                                            //change cluster for sample j to new_cluster_no

//  Adjust the value of F for samples that belong to old_cluster_no and new_cluster_no.
      for ( j2 = 0; j2 < no_of_sample; j2++ )
      {
        k = cluster[j2];

        if ( k == old_cluster_no || k == new_cluster_no )            //both old_cluster_no and new_cluster_no had change, decrese and increase in sample respectively
        {
          f[j2] = 0.0;
          for ( i = 0; i < no_of_dimension; i++ )
          {
            f[j2] = f[j2] + pow ( sample[j2][i] - cluster_center[k][i], 2 );
          }

          if ( 1 < cluster_population[k] )
          {
            f[j2] = f[j2] * ( double ) ( cluster_population[k] ) / ( ( double ) ( cluster_population[k] - 1 ) );
          }
        }
      }
      swap = swap + 1;
    }
//  Exit if no reassignments were made during this iteration.
    if ( swap == 0 )
    {
      break;
    }
  }

//  ********************************END: Iteration for clustering *******************************
//  Compute the cluster energies again .
  init_DOUBLE ( no_of_cluster, cluster_entropy );
  double sample_entrpy;
  for ( j = 0; j < no_of_sample; j++ )
  {
    k = cluster[j];

    sample_entrpy = 0.0;
    for ( i = 0; i < no_of_dimension; i++ )
    {
      sample_entrpy = sample_entrpy +
        pow ( sample[j][i] - cluster_center[k][i], 2 );
    }
    cluster_entropy[k] = cluster_entropy[k] + sample_entrpy;
  }
  delete [] f;
  return;
}
