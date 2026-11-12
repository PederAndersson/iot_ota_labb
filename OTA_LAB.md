# OTA via HTTP med befintlig container

ESP:n söker efter datorn med ett UDP-anrop på port 8001. Den lilla
upptäcktstjänsten i `ota_discovery/` svarar från datorns nätverksadress. ESP:n
hämtar sedan `http://<datorns IP>:8000/OTA_labb_code.bin` från den befintliga
OTA-servern i `../OTA_labb_server_container`.

1. Låt **OTA lab → Firmware URL override (HTTP)** vara tom i `idf.py menuconfig`.
   Då används automatisk upptäckt. Kontrollintervallet är 60 sekunder som standard.
2. Starta den befintliga HTTP-servern om den inte redan körs:

   ```sh
   docker compose -f ../OTA_labb_server_container/compose.yaml up --build -d
   ```

3. Starta upptäcktstjänsten i en separat liten container på samma dator:

   ```sh
   docker compose -f ota_discovery/compose.yaml up -d
   ```

4. Bygg den här versionen och lägg samma firmwarefil på servern innan kortet
   startas. Den befintliga serverns Docker-volym kan annars fortfarande innehålla
   en äldre bild:

   ```sh
   idf.py build
   curl -f -X PUT --data-binary @build/OTA_labb_code.bin \
     http://localhost:8000/api/firmware
   ```

5. Flasha den här firmwareversionen en gång via USB med `idf.py flash monitor`.
6. Ändra programmet, bygg en ny version och ladda upp den till samma server:

   ```sh
   idf.py build
   curl -f -X PUT --data-binary @build/OTA_labb_code.bin \
     http://localhost:8000/api/firmware
   ```

ESP:n hittar servern vid nästa kontroll, jämför firmwarebilden med den som körs
och startar om efter en lyckad uppdatering. Samma bild installeras inte igen.
Den gamla bilden fortsätter att köras om servern inte kan nås.
Efter första lyckade sökningen minns ESP:n serveradressen. Om HTTP-kontakten
misslyckas söker den efter servern igen vid nästa kontroll.

Datorn måste kunna nås från ESP:ns Wi-Fi. Datorns brandvägg behöver släppa igenom
UDP 8001 och TCP 8000. Om nätverket blockerar broadcast kan en fast adress anges
i **Firmware URL override (HTTP)**. HTTP används här endast för labben.
