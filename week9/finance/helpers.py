import os
import requests
import urllib.parse

from flask import redirect, render_template, request, session
from functools import wraps
from cs50 import SQL


def fetch_owned(user_id):
    db = SQL("sqlite:///finance.db")
    bought = db.execute(
        "SELECT symbol, SUM(shares) AS shares FROM (SELECT * FROM transactions WHERE users_id = ?) WHERE credit < 0 GROUP BY symbol", user_id)
    sold = db.execute(
        "SELECT symbol, SUM(shares) AS shares FROM (SELECT * FROM transactions WHERE users_id = ?) WHERE credit > 0 GROUP BY symbol", user_id)
    # sets the owned list to all bought stocks, so we can now subtract the sold stocks from this list by iterating over both lists below
    owned = bought
    for buy in bought:
        for sale in sold:
            if buy["symbol"] == sale["symbol"]:
                new_shares = buy["shares"] - sale["shares"]
                for item in owned:
                    if item["symbol"] == buy["symbol"]:
                        item["shares"] = new_shares
                        if new_shares == 0:
                            owned.remove(item)
    return owned


def apology(message, code=400):
    """Render message as an apology to user."""
    def escape(s):
        """
        Escape special characters.

        https://github.com/jacebrowning/memegen#special-characters
        """
        for old, new in [("-", "--"), (" ", "-"), ("_", "__"), ("?", "~q"),
                         ("%", "~p"), ("#", "~h"), ("/", "~s"), ("\"", "''")]:
            s = s.replace(old, new)
        return s
    return render_template("apology.html", top=code, bottom=escape(message)), code


def login_required(f):
    """
    Decorate routes to require login.

    https://flask.palletsprojects.com/en/1.1.x/patterns/viewdecorators/
    """
    @wraps(f)
    def decorated_function(*args, **kwargs):
        if session.get("user_id") is None:
            return redirect("/login")
        return f(*args, **kwargs)
    return decorated_function


def lookup(symbol):
    """Look up quote for symbol."""

    # Contact API
    try:
        api_key = os.environ.get("API_KEY")
        url = f"https://cloud.iexapis.com/stable/stock/{urllib.parse.quote_plus(symbol)}/quote?token={api_key}"
        response = requests.get(url)
        response.raise_for_status()
    except requests.RequestException:
        return None

    # Parse response
    try:
        quote = response.json()
        return {
            "name": quote["companyName"],
            "price": float(quote["latestPrice"]),
            "symbol": quote["symbol"]
        }
    except (KeyError, TypeError, ValueError):
        return None


def usd(value):
    """Format value as USD."""
    return f"${value:,.2f}"
