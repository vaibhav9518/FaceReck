#include <math.h>
#include <stdio.h>
#include <assert.h>
#include <malloc.h>
static const char* const copyright =
  "Calculate squared rho-path distances of a set of points to a subset on a nearest neighbor graph.\n"
  "Written by Alexander Zien, MPI-Kyb, 2004.";

static const char* const syntax =
  "USAGE: [ E2 ] = rhoPathDists2( D2, NN, subset, [rho=2] )\n";


//=============================================================================//
//=== Auxilliary
//=============================================================================//

//-----------------------------------------------------------------------------//
//--- elementary computations
//-----------------------------------------------------------------------------//

inline double min( double a, double b )
{
  return a < b ? a : b;
}


inline double max( double a, double b )
{
  return a > b ? a : b;
}



//=============================================================================//
//=== Priority Queue (implemented as Binary Heap)
//=============================================================================//

//-----------------------------------------------------------------------------//
//--- insert (reduce distance if already queued)
//-----------------------------------------------------------------------------//

inline void insertIntoPq( int& n, int* const heap, int* const track, const double* const dists, int element )
{
  register const double d_element = dists[ element ];
  register int actual;
  actual = track[ element ];
  if( actual < 0 ) {
    actual = n;
    ++n;
  }
  while( actual > 0 ) {
    register const int parent = ( actual - 1 ) / 2;
    register const int h_parent = heap[ parent ];
    if( dists[ h_parent ] <= d_element ) {
      break;
    }
    heap[ actual ] = h_parent;
    track[ h_parent ] = actual;
    actual = parent;
  }
  heap[ actual ] = element;
  track[ element ] = actual;
}


//-----------------------------------------------------------------------------//
//--- remove first (shortest distance) and return it
//-----------------------------------------------------------------------------//

inline int fetchFirstFromPq( int& n, int* const heap, int* const track, const double* const dists )
{
  --n;
  const double d_last = dists[ heap[ n ] ];
  const int nHalf = n / 2;
  const int result = heap[ 0 ];
  register int actual;
  actual = 0;
  while( actual < nHalf ) {
    const int leftChild = 2*actual + 1;
    const int rightChild = leftChild + 1;
    register int child;
    if( rightChild >= n ) {
      child = leftChild;
    } else if( dists[heap[leftChild]] <= dists[heap[rightChild]] ) {
      child = leftChild;
    } else {
      child = rightChild;
    }
    register const int h_child = heap[ child ];
    assert( track[h_child] == child );
    if( d_last <= dists[ h_child ] ) {
      break;
    }
    heap[ actual ] = h_child;
    track[ h_child ] = actual;
    actual = child;
  }
  const int h_last = heap[ n ];
  heap[ actual ] = h_last;
  track[ h_last ] = actual;
  assert( track[result] == 0 );
  track[ result ] = -1;
  return result;
}



//=============================================================================//
//=== Distance Computations (implented by Dijksta-style algorithm)
//=============================================================================//

//-----------------------------------------------------------------------------//
//--- path rho = 0 [back to Euclidean distance]
//-----------------------------------------------------------------------------//

