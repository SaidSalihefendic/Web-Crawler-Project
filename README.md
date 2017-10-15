# Web-Crawler-Project
A simple beginner Web Crawler (Uni Project) capable to take the URL input from the user, then the user inputs how many sites he/she wants to open that are connected to the initial site, and the project shows the 5 most used words that have 5 or more letters in each site and overall in folder Rijeci!

NOTE
 Before use, you have to change some code in order to work properly! Otherwise, the program will crash!

At line 78 and 268, the variable "stranica" contains the location of the cURL binary file inside the project folder, but in order to use it effectively, you have to copy the path of the cURL executable file, then put it inside this variable and it should be OK after that!
So, this is what it looks like for me

string stranica = "<PATHTOCURL> -o test.txt " + URL; =(<PATHTOCURL> change with the actual path)=> 
string stranica = "C:/Users/Said_2/Desktop/ASK/curl/bin/curl -o test.txt " + URL;
(after I copied the cURL path, which was inside my project folder)

And, voila! Build the whole thing and it should work!

If you don't have the cURL executable, download the binaries from here: https://curl.haxx.se/, then copy the path to the curl file and paste it in the variable "stranica", where <PATHTOCURL> stands!



Any bug reports, advices and comments will be appreciated!
I used cURL binary from https://curl.haxx.se/ in order to achieve my goal!

Credits: Anes Cehic, Kristijan Jelic (colleagues)

NOTE: The names of the functions, variables and files are in our native language, we will try in the future to change them to English, including the comments.
