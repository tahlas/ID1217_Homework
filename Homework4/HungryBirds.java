package ID1217_Homework.Homework4;

public class HungryBirds {
    /*
        The solution is not entirely fair because it is not guaranteed that all
        threads will get a chance to run. Although, because there are only a few 
        threads and each thread sleeps after performing its action, it is likely
        that all threads will get a chance to run. 
        
        However, in a more complex 
        system with many threads, it is possible that some threads may be starved 
        and never get a chance to run.
    */
    public static void main(String[] args) {
        System.out.println("Starting the Hungry Birds simulation...");
        int initialWorms = 0;
        int dishCapacity = 10;
        int numberOfBabyBirds = 3;
        Dish.BabyBird[] babyBirds = new Dish.BabyBird[numberOfBabyBirds];
        Dish dish = new Dish(initialWorms, dishCapacity);
        Dish.ParentBird parentBird = dish.new ParentBird(dish);
        for (int i = 0; i < babyBirds.length; i++) {
            babyBirds[i] = dish.new BabyBird(dish, i);
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

    public synchronized void eat(int id) {
        while(numberOfWorms == 0){
            System.out.println("Baby birds wait for the dish to be filled.");
            try{
                wait();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
        numberOfWorms--;
        System.out.println("Baby bird " + id +" ate a worm. Worms left: " + numberOfWorms);

        if(numberOfWorms == 0){
        
            System.out.println("\nBaby bird " + id + " ate the last worm! Waking up the parent bird to fill the dish.\n");
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
        System.out.println("Parent bird filled the dish. Parent bird goes back to sleep.\n");
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
        int id;

        public BabyBird(Dish dish, int id) {
            this.dish = dish;
            this.id = id;
        }

        public void run(){
            while(true){
                dish.eat(id);    
                try {
                    int oneSecond = 1000;
                    sleep(oneSecond); // Simulate time taken to eat a worm
                } catch (InterruptedException e) {
                    e.printStackTrace();
                } 
            }
        }
    }
}