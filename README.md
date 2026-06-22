# jarreds-bytebeat-composer
(a,k.a JarredsBeatComposer) compose your C-Compatible bytebeat formulas for both Windows and Android
<img width="1080" height="651" alt="JarredsBeatComposerSplash" src="https://github.com/user-attachments/assets/a0e5739e-c8ef-4455-9d9f-d67532ae3fe1" />

## ❓ What is Bytebeat?
Bytebeat is a type of experimental music just using C/C++ math functions or
others used JavaScript for more advanced math functions. However,
JavaScript was most used and popular when it comes to making Bytebeat formulas.

### The History
It was discovered and coined in September 2011 by **viznut (Ville-Matias Heikkilä)**.
He was experimenting with one-line C programming formulas. He accidentally
discovered that if he put on a loop that outputs the raw outputting single-character bytes 
into a computer's raw audio terminal at a low sampling rate (8000 or 8khz), the output creates
an glitchy, melodic, and complex sounds that we all know today.

From his discovery, viznut uploaded a video on YouTube named "*Bytebeat: Experimental music from very short C programs*" and 
published blog posts that maps out the formal theory of the music style. 

What fascinated Viznut and the programming community most was that bytebeat wasn't a brand-new invention, 
but rather a belated historical discovery. The simple mathematical and bitwise operations he used had been native to computers since the 1950s. 
The community was bewildered that in over 50 years of computer science, nobody had thought to pipe 
these elementary logic loops directly into an audio buffer to see what they sounded like.

Here is the video:
[https://www.youtube.com/wtCRPUv8V22o?si=2AKoBF6OUEeo3GAH](https://www.youtube.com/watch?v=GtQdIYUtAHg)

Today, you can experiment these formulas on your web browsers but I have made a project written in C++ and MFC
for an offline version designed for Windows.

## ❓ How it works?
Bytebeat works on a time counter variable, ***t***
* The Math: A function is used to output a value between 0 and 255.
* The Loop: A program continuously increments the variable \(t\) (e.g., \(t=0, 1, 2, 3...\)) 
and evaluates the equation.
* The Result: The evaluated numbers are sent directly to the computer's sound card as PCM audio.

Note: Short bitwise operators (like &, |, ^, >>) are often combined to create complex, polyrhythmic, 
and melodic sounds. For example, the famous one-liner ```(t>>6|t|t>>(t>>16))*10+46``` generates a thumping, arpeggiated bassline.

## ❓ Why I made this app but there are some websites you can experiment?
I made this because with people wanted to make Bytebeat formulas into their computers,
online needs WiFi or Ethernet, while offline doesn't need it.

Which resulted me to build this project and with the help of AI, I can build my own parser, lexer, engine, and exporter from scratch without needing help of external libraries. Also, my parser, lexer, engine, and exporter is free and open source.

## ⚠️ Minimum Requirements:
For Windows:
* Windows 7 32/64 bit and above
* Visual C++ Redistributable 2013 (x86)

For Android:
* Android Lollipop (5.1) and above

## Known Bugs:
* Stuttering while playing formula.
* Fix: It is sometimes normal due to memory saving to avoid memory leaks but you need to close some apps as it increases CPU Usage.
* My formula sounds different when I paste my formula from a bytebeat composer website.
* Fix: Your formula is sometimes C-Compatible or not. Since these websites compiled through JavaScript, it will sound different. Also, the engine, parser, and lexer are almost perfect since I made this using AI. Make sure you double check your formula if it is C or JavaScript, or check for latest versions of my composer app.
* When I export to WAV, it crashed.
* Fix: Do not click on any part of this app because clicking repeatedly on an unresponsive window makes the problem worse. Therefore, it floods the operating system with unprocessed commands. This forces the system to queue up more tasks while it is already struggling to process the initial freeze. Since the app is not threaded on export, it will say not responding for short while.
* Visual C++ Runtime Error popup on splash screen?!
* Reason: I compiled on debug and added debug version of VC++ 2013 Redistributable DLLs. Which is why I added this? Because I will handle this app for errors and crashes. So if you see this, do not click anything else on the popup, it will just close for split 1 second.

## For bug reporting or feature request, email reyesgavinjarred@gmail.com or go to the issues tab and write there...