void calcPathDists0( double* const E2,
		     const int n, const int l, const int k,
		     const double* const D2, const int* const NN,
		     const int* const subset )
{
  // === variables
  double* D0 = ( k == 0 ) ? new double[ n * n ] : new double[ k * n ];
  double* e0;
  int* openHeap = new int[ n ];
  int* openTrack = new int[ n ];
  int nofOpen;
  int ii;
  register int j;
  // === un-square distances
  if( k == 0 ) {
    for( j = 0; j < n*n; ++j ) {
      D0[j] = sqrt( D2[j] );
    }
  } else {
    register int p;
    for( p = 0; p < k*n; ++p ) {
      D0[p] = sqrt( D2[p] );
    }
  }
  // === compute
  e0 = E2;
  for( ii = 0; ii < l; ++ii ) {
    const int i = subset[ ii ];
    // --- prepare (squared distances, set of open nodes)
    for( j = 0; j < n; ++j ) {
      e0[j] =  pow(10,320);
      openTrack[j] = -1;
    }
    e0[i] = 0;
    nofOpen = 0;
    insertIntoPq( nofOpen, openHeap, openTrack, e0, i );
    // --- iteratively update squared distances
    while( nofOpen > 0 ) {
      const int jStar = fetchFirstFromPq( nofOpen, openHeap, openTrack, e0 );
      register const double d0Star = e0[ jStar ];
      if( k == 0 ) {
	const double* const d0 = D0 + jStar*n;
	for( j = 0; j < n; ++j ) {
	  if( d0Star + d0[j] < e0[j] ) {
	    e0[j] = d0Star + d0[j];
	    insertIntoPq( nofOpen, openHeap, openTrack, e0, j );  // or update, if already in queue
	  }
	}
      } else {
	register const int* const nn = NN + jStar*k;
	const double* const d0 = D0 + jStar*k;
	register int p;
	for( p = 0; p < k; ++p ) {
	  register const int j = nn[ p ] - 1;
	  if( d0Star + d0[p] < e0[j] ) {
	    e0[j] = d0Star + d0[p];
	    insertIntoPq( nofOpen, openHeap, openTrack, e0, j );  // or update, if already in queue
	  }
	}
      }
    }
    // --- re-square path distances
    for( j = 0; j < n; ++j ) {
      register double e0_j = e0[j];
      e0[j] = e0_j * e0_j;
    }
    // --- next element of subset
    e0 += n;
  }
  // === clean up
  delete[] D0;
  delete[] openHeap;
  delete[] openTrack;
}


//-----------------------------------------------------------------------------//
//--- path rho = +inf [max]
//-----------------------------------------------------------------------------//

void calcPathDistsInf( double* const E2,
		       const int n, const int l, const int k,
		       const double* const D2, const int* const NN,
		       const int* const subset )
{
  // === variables
  double* e2;
  int* openHeap = new int[ n ];
  int* openTrack = new int[ n ];
  int nofOpen;
  int ii;
  register int j;
  // === compute
  e2 = E2;
  for( ii = 0; ii < l; ++ii ) {
    const int i = subset[ ii ];
    // --- prepare (squared distances, set of open nodes)
    for( j = 0; j < n; ++j ) {
      e2[j] =  pow(10,320);
      openTrack[j] = -1;
    }
    e2[i] = 0;
    nofOpen = 0;
    insertIntoPq( nofOpen, openHeap, openTrack, e2, i );
    // --- iteratively update squared distances
    while( nofOpen > 0 ) {
      const int jStar = fetchFirstFromPq( nofOpen, openHeap, openTrack, e2 );
      register const double d2Star = e2[ jStar ];
      if( k == 0 ) {
	register const double* const d2 = D2 + jStar*n;
	for( j = 0; j < n; ++j ) {
	  if( max( d2Star, d2[j] ) < e2[j] ) {
	    e2[j] = max( d2Star, d2[j] );
	    insertIntoPq( nofOpen, openHeap, openTrack, e2, j );  // or update, if already in queue
	  }
	}
      } else {
	register const double* const d2 = D2 + jStar*k;
	register const int* const nn = NN + jStar*k;
	register int p;
	for( p = 0; p < k; ++p ) {
	  register const int j = nn[ p ] - 1;
	  if( max( d2Star, d2[p] ) < e2[j] ) {
	    e2[j] = max( d2Star, d2[p] );
	    insertIntoPq( nofOpen, openHeap, openTrack, e2, j );  // or update, if already in queue
	  }
	}
      }
    }
    // --- next element of subset
    e2 += n;
  }
  // === clean up
  delete[] openHeap;
  delete[] openTrack;
}


//-----------------------------------------------------------------------------//
//--- general case
//-----------------------------------------------------------------------------//

