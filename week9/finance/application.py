import os

from cs50 import SQL
from datetime import datetime
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd, fetch_owned

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

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    user_id = session.get("user_id")
    grand_total = 0
    balance = db.execute("SELECT cash FROM users WHERE id = ?", user_id)[0]["cash"]

    owned = fetch_owned(user_id)

    for row in owned:
        row["price"] = lookup(row["symbol"])["price"]
        row["name"] = lookup(row["symbol"])["name"]
        row["total"] = row["shares"] * row["price"]
        grand_total += row["total"]
    grand_total += balance
    return render_template("index.html", owned=owned, grand_total=grand_total, balance=balance)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "GET":
        return render_template("buy.html")
    else:
        try:
            buy_quantity = int(request.form.get("shares"))
        except ValueError:
            return apology("Please enter a positive integer.")
        if buy_quantity <= 0:
            return apology("Cannot buy negative shares.")

        # purchase the stock if user can afford it
        stock = lookup(request.form.get("symbol"))
        if stock == None:
            return apology("Stock not found.")
        date = datetime.now().strftime("%d/%m/%Y, %H:%M:%S")
        user_id = session.get("user_id")
        balance = db.execute("SELECT cash FROM users WHERE id = ?", user_id)[0]["cash"]
        credit = buy_quantity * stock["price"] * -1
        if balance + credit >= 0:
            # change user's balance
            new_balance = balance + credit
            db.execute("UPDATE users SET cash = ? WHERE id = ?", new_balance, user_id)
            # insert transaction data into transactions table
            db.execute("INSERT INTO transactions (users_id, symbol, shares, price, credit, time) VALUES (?, ?, ?, ?, ?, ?)",
                        user_id, stock["symbol"], buy_quantity, stock["price"], credit, date)
            return redirect("/")
        else:
            return apology("Insufficient funds.")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    user_id = session.get("user_id")
    # query db for required variables
    transactions = db.execute("SELECT * FROM transactions WHERE users_id = ? ORDER BY time ASC", user_id)
    for row in transactions:
        if row["credit"] > 0:
            row["shares"] *= -1
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

        # Query database for usernameBHN
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

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
    stock = {}
    """Get stock quote."""
    if request.method == "GET":
        return render_template("quote.html")
    else:
        stock = lookup(request.form.get("symbol"))
        if stock == None:
            return apology("Invalid stock symbol")
        return render_template("quote.html", stock=stock)


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "GET":
        return render_template("register.html")
    else:
        username = request.form.get("username").lower()
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")
        if not username:
            return apology("Missing username")
        if db.execute("SELECT * FROM users WHERE username IS ?", username):
            return apology("Username taken")
        if not password:
            return apology("Missing password")
        if password != confirmation:
            return apology("Passwords don't match")

        # hash the password
        pw_hash = generate_password_hash(password, method='pbkdf2:sha256', salt_length=8)
        # store the username and hashed pw in the users table in db
        db.execute("INSERT INTO users (username, hash) VALUES (?, ?)", username, pw_hash)
        # TODO: notify user of 'Registration success. Redirecting to login' perhaps using AJAX & JS alert? ------------------------------------------!!!!!!!
        return redirect("/")
        # NOTE - redirect to / will then redirect to login, because of the login_required function wrapping the / route


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    user_id = session.get("user_id")
    owned = fetch_owned(user_id)
    if request.method == "GET":
        return render_template("sell.html", owned=owned)
    else:
        date = datetime.now().strftime("%d/%m/%Y, %H:%M:%S")
        try:
            sell_quantity = int(request.form.get("shares"))
        except ValueError:
            return apology("Please enter a positive integer.")
        if sell_quantity <= 0:
            return apology("Cannot sell negative shares.")
        stock = lookup(request.form.get("symbol"))
        if stock == None:
            return apology("Stock not found.")
        balance = db.execute("SELECT cash FROM users WHERE id = ?", user_id)[0]["cash"]
        credit = sell_quantity * stock["price"]
        owned_quantity = 0
        for row in owned:
            if row["symbol"] == stock["symbol"]:
                owned_quantity = row["shares"]
        if owned_quantity >= sell_quantity:
            # change user's balance
            new_balance = balance + credit
            db.execute("UPDATE users SET cash = ? WHERE id = ?", new_balance, user_id)
            # insert transaction data into transactions table
            db.execute("INSERT INTO transactions (users_id, symbol, shares, price, credit, time) VALUES (?, ?, ?, ?, ?, ?)",
                      user_id, stock["symbol"], sell_quantity, stock["price"], credit, date)
            return redirect("/")
        else:
            return apology("Insufficient shares.")


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
