/**
Question 1 - Assignment 2 (CSI 3131)
Name: Ladan Guled
Student #: 300021664
 */

class Prime extends Thread {

    int n;

    Prime(int n){
        this.n=n;
        start();
    }

    public void run(){
        int i,j;
        System.out.println("\nPrimes series: \n");
            for(i = 1; i <= n ; i++){
                for(j = 2; j < i ;j++){
                    if(i % j == 0){
                        break;
                    }
                }
                if(i == j){
                    System.out.print(i + " ");
                }
            }
    System.out.println("\n");
    }
}

class PrimeMain {
    
    public static void main(String args[]){
        int n = Integer.parseInt(args[0]);
        Prime p = new Prime(n);
    }

}

