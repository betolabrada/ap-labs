# Using sliding window algorithm with optimization

import sys

def length_longest_substring(s):
    a = {}
    i = j = r = 0

    for j in range(len(s)):
        if s[j] in a:            
            i = max(a[s[j]] + 1, i)
        r = max(r, j - i + 1)
        a[s[j]] = j

    return r


print(length_longest_substring(sys.argv[1]))