public class Account {
    private double balance;
    private int id;

    public Account(int id, double initialBalance) {
        this.id = id;
        this.balance = initialBalance;
    }

    // Methods to interact with the object
    public double getBalance() {
        return balance;
    }

    public void withdraw(double amount) {
        balance -= amount;
    }

    public void deposit(double amount) {
        balance += amount;
    }

    public void setBalance(double balance) {
        this.balance = balance;
    }

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }
}