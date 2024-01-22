-- Get crime scene report from July 28
SELECT description FROM crime_scene_reports
WHERE month = 7 and day = 28 and street = "Humphrey Street";

-- Check interviews on day of theft
SELECT * FROM interviews WHERE month = 7 AND day = 28;
-- Clues gained from interviews:
-- Thief left bakery parking lot by car between 10:15 and 10:25
-- Thief withdrew money from ATM on Leggett Street before going to bakery
-- Thief called someone(= accomplice) for less than a minute after leaving the bakery
-- Thief planned to take the earliest flight from Fiftyville on the next morning


-- Check destination city and id from earliest flight out of Fiftyville on July 29th
SELECT flights.id, airports.city FROM airports
JOIN flights ON airports.id = flights.destination_airport_id
    WHERE month = 7 and day = 29 and origin_airport_id =
        (SELECT origin_airport_id FROM flights
        JOIN airports ON flights.origin_airport_id = airports.id
            WHERE airports.city = "Fiftyville")
    ORDER BY flights.hour, flights.minute
    LIMIT 1;
-- Flight to New York with id: 36

-- Combine clues and find the thief
SELECT name FROM people
JOIN bank_accounts ON people.id = bank_accounts.person_id
    -- Bank accounts from which money was withdrawn on Leggett Street
    WHERE bank_accounts.account_number IN
        (SELECT account_number FROM atm_transactions
            WHERE month = 7 and day = 28 and transaction_type = "withdraw" and atm_location = "Leggett Street")
    -- License plates from cars that left the bakery between 10.15 and 10.25
    and people.license_plate IN
        (SELECT license_plate FROM bakery_security_logs
            WHERE month = 7 and day = 28 and hour = 10 and minute > 15  and  minute < 25)
    -- Phone numbers from which a call under a minute was made
    and people.phone_number IN
        (SELECT caller FROM phone_calls
            WHERE month = 7 and day = 28 and duration < 60)
    -- Passport numbers from people that were on the earliest flight out of Fiftyville (from last query)
    and people.passport_number IN
        (SELECT passport_number FROM passengers
            WHERE flight_id = 36);
-- Thief is found to be Bruce

-- Find accomplice by looking for the person that Bruce called
SELECT name FROM people
WHERE phone_number =
        (SELECT receiver FROM phone_calls
            WHERE month = 7 and day = 28 and duration < 60 and caller =
                (SELECT phone_number FROM people
                    WHERE name = "Bruce"));
-- Accomplice is named Robin
