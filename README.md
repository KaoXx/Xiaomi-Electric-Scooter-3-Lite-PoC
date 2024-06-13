# Xiaomi Electric Scooter 3 Lite PoC

In this repository you will find all the proofs of concept regarding the hacking section of my Xioami electric scooter Lite 3 thesis.


## Attacks

I have tested against the scooter:

- [ESP32 Attacks (RF and DoS)](https://github.com/KaoXx/Xiaomi-Electric-Scooter-3-Lite-PoC/tree/main/ESP32%20Attacks)
- [ESP32 Bluetooth Spam](https://github.com/KaoXx/Xiaomi-Electric-Scooter-3-Lite-PoC/tree/main/ESP32%20Spam) 
- [ESP32 Spoofing](https://github.com/KaoXx/Xiaomi-Electric-Scooter-3-Lite-PoC/tree/main/ESP32%20Spoof/ESP32ScooterSpoof)
- [Flipper Zero BLE Spam](https://github.com/KaoXx/Xiaomi-Electric-Scooter-3-Lite-PoC/tree/main/Flipper%20Zero%20ble_spam)




## Logs

In the bluetooth communication of the scooter with the Xiaomi application I have collected the following logs:

- [Scooter Logs](https://github.com/KaoXx/Xiaomi-Electric-Scooter-3-Lite-PoC/tree/main/Xiaomi%20Logs)


## CVE-2019-12500

I have updated a script based on [CVE-2019-12500](https://nvd.nist.gov/vuln/detail/CVE-2019-12500), which affected scooter firmware versions <1.5.1. 

Script based on https://github.com/louiscb/Mi365Locker-RASPI that allows us to send lock and unlock packets to the scooter through its BLE module for linux systems:

- [CVE-2019-12500.py](https://github.com/KaoXx/Xiaomi-Electric-Scooter-3-Lite-PoC/tree/main/Xiaomi%20M365%20-%20CVE-2019-12500)

