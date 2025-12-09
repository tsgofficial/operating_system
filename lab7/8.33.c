// import java.util.concurrent.locks.Lock;
// import java.util.concurrent.locks.ReentrantLock;

// public class Transaction
// {

//     // Locks for account synchronization
//     private static final Lock checkingLock = new ReentrantLock();
//     private static final Lock savingsLock = new ReentrantLock();

//     // Transaction method to transfer money
//     public void transaction(Account from, Account to, double amount)
//     {
//         // Get the lock identifiers (hash codes) to ensure ordering
//         int fromLockId = System.identityHashCode(from);
//         int toLockId = System.identityHashCode(to);

//         // Acquire the locks in a consistent order based on the account hash codes
//         Lock lock1, lock2;
//         if (fromLockId < toLockId)
//         {
//             lock1 = getLock(from);
//             lock2 = getLock(to);
//         }
//         else
//         {
//             lock1 = getLock(to);
//             lock2 = getLock(from);
//         }

//         // Acquire the locks in the correct order to prevent deadlock
//         lock1.lock();
//         lock2.lock();

//         try
//         {
//             // Withdraw from the 'from' account
//             withdraw(from, amount);

//             // Deposit into the 'to' account
//             deposit(to, amount);
//         }
//         finally
//         {
//             // Release the locks after the transaction
//             lock2.unlock();
//             lock1.unlock();
//         }
//     }

//     // Utility method to get the lock for a given account
//     private Lock getLock(Account account)
//     {
//         if (account.getType().equals("checking"))
//         {
//             return checkingLock;
//         }
//         else
//         {
//             return savingsLock;
//         }
//     }

//     // Simulate withdrawing money from an account
//     private void withdraw(Account account, double amount)
//     {
//         account.setBalance(account.getBalance() - amount);
//         System.out.println("Withdrawn " + amount + " from " + account.getType() + " account");
//     }

//     // Simulate depositing money into an account
//     private void deposit(Account account, double amount)
//     {
//         account.setBalance(account.getBalance() + amount);
//         System.out.println("Deposited " + amount + " into " + account.getType() + " account");
//     }

//     public static void main(String[] args)
//     {
//         Account checking = new Account("checking", 1000);
//         Account savings = new Account("savings", 2000);

//         Transaction transaction = new Transaction();

//         // Simulate two transactions
//         new Thread(()->transaction.transaction(checking, savings, 25.0)).start();
//         new Thread(()->transaction.transaction(savings, checking, 50.0)).start();
//     }
// }

// // Account class to represent bank accounts
// class Account
// {
//     private String type;
//     private double balance;

//     public Account(String type, double balance)
//     {
//         this.type = type;
//         this.balance = balance;
//     }

//     public String getType()
//     {
//         return type;
//     }

//     public double getBalance()
//     {
//         return balance;
//     }

//     public void setBalance(double balance)
//     {
//         this.balance = balance;
//     }
// }
