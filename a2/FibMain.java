/**
Question 2 - Assignment 2 (CSI 3131)
Name: Ladan Guled
Student #: 300021664
*/

import java.util.*;

class Fibbonacci extends Thread {

    public void run(){ 
        FibMain.array[0] = 0;
        FibMain.array[1] = 1;

            for(int i = 2; i < FibMain.num; i++){
                FibMain.array[i] = FibMain.array[i-1] + FibMain.array[i-2]; 
            }
        }
    }


public class FibMain {

static int num;
static int array[] = new int[100];

public static void main(String args[]){
    System.out.println("Please enter the number of fibbonacci series length :");
    Scanner sc = new Scanner(System.in);
    String st = sc.nextLine();
    num = Integer.parseInt(st);
    
    Fibbonacci thread1 = new Fibbonacci();
    thread1.start();
    
    try{
        thread1.join();
    }

    catch(Exception e){

    }

    System.out.println("Fibbonacci series:");

    for(int i = 0; i < num; i++){
        System.out.println(array[i]);
    }

}

}

