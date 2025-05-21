#include<stdio.h>

int powMod (int base, int pow , int mod){
    int temp = 1;
    for(int i = 0;i<pow;i++){
        temp = (temp * base) % mod;
    }
    return temp;
}

int primitive_check (int omega , int n , int q){
    if(powMod(omega,n,q)!=1){
        return 0;
    }

    for(int j = 1;j<n;j++){
        if(powMod(omega,j,q)==1){
            return 0;
        }
    }
    return 1;
}

int main(){
    int n,q;

    printf("enter the degree of polynomial:");
    scanf("%d",&n);
    n +=1;
    printf("enter the modulus:");
    scanf("%d",&q);

    if((q-1)%n != 0){
        printf("nth root of unity does not exist\n");
        return 0;
    }
    for(int omega = 2;omega<q;omega++){
        if(primitive_check(omega,n,q)){
            printf("the primitive nth root is %d\n",omega);
            return 0;
        }

    }
    printf("no primitve root found\n");
    return 0;
}