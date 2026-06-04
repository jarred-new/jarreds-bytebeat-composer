# jarreds-bytebeat-composer
(a,k.a JarredsBeatComposer) compose your C-Compatible bytebeat formulas for Windows
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

## ❓ Why I made this but there is some websites you can experiment?
I made this because with people wanted to make Bytebeat formulas into their computers,
online needs WiFi or Ethernet while offline doesn't need it.

Which I build this project and with the help of AI, I can build my own parser, lexer, engine, and exporter from scratch without
needing help of external libraries.

## ⚠️ Minimum Requirements:
* Windows XP 32 bit and above
* Visual C++ Redistributable 2013

## For bug reporting or feature request, email reyesgavinjarred@gmail.com or go to the issues tab and write there...
