/*
Comparison of NDKV algorithms in applications
Optimized for large databases
Original Author: David Arnas
Edited by: Carl Leake
04-04-2018
*/

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include "kdtree.h"

#define N 1E6
#define ND 2
#define NDB 10 // Number of databases
#define NM 10  // k-vector's mean density
#define Niter 10000

void sort(double *, int *, int, int);
void intsort(int *, int *, int, int);

int main()
{
    /* Variable declaration */
    int i, j, k, m, r, s, p, q, it, itn, flag, ne;
    int NP, NP2, NK, empty;
    double xb, smin, smax, timer, gain;

    double     *x = malloc(N*ND*sizeof(double));
    double    *x0 = malloc(N*ND*sizeof(double));
    double   *x00 = malloc(N*ND*sizeof(double));

    double *rangelower = malloc(Niter*ND*sizeof(double));
    double *rangeupper = malloc(Niter*ND*sizeof(double));
    int     *krange = malloc(2*ND*sizeof(int));
    int    *kpoints = malloc(ND*sizeof(int));
    int *kdimension = malloc(ND*sizeof(int));

    int kdw, kup;
    long int start, end;

    unsigned long long x1[3];
    unsigned long long x2[3];
    unsigned long long xt, yt, m1, m2, ax2p, ax3n, ay1p, ay3n;

    void *ptree;
    double *pos = malloc(ND*sizeof(double));
    double *min = malloc(ND*sizeof(double));
    double *max = malloc(ND*sizeof(double));
    int *data = malloc(N*sizeof(int));

////////////////////////////////////////////////////////
////////////////////////////////////////////////////////

    /* Random number seed and parameters */
    ax2p = 1403580;
    ax3n = 810728;
    ay1p = 527612;
    ay3n = 1370589;
    x1[0] = 12345;
    x1[1] = 12345;
    x1[2] = 12345;
    x2[0] = 12345;
    x2[1] = 12345;
    x2[2] = 12345;
    m2 = 1;
    for (i = 0; i < 32; ++i){
        m2 *= 2;
    }
    m1 = m2 - 209;
    m2 = m2 - 22853;

    /* Database generation */
    for(i = 0; i < ND*N; i++){
        // Random number generation
        xt = fmod(ax2p*x1[1] - ax3n*x1[2],m1);
        yt = fmod(ay1p*x2[0] - ay3n*x2[2],m2);
        if (xt <= yt){
            x0[i] = 1.0*(xt - yt + m1)/(m1 + 1);
        }
        else {
            x0[i] = 1.0*(xt - yt)/(m1 + 1);
        }
        x1[2] = x1[1];
        x1[1] = x1[0];
        x1[0] = xt;
        x2[2] = x2[1];
        x2[1] = x2[0];
        x2[0] = yt;

        // Copy of the database
	    x[i] = x0[i];
	    x00[i] = x0[i];
    };


////////////////////////////////////////////////////////
////////////////////////////////////////////////////////

    /* Searching interval */
    for (it = 0; it < Niter; it++){
        for (j = 0; j < ND; j++){
            rangelower[it*ND + j] = 0.99;//(float) rand()/RAND_MAX;
            rangeupper[it*ND + j] = 1.0;//0.1*(it+1.0)/Niter;//(float) rand()/RAND_MAX;
            if (rangelower[it*ND + j] > rangeupper[it*ND + j]){
                xb                    = rangelower[it*ND + j];
                rangelower[it*ND + j] = rangeupper[it*ND + j];
                rangeupper[it*ND + j] = xb;
            }
        }
    }

////////////////////////////////////////////////////////
////////////////////////////////////////////////////////

    /* Searching process by brute force */

    // Reference algorithm based on a brute force approach
    start = clock();

    for (it = 0; it < Niter; it++){

        /* Searching process by brute force */
        k = 0;
        for (i = 0; i < N; i++){
            flag = 1;
            for (j = 0; j < ND; j++){
                m = j + ND*i;
                if ((   x0[m] < rangelower[it*ND + j])
                    || (x0[m] > rangeupper[it*ND + j])){
                    flag = 0;
                    break;
                }
            }
            if (flag == 1){
                ++k;
            }
        }
        ne = k;
    }
    end = clock();
    timer = (double)(end - start) / CLOCKS_PER_SEC;

    printf("\nBrute force\n");
    printf("Time: %.16f\n", timer);
    printf("Number of elements: %d\n\n", ne);

////////////////////////////////////////////////////////
////////////////////////////////////////////////////////

/* Copy original database back into x */

    for (i = 0; i < N*ND; i++){
        x[i] = x0[i];
    }

////////////////////////////////////////////////////////
////////////////////////////////////////////////////////

    // Set the size of the databases
    NP = N/NDB;
    NP2 = N - (NDB - 1)*NP;

    // Set the size of the k-vectors
    NK = NP/NM;

    // Prediction of the gain by the projection method
    if (log10(NP) > 3.0){
        gain = 1.5*(log10(NP) - 3);
    }
    else {
        gain = 1.0;
    }


    {double *xsort = malloc(NP2*sizeof(double));
    int  *indsort = malloc(NP2*sizeof(int));
    int *index = malloc(N*ND*sizeof(int));
    double *xsortt = malloc(N*sizeof(double));
    int  *indsortt = malloc(N*sizeof(int));

    int    *kvec = malloc(NDB*NK*ND*sizeof(int));
    double *mvec = malloc(NDB*ND*sizeof(double));
    double *qvec = malloc(NDB*ND*sizeof(double));


////////////////////////////////////////////////////////
////////////////////////////////////////////////////////

    /* k-vector preprocessing */
        smin = 0;
        smax = 1;


    // Global sorting
    for (j = 0; j < N; j++){
        xsortt[j] = x0[ND - 1 + ND*j];
        indsortt[j] = j;
    }
    sort(xsortt, indsortt, 0, N-1);
    for (j = 0; j < N; j++){
        for (i = 0; i < ND; i++){
            x0[i + ND*j] = x[i + ND*indsortt[j]];
        }
    }

    for (i = 0; i < N*ND; i++){
        x[i] = x0[i];
    }


    // Selection of the database

    // First database

    // Selection of the dimension
    for (m = 0; m < ND; m++){
        // Sorting process
        for (j = 0; j < NP2; j++){
            xsort[j]   = x0[m + ND*j];
            indsort[j] = j;
        }
        sort(xsort, indsort, 0, NP2-1);
        if (m == 0){
            for (j = 0; j < NP2; j++){
                for (i = 0; i < ND; i++){
                    x0[i + ND*j] = x[i + ND*indsort[j]];
                }
            }
        }
        for (j = 0; j < NP2; j++){
            x[m + ND*j]     = xsort[j];
            if (m == 0){
                index[ND*j] = j;
            }
            else {
                index[m + ND*j] = indsort[j];
            }
        }


        // k-vector
        i = 0;
        kvec[m*NK] = 0;
        for (j = 1; j < NK-1; j++){
            xb = smin + j*(smax - smin)/(NK - 1);
            while (xsort[i] < xb){
                ++i;
                if (i == NP2){
                    break;
                }
            }
            if (i >= NP2){
                for (k = j; k < NK; k++){
                    kvec[m*NK + k] = NP2;
                }
                break;
            }
            else{
                kvec[m*NK + j] = i;
            }
        }
        kvec[m*NK + NK-1] = NP2;
        mvec[m] = (NK - 1)/(smax - smin);
        qvec[m] = m*NK - smin*mvec[m];
    }

    // Remaining databases
    for (p = 1; p < NDB; p++){

        // Selection of the dimension
        for (m = 0; m < ND; m++){
            // Sorting process
            for (j = 0; j < NP; j++){
                indsort[j] = j + NP2 + (p - 1)*NP;
                xsort[j]   = x0[m + ND*indsort[j]];
            }
            sort(xsort, indsort, 0, NP-1);
            if (m == 0){
                for (j = 0; j < NP; j++){
                    for (i = 0; i < ND; i++){
                        x0[i + ND*(j + NP2 + (p - 1)*NP)] = x[i + ND*indsort[j]];
                    }
                }
            }
            for (j = 0; j < NP; j++){
                x[m + ND*(j + NP2 + (p - 1)*NP)]     = xsort[j];
                if (m == 0){
                    index[ND*(j + NP2 + (p - 1)*NP)] = j + NP2 + (p - 1)*NP;
                }
                else {
                    index[m + ND*(j + NP2 + (p - 1)*NP)] = indsort[j];
                }
            }

            // k-vector
            i = 0;
            kvec[ND*NK*p + m*NK] = NP2 + NP*(p - 1);
            for (j = 1; j < NK-1; j++){
                xb = smin + j*(smax - smin)/(NK - 1);
                while (xsort[i] < xb){
                    ++i;
                    if (i == NP){
                        break;
                    }
                }
                if (i >= NP){
                    for (k = j; k < NK; k++){
                        kvec[ND*NK*p + m*NK + k] = NP2 + NP*p;
                    }
                    break;
                }
                else{
                    kvec[ND*NK*p + m*NK + j] = NP2 + NP*(p - 1) + i;
                }
            }
            kvec[ND*NK*p + m*NK + NK-1] = NP2 + NP*p;
            mvec[p*ND + m] = (NK - 1)/(smax - smin);
            qvec[p*ND + m] = p*NK*ND + m*NK - smin*mvec[m];
        }
    }

////////////////////////////////////////////////////////
////////////////////////////////////////////////////////

    /* Searching process by ND-kvector */

    // Enhanced intersection process for the NDKV

    start = clock();

    for (it = 0; it < Niter; it++){

        ne = 0;
        itn = it*ND;

        /* Searching process by NDKV + Brute force */
        for (p = 0; p < NDB; p++){

            empty = 0;

            for (j = ND - 1; j >=  0; j--){

                /* Approximate boundaries of points */
                k = p*ND + j;
                kdw = floor(rangelower[it*ND + j]*mvec[k] + qvec[k]);
                kup =  ceil(rangeupper[it*ND + j]*mvec[k] + qvec[k]);
                k = 2*j;
                krange[k] = kvec[kdw];
                k = k + 1;
                krange[k] = kvec[kup] - 1;

                kpoints[j] = krange[k] - krange[k - 1] + 1;

                if (kpoints[j] <= 0){
                    empty = 1;
                    break;
                }

                kdimension[j] = j;

            }

            if (empty == 0){

                // Reference for branch selection
                m = kpoints[0]/gain;

                /* Sorting number of points per dimension */
                intsort(kpoints, kdimension, 0, ND-1);

                if (kpoints[0] > m) { // Projection method
                    // Real boundaries of the first dimension
                    kdw = krange[0];
                    kup = krange[1];

                    for (i = krange[0]; i <= krange[1]; i++){
                        if (x0[ND*i] >= rangelower[itn]){
                            kdw = i;
                            break;
                        }
                    }

                    for (i = krange[1]; i >= krange[0]; i--){
                        if (x0[ND*i] <= rangeupper[itn]){
                            kup = i;
                            break;
                        }
                    }

                    // Remaining dimensions
                    for (i = kdw; i <= kup; i++){
                        flag = 1;
                        k = ND*i;
                        for (r = 1; r < ND; r++){
                            s = r + k;
                            q = itn + r;
                            if (   (x0[s] < rangelower[q])
                                || (x0[s] > rangeupper[q])){
                                flag = 0;
                                break;
                            }
                        }
                        if (flag == 1){
                            ++ne;
                        }
                    }
                }
                else {
                    // Real boundaries of the dimension selected
                    j = kdimension[0];
                    k = 2*j;
                    kdw = krange[k];
                    k = k + 1;
                    kup = krange[k];

                    for (i = kdw; i <= kup; i++){
                        if (x0[j + ND*index[j + ND*i]] >= rangelower[itn + j]){
                            break;
                        }
                    }
                    kdw = i;

                    for (i = kup; i >= kdw; i--){
                        if (x0[j + ND*index[j + ND*i]] <= rangeupper[itn + j]){
                            break;
                        }
                    }
                    kup = i;

                    // Remaining dimensions
                    for (i = kdw; i <= kup; i++){
                        flag = 1;
                        k = ND*index[j + ND*i];
                        for (r = 1; r < ND; r++){
                            q = kdimension[r];
                            s = q + k;
                            q = q + itn;
                            if (   (x0[s] < rangelower[q])
                                || (x0[s] > rangeupper[q])){
                                flag = 0;
                                break;
                            }
                        }
                        if (flag == 1){
                            ++ne;
                        }
                    }
                }
            }
        }
    }

    end = clock();
    timer = (double)(end - start) / CLOCKS_PER_SEC;

    printf("ND-kvector\n");
    printf("Time: %.16f\n", timer);
    printf("Number of elements: %d\n\n", ne);

////////////////////////////////////////////////////////
////////////////////////////////////////////////////////

    free(index); free(kvec); free(mvec); free(qvec); free(indsort); free(xsort);
    }
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////

/* Copy original database back into x */

    for (i = 0; i < N*ND; i++){
        x[i] = x00[i];
        x0[i] = x00[i];
    }

////////////////////////////////////////////////////////
////////////////////////////////////////////////////////

	    /* k-d tree pre-processing */

	    // Create the k-d tree
	    ptree = kd_create(ND);

	    // Add points to the k-d tree
	    for (i=0;i<N;i++){
		for (j=0;j<ND;j++){
		     pos[j] = x0[i*ND+j];
		};
		data[i] = i;
		assert(0==kd_insert(ptree,pos,&data[i]));
	    };

////////////////////////////////////////////////////////
////////////////////////////////////////////////////////

    /* Search via k-d tree */
    start = clock();
    for (it = 0; it < Niter; it++){
        for (j = 0; j < ND; j++){
            min[j] = rangelower[it*ND + j];
            max[j] = rangeupper[it*ND + j];
        };

        ne = 0;

        ne = FullOrthRangeSearch(ptree,min,max,ND);
    }
    end = clock();
	timer = (double)(end - start) / CLOCKS_PER_SEC;

	kd_free(ptree);

    printf("k-d tree search\n");
    printf("Time: %.16f\n", timer);
    printf("Number of elements: %d\n\n", ne);

//////////////////////////////////////////////////////
//////////////////////////////////////////////////////

    return 0;
}

