# HackerHat
 How to build your own Hacker Hat

## How the idea start
Nowadays there are a lot of devices that work using of 433MHz frequency, such as car and home alarms, wireless door bells, etc.
I thinked about it and said: Why can't I create a device that in constantly sniffing on this frequency? How can I make this ridiculously funny? 
So, I created a challenge that if you have to transmiting an special code in 433MHz, you will get a secret code in hexadecimal. Obviously, I mounted all of this in a hat, why? Because, I think that is cool :D

## Materials
- Hat
- 1 x Arduino Uno
- 1 x 433 MHz Module (MX-05V)
- 1 x Neopixel Ring WS2812
- 1 x 1000 uF Capacitor
- 1 x 300-500 Ohm Resistor

## How It Works
When the hat turns on, starts to show a cool pattern in the Neopixel Ring and after some seconds stops 4 sec and then continue. In this 4 seconds, the hat stops and entered in a "Listen Mode", a mode where start to hear all the signals that are transmitted in 433 MHz. If the hat recognize that a correct signal was transmitted, depending the code transmited to the hat, enters on a "Demo Mode" or shows the secret code in hexadecimal.

## Diagram
Coming Soon :D
