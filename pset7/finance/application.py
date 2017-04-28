from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session, url_for
from flask_session import Session
from passlib.apps import custom_app_context as pwd_context
from tempfile import gettempdir

from helpers import *

# configure application
app = Flask(__name__)

# ensure responses aren't cached
if app.config["DEBUG"]:
    @app.after_request
    def after_request(response):
        response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
        response.headers["Expires"] = 0
        response.headers["Pragma"] = "no-cache"
        return response

# custom filter
app.jinja_env.filters["usd"] = usd

# configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = gettempdir()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

@app.route("/")
@login_required
def index():
    portRows = db.execute("SELECT * FROM portfolio WHERE user = :username", username=session["user_id"])
    rows = db.execute("SELECT * FROM users WHERE username = :username", username=session["user_id"])
    stockValue = 0
    for row in portRows:
        print(row)
        tempStock = lookup(row['ticker'])
        row['price'] = usd(tempStock['price'])
        row['total'] = usd(row['shares'] * tempStock['price'])
        stockValue += row['shares'] * tempStock['price']
    return render_template("index.html", portfolio=portRows, cash=usd(rows[0]["cash"]), shareWorth=usd(stockValue), netWorth=usd(rows[0]["cash"] + stockValue))

@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    if request.method == "GET":
        rows = db.execute("SELECT * FROM users WHERE username = :username", username=session["user_id"])
        return render_template("buy.html", name=session["user_id"], cash=usd(rows[0]["cash"]))
    
    if request.method == "POST":
        
        # (1) Get stock info
        ticker = request.form.get("ticker")
        stockObj = lookup(ticker)
        cost = float(request.form.get("numShares")) * stockObj['price']
        print("Cost is: ")
        print(cost)
        
        # (2) check if sufficient funds
        print("user is: " + session["user_id"])
        
        rows = db.execute("SELECT * FROM users WHERE username = :username", username=session["user_id"])
        print("Cash is: ")
        print(rows[0]["cash"])
        if (cost >= rows[0]['cash']):
            return apology("You don't have enough cash to buy that many shares, ya dingus. Press buttons more carefully next time.")
            print("Can't do it.")
        else:
            print("Plenty of cash boss.")
        
        # (3) If so, record the transaction to the transactions table
        db.execute("INSERT INTO transactions (user, ticker, shares, price, total) VALUES(:username, :ticker, :shares, :price, :total)", username=session["user_id"], ticker=stockObj['symbol'], shares=float(request.form.get("numShares")), price=usd(stockObj['price']), total=usd((0-float(request.form.get("numShares"))*stockObj['price'])) )
        
        # (4) Then deduct cash from the users table
        db.execute("UPDATE users SET cash = cash - :theCost WHERE username = :username", username=session["user_id"], theCost=cost)
        
        # (5) Check if shares already owned
        portRows = db.execute("SELECT * FROM portfolio WHERE user = :username AND ticker = :ticker", username=session["user_id"], ticker=stockObj['symbol'])
        if len(portRows) == 0:
            db.execute("INSERT INTO portfolio (user, ticker, shares) VALUES(:username, :ticker, :shares)", username=session["user_id"], ticker=stockObj['symbol'], shares=float(request.form.get("numShares")))
        else:
            newShares = portRows[0]["shares"] + float(request.form.get("numShares"))
            db.execute("UPDATE portfolio SET shares = :newshares WHERE user = :username AND ticker = :ticker", username=session["user_id"], ticker=stockObj['symbol'], newshares=newShares)

        
        # (6) Add new shares if not owned, or udpate shares if already owned

        return redirect(url_for("index"))

@app.route("/history")
@login_required
def history():
    transRows = db.execute("SELECT * FROM transactions WHERE user = :username", username=session["user_id"])
    rows = db.execute("SELECT * FROM users WHERE username = :username", username=session["user_id"])
    return render_template("history.html", transactions=transRows, cash=usd(rows[0]["cash"]))
    
    
@app.route("/addcash", methods=["GET", "POST"])
@login_required
def addcash():
    if request.method == "GET":
        rows = db.execute("SELECT * FROM users WHERE username = :username", username=session["user_id"])
        return render_template("addcash.html", cash=usd(rows[0]["cash"]))
    if request.method == "POST":
        newCash = float(request.form.get("cashtoadd"))
        db.execute("UPDATE users SET cash = cash + :theCost WHERE username = :username", username=session["user_id"], theCost=newCash)
        return redirect(url_for("index"))
        

