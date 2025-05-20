#include <stdio.h>
#include <stdlib.h>

void multiply_poly(int a[],int b[],int dega,int degb,int result[]){
    for(int i = 0;i<=dega;i++){
        for(int j = 0;j<=degb;j++){
            result[i+j] += a[i] * b[j];
        }
    }
}

void print_poly(int poly[],int degree){
    for(int i = 0;i<=degree;i++){
        printf("%d",poly[i]);
        if(i>0) printf("x^%d",i);
        if(i!=degree)printf(" + ");
    }
    printf("\n");

}

int main(){
    int dega,degb;
    printf("enter the degree of first polynomial: ");
    scanf("%d", &dega);
    int *A = (int *)malloc((dega + 1) * sizeof(int));

    printf("enter the coefficients from ascending order: ");
    for(int i = 0;i<=dega;i++){
        scanf("%d",&A[i]);
    }
    print_poly(A,dega);



    free(A);
    return 0;
}