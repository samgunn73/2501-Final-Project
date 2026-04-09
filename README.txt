2501 FINAL PROJECT
by:Sam Gunn 101195559, Oluwaseyi Olorunfemi

Controls:
W : Jump
A : Move Left
D : Move Right
F : Fire projectile
Space: Use melee attack
Mouse: aim attacks

Checklist:

1. Game Mechanics:
The finishing condition of the game is simple but more than reaching the end.
You must collect several of your crab children around the map and then return to your friend at the start of the level
You cannot finish the level without a certain amount of crab collectibles completed. There is also a life system,
if you reach 0 then you lose.

2.Enemies
There are 3 types of enemies that try and hurt the player, They are harpoons, hooks and nets. They all have different AI associated with them. harpoons use parametric motion and orbit an ellipse until the player is in a certain distance, and then they perform a chasedown. The hook will move up and down until the player is within a certain distance and it will also perform a chasedown. The Net performs random wandering around the level.

3 Weapons
There are 2 types of weapons as outlined in the controls, F fires a projectile that uses ammunition. it follows the mouse position and will collide with any enemies hit. Space bar uses a melee attack that currently has not had an animation created for it. It generates an invisible hitbox either left or right of the player depending on where the mouse is, and it follows the player as it moves. 

4 Collectibles,
There are 4 collectible items in the game. The first is an invincibility powerup that makes the player invincible for 10 seconds after picking it up. the next 3 all increment certain aspects of the game, wether its health, ammo or incrementing the finishing condition.

5Motion

Most motion in this game is handeled through physics based euler integration, however some enemies will use parametric based motion in some states

6. Collision Detection
There are numerous different checks for collision, wether its between players and enemies, enemies and projectiles or players and platforms.

7.GameWorld
The player can access a large amount of the gameworld, around 8 times the size of the game screen, smart scrolling occurs at the limits of the world, and the player cannot escape it. Tiling is used in the platforms and background to make the gameworld larger.

8.There are two particle systems used: one is when a player collides with the ground, it creates a particle system that is supposed to imitate sand being kicked up. The second one is when an enemy dies, it creates an explosion of bubbles.

9 UI
The UI in this game keeps track of all resources that the player has in the top right and also the timer in the top left

10 Advanced methods
The advanced method implemented is the wandering of the net, I tried to implement so that the net could only wander downwards but this proved to complicated for the time i had.



