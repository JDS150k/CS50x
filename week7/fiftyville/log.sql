
----------------------------------------------------------------------------------

Action:
Find crimes from that day

SELECT
description
FROM
crime_scene_reports
WHERE
year = 2020
AND
month = 7
AND
day = 28

Result:
Theft of the CS50 duck took place at 10:15am at the Chamberlin Street courthouse.
Interviews were conducted today with three witnesses who were present at the time
— each of their interview transcripts mentions the courthouse.

Notes:

We now have a more specific time (10:15am) and a more specific place (courthouse).
We also learn that 3 interviews were conducted same day (viz interviews table).
----------------------------------------------------------------------------------

Action:
Find the content of the interviews

SELECT
name, transcript
FROM
interviews
WHERE
year = 2020
AND
month = 7
AND
day = 28

Result:
> Ruth - Sometime within ten minutes of the theft, I saw the thief get into a car in the courthouse parking lot and drive away.
If you have security footage from the courthouse parking lot, you might want to look for cars that left the parking lot in that time frame.
> Eugene - I don't know the thief's name, but it was someone I recognized. Earlier this morning, before I arrived at the courthouse,
I was walking by the ATM on Fifer Street and saw the thief there withdrawing some money.
> Raymond - As the thief was leaving the courthouse, they called someone who talked to them for less than a minute. In the call,
I heard the thief say that they were planning to take the earliest flight out of Fiftyville tomorrow. The thief then asked the person
on the other end of the phone to purchase the fli..

Notes:
> Ruth - thief made exit from courthouse in a car between 10:15 and 10:25
> Eugene - thief used the ATM on Fifer Street to withdraw before Eugene arrived at courthouse
> Raymond - thief called accomplice for <60sec straight after the crime.
          - thief plans to take earliest flight departing fiftyville tomorrow
          - accomplice plans to purchase the flight ticket

----------------------------------------------------------------------------------

Action:
Find the thiefs license plate from courthouse security footage

SELECT
license_plate
FROM
courthouse_security_logs
WHERE
year = 2020
AND
month = 7
AND
day = 28
AND
hour = 10
AND
minute BETWEEN 15 AND 26
AND
activity = "exit"

Result:
Inconclusive. Query returned seven values:

Notes:
Will create a table in which to store these for later queries

...

Action:
Store all possible license plates of the thief in new sus_cars list.

CREATE TABLE sus_cars AS
SELECT
license_plate
FROM
courthouse_security_logs
WHERE
year = 2020
AND
month = 7
AND
day = 28
AND
hour = 10
AND
minute BETWEEN 15 AND 26
AND
activity = "exit"

Result:
Stored data for later use.

----------------------------------------------------------------------------------

Action:
Find the thiefs account details from Fifer street ATM withdrawal

SELECT account_number
FROM
atm_transactions
WHERE
year = 2020
AND
month = 7
AND
day = 28
AND
atm_location = "Fifer Street"
AND
transaction_type = "withdraw"

Results:
Inconclusive. Query returned 8 values...

Notes:
Will store these results in a list for later use.

...

Action:
Create table to store potential account numbers of thief

CREATE TABLE sus_cars AS
SELECT account_number
FROM
atm_transactions
WHERE
year = 2020
AND
month = 7
AND
day = 28
AND
atm_location = "Fifer Street"
AND
transaction_type = "withdraw"

Results:
Stored data for later use.

----------------------------------------------------------------------------------

Action:
Make a list of sus_calls that could have been between thief and accomplice

CREATE TABLE sus_calls AS
SELECT caller, receiver
FROM
phone_calls
WHERE
year = 2020
AND
month = 7
AND
day = 28
AND
duration < 60

Results:
Table of 9 possible calls stored, with number for caller(thief) and receiver(accomplice)

----------------------------------------------------------------------------------

Action:
Find the id of the earliest flight out of Fiftyville tomorrow

SELECT
flights.id
FROM
flights
JOIN
airports
ON
flights.origin_airport_id=airports.id
WHERE
airports.city LIKE "fiftyville"
AND
flights.year = 2020
AND
flights.month = 7
AND
flights.day = 29
ORDER BY
flights.hour, flights.minute
LIMIT
1

Results:
The flight id is 36

----------------------------------------------------------------------------------

Action:
Find the flight's destination

SELECT
city
FROM
airports
WHERE
id=(
SELECT
destination_airport_id
FROM
flights
WHERE
flights.id = 36)

Results:
The thief flew to LONDON

----------------------------------------------------------------------------------

Action:
Make a list of the flight's passengers

CREATE TABLE sus_passengers AS
SELECT
passport_number
FROM
passengers
WHERE
flight_id = 36

Results:
Stored 8 passengers' passport_numbers in list

-----------------------------------------------------------------------------------

REVIEW

We now have:
    sus_accounts    | account numbers of suspects
    sus_calls       | possible caller/reciever phone numbers of thief/accomplice
    sus_cars        | license plate numbers of suspects
    sus_passengers  | passport numbers of all people on flight #36

We can search these tables to see if there is one person who exists in all sets.

----------------------------------------------------------------------------------

Action:
Find name of person whose information exists in all sets

SELECT
name
FROM
people
WHERE
id IN
(
SELECT
id
FROM
people
JOIN
bank_accounts
ON
people.id=bank_accounts.person_id
JOIN
sus_accounts
ON
bank_accounts.account_number=sus_accounts.account_number
)
AND
id IN
(
SELECT
id
FROM
people
JOIN
sus_calls
ON
people.phone_number=sus_calls.caller
)
AND
id IN
(
SELECT
id
FROM
people
JOIN
sus_cars
ON
people.license_plate=sus_cars.license_plate
)
AND
id IN
(
SELECT
id
FROM
people
JOIN
sus_passengers
ON
people.passport_number=sus_passengers.passport_number
)

Results:
The thief's name is Ernest

-----------------------------------------------------------------------------------

Action:
Find the receiver of the call which Ernest made after the crime

SELECT
name
FROM
people
WHERE
phone_number =
(
SELECT
receiver
FROM
sus_calls
JOIN
people
ON
sus_calls.caller=people.phone_number
WHERE
people.name = "Ernest"
)

Results:
The accomplices name is Berthold.

-----------------------------------------------------------------------------------

The THIEF is: Ernest
The thief ESCAPED TO: London
The ACCOMPLICE is: Berthold

------------------------------------------------------------------------------------