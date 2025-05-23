#include<stdio.h>
#include<stdlib.h>

int ntt_prod(int ntta[],int nttb[],int nttc[],int n,int q){
    for(int i=0;i<n;i++){
        nttc[i] = (ntta[i]*nttb[i])%q;
    }
}
int inverse_find(int val,int q){
    for(int i=1;i<q;i++){
        if(((val%q)*(i%q))%q==1){
            return i;
        }
    }
}
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

int * psi_find(int n,int q,int omega){
    int * temp = (int *)malloc(sizeof(int)*5);
    int i =0;
    int psi = 1;
    while(psi<q){
        if((psi*psi)%q==omega%q){
            if(powMod(psi,n,q)==-1);
            temp[i]=psi;
            i++;
        }
        psi++;
    }
    return temp;
    
}
void Ntt_cal_psi(int poly[],int ntt[],int n,int psi,int q){
    for(int j=0;j<n;j++){
        ntt[j] = 0;
        for(int i = 0;i<n;i++){
            int pow = ((2*i*j)+i)%(2*n);
            int psi_val = powMod(psi,pow,q);
            int term = (poly[i]*psi_val)%q;
            ntt[j] = (ntt[j]+term)%q;
        }
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
void INtt_cal_psi(int ntt[],int intt[],int n,int psi,int q){
    int ni = inverse_find(n,q);
    int psi_i = inverse_find(psi,q);
    for(int j=0;j<n;j++){
        intt[j] = 0;
        for(int i = 0;i<n;i++){
            int pow = ((2*i*j)+j)%(2*n);
            int psi_val = powMod(psi_i,pow,q);
            int term = (ntt[i]*psi_val)%q;
            intt[j] = (intt[j]+term)%q;
        }
        intt[j] = (intt[j]*ni)%q;
    }
}

int main(){
    int q,n;
    
    printf("enter the value of modulus: ");
    scanf("%d",&q);
    printf("enter the degree of the polynomial : ");
    scanf("%d",&n);
    n+=1;
    int *polya = (int *)malloc(sizeof(int)*n);
    int *polyb = (int *)malloc(sizeof(int)*n);
    int *ntta_psi =  (int *)malloc(sizeof(int)*n);
    int *nttb_psi =  (int *)malloc(sizeof(int)*n);
    int *nttc_psi =  (int *)malloc(sizeof(int)*n);
    int *intt = (int *)malloc(sizeof(int)*n);
    int omega = omega_find(n,q);
    int *psi = psi_find(n,q,omega);
    printf("enter the coefficients of first polynomial in the ascending order: ");
    for(int i =0;i<n;i++){
        scanf("%d",&polya[i]);
    }
    printf("enter the coefficients of second polynomial in the ascending order: ");
    for(int i =0;i<n;i++){
        scanf("%d",&polyb[i]);
    }
    printf("the omega is %d\n",omega);
    for(int i =0;i<2;i++){
        printf("%d\n",psi[i]);
    }
    Ntt_cal_psi(polya,ntta_psi,n,psi[0],q);
    print_poly(ntta_psi,n);
    Ntt_cal_psi(polyb,nttb_psi,n,psi[0],q);
    print_poly(nttb_psi,n);
    ntt_prod(ntta_psi,nttb_psi,nttc_psi,n,q);
    INtt_cal_psi(nttc_psi,intt,n,psi[0],q);
    print_poly(intt,n);
    
    free(ntta_psi);
    free(polya);
    free(polyb);
    free(nttb_psi);
    free(nttc_psi);
    free(intt);
    free(psi);
    return 0;
}