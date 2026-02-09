Question 1: If we didn't lock the header, then multiple processes may have been reading the same offset and count number
at the same time, which would result in the values being incorrect and data overwriting later on.

Question 2: If we didn't lock the record and a second program was reading the file while we were writing to it, then that process would get the incorrect value from the read.