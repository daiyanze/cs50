-- Keep a log of any SQL queries you execute as you solve the mystery.

-- 1. Get the detail from the crime scene report
SELECT description FROM crime_scene_reports
WHERE
    year = 2020 AND
    month = 7 AND
    day = 28 AND
    street = 'Chamberlin Street';
-- description
-- Theft of the CS50 duck took place at 10:15am at the Chamberlin Street courthouse.
-- Interviews were conducted today with three witnesses who were present at the time — each of their interview transcripts mentions the courthouse.


--2.  According to the report, there were 3 witnesses with some transcripts worth further investigations.
SELECT name, transcript FROM interviews
WHERE
    year = 2020 AND
    month = 7 AND
    day = 28 AND
    transcript LIKE '%courthouse%';

-- name | transcript
-- Ruth | Sometime within ten minutes of the theft, I saw the thief get into a car in the courthouse parking lot and drive away. If you have security footage from the courthouse parking lot, you might want to look for cars that left the parking lot in that time frame.
-- Eugene | I don't know the thief's name, but it was someone I recognized. Earlier this morning, before I arrived at the courthouse, I was walking by the ATM on Fifer Street and saw the thief there withdrawing some money.
-- Raymond | As the thief was leaving the courthouse, they called someone who talked to them for less than a minute. In the call, I heard the thief say that they were planning to take the earliest flight out of Fiftyville tomorrow. The thief then asked the person on the other end of the phone to purchase the flight ticket.


-- Ruth & Eugene & Raymond's transcripts possibly implies the following behaviors of the thief and accomplice (chronologically):
-- 1. The thief withdraw money from ATM on Fifer Street (2020.07.28 early morning)
-- 2. The thief took a phone call to his/her accomplice for less than a minute (2020.07.28 before leaving courthouse)
-- 3. The thief drove a car away from the courthouse parking lot (2020.07.28 10:15~10:25)
-- 4. The thief took the earliest fight out of Fiftyville on July 29th

-- So we will investigate and build SQL query based on the conditions
SELECT
    people.name AS 'THIEF',
    airports.city AS 'ESCAPED TO',
    callee.name AS 'CALLEE'
FROM people
JOIN
    airports,
    passengers,
    flights,
    (
        -- Accomplice is the callee on July 28 (duration less than a minute)
        SELECT * FROM phone_calls
        JOIN people
        ON
            people.phone_number = phone_calls.receiver
        WHERE
            year = 2020 AND
            month = 7 AND
            day = 28 AND
            duration < 60
    ) AS callee
ON
    airports.id = flights.destination_airport_id AND
    flights.id = passengers.flight_id AND
    passengers.passport_number = people.passport_number AND
    callee.caller = people.phone_number
WHERE
    -- 1. withdraw money on July 28th
    people.id IN (
        SELECT person_id FROM bank_accounts
        JOIN atm_transactions
        ON
            bank_accounts.account_number = atm_transactions.account_number
        WHERE
            atm_transactions.year = 2020 AND
            atm_transactions.month = 7 AND
            atm_transactions.day = 28 AND
            atm_transactions.atm_location = 'Fifer Street' AND
            atm_transactions.transaction_type = 'withdraw'
    ) AND
    -- 2. phone call less than a minute on July 28th
    people.phone_number IN (
        SELECT caller FROM phone_calls
        WHERE
            year = 2020 AND
            month = 7 AND
            day = 28 AND
            duration < 60
    ) AND
    -- 3. exit courthouse between 10:15 and 10:25 on July 28th
    people.license_plate IN (
        SELECT license_plate FROM courthouse_security_logs
        WHERE
            courthouse_security_logs.month = 7 AND
            courthouse_security_logs.day = 28 AND
            courthouse_security_logs.hour = 10 AND
            courthouse_security_logs.minute >= 15 AND
            courthouse_security_logs.minute < 15 + 10 AND
            courthouse_security_logs.activity = 'exit'
    ) AND
    -- 4. earlist flight on July 29th
    people.passport_number IN (
        SELECT passengers.passport_number FROM passengers
        JOIN flights
        ON
            flights.id = passengers.flight_id
        WHERE
            flights.id = (
                SELECT id from flights
                WHERE
                    flights.month = 7 AND
                    flights.day = 29
                ORDER BY flights.hour, flights.minute ASC
                LIMIT 1
            )
    );
