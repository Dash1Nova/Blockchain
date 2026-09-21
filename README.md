# Blockų grandinių technologijos, laboratorinis darbas 1

Pseudokodas:

```text
Hash(input):
    currentMain <- 0, 0, 0, 0
    bucketSize <- 8
    for i <- 0 to length(input) - 1 step bucketSize:
        do end <- min(i + bucketSize, length(input))
        do bucket <- input[i ... end - 1]
        do bucketState <- HashBucket(bucket)
        for j <- 0 to 3:
            currentMain[j] <- currentMain[j] XOR bucketState[j]
    result <- ""
    hexDigits <- "0123456789abcdef"
    reserve(result, 64)
    for i <- 3 down to 0:
        for shift <- 60 down to 0 step 4:
            do nibble <- (currentMain[i] >> shift) AND 0xF
            do result <- result || hexDigits[nibble]
    return result
```
