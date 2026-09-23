Shooting Game C++

YOUSFI IDRIS

Game Description

This project is a simple 2D shooting game developed in C++ using the Grapic library.

The player controls a spaceship that can move horizontally and vertically using the arrow keys. The player can also shoot projectiles using the space bar.

The spaceship is located at the bottom of the window, while enemies appear at the top and move down towards the player.

The game uses complex numbers to represent positions and relies on the Complex and Particle structures.

Projectiles are represented by circles and enemies by rectangles. The game checks for collisions between projectiles and enemies.

⸻

Objective

The goal of the game is to destroy the enemies before they reach the player’s spaceship.

* Destroy the enemies by shooting them
* Each destroyed enemy gives 100 points
* Reach 2000 points to win the game
* If an enemy reaches the bottom of the window, the player loses

⸻

Controls

* Arrow keys: Move the spaceship
* Space bar: Shoot

⸻

Implemented Features

* Spaceship movement
* Projectile shooting
* Projectile movement
* Enemy movement
* Projectile-enemy collision detection
* Spaceship-enemy collision detection
* Score system
* Win condition
* Lose condition
* Image rendering
* Moving enemies

⸻

Possible Improvements

The following features could be added to further improve the game:

* Different types of spaceships with different shooting speeds
* The ability to shoot multiple projectiles at the same time
* Enemies that can shoot projectiles
* Enemies with different movement patterns
* Enemies with health points that require multiple hits to destroy
* Different types of enemies with different abilities

⸻

Main Functions

The game is organized around several main functions:

* InitJeu — Initializes the game
* MouvementVaisseau — Handles spaceship movement
* TirerProjectile — Creates and shoots projectiles
* MouvementProjectile — Handles projectile movement
* MouvementEnnemi — Handles enemy movement
* Update — Updates the game state
* Collision — Checks collisions between projectiles and enemies
* CollisionVaisseau — Checks collisions between enemies and the spaceship
* Draw — Draws the game elements
* Draw_Image — Displays images
* Score — Handles the scoring system

⸻

Development History

Week 1 — April 10

* Added the InitJeu procedure
* Created the main game structures
* Implemented the MouvementVaisseau procedure

Week 3 — April 24 — Final Submission

* Added the TirerProjectile and MouvementProjectile procedures
* Started implementing Draw with images for the spaceship and enemies
* Fixed the TirerProjectile procedure
* Added the Collision and CollisionVaisseau procedures
* Created the Draw_Image procedure to display the spaceship, enemies, and background
* Added the MouvementEnnemi procedure
* Added the lose condition when an enemy reaches the bottom of the window
* Implemented the scoring system:
    * 100 points per destroyed enemy
    * 2000 points required to win

⸻

Inspiration and References

Main Inspiration

Galaxy Attack: Alien Shooter — App Store

Other References

* Reddit — Shoot ’em up game mockup
* Grapic — print function
* Grapic — setKeyRepeatMode function