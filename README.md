# Web-Crawler-Project
A simple Web Crawler (Uni Project) capable to take the URL input from the user, user then tells it how many sites he/she wants to open that are connected to the initial site, and saves the 5 most used words that have 5 or more letters in each site and overall in folder Rijeci!

NOTE
If you wish to use this product, feel free, but before you use it, you have to change some code in order work properly! Otherwise, it won't work!

At line 78 and 268, the variable "stranica" contains the location of the cURL binary file inside the project folder, but in order to use it effectively, you have to copy the path of the cURL executable file, then put it inside this variable and it should be OK after that!
So, this is what it looks like for me

string stranica = "<PATHTOCURL> -o test.txt " + URL; => string stranica = "C:/Users/Said_2/Desktop/ASK/curl/bin/curl -o test.txt " + URL;
(after I copied the cURL path inside the project folder)

And, voila! Build the whole thing and it should work!

Any bug reports, advices and comments will be appreciated!

I used cURL binary from https://curl.haxx.se/ in order to achieve my goal with this project!

Credits: Anes Cehic, Kristijan Jelic (colleagues)
