### Detailed Specification for Portfolio Performance Tracker

**Project Overview:**
The Portfolio Performance Tracker is a beginner-level project designed to help users track the performance of their stock portfolio. It will allow users to input their stock purchases and track their portfolio’s value over time. The tracker will provide information like current portfolio value, gains/losses, and the performance of individual stocks. Additionally, the project will allow for a smooth transition to intermediate-level projects by introducing more advanced concepts, such as real-time stock data fetching from an API, user authentication, and basic data analysis.

---

### **Functional Requirements:**

#### **1. User Authentication**
- **Description**: Allow users to create an account, log in, and manage their portfolio securely.
- **Features**:
    - User Registration: Store user details (username, email, password).
    - User Login: Verify credentials and allow access to their portfolio.
    - Password Recovery: Provide a way to reset passwords via email.
- **Technology**:
    - Use a file-based system (like storing user details in a text file) for simplicity or SQLite for intermediate-level learning.
    - Implement password hashing (e.g., SHA-256 or bcrypt) to store passwords securely.
    - Use a basic text-based menu for interaction.

#### **2. Stock Portfolio Management**
- **Description**: Allow the user to add, update, and remove stocks from their portfolio.
- **Features**:
    - Add Stock: The user can add stocks to their portfolio by specifying the stock symbol (e.g., AAPL for Apple) and the number of shares purchased.
    - Remove Stock: The user can remove a stock from their portfolio.
    - Update Stock: Allow users to update the number of shares of a stock they own.
    - Display Portfolio: Show a summary of the portfolio, including stock symbols, number of shares owned, purchase price, and the current value of the stock.
    - Track Performance: Display the gains/losses for each stock in the portfolio.
  
#### **3. Real-Time Stock Data (Optional)**
- **Description**: Fetch real-time stock data from an API and display the current price of stocks in the portfolio.
- **Features**:
    - Fetch live stock prices from a free API such as [Alpha Vantage](https://www.alphavantage.co) or [Yahoo Finance API](https://www.yahoofinanceapi.com).
    - Update portfolio with real-time stock prices and calculate the portfolio’s current value.
    - Display the change in stock prices (e.g., daily or overall performance).

#### **4. Portfolio Performance**
- **Description**: Display overall portfolio performance, including gains, losses, and total value.
- **Features**:
    - Calculate and display the current value of the portfolio based on real-time stock prices.
    - Calculate total gains/losses for the portfolio.
    - Display performance over time (optional for future features).

#### **5. Data Persistence**
- **Description**: Store the user’s stock portfolio data between program runs.
- **Features**:
    - Use a text file or a simple database (like SQLite) to persist the portfolio data.
    - Allow users to save and load their portfolio data when the application starts and exits.

---

### **Non-Functional Requirements:**
- **Performance**: The program should be responsive, with quick access to the user’s portfolio and stock data.
- **Usability**: The program should have a simple text-based interface that is easy to use.
- **Scalability**: While not a priority for this beginner-level project, it should be structured in a way that can easily be extended to handle larger datasets or integrate more features.
- **Security**: For the optional authentication system, user passwords should be securely hashed. Data should be stored safely.

---

### **Technology Stack:**
- **Programming Language**: C++ (for learning purposes, with standard libraries)
- **Storage**: Simple file-based storage (CSV or plain text) or SQLite for storing portfolio data.
- **API**: Alpha Vantage or Yahoo Finance API (for real-time stock data fetching).
- **Optional Libraries**: 
    - cURL or another HTTP library for API requests.
    - SQLite3 library for database management (if using a database).

---

### **Sample Run:**

```bash
Welcome to the Portfolio Performance Tracker!

Select an option:
1. Register
2. Login

Enter your choice: 1

Enter your username: john_doe
Enter your email: john@example.com
Enter your password: p@ssWoRD
Account created successfully!

Welcome back, john_doe!

Select an option:
1. Buy Stock
2. Sell Stock
3. View Stock (fetch real-time data)
4. View Portfolio Performance
5. Exit

Enter your choice: 1

Enter the stock symbol: AAPL
Enter the number of shares: 10

Stock: APPL
Open: 149.8900
High: 151.3400
Low: 147.9000
Price: 150.3100
Volume: 1752886
Latest Trading Day: 2024-12-27
Previous Close: 152.0300
Change: -1.7200
Change Percent: -0.473591%

Are you sure you want place the order ? (Y or n): Y

Success! Bought 10 shares of APPL at booking price: $1503.10

========================================================
Portfolio:
Stock       No. of shares        Booking Price
APPL            10                   1503.10
========================================================

# if market is closed then prompt the user to come later.
# if the user enters No, then display appropriate message
    # and display their current portfolio.

Select an option:
1. Buy Stock
2. Sell Stock
3. Fetch Real-Time Data
4. View Portfolio Performance
5. Exit

Enter your choice: 3

========================================================
Portfolio:
Stock       No. of shares        Booking Price
APPL            10                   1503.10
GOOGL           12                   983.93
========================================================

Is there any specific stock you'd like to see data about? (Y or n): Y
Enter stock: APPL

Fetching real-time data for APPL...

Stock: APPL
Open: 149.8900
High: 151.3400
Low: 147.9000
Price: 150.3100
Volume: 1752886
Latest Trading Day: 2024-12-27
Previous Close: 152.0300
Change: -1.7200
Change Percent: -0.473591%

# OR

Is there any specific stock you'd like to see data about? (Y or n): N

Fetching real-time data for portfolio...
Stock: APPL
Open: 149.8900
High: 151.3400
Low: 147.9000
Price: 150.3100
Volume: 1752886
Latest Trading Day: 2024-12-27
Previous Close: 152.0300
Change: -1.7200
Change Percent: -0.473591%

Stock: GOOGL
Open: 82.0900
High: 82.8400
Low: 81.0100
Price: 81.9933
Volume: 1284823
Latest Trading Day: 2024-12-27
Previous Close: 80.8321
Change: +1.1612
Change Percent: +1.43656%


Select an option:
1. Buy Stock
2. Sell Stock
3. Fetch Real-Time Data
4. View Portfolio Performance
5. Exit

Enter your choice: 4

========================================================
Portfolio:
Stock       No. of shares        Booking Price      Current Price       Profit/Loss
APPL            10                   1503.10            1505.20         +1.2 (+0.1397%)
GOOGL           12                   983.93             980.12          -3.81 (-0.3872%)
========================================================


Total Portfolio Value: $2485.32
Total Profit/Loss: -$1.71

Select an option:
1. Buy Stock
2. Sell Stock
3. Fetch Real-Time Data
4. View Portfolio Performance
5. Exit

Enter your choice: 5

Goodbye, john_doe!
```

---

### **Future Enhancements:**
- **Data Visualization**: Add charts or graphs to visualize portfolio performance over time (using a library like `matplotlib` in Python or `Qt` in C++).
- **Stock Analysis**: Implement features such as historical data analysis or compare performance with market indices (S&P 500).
- **Portfolio Optimization**: Introduce basic concepts of portfolio optimization, such as adjusting the weight of stocks based on performance or risk.
- **Automated Trading**: For more advanced learners, explore integrating with a real stock trading API (e.g., Alpaca or Robinhood) to simulate or even execute trades programmatically.

This project will help you build a solid foundation in C++, with a clear path to more complex projects as you expand the system's features.