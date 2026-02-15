package ID1217_Homework.Homework4;

public class HungryBirds {
    
    public static void main(String[] args){

    }
}
//monitor
class Dish{
    private int numberOfWorms;
    private int capacity;
    Boolean notified = false;
   
    public Dish(int numberOfWorms, int capacity){
        this.capacity = capacity;
        this.numberOfWorms = numberOfWorms;
    } 

    public synchronized void eat(){

        if(numberOfWorms > 0){
            numberOfWorms--;
            System.out.println("Baby bird eats a worm.");
            if(numberOfWorms == 0){
                System.out.println("Baby bird ate the last worm!");

                //TODO: notify parent bird to fill the dish
                if(notified){
                    System.out.println("Parent bird is already notified to fill the dish.");
                } else {
                    System.out.println("Baby bird notifies the parent bird to fill the dish.");
                    notified = true;
                    notify();
                }
                notifyAll();
            }  
        }
        else{
            System.out.println("Baby bird waits for the dish to be filled.");
            wait();
        }
    }

    public synchronized void fill(){
        while(!isEmpty()){
            System.out.println("Parent bird waits for the dish to be empty");
            wait();
        }
    }

class ParentBird extends Thread{
    public void run(){
        while(true){

        }
    }

    Dish d;
    public ParentBird(Dish d){
        this.d = d;
    }

    public void run(){
        
    }

    // public void run(){
    //     while(true){
    //         synchronized(d){
    //             if(d.value == 0){
    //                 d.value = 1;
    //                 System.out.println("Parent bird fills the dish.");
    //                 d.notifyAll();
    //             }
    //         }
    //     }
    // }
}

class BabyBird extends Thread{
    public void run(){
        
    }
    Dish d;
    public BabyBird(Dish d){
        this.d = d;
    }

    // public void run(){
    //     while(true){
    //         synchronized(d){
    //             if(d.value == 1){
    //                 d.value = 0;
    //                 System.out.println("Baby bird eats the food.");
    //                 d.notifyAll();
    //             }
    //         }
    //     }
    // }
}