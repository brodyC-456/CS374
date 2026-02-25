# Brody's Seat Reserver (using threads and mutexes)

## Building

Command line:

* `make` to build. An executable called `reservations` will be produced.

## Files

* `reservations.c`: The main code that defines seat reservation logic

## Data

*seat_taken points to an array of "seats", with each byte representing a seat, initially full of 0's. When a seat n is reserved, the value of its corresponding byte is changed to 1.

## Functions

* `main()`
  * `reserve_seat(int n)` takes the seat in memory location n and "reserves" it by setting its value to 1, then increases the count of reserved seats.
  * `free-seat(int n)` does the same thing as reserve seat but instead sets the seat value to 0 and deincrements the reserved seat count.
      * `is_free(int n)` checks if seat n is free, returning 1 if it is and 0 if it isnt
  * `verify_seat_count()` checks if the number of reserved seats is correct.
  * `seat_broker()` runs each thread, reserving or freeing seats and verifying if the reserved seat count is correct afterwards.
