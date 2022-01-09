import os
from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session

app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///birthdays.db")
BDAYS = {}

@app.route("/", methods=["GET", "POST"])
def index():
    if request.method == "POST":
        # TODO: Add the user's entry into the database
        # ...your web application should add a new birthday to your database and then re-render the index page.
        # **4)** ...add logic in your POST request handling to INSERT a new row into the birthdays table based on the data supplied by the user.
        name=request.form.get("name")
        month=request.form.get("month")
        day=request.form.get("day")
        db.execute("INSERT INTO birthdays (name, month, day) VALUES (?, ?, ?)", name, month, day)
        return redirect("/")
    else:
        # TODO: Display the entries in the database on index.html
        # ...your web application should display, in a table, all of the people in your database along with their birthdays.
        # **1)** ... add logic in your GET request handling to query the birthdays.db database for all birthdays. Pass all of that data to your index.html template
        # SQL code for selecting all birthdays and assigning that into the variable "bdays"
        BDAYS = db.execute("SELECT * FROM birthdays")
        return render_template("index.html", bdays=BDAYS)



# **5)** Add the ability to delete and/or edit birthday entries.

