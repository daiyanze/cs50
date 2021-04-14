import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Ensure responses aren't cached


@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Create table Transactions
# Transactions
# id INTEGER PRIMARY KEY AUTOINCREMENT
# symbol TEXT NOT NULL
# shares INTEGER NOT NULL
# price REAL NOT NULL
# transacted TEXT NOT NULL
# user_id INTEGER FOREIGN KEY
db.execute('''
CREATE TABLE IF NOT EXISTS transactions (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    name TEXT NOT NULL,
    symbol TEXT NOT NULL,
    shares INTEGER NOT NULL,
    price REAL NOT NULL,
    transacted DATETIME DEFAULT (strftime('%Y-%m-%d %H:%M:%S', 'now')),
    user_id INTEGER NOT NULL,
    FOREIGN KEY (user_id) REFERENCES users(id)
)''')

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    # get all user's stocks based on
    transactions = db.execute('''
        SELECT name, symbol, sum(shares) as shares, price, sum(shares * price) AS total
        FROM transactions
        WHERE user_id = (?)
        GROUP BY symbol
        HAVING sum(shares) > 0
        ''', session['user_id'])
    cash = db.execute("SELECT cash FROM users WHERE id = (?)", session['user_id'])[0]['cash']
    stocks = []
    total = 0
    for idx, stock in enumerate(transactions):
        quote_result = lookup(stock['symbol'])
        stocks.append({
            'symbol': quote_result['symbol'],
            'name': quote_result['name'],
            'price': quote_result['price'],
            'shares': stock['shares'],
            'total': round(stock['shares'] * quote_result['price'], 2)
        })
        total += stocks[idx]['total']

    return render_template("index.html", stocks=stocks, cash=cash, total=total+cash, bought=False)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":
        symbol = request.form.get("symbol").strip()
        shares = request.form.get("shares").strip()

        # return 400 on user input empty
        if not symbol:
            return apology("must provide symbol", 400)
        if not shares:
            return apology("must provide shares", 400)
            
        if not shares.isdecimal():
            return apology("shares must be integer", 400)
            
        shares = int(shares)

        # return 400 on symbol not exists
        quote_result = lookup(symbol)
        if not quote_result:
            return apology("invalid symbol", 400)

        expense = quote_result['price'] * shares

        cash = db.execute("SELECT cash FROM users WHERE id = (?)", session['user_id'])[0]['cash']

        left = cash - expense

        # return 400 on cash not enough
        if left < 0:
            return apology("can't afford", 400)

        # update user cash
        db.execute("UPDATE users SET cash = (?) WHERE id = (?)",
                   left, session['user_id'])

        # Insert record to db
        db.execute("INSERT INTO transactions (name, symbol, shares, price, user_id) VALUES (?, ?, ?, ?, ?)",
                   quote_result['name'],
                   symbol,
                   shares,
                   quote_result['price'],
                   session['user_id'])
        flash('Bought!')
        return redirect('/')
    else:
        return render_template("buy.html")


@app.route("/change_password", methods=["GET", "POST"])
@login_required
def change_password():
    """Change account password"""
    if request.method == 'POST':
        current_password = request.form.get('current_password').strip()

        password_hash = db.execute('SELECT hash FROM users WHERE id = (?)', session['user_id'])[0]['hash']

        if not check_password_hash(password_hash, current_password):
            return apology("password is incorrect", 400)

        new_password = request.form.get('new_password').strip()
        confirm_password = request.form.get('confirm_password').strip()

        if new_password != confirm_password:
            return apology("passwords don't match", 400)

        if confirm_password == current_password:
            return apology("must provide a different password", 400)

        db.execute('UPDATE users SET hash = (?) WHERE id = (?)',
                   generate_password_hash(confirm_password), session['user_id'])

        flash('Password updated')
        return redirect('/')
    else:
        return render_template('change_password.html')


# Personal touch: allow users to add additional cash to their account
@app.route("/deposit", methods=["GET", "POST"])
@login_required
def deposit():
    """Add cash to account"""
    if request.method == 'POST':
        amount = request.form.get('amount').strip()

        if not amount:
            return apology("must provide deposit amount", 400)

        amount = float(amount)

        if amount < 500:
            return apology("must deposit more than %500", 400)

        cash = db.execute('SELECT cash FROM users WHERE id = (?)', session['user_id'])[0]['cash']

        db.execute('UPDATE users SET cash = (?) WHERE id = (?)', amount + cash, session['user_id'])

        flash(f'Deposit {usd(amount)}')
        return redirect('/')
    else:
        return render_template("deposit.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    transactions = db.execute(
        "SELECT * FROM transactions WHERE user_id = (?)", session['user_id'])

    return render_template("history.html", transactions=transactions)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?",
                          request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "POST":
        symbol = request.form.get("symbol")

        if not symbol:
            return apology("missing symbol", 400)

        quote_result = lookup(symbol.strip())

        if not quote_result:
            return apology("invalid symbol", 400)

        return render_template("quoted.html", quote=quote_result)
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "POST":
        username = request.form.get("username").strip()
        password = request.form.get("password").strip()
        confirmation = request.form.get("confirmation").strip()

        if not username:
            return apology("must provide username", 400)
        elif not password:
            return apology("must provide password", 400)
        elif not confirmation:
            return apology("must enter password again", 400)
        elif not password == confirmation:
            return apology("password does not match", 400)

        rows = db.execute("SELECT id FROM users WHERE username = (?)", username)

        if len(rows) > 0:
            return apology("username already exists", 400)

        db.execute("INSERT INTO users (username, hash) VALUES (?, ?)",
                   username,
                   generate_password_hash(password))

        return redirect("/")
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    if request.method == "POST":
        symbol = request.form.get("symbol").strip()
        shares = request.form.get("shares").strip()

        if not symbol:
            return apology("must provide symbol", 400)
        if not shares:
            return apology("must provide shares", 400)

        shares = int(shares)

        stock = db.execute('''
            SELECT sum(shares) as shares
            FROM transactions
            WHERE symbol = (?) AND user_id = (?)
            GROUP BY symbol
            ''', symbol, session['user_id'])[0]

        if not stock:
            return apology("don't own that stock", 400)

        if stock['shares'] < shares:
            return apology("too many shares", 400)

        quote_result = lookup(symbol)

        cash = db.execute("SELECT cash FROM users WHERE id = (?)",
                          session['user_id'])[0]['cash']

        sold_value = quote_result['price'] * shares

        # Update cash
        db.execute("UPDATE users SET cash = (?) WHERE id = (?)",
                   cash + sold_value, session['user_id'])

        # Insert record
        db.execute("INSERT INTO transactions (name, symbol, shares, price, user_id) VALUES (?, ?, ?, ?, ?)",
                   quote_result['name'],
                   symbol,
                   -shares,
                   quote_result['price'],
                   session['user_id'])

        return redirect('/')
    else:
        symbols = db.execute("SELECT DISTINCT(symbol) FROM transactions")
        return render_template("sell.html", symbols=symbols)


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
