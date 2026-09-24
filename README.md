# Blockų grandinių technologijos, laboratorinis darbas 1

## Idėja

Mano hash funkcija turi struktūra Current, kurioje yra 4 kintamieji tipo uint64_t, kiekvienas po 64 bitus, kas iš viso išvestyje sudaro 256 bitų reikšmę. Pradžioje kiekviena iš jų turi savo pradinę reikšmę
(skaičius 11, 12, 13 ir 14, užrašytus hex formatu), todėl nepriklausomai nuo įvesties dydžio, išvestis visada bus gaunama 64 hex simboliu formatu. Įvestis yra padalinama į 8 baitų blokus. Kiekvienas blokas apdorojamas atskirai, kad nereiketu visos įvesties (jei ji yra didelė) apdoroti iš karto, todėl galima dirbti su didesniais failais.

Idėja bitų maišymo yra padaryta pagal analogiją su skaičiavimais, naudojamais finansų rinkose, kur skirtingoms reikšmėms gali būti suteikiami skirtingi svoriai (EMA indikatorius). Mano atveju naujai gaunama baito reikšmė turi didesnį svorį negu ankstesnė hash reikšmė.

Kiekvienas bloke esantis baitas pagal savo poziciją yra priskiriamas vienai iš keturių Current dalių. Tam naudoju i % 4. Tai reiškia, kad pirmas baitas keičia pirmą dalį, antras – antrą, trečias – trečią, ketvirtas – ketvirtą, o po to šis ciklas kartojasi. Tokį būdą pasirinkau todėl, kad jis yra paprastas ir leidžia tolygiai paskirstyti įvesties baitus tarp keturių dalių. Po to paimama esama pasirinktos dalies reikšmė ir naujo baito reikšmė. Joms priskiriami skirtingi svoriai. Naujam baitui suteikiu svorį 7, o senai reikšmei – 4. Taip nauja įvestis turi didesnę įtaką rezultatui, tačiau ankstesnė reikšmė taip pat nėra visiškai prarandama. Šie skaičiai nėra paimti iš standartinės hash funkcijos, o pasirinkti kaip mano algoritmo dalis. Dalinimas iš 11 atliekamas todėl, kad 11 yra abiejų svorių suma: 7 + 4 = 11. Tokiu būdu gaunamas svertinis vidurkis.

Kadangi naudoju sveikus skaičius dalinimui, gali būti prarandama dalis informacijos dėl apvalinimo žemyn. Tai yra vienas iš mano hash funkcijos trūkumų, kurį vėliau ketinu patikrinti eksperimentiniuose tyrimuose.

## Pseudokodas:

```
Current:
    value <- 0x0B0B0B0B0B0B0B0B, 0x0C0C0C0C0C0C0C0C, 0x0D0D0D0D0D0D0D0D, 0x0E0E0E0E0E0E0E0E


hashBucket(bucket, startBucket):
    currentBucket <- startBucket
    newWeight <- 7
    oldWeight <- 4
    totalWeight <- newWeight + oldWeight

    for i <- 0 to length(bucket) - 1:
        do part <- i mod 4
        do newValue <- bucket[i]
        do oldValue <- currentBucket.value[part]
        do currentBucket.value[part] <- (newValue * newWeight + oldValue * oldWeight) / totalWeight

    return currentBucket


hash(data):
    currentMain <- Current()
    bucketSize <- 8

    for i <- 0 to length(data) - 1 step bucketSize:
        do end <- min(i + bucketSize, length(data))
        do bucket <- data[i ... end - 1]
        do currentMain <- hashBucket(bucket, currentMain)

    length <- length(data)
    for part <- 0 to 3:
        do oldValue <- currentMain.value[part]
        do currentMain.value[part] <- (length * 7 + oldValue * 4) / 11

    result <- ""
    hexDigits <- "0123456789abcdef"
    reserve(result, 64)

    for i <- 3 down to 0:
        for shift <- 60 down to 0 step 4:
            do digit <- (currentMain.value[i] >> shift) and 0xF
            do result <- result || hexDigits[digit]

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
<img width="666" height="337" alt="Ekrano kopija 2026-09-24 165126" src="https://github.com/user-attachments/assets/9c0a2618-1ccc-4567-8cef-be7ab11f399b" />

## Efektyvumas

Naudojant meniu punktą "4 - ismatuoti laika", buvo išmatuotas maišos skaičiavimo laikas
su skirtingo dydžio atsitiktiniais duomenimis:

| Dydis (baitais) | Laikas (ms) |
| --------------- | ----------- |
| 1 000           | 0           |
| 10 000          | 2           |
| 100 000         | 18          |
| 1 000 000       | 190         |
