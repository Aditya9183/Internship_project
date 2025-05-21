#include<stdio.h>
#include<stdlib.h>
int powMod (int base, int pow , int mod){
    int temp = 1;
    for(int i = 0;i<pow;i++){
        temp = (temp * base) % mod;
    }
    return temp;
}

int ntt_prod(int ntta[],int nttb[],int nttc[],int n,int q){
    for(int i=0;i<n;i++){
        nttc[i] = (ntta[i]*nttb[i])%q;
    }
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

int omega_find(int n,int q){
    if((q-1)%n != 0){
        return 0;
    }
    for(int omega = 2;omega<q;omega++){
        if(primitive_check(omega,n,q)){
            return omega;
        }

    }
    return 0;

}
void print_poly(int poly[],int degree){
    for(int i = 0;i<degree;i++){
        printf("%d",poly[i]);
        if(i>0) printf("x^%d",i);
        if(i!=degree-1)printf(" + ");
    }
    printf("\n");

}
void Ntt_cal(int poly[],int ntt[],int n,int omega,int q){
    for(int j=0;j<n;j++){
        ntt[j] = 0;
        for(int i = 0;i<n;i++){
            int pow = (i*j)%n;
            int omega_val = powMod(omega,pow,q);
            int term = (poly[i]*omega_val)%q;
            ntt[j] = (ntt[j]+term)%q;
        }
    }
}
int inverse_find(int val,int q){
    for(int i=1;i<q;i++){
        if(((val%q)*(i%q))%q==1){
            return i;
        }
    }

}
void INtt_cal(int ntt[],int intt[],int n,int omega,int q){
    int ni = inverse_find(n,q);
    int omegai = inverse_find(omega,q);
    for(int j=0;j<n;j++){
        intt[j] = 0;
        for(int i = 0;i<n;i++){
            int pow = (i*j)%n;
            int omega_val = powMod(omegai,pow,q);
            int term = (ntt[i]*omega_val)%q;
            intt[j] = (intt[j]+term)%q;
        }
        intt[j] = (intt[j]*ni)%q;
    }
}


int main(){
    int n,q;
     
    printf("enter the degree of the polynomial: ");
    scanf("%d",&n);
    n+=1;
    int *polya = (int *)malloc(sizeof(int)*n);
    int *polyb = (int *)malloc(sizeof(int)*n);
    int *ntta = (int *)malloc(sizeof(int)*n);
    int *nttb = (int *)malloc(sizeof(int)*n);
    int *nttc = (int *)malloc(sizeof(int)*n);
    int *intt = (int *)malloc(sizeof(int)*n);
    printf("enter the coefficients of first polynomial in the ascending order: ");
    for(int i =0;i<n;i++){
        scanf("%d",&polya[i]);
    }

    printf("enter the coefficients of second polynomial in the ascending order: ");
    for(int i =0;i<n;i++){
        scanf("%d",&polyb[i]);
    }
    printf("enter the value of the modulus: ");
    scanf("%d",&q);
    
    if(omega_find(n,q)){
        int ans = omega_find(n,q);
        printf("the omega is found to be %d \n",ans);
        Ntt_cal(polya,ntta,n,ans,q);
        Ntt_cal(polyb,nttb,n,ans,q);
        ntt_prod(ntta,nttb,nttc,n,q);
        INtt_cal(nttc,intt,n,ans,q);
        printf("the ntt matrix is....\n");
        print_poly(ntta,n);
        print_poly(nttb,n);
        print_poly(nttc,n);
        printf("the intt matrix is....\n");
        print_poly(intt,n);
        

        
    }


    return 0;
}