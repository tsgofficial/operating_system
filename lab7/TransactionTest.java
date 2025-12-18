public class TransactionTest {

    // The "gatekeeper" lock for hash collisions
    private static final Object tieLock = new Object();

    // Simple Account class nested for easy testing
    static class Account {
        private String name;
        private double balance;

        public Account(String name, double balance) {
            this.name = name;
            this.balance = balance;
        }

        public double getBalance() {
            return balance;
        }

        public String getName() {
            return name;
        }

        public void withdraw(double amount) {
            balance -= amount;
        }

        public void deposit(double amount) {
            balance += amount;
        }
    }

    public static void transaction(Account from, Account to, double amount) {
        int fromHash = System.identityHashCode(from);
        int toHash = System.identityHashCode(to);

        if (fromHash < toHash) {
            synchronized (from) {
                System.out.println(Thread.currentThread().getName() + " locked " + from.getName());
                synchronized (to) {
                    System.out.println(Thread.currentThread().getName() + " locked " + to.getName());
                    performTransfer(from, to, amount);
                }
            }
        } else if (fromHash > toHash) {
            synchronized (to) {
                System.out.println(Thread.currentThread().getName() + " locked " + to.getName());
                synchronized (from) {
                    System.out.println(Thread.currentThread().getName() + " locked " + from.getName());
                    performTransfer(from, to, amount);
                }
            }
        } else {
            // Tie-breaker for hash collisions
            synchronized (tieLock) {
                synchronized (from) {
                    synchronized (to) {
                        performTransfer(from, to, amount);
                    }
                }
            }
        }
    }

    private static void performTransfer(Account from, Account to, double amount) {
        if (from.getBalance() >= amount) {
            from.withdraw(amount);
            to.deposit(amount);
            System.out.println(">>> SUCCESS: " + amount + " moved from " + from.getName() + " to " + to.getName());
        }
    }

    public static void main(String[] args) {
        Account accA = new Account("Account A", 1000.0);
        Account accB = new Account("Account B", 1000.0);

        // Thread 1: Wants to move money A -> B
        Thread t1 = new Thread(() -> {
            for (int i = 0; i < 5; i++) {
                transaction(accA, accB, 10.0);
            }
        }, "Thread-1");

        // Thread 2: Wants to move money B -> A (The opposite direction!)
        Thread t2 = new Thread(() -> {
            for (int i = 0; i < 5; i++) {
                transaction(accB, accA, 10.0);
            }
        }, "Thread-2");

        t1.start();
        t2.start();
    }
}