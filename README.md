# 🎊 Welcome to Jarred's Bytebeat Composer!
### (a.k.a JarredsBeatComposer)

### Finding the old repository? [Click here to go to the legacy version](https://github.com/jarred-new/jarreds-bytebeat-composer-legacy)
### Reminder: The legacy version is now officially archived. You cannot make issues, forks, and etc.

This is a simple (but little bit advanced) bytebeat composer that allows you to create and play bytebeat music using your phone, tablet, or computer both online or offline. 

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

Today, you can experiment these formulas on your web browsers and mobile apps both online and offline, but I have made a project written in C++ (Windows) and Java (Android) for an offline version.

## ❓ How it works?
Bytebeat works on a time counter variable, ***t***
* The Math: A function is used to output a value between 0 and 255.
* The Loop: A program continuously increments the variable \(t\) (e.g., \(t=0, 1, 2, 3...\)) 
and evaluates the equation.
* The Result: The evaluated numbers are sent directly to the computer's sound card as PCM audio.

Note: Short bitwise operators (like &, |, ^, >>, <<) are often combined to create complex, polyrhythmic, and melodic sounds. For example, the famous one-liner ```(t>>6|t|t>>(t>>16))*10+46``` generates a thumping, arpeggiated bassline.

Also, for ternary operators, you can use them to create more complex and melodic sounds. For example, the one-liner ```(t>>6|t|t>>(t>>16))*10+46 + (t%100<50?0:((t>>4|t>>5)&(t>>7|t>>8)))``` generates a thumping, arpeggiated bassline with a melody. 
If you're using the offline version you can use ``if()`` function based by libraries that I used.

## License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Requirements:
For Windows:
* Windows 7 (64 bit only!) and above
* Visual C++ Redistributable 2014-2022 (x64)
* Minimum 2GB RAM and 1.5GHz CPU
* Minimum 1GB free storage space

For Android:
* Android Lollipop (5.1) and above
* Minimum 2GB RAM and 1.5GHz CPU
* Minimum 1GB free storage space

## Downloads
* [Get here for Windows](https://github.com/jarred-new/jarreds-bytebeat-composer-winqt/releases)
* [Get here for Android](https://github.com/jarred-new/jarreds-bytebeat-composer-android/releases)

### Can't find the right version for your old device? [Go to the legacy releases](https://github.com/jarred-new/jarreds-bytebeat-composer-legacy/releases)

## If you have issues, please report them here:
* [Report issues here for Windows](https://github.com/jarred-new/jarreds-bytebeat-composer-winqt/issues)
* [Report issues here for Android](https://github.com/jarred-new/jarreds-bytebeat-composer-android/issues)

## Special Thanks
* [viznut](https://github.com/viznut) for discovering Bytebeat and creating the first Bytebeat formulas.
* [the graphic zone](https://www.facebook.com/profile.php?id=61589491050574) for the palette themes I put to my Windows version.