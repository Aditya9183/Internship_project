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

    printf("enter the degree of second polynomial: ");
    scanf("%d", &degb);
    int *B = (int *)malloc((degb + 1) * sizeof(int));

    printf("enter the coefficients from ascending order: ");
    for(int i = 0;i<=degb;i++){
        scanf("%d",&B[i]);
    }

    int *result = (int *)calloc(dega + degb + 1,sizeof(int));
    multiply_poly(A,B,dega,degb,result);

    print_poly(A,dega);
    print_poly(B,degb);
    print_poly(result,dega + degb);
    
    



    free(A);
    free(B);
    free(result);
    return 0;
}