@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in."""

    # forget any user_id
    session.clear()

    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username")

        # ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password")

        # query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username", username=request.form.get("username"))

        # ensure username exists and password is correct
        # if (len(rows) != 1) or (request.form.get("password") != rows[0]["hash"]):
        if len(rows) != 1 or not pwd_context.verify(request.form.get("password"), rows[0]["hash"]):
            return apology("invalid username and or password")

        # remember which user has logged in
        session["user_id"] = rows[0]["username"]

        # redirect user to home page
        return redirect(url_for("index"))

    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")

@app.route("/logout")
def logout():
    """Log user out."""

    # forget any user_id
    session.clear()

    # redirect user to login form
    return redirect(url_for("login"))

@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    if request.method == "GET":
        """Get stock quote."""
        return render_template("quote.html")
    if request.method == "POST":
        ticker = request.form.get("ticker")
        stockObj = lookup(ticker)
        print("This is the stock object:")
        print(stockObj)
        print("Symbol: " + stockObj['symbol'])
        return render_template("quoted.html", name=stockObj['name'], symbol=stockObj['symbol'], price = str(stockObj['price']))
        

@app.route("/register", methods=["GET", "POST"])
def register():
    
    # forget any user_id
    session.clear()
    
    """Register user."""
    if request.method == "POST":
        # register the user
        # ensure username was submitted, password and verify were submitted, and password and verify match
        if not request.form.get("username"):
            return apology("must provide username")
        if (not request.form.get("password") or not request.form.get("confirmPassword")):
            return apology("must provide and confirm password")
        if (request.form.get("password") != request.form.get("confirmPassword")):
            return apology("passwords must match ya dingus")
        
        # pwd_context.encrypt
        
        result = db.execute("INSERT INTO users (username, hash) VALUES(:username, :hash)", username=request.form.get("username"),hash=pwd_context.encrypt(request.form.get("password")))
        
        # if result was a failure...
        
        # else:
        session["user_id"] = request.form.get("username")
        return redirect(url_for("index"))
        

        
        # session["user_id"]
        
        
            
            
            
    else:
        return render_template("register.html")

@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    if request.method == "GET":
        rows = db.execute("SELECT * FROM users WHERE username = :username", username=session["user_id"])
        return render_template("sell.html", name=session["user_id"], cash=usd(rows[0]["cash"]))
    
    if request.method == "POST":
        
        # (1) Get stock info
        ticker = request.form.get("ticker")
        stockObj = lookup(ticker)
        cost = float(request.form.get("numShares")) * stockObj['price']
        print("Cost is: ")
        print(cost)
        
        # (2) check if there are sufficient shares to sell 
        print("user is: " + session["user_id"])
        
        portRows = db.execute("SELECT * FROM portfolio WHERE user = :username AND ticker = :ticker", username=session["user_id"], ticker=stockObj['symbol'])
        if len(portRows) == 0:
            return apology("You don't own that stock, and can't sell it.")
        if (portRows[0]["shares"] < float(request.form.get("numShares"))):
            return apology("You can't sell more shares than you own, ya dingus")
            
        # (3) If so, record the transaction to the transactions table
        db.execute("INSERT INTO transactions (user, ticker, shares, price, total) VALUES(:username, :ticker, :shares, :price, :total)", username=session["user_id"], ticker=stockObj['symbol'], shares=(0-float(request.form.get("numShares"))), price=usd(stockObj['price']), total=usd((0-(0-float(request.form.get("numShares")))*stockObj['price'])) )
            
        # (4) Then add cash from the users table
        db.execute("UPDATE users SET cash = cash + :theCost WHERE username = :username", username=session["user_id"], theCost=cost)
        
        # (5) then remove shares from the portfolio table
        newShares = portRows[0]["shares"] - float(request.form.get("numShares"))
        if newShares > 0:
            db.execute("UPDATE portfolio SET shares = :newshares WHERE user = :username AND ticker = :ticker", username=session["user_id"], ticker=stockObj['symbol'], newshares=newShares)
        else:
            db.execute("DELETE FROM portfolio WHERE user = :username AND ticker = :ticker", username=session["user_id"], ticker=stockObj['symbol'])
            
        # (6) If we now have 0 shares, remove the entry
        

        return redirect(url_for("index"))