/* Auxiliary functions */

////////////////////////////////////////////////////////
////////////////////////////////////////////////////////

void sort(double *xsort, int *ind, int left, int right) {

    int i = left, j = right;
    double pivot;

    int tempi;
    double temp;

    pivot = xsort[(left + right) / 2];

    /* partition */
    while(i <= j){
        while(xsort[i] < pivot)
            ++i;
        while(xsort[j] > pivot)
            --j;
        if(i <= j){
            temp     = xsort[i];
            xsort[i] = xsort[j];
            xsort[j] = temp;
            tempi    = ind[i];
            ind[i]   = ind[j];
            ind[j]   = tempi;
            ++i;
            --j;
        }
    }

    /* recursion */
    if(left < j)
        sort(xsort, ind, left, j);
    if(i < right)
        sort(xsort, ind, i, right);

}

void intsort(int *xsort, int *ind, int left, int right) {

    int i = left, j = right;
    int pivot;

    int tempi;
    int temp;

    pivot = xsort[(left + right) / 2];

    /* partition */
    while(i <= j){
        while(xsort[i] < pivot)
            ++i;
        while(xsort[j] > pivot)
            --j;
        if(i <= j){
            temp     = xsort[i];
            xsort[i] = xsort[j];
            xsort[j] = temp;
            tempi    = ind[i];
            ind[i]   = ind[j];
            ind[j]   = tempi;
            ++i;
            --j;
        }
    }

    /* recursion */
    if(left < j)
        intsort(xsort, ind, left, j);
    if(i < right)
        intsort(xsort, ind, i, right);

}
