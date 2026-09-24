# Blockų grandinių technologijos, laboratorinis darbas 1

## Idėja

Mano hash funkcija turi struktūra Current, kurioje yra 4 kintamieji tipo uint64_t, kiekvienas po 64 bitus, kas iš viso išvestyje sudaro 256 bitų reikšmę. Pradžioje kiekviena iš jų turi savo pradinę reikšmę
(skaičius 11, 12, 13 ir 14, užrašytus hex formatu), todėl nepriklausomai nuo įvesties dydžio, išvestis visada bus gaunama 64 hex simboliu formatu. Įvestis yra padalinama į 8 baitų blokus. Kiekvienas blokas apdorojamas atskirai, kad nereiketu visos įvesties (jei ji yra didelė) apdoroti iš karto, todėl galima dirbti su didesniais failais.

Kiekvienam baitui parenkama viena iš keturių 256 bitų reikšmės dalių. Ji parenkama pagal formulę: (pozicija + baito reikšmė) % 4.
Tokiu būdu skirtingi baitai gali paveikti skirtingas būsenos dalis. Tada nauja baito reikšmė sujungiama su ankstesne pasirinktos būsenos
reikšme pagal analogija skaičiavimų, atliekamų finansų rinkose skaičiuojant EMA indikatorių. Naujam baitui suteikiamas didesnis svoris
negu senai reikšmei. Šį būdą pasirinkau kaip vieną iš savo algoritmo idėjų, tačiau jis turi ir trūkumų, nes aritmetinis vidurkis sumažina
mažų pokyčių poveikį ir suglotnina maišą, o šiuo atveju reikalinga daugiau "triukšmo".

## Pseudokodas:

```text
Current():
    value <- 0x0B0B0B0B0B0B0B0B, 0x0C0C0C0C0C0C0C0C, 0x0D0D0D0D0D0D0D0D, 0x0E0E0E0E0E0E0E0E

HashBucket(bucket, startState):
    state <- startState
    newWeight <- 7
    oldWeight <- 4
    totalWeight <- newWeight + oldWeight
    for i <- 0 to length(bucket) - 1:
        do part <- (i + bucket[i]) mod 4
        do newValue <- bucket[i]
        do oldValue <- state[part]
        do state[part] <- (newValue * newWeight + oldValue * oldWeight) / totalWeight
    return state

Hash(input):
    currentMain <- Current()
    bucketSize <- 8
    for i <- 0 to length(input) - 1 step bucketSize:
        do end <- min(i + bucketSize, length(input))
        do bucket <- input[i ... end - 1]
        do currentMain <- HashBucket(bucket, currentMain)
    result <- ""
    hexDigits <- "0123456789abcdef"
    reserve(result, 64)
    for i <- 3 down to 0:
        for shift <- 60 down to 0 step 4:
            do nibble <- (currentMain[i] >> shift) AND 0xF
            do result <- result || hexDigits[nibble]
    return result
```

## Įvesties dydžio ribos

Visas įvesties turinys sukraunamas į RAM (`std::vector<uint8_t>`). Taigi didžiausias galimas failo dydis priklauso nuo laisvos RAM kiekio
kompiuteryje. Labai didelių failų programa gali sulėtėti arba nepavykti dėl atminties trūkumo.

## Enter / naujos eilutės simbolis

Įvedant tekstą ranka naudojama `std::getline`, kuri Enter paspaudimo sukurto `\n` simbolio NEĮTRAUKIA į nuskaitytą tekstą.

## Determinizmas

Maišos funkcija nenaudoja random, chrono ar kitų bibliotekų, kurios generuoja kiekvieną kartą kintančias reikšmes, todėl ta pati įvestis
visada duoda tą pačią maišą.

Patikrinta praktiškai:

## Efektyvumas

Naudojant meniu punktą "4 - ismatuoti laika", buvo išmatuotas maišos skaičiavimo laikas
su skirtingo dydžio atsitiktiniais duomenimis:

| Dydis (baitais) | Laikas (ms) |
| --------------- | ----------- |
| 1 000           | 0           |
| 10 000          | 2           |
| 100 000         | 18          |
| 1 000 000       | 190         |
