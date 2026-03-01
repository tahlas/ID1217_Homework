The solution works by first starting the Server which creates a TableImpl object that is used
to manage the forks.

Each philosopher runs in an infinite while-loop and does the following:
1. Think (sleep) for a bit
2. Take the forks to the right and left
3. Eat some food on the table
4. Put down the forks on the table

takeForks() works by checking if the fork to the left and right are available. While they
are not available the philosopher will wait until they both are. When they are they are marked
as unavailable by setting the forks to false.

putDownForks() works by simply setting the left and right forks to true to make them available
and then notifies any waiting philosophers.

This is repeated forever.

There can only be a maximum of five philosophers in the table. 
