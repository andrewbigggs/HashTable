# HashTable

This code performs the function of a hash table. It reads a CSV file with certain specifications, stores the values of each line, and sorts each line’s
element into its appropriate bucket in hash table organized using chaining. I chose to use chaining with a constant size of 179 that was already in the
file because it seemed that amount didn’t result in any buckets size being disproportionate, even for the CSV file containing over ten thousand lines. The
advantage of chaining is that the size of the vector or array can remain small. Although a hash table using quadratic probing or double hashing could be a
bit more efficient, I think chaining worked well for hashing as all search operations that turned up empty (so traversed the entire bucket) spent clock
ticks < 179, and most were < 100 clock ticks.

I ran into several issues and have spent more hours developing the code for this hash table than I have for any other single coding assignment up to this
point. My first vision was to create an array of linked lists. This assignment tested and refined my knowledge of how pointers and references operate. I
ended up switching to a vector of linked lists instead of an array because the push_back(x) method didn’t cause me the same problems using bracket access
to update the elements of the array. I ran into an issue where the sizeof(x) method was returning 24 for the size of the vector no matter what and 1432 for
the size of the array no matter what, but I was able to determine that there were actually the appropriate number of elements in the vector after it was
constructed, so I just abandoned using the sizeof(x) method in this project.
