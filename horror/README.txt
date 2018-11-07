Hall of Memory Management Horrors
---------------------------------

Based on slides from Bryant and O’Hallaron, _Computer Systems: A
Programmer’s Perspective_, Third Edition.

h1.c
----

Look at the program and try compiling with

  gcc h1.c

Then run `./a.out`. What happens? Why?

(Hint: the solution is to add a `&` in front of one of the `v`s.)

See "h1.txt" for an explanation.


h2.c
----

Compile and run "h2.c". You should get

  43210 87650 43210 87650

That's what you got? Great! The program must be working...

In main(), try repeating

  y = matrix_times_vector(A, x);
  show_vector(y);
  free(y);

a second time, or add a loop to run multple times. Of course, all
results should be the same, but probably they're not.

What happens if you take out the free()s?

What's the real problem?

See "h2.txt" for an explanation.


h3.c
----

The "h3.c" example is similar to "h2.c", but the author of "h3.c"
remembered to initialize allocated memory. Unfortunately, the author
made a different mistake.

You noticed the missing free()? Good. But maybe we don't care if the
program exits right away anyway.

The output should be

 132750 132750 132750 132750 132750 132750 132750 132750 132750 132750

Probably that's what you see. And if you put the

  y = matrix_times_vector(A, x);
  show_vector(y);

in a loop, you keep getting that output, probably. Great!

But try this:

  y1 = matrix_times_vector(A, x);
  y2 = matrix_times_vector(A, x);
  show_vector(y1);
  show_vector(y2);

Ok, so what's wrong?

See "h3.txt" for an explanation.


h4.c
----

The "h4.c" example is similar to "h3.c", and the author of "h4.c" both
allocated the correct size and remembered to include a loop that
initializes allocated memory.

Try the things we tried before: put the calls in a loop, save a couple
of vectors and print them --- all good, right?

Just in case, maybe we should free `y`. Probably, that will provoke a
crash, though. Why?

See "h4.txt" for an explanation.


h5.c
----

The "h5.c" example is ok as long as the user types 7 characters or
less.

What's the solution if you have no control over the input and really
want only 7 characters or less?

See "h5.txt".


h6.c
----

This one allocates a "school of fish" in a way similar to how
allocators tend to work: with some macro to explicitly manipulate a
generic pointer to access the count or an individual fish.

It seems ok, until you improve the program by adding a `free(p)` to
the end.

Valgrind will tell you where this one goes wrong, but figuring out why
that point in the program goes wrong is not so obvious.

See "h6.txt".

Others
------

Some other things that can go wrong:

 * free() multiple times on the same pointer

   The C library tends to catch that error if it happens soon enough,
   but if the second free() is long after additional malloc() and
   free() calls, it might not.

 * accessing memory after free()

   Using memory after free() is, unfortunately, likely to work ok if
   it happens soon after free(), and especially if the use stays away
   frmo the start or end of the block. (Why?)

 * not free()ing memory

   In a long-running application, failing to free memory can cause an
   application to use more and more memory unnecessarily. Valgrind can
   help find such memory leaks, too.
