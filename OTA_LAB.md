# OTA-labb via HTTP

Labbvärden har ESP:n ansluten till sin dator och kör både den befintliga
HTTP-servern och upptäcktstjänsten i `ota_discovery/`. Deltagarna behöver inte
starta några containrar. De ändrar programkoden; labbvärden bygger och laddar
upp firmware från datorn där ESP:ns Wi-Fi är konfigurerat.

ESP:n söker efter labbvärdens dator med UDP på port 8001. Upptäcktstjänsten
svarar med datorns IP-adress, och ESP:n hämtar firmware från
`http://<labbvärdens IP>:8000/OTA_labb_code.bin`.

## Labbvärden: förbered miljön

1. Låt **OTA lab → Firmware URL override (HTTP)** vara tom i
   `idf.py menuconfig`, så att ESP:n använder automatisk upptäckt. Konfigurera
   Wi-Fi för nätverket som ESP:n ska använda. Kontrollintervallet är 60 sekunder
   som standard.
2. Starta den befintliga HTTP-servern på din dator om den inte redan körs:

   ```sh
   docker compose -f ../OTA_labb_server_container/compose.yaml up --build -d
   ```

3. Starta upptäcktstjänsten på samma dator:

   ```sh
   docker compose -f ota_discovery/compose.yaml up -d
   ```

4. Bygg och ladda upp den firmwareversion som först ska köras. Den befintliga
   serverns Docker-volym kan annars fortfarande innehålla en äldre bild:

   ```sh
   idf.py build
   curl -f -X PUT --data-binary @build/OTA_labb_code.bin \
     http://localhost:8000/api/firmware
   ```

5. Flasha samma version till ESP:n en gång via USB med `idf.py flash monitor`.

## Deltagare: gör en uppdatering

Ändra programkoden och lämna ändringarna till labbvärden. Labbvärden bygger med
sin lokala Wi-Fi-konfiguration och laddar upp den nya bilden från sin dator:

```sh
idf.py build
curl -f -X PUT --data-binary @build/OTA_labb_code.bin \
  http://localhost:8000/api/firmware
```

`localhost:8000` i kommandot ovan är labbvärdens dator. Deltagarna behöver
varken köra `ota_discovery` eller ha ESP:n ansluten till sin egen dator.

ESP:n hittar servern vid nästa kontroll, jämför firmwarebilden med den som körs
och startar om efter en lyckad uppdatering. Samma bild installeras inte igen.
Den gamla bilden fortsätter att köras om servern inte kan nås. Efter första
lyckade sökningen minns ESP:n serveradressen. Om HTTP-kontakten misslyckas
söker den efter servern igen vid nästa kontroll.

ESP:n måste kunna nå labbvärdens dator över Wi-Fi. Brandväggen på labbvärdens
dator behöver släppa igenom UDP 8001 och TCP 8000. Om nätverket blockerar
broadcast kan en fast adress anges i **Firmware URL override (HTTP)**.
HTTP används här endast för labben.
