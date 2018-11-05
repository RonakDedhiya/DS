#!/bin/bash
for i in {1..15..1}
 do 
  myvar=$( ls /home/aitoe/Documents/RonakDataset/ATM/Data/ATMOffice$i/img1/ | wc -l )
  ./DS 1 /home/aitoe/Documents/RonakDataset/ATM/Data/ATMOffice$i/img1/ /home/aitoe/AitoeLabs/ATM/detections/ATMOffice$i.txt $myvar /home/aitoe/AitoeLabs/ATM/video/ATMOfficeTest$i.avi
 done
