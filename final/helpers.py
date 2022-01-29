

# https://min-api.cryptocompare.com/documentation

# API_key = f766acea6e0e6deb31d36258b7392e0a6c9b414d358e5e05ba0b72478b731872

# RETRIEVE A COIN PRICE IN USD:

# https://min-api.cryptocompare.com/data/price?fsym={COIN_SYMBOL}&tsyms=USD&api_key={API_KEY}

# E.G., WITH BITCOIN:

# https://min-api.cryptocompare.com/data/price?fsym=btc&tsyms=USD&api_key=f766acea6e0e6deb31d36258b7392e0a6c9b414d358e5e05ba0b72478b731872


import requests
from flask import redirect, render_template, request, session
from functools import wraps

def usd(value):
    """Format value as USD."""
    return f"${value:,.2f}"
