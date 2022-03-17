import java.*;
import java.util.*;

class fact { 

    int factorial(int a) {
        int b=1,i;
    
        for(i=1;i<=a;i++){
            b=b*i;
        }
    
        return b;

    }
}

public class calatan {

    public static void main(String args[]){
        Scanner ss = new Scanner(System.in);  
        int n,y,x,p1,p2,p3,z;
        System.out.println("Please enter the number");
        n = ss.nextInt();
        fact obj=new fact();

        for(y = 0; y <= n; y++){
            p1=obj.factorial(y+1);
            p2=obj.factorial(y);
            p3=obj.factorial(2*y);
            z=p1*p2;
            x=p3/z;
            System.out.println(x);
        }
    }
}