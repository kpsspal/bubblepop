# bubblepop
bubble pop game example

How to build:

Just use cmake to create project files for visual studio or mingw makefiles.

Then build from corressponding toolchain (visual studio or mingw32-make tool)  

All dependencies to build already included in deps folder of bubblepop.

All the events of bubble explodes are recorded in log file at location %appdata%\bubblepop.

How to play:

Controls:

Use mouse to aim cannon and click to shoot bubbles.

Rules:

	Match three or more bubbles of same color to remove them and gain score.

	After each 6 bubbles fired, bubbles on board will shift down, also causes score gain.

	On touching any bubble of board , game will over , final score displayed on right panel.

	After this you need to restart game.
	
Editor of Balloon Pop game:

Just change various initial game settings.

And click Start game button to play game with configured window sizes, bubble radius and others.
