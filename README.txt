--- HOW TO RUN THE PROGRAM ---
before compilation, you should run this command to copy the required file to /usr/lib this file is for sound library, had to do this.

	sudo scp build/libIrrKlang.so /usr/lib

Alternative method: run

	export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib
	sudo scp build/libIrrKlang.so /usr/local/lib

which copy to /usr/local/lib directory. NOTE: need to run the "export ..." command above again if you close the terminal.

(If those two methods won't work, then you need to remove "libIrrKlang.so" from "premake4.lua" and comment out the line where the macro ENABLE_SOUND is defined in "gameobj/world.hpp", by doing this will disable sound features at all).

Then run the standard compilation script:

	premake4 gmake
	make

Then, run

	./A5

will run the program. No arguments need to be supplied.



For game interaction:

- Pressing up/down/left/right on your keyboard will move the tank. If the tank's current orientation is different from the direction you want it to move, the tank will change its orientation but it won't move. You can only move once in a small time period.

- Pressing Space key will fire a bullet. You can only fire once in a small time period.

NOTE: Before pressing start or S key, you can not move or fire, and enemy will not be generated.

- Each enemy and destoryable walls has 2 hp. The player has 4 hp. The puppet has 1 hp. If a bullet hits a tank/the puppet, the tank/the puppet decrements its hp by 1. Enemy has NO friendly fire demage (but you can attack your puppet)

- If your tank has 0 hp or the puppet has 0 hp, you loss.




For UI interaction:

- Pressing Quit Application button or Q will quit the program.

- Pressing Reset button or R will reset the view and the game. (will not start the game)

- Pressing retry button or N will reset the map and put the player to default position. (will not start the game)

- Pressing start button or S will start the game. 

- Toggle Show Shadow will draw shadows. Default value for it is true.

- Toggle Show Shadow Volume will draw the shadow volume in green (only if Show Shadow is toggled). Default value is false.

- Toggle Texture mapping will map textures on blocks and the floor. NOTE: textures for skybox cannot be disabled by this checkbox.

- Drag using the left mouse will rotate the view.

- Scroll the middle mouse will zoom in and out.

- You can change the volume by sliding the volume bar.

- You can change the opacity of the tree leaves to see the tree trunks by sliding the opacity bar.


--- Objective 8 in this project, Animation, is not implemented ---

OBJECTIVE LIST

1:  Create models for the tanks and the base

2:  Create L-system tree trunk

3:  Use shadow volume

4:  Apply texture mapping

5:  Implement alpha blending 

6:  Implement particle systems

7:  Create a skybox for the background of the scene

8:  Implement animations using interpolated keyframes

9:  Implement enemy logic

10:  Add sound effects and background music.


