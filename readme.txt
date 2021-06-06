Controls:
R1 / =     		   : Next gamemode
START / P  		   : Pause
WASD / Joystick / Buttons  : Move

Design choices:
I decided to make QBert without using prefabs, I implemented a Game Loop, Update method,
 Command, Observer, EventQueue and Component. I used Vector3s as positions where the Z axis decides
what's rendered first. I gave every gameObjects tags and names to access them. The tags are enums
which I try to use as frequently as possible as they are cheaper to compare. 


https://github.com/BrianBoddaert/QBert