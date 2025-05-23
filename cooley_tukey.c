#include<stdio.h>
#include<stdlib.h>
int powMod (int base, int pow , int mod){
    int temp = 1;
    for(int i = 0;i<pow;i++){
        temp = (temp * base) % mod;
    }
    return temp;
}

int summation_A(int n,int poly[],int j,int psi,int q){
    int temp = 0;
    for(int i = 0;i<=(n/2)-1;i++){
        int pow = ((4*i*j)+(2*i))%(2*n);
        temp += (powMod(psi,pow,q)*poly[2*i]);
    }
    return temp;
}
int summation_B(int n,int poly[],int j,int psi,int q){
    int temp = 0;
    for(int i = 0;i<=(n/2)-1;i++){
        int pow = ((4*i*j)+(2*i))%(2*n);
        temp += (powMod(psi,pow,q)*poly[2*i+1]);
    }
    return temp;
}

void CT_FFT(int poly[],int n,int q,int psi,int ntt[]){
    
    for(int j=0;j<n/2;j++){
        int tempA=summation_A(n,poly,j,psi,q);
        int tempB=summation_B(n,poly,j,psi,q);
        int pow = (2*j + 1)%(2*n);
        int term = powMod(psi,pow,q);
        int temp1 = (tempA + (term * tempB))%q;
        ntt[j] = temp1;
        int temp2 = (tempA - (term * tempB));
        ntt[j+(n/2)] = (temp2%q)+q;
    }
}

void print_poly(int poly[],int degree){
    for(int i = 0;i<degree;i++){
        printf("%d",poly[i]);
        if(i>0) printf("x^%d",i);
        if(i!=degree-1)printf(" + ");
    }
    printf("\n");

}

int main(){
    int n=4,q=7681,psi=1925;
    int poly[] = {1,2,3,4};
    int *ntt = (int *)malloc(sizeof(int)*n);
    CT_FFT(poly,n,q,psi,ntt);
    print_poly(ntt,n);
    return 0;
}