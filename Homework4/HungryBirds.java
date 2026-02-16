package ID1217_Homework.Homework4;

public class HungryBirds {

    public static void main(String[] args) {
        System.out.println("Starting the Hungry Birds simulation...");
        int initialWorms = 0;
        int dishCapacity = 10;
        int numberOfBabyBirds = 3;
        Dish.BabyBird[] babyBirds = new Dish.BabyBird[numberOfBabyBirds];
        Dish dish = new Dish(initialWorms, dishCapacity);
        Dish.ParentBird parentBird = dish.new ParentBird(dish);
        for (int i = 0; i < babyBirds.length; i++) {
            babyBirds[i] = dish.new BabyBird(dish);
        }

        parentBird.start();
        for (Dish.BabyBird babyBird : babyBirds) {
            babyBird.start();
        }
    }
}

// monitor
class Dish {
    private int numberOfWorms;
    private int capacity;

    public Dish(int numberOfWorms, int capacity) {
        this.capacity = capacity;
        this.numberOfWorms = numberOfWorms;
    }

    public synchronized void eat() {
        while(numberOfWorms == 0){
            System.out.println("Baby bird waits for the dish to be filled.");
            try{
                wait();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
        numberOfWorms--;
        System.out.println("Baby bird ate a worm. Worms left: " + numberOfWorms);

        if(numberOfWorms == 0){
        
            System.out.println("\nBaby bird ate the last worm! Waking up the parent bird to fill the dish.\n");
            notifyAll();
        } 
    }    

    public synchronized void fill() {
        while(numberOfWorms > 0){
            try{
                wait();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
        //isBeingFilled = true;
        for(int i = 0; i < capacity; i++){
            numberOfWorms++;
            System.out.println("Parent bird adds a worm to the dish. Current number of worms: " + numberOfWorms);
            try {
                int oneSecond = 1000;
                Thread.sleep(oneSecond); // Simulate time taken to add a worm
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
        //isBeingFilled = false;
        System.out.println("Parent bird filled the dish. Parent bird goes back to sleep.\n");
        //notify baby birds that the dish is filled
        notifyAll();
    }

    class ParentBird extends Thread {

        Dish dish;

        public ParentBird(Dish dish) {
            this.dish = dish;
        }

        public void run() {
            while(true){
                dish.fill();
                try {
                    int oneSecond = 1000;
                    sleep(oneSecond); // Simulate time taken to fill the dish
                } catch (InterruptedException e) {
                    e.printStackTrace();
                } 
            }
        }
    }

    class BabyBird extends Thread {

        Dish dish;

        public BabyBird(Dish dish) {
            this.dish = dish;
        }

        public void run(){
            while(true){
                dish.eat();
                try {
                    int oneSecond = 1000;
                    sleep(oneSecond); // Simulate time taken to eat a worm
                } catch (InterruptedException e) {
                    e.printStackTrace();
                } 
            }
        }

        // public void run(){
        // while(true){
        // synchronized(d){
        // if(d.value == 1){
        // d.value = 0;
        // System.out.println("Baby bird eats the food.");
        // d.notifyAll();
        // }
        // }
        // }
        // }
    }
}