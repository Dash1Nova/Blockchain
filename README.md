# Blockų grandinių technologijos, laboratorinis darbas 1

Pseudokodas:

```text
Hash(input):
currentValue <- 0
for i <- 0 to length(input) - 1:
    do currentValue <- currenValue XOR byte
    if position(byte) is even
        then currentValue <- currentValue LEFT SHIFT 5
        else currentValue <- currentValue RIGHT SHIFT 3
    result <- ""

    groups <- 256/4
    for i <- 1 to groups:
        do group <- 4 bits of currentValue
    result <- result || hex(group)

    return currentValue
```