void calcPathDistsRho( double* const E2,
		       const int n, const int l, const int k,
		       const double* const D2, const int* const NN,
		       const int* const subset,
		       const double rho )
{
  // === variables
  double* Dr = ( k == 0 ) ? new double[ n * n ] : new double[ k * n ];
  double* er;
  int* openHeap = new int[ n ];
  int* openTrack = new int[ n ];
  int nofOpen;
  int ii;
  register int j;
  // === prepare function of distances
  if( k == 0 ) {
    for( j = 0; j < n*n; ++j ) {
      Dr[j] = exp( rho * sqrt(D2[j]) ) - 1.0;
    }
  } else {
    register int p;
    for( p = 0; p < k*n; ++p ) {
      Dr[p] = exp( rho * sqrt(D2[p]) ) - 1.0;
    }
  }
  // === compute
  er = E2;
  for( ii = 0; ii < l; ++ii ) {
    const int i = subset[ ii ];
    // --- prepare (squared distances, set of open nodes)
    for( j = 0; j < n; ++j ) {
      er[j] =  pow(10,320);
      openTrack[j] = -1;
    }
    er[i] = 0;
    nofOpen = 0;
    insertIntoPq( nofOpen, openHeap, openTrack, er, i );
    // --- iteratively update squared distances
    while( nofOpen > 0 ) {
      const int jStar = fetchFirstFromPq( nofOpen, openHeap, openTrack, er );
      register const double drStar = er[ jStar ];
      if( k == 0 ) {
	const double* const dr = Dr + jStar*n;
	for( j = 0; j < n; ++j ) {
	  if( drStar + dr[j] < er[j] ) {
	    er[j] = drStar + dr[j];
	    insertIntoPq( nofOpen, openHeap, openTrack, er, j );  // or update, if already in queue
	  }
	}
      } else {
	register const int* const nn = NN + jStar*k;
	const double* const dr = Dr + jStar*k;
	register int p;
	for( p = 0; p < k; ++p ) {
	  register const int j = nn[ p ] - 1;
	  if( drStar + dr[p] < er[j] ) {
	    er[j] = drStar + dr[p];
	    insertIntoPq( nofOpen, openHeap, openTrack, er, j );  // or update, if already in queue
	  }
	}
      }
    }
    // --- compute inverse function of distances
    for( j = 0; j < n; ++j ) {
      register const double t = log( 1 + er[j] ) / rho;
      er[j] = t * t;
    }
    // --- next element of subset
    er += n;
  }
  // === clean up
  delete[] Dr;
  delete[] openHeap;
  delete[] openTrack;
}



//=============================================================================//
//=== Main
//=============================================================================//

double ** rhoPathDists2(double **D2_,long k,long n,double ** NN_,double rho,double * subset_,long l)	
{
  int* subset = new int[ l ];
  int i,j;
  double* const doubleSubset =new double[l];
  double* const D2 = new double[ k * n ];
  int* const NN= new int[k*n];
  for(i=0;i<n;i++)
  {
  	for(j=0;j<k;j++)
  	{
	  	D2[i*k+j]=D2_[j][i];
	  	if(NN_!=NULL)NN[i*k+j]=(int)NN_[j][i];
    }
  }
  for(i=0;i<l;i++)doubleSubset[i]=subset_[i];
  for( i = 0; i < l; ++i ) 
  {
    register const int subset_i = (int) doubleSubset[ i ] - 1;
    subset[i] = subset_i;
  }
  double* const E2 = new double[n*l];
  double** return_mat=new double*[n];
  for(i=0;i<n;i++)return_mat[i]=new double[l];
  if(NN_==NULL)k=0;
  if( rho == 0.0 ) 
  {
    calcPathDists0( E2, n, l, k, D2, NN, subset );
  } 
  else if( rho >=pow(10,320) ) 
  {
    calcPathDistsInf( E2, n, l, k, D2, NN, subset );
  } 
  else 
  {
    calcPathDistsRho( E2, n, l, k, D2, NN, subset, rho );
  }
  for(i=0;i<l;i++)
  {
	  for(j=0;j<n;j++)
	  {
	  	return_mat[j][i]=E2[i*l+j];
	  }
  } 
  delete[] subset;
  return return_mat;
  // - clean up
}
MAT final(MAT D2,MAT NN,double* subset,long rows,double rho)
{
	double **E2=rhoPathDists2(D2.matrix,D2.rows,D2.columns,NN.matrix,rho,subset,rows);
	MAT out;
	out.matrix=E2;
	out.rows=rows;
	out.columns=D2.columns;
	return out;
}